#ifndef EMAIL_H
#define EMAIL_H

#include <string>
#include <list>
#include <vector>
#include "../IPlugin.h"

#define PROTO_POP3 0
#define PROTO_SMTP 1
#define PROTO_IMAP 2

#define POP3_NEGATIVE 0
#define POP3_POSITIVE 1
#define POP3_STATE_AUTHORIZATION 2
#define POP3_STATE_TRANSACTION 3
#define POP3_STATE_UPDATE 4
#define POP3_STATE_RESPONSE 5

typedef struct {
    unsigned int status;
    unsigned int rcptCount;
    unsigned int cLength;
    std::string path;
    std::string from, to;
    void *contents;
} EmailMsg;

class IEmail : public IPlugin {
public:
    virtual ~IEmail() {}
    unsigned int state;
    std::string user, pass, homePath;
    std::list<EmailMsg> list;
};

class Email : public IPlugin {
public:
    Email();
    Email(unsigned int proto);
    ~Email();
    void SetProtocol(unsigned int p);
    Code Process(std::string data);
private:
    unsigned int protocol;
    IEmail *session;
};

#endif // EMAILSESSION_H
