// A simple demo for the muses library.
// Contrarily to the library it uses, this demo is public domain.

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <muses/Server.hh>
#include <muses/ServerPort.hh>
#include <muses/Connection.hh>
#include <muses/ConnectionData.hh>
#include <muses/ConnectHandler.hh>
#include <muses/ReceiveHandler.hh>
#include <muses/DisconnectHandler.hh>
#include <muses/TimeoutHandler.hh>
#include <muses/RH_LineSplitter.hh>
#include <muses/RH_Telnet.hh>
#include <muses/LineHandler.hh>
#include <muses/MusesException.hh>
#include <muses/SF_Telnet.hh>

using namespace muses;

/////////////////////////////////////////////////////////////////////
// In order to build a server using libmuses, some handler classes //
// need to be defined. The following are very simple examples.     //
/////////////////////////////////////////////////////////////////////

// First of all a ReceiveHandler, which interprets whatever arrives.

class MyReceiveHandler : public ReceiveHandler
{
public:
  MyReceiveHandler();
  virtual ~MyReceiveHandler();
  virtual unsigned int operator () (Connection& connection, unsigned int len);
};

MyReceiveHandler::MyReceiveHandler()
  : ReceiveHandler()
{
  ;
}

MyReceiveHandler::~MyReceiveHandler()
{
  ;
}

unsigned int MyReceiveHandler::operator () (Connection& connection,
					    unsigned int len)
{
  cout << "Receiving " << len
       << " bytes of data on socket " << connection.socketNumber()
       << ", port " << connection.port()->portNumber() << "." << endl;

  connection << "You sent me " << len << " bytes of data."
	     << endl;

  return len; // Throw it all away
}

// In order to store long-lived data into a connection,
// we need to derive some class from ConnectionData.
// The ConnectionData base class does not have any method or data member:
// it is only used for dynamic casting, and it can be used (as below) as a
// mix-in class in multiple inheritance.

class MyData : public ConnectionData, public std::string
{
public:
  MyData(const std::string& s) : std::string(s) {};
  virtual ~MyData();
};

MyData::~MyData()
{
  ;
}

// Instead of writing a custom ReceiveHandler we can use RH_LineSplitter,
// which splits received text into lines and invokes a LineHandler on each.
// In this case we need to define one LineHandler, or several.

class MyLineHandler : public LineHandler
{
public:
  MyLineHandler();
  virtual ~MyLineHandler();
  virtual void operator () (Connection& connection, std::string& line);
};

MyLineHandler::MyLineHandler()
  : LineHandler()
{
  ;
}

MyLineHandler::~MyLineHandler()
{
  ;
}

void MyLineHandler::operator () (Connection& connection, std::string& line)
{
  // Retrieve custom data from the Connection.
  // Return zero if there is no data of the correct kind.
  MyData *data = dynamic_cast<MyData*>(connection.data());

  // Do something
  cout << "Socket " << connection.socketNumber()
       << " sent the following to port " << connection.port()->portNumber()
       << ":" << endl << line << endl;

  connection << "You sent me " << line.length() << " chars." << endl;
  if (data)
    connection << "The previous line you sent was: " << *data << endl;

  if (line == std::string("DISCONNECT"))
    connection.disconnect();            // Only the current Connection
  else if (line == std::string("CLOSE PORT"))
    connection.port()->shutdown();      // Only the current ServerPort
  else if (line == std::string("SHUTDOWN"))
    connection.port()->server()->shutdown();       // The whole server
  else {
    // Change Connection data
    delete data;
    data = new MyData(line);
    connection.setData(data);
  }
}

// Next, a DisconnectHandler, called when a connection is closed - either
// by the server or by the peer.

class MyDisconnectHandler: public DisconnectHandler
{
public:
  MyDisconnectHandler();
  virtual ~MyDisconnectHandler();
  virtual void operator () (Connection& deadConnection);
};

MyDisconnectHandler::MyDisconnectHandler()
  : DisconnectHandler()
{
  ;
}

