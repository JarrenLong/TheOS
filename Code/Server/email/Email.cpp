#include "Email.h"
#include "Pop3.h"
#include "Smtp.h"

void __declspec(dllexport) GetPlugin( IPlugin **pOut) {
    *pOut = new Email;
}

Email::Email() {
    Email(PROTO_POP3);
}

Email::Email(unsigned int proto) : protocol(proto) {
    switch (protocol) {
    case PROTO_SMTP:
        session = new Smtp();
        break;
    case PROTO_POP3:
    default:
        session = new Pop3();
        break;
    }
}

Email::~Email() {
    delete session;
}

void Email::SetProtocol(unsigned int p) {
    protocol = p;
}

Code Email::Process(std::string data) {
    Code tmp = session->Process(data);
    return Code(tmp.GetLong(),tmp.GetStr());
}

