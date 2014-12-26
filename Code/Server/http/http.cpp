#include "http.h"

void __declspec(dllexport) GetPlugin( IPlugin **pOut) {
    *pOut = new Http;
}

Code Http::Process(std::string data) {
    //Check for valid input
    if (!Clean(data)) {
        return Code(-1,"Dirty Input!");
    }
    std::string command, options, protocol;

    //Tokenize input string
    std::vector<std::string> tokens;
    Tokenize(data,tokens," ");

    command = tokens.at(0);
    options = tokens.at(1);
    protocol = tokens.at(2);

    if (protocol != "HTTP/1.0" && protocol != "HTTP/1.1") {
        return Code(-1,"Bad Protocol");
    }

    if (command == "HEAD") {
        return Head(options);
    } else if (command == "GET")  {
        return Get(options);
    } else if (command == "POST") {
        return Post(options);
    } else if (command == "PUT") {
        return Put(options);
    } else if (command == "DELETE") {
        return Delete(options);
    } else if (command == "TRACE") {
        return Trace(options);
    } else if (command == "OPTIONS") {
        return Options(options);
    } else if (command == "CONNECT") {
        return Connect(options);
    } else { //No Command
        return Code(-1,"Invalid Command!");
    }
    //Shouldn't ever get here
    return Code(-1,"End of Switch");
}

bool Http::Clean(std::string data) {
    int len = data.length();
    //Check input length
    if (len < 0 | len > 1024) {
        return false;
    }
    //Check for invalid characters & duplicates
    int i = 0, c = 0;
    char tmp = 0;
    while (i<len) {
        if (data[i] == '*' |
                data[i] == ':' |
                data[i] == ';' |
                data[i] == '<' |
                data[i] == '>' |
                data[i] == '|') {
            return false;
        }
        //Check for duplicate characters
        if (data[i] == tmp) {
            c++;
        } else {
            c = 0;
        }
        //If more than 4 duplicates in a row, exit
        if (c > 4) {
            return false;
        }
        tmp = data[i];
        i++;
    }
    return true;
}

Code Http::Head(std::string opt) {
    return Code(0,"HEAD");
}

Code Http::Get(std::string opt) {
    return Code(1,"GET");
}

Code Http::Put(std::string opt) {
    return Code(2,"PUT");
}

Code Http::Post(std::string opt) {
    return Code(3,"POST");
}

Code Http::Delete(std::string opt) {
    return Code(4,"DELETE");
}

Code Http::Trace(std::string opt) {
    return Code(5,"TRACE");
}

Code Http::Options(std::string opt) {
    return Code(6,"OPTIONS");
}

Code Http::Connect(std::string opt) {
    return Code(7,"CONNECT");
}