MyDisconnectHandler::~MyDisconnectHandler()
{
  ;
}

void MyDisconnectHandler::operator() (Connection& deadConnection)
{
  cout << "Closing connection on socket " << deadConnection.socketNumber()
       << endl;

  // Retrieve custom data from the Connection
  MyData *data = dynamic_cast<MyData*>(deadConnection.data());
  if (data) {
    cout << "Last line sent was: " << *data << endl;
    delete data;
    deadConnection.setData(0); // Clear deleted pointer
  }
}

// We also need a ConnectHandler, called on new connections. It should
// perform whatever initialization is required, set up receive and
// disconnect handlers, and possibly say hello.

class MyConnectHandler: public ConnectHandler
{
public:
  MyConnectHandler(ReceiveHandler *rec, DisconnectHandler *disc);
  virtual ~MyConnectHandler();
  virtual void operator () (Connection& newConnection);
private:
  // Here we suppose we can use a single global handler.
  // A different application may need to allocate a different handler for
  // each connection.
  ReceiveHandler *_rec;
  DisconnectHandler *_disc;
};

MyConnectHandler::MyConnectHandler(ReceiveHandler *rec,
				   DisconnectHandler *disc)
  : ConnectHandler(),
    _rec(rec),
    _disc(disc)
{
  ;
}

MyConnectHandler::~MyConnectHandler()
{
  ;
}

void MyConnectHandler::operator() (Connection& newConnection)
{
  cout << "New connection on socket " << newConnection.socketNumber() << endl;
  newConnection.setReceiveHandler(_rec);
  newConnection.setSendFilter(sf_telnet);
  newConnection.setDisconnectHandler(_disc);
  newConnection << "Welcome to the MUSES demo." << endl
		<< "At the moment there are "
		<< newConnection.port()->connections().size()
		<< " active connections on port "
		<< newConnection.port()->portNumber()
		<< endl;
}

// Lastly, an optional TimeoutHandler, to deal with whatever should be done
// if nothing happens.

class MyTimeoutHandler: public TimeoutHandler
{
public:
  MyTimeoutHandler(int n);
  virtual ~MyTimeoutHandler();
  virtual void operator () (Server& server);

private:
  int _remaining;
};

MyTimeoutHandler::MyTimeoutHandler(int n)
  : TimeoutHandler(),
    _remaining(n)
{
  ;
}

MyTimeoutHandler::~MyTimeoutHandler()
{
  ;
}

void MyTimeoutHandler::operator() (Server& server)
{
  --_remaining;
  cout << "Nothing happened, counter is " << _remaining << endl;
  if (!_remaining)
    {
      cout << "Shutting down." << endl;
      server.shutdown();
    }
}

//////////////////////////////////////////
// It is now time to set up the server. //
//////////////////////////////////////////

int main()
{
  const unsigned int MY_PORT_RAW  = 1234;
  const unsigned int MY_PORT_LINE = 2345;

  MyReceiveHandler    myReceive;
  MyLineHandler       myLine;
  RH_LineSplitter     mySplitter;    mySplitter.setLineHandler(&myLine);
  RH_Telnet           myTelnet1(&myReceive), myTelnet2(&mySplitter);
  MyDisconnectHandler myDisconnect;
  MyTimeoutHandler    myTimeout(100);
  MyConnectHandler    myConnectR(&myTelnet1, &myDisconnect);
  MyConnectHandler    myConnectL(&myTelnet2, &myDisconnect);

  Server myServer;
  cout << "Setting up server on ports " << MY_PORT_RAW << " (raw) and "
       << MY_PORT_LINE << " (line-based)." << endl;

  try {
    myServer.addPort(MY_PORT_RAW, &myConnectR);
    myServer.addPort(MY_PORT_LINE, &myConnectL);

    while (myServer.ports().begin() != myServer.ports().end())
      myServer.run(1000000, &myTimeout); // Poll each second
  }
  catch (const std::exception& e) {
    cerr << e.what() << endl;
  }

  return 0;
}
