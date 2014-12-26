#include "Email.h"

class Pop3 : public IEmail {
public:
    Code Process(std::string data);
protected:
    bool LockMailDrop(void) {
        //Load list of messages and query stats
        return true;
    }
    void UpdateMails(void) {
        //Run through messages and delete marked from server
    }
    int SendMessageFile(std::string filePath) {
        //Open file and send contents
        return 0;
    }
    void Login(std::string User, std::string Pass);
private:
    Code Rpop(std::string data);
    Code Top(std::string data);
    Code User(std::string data);
    Code Pass(std::string data);
    Code Retr(std::string data);
    Code Dele(std::string data);
    Code Last(std::string data);
    Code Stat(std::string data);
    Code List(std::string data);
    Code Quit(std::string data);
    Code Noop(std::string data);
    Code Rset(std::string data);
};
