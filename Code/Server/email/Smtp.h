#include "Email.h"

class Smtp : public IEmail {
public:
    Code Process(std::string data) {return Code(0,"SMTP");}
    int ProcessDATAEnd(void);
    bool CreateNewMessage(void);
private:
    Code Helo(std::string data);
    Code Rcpt(std::string data);
    Code Mail(std::string data);
    Code Data(std::string data);
    Code Quit(std::string data);
    Code Noop(std::string data);
    Code Rset(std::string data);
};
