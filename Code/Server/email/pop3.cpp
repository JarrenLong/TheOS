#include "Pop3.h"

void Pop3::Login(std::string User, std::string Pass) {
    std::string home, pwdFile;
    home = ".\\domainpath\\" + User;
    pwdFile = home + "\\" + Pass + ".pwd";
    /*
        if(pwdFile = exists) {
            Password ok?
            SetHomePath = home
            LockMailDrop();
        }
    */
}

Code Pop3::Process(std::string data) {
    std::string command, options, protocol;

    //Tokenize input string
    std::vector<std::string> tokens;
    Tokenize(data,tokens," ");

    command = tokens.at(0);
    options = tokens.at(1);

    if (command == "RPOP") {
        return Rpop(options);
    } else if (command == "TOP")  {
        return Top(options);
    } else if (command == "USER") {
        return User(options);
    } else if (command == "PASS") {
        return Pass(options);
    } else if (command == "RETR") {
        return Retr(options);
    } else if (command == "DELE") {
        return Dele(options);
    } else if (command == "LAST") {
        return Last(options);
    } else if (command == "STAT") {
        return Stat(options);
    } else if (command == "LIST") {
        return List(options);
    } else if (command == "QUIT") {
        return Quit(options);
    } else if (command == "NOOP") {
        return Noop(options);
    } else if (command == "RSET") {
        return Rset(options);
    }
    return Code(-1,"Invalid Command!");
}

Code Pop3::Rpop(std::string data) {
    return Code(POP3_NEGATIVE, "POP3_RPOP");
}

Code Pop3::Top(std::string data) {
    return Code(POP3_NEGATIVE, "POP3_TOP");
}

Code Pop3::User(std::string data) {
    user = data;
    homePath +=  "\\";
    homePath += data;

    if (1) { //!PathFileExists(m_szUserHome)) {
        return Code(POP3_NEGATIVE, std::string("User " + data + " not found."));
    } else if (0) { //state != POP3_STATE_AUTHORIZATION) {
        return Code(POP3_NEGATIVE, "Cannot authorize");
    } else {
        return Code(POP3_POSITIVE, "OK");
    }
}

Code Pop3::Pass(std::string data) {

    int len = data.length();
    data[len-2]=0;
    data+=5;
    if (data[len-2]==10) data[len-2]=0;
    pass = data;

    if (/* state != POP3_STATE_AUTHORIZATION || */ user.length()<1) {
        return Code(POP3_NEGATIVE,"Cannot authorize");
    }

    Login(user,pass);
    return Code(POP3_POSITIVE,"OK");
}

Code Pop3::Retr(std::string data) {
    int len = data.length();
    data=4;
    data[4]='0';
    data[len-2]=0;
    int msg_id=0; //atol(data);

    if (state!=POP3_STATE_TRANSACTION) {
        return Code(POP3_NEGATIVE,"Retr");
    }
    if ((unsigned int)msg_id>list.size()) {
        return Code(0,"-ERR Invalid message number\r\n");
    }
    std::string msg = "+OK ";
    //msg += list[msg_id-1].cLength;
    msg += " octets\r\n";
    //SendMessageFile(m_pPop3MessageList[msg_id-1].GetPath());
    msg += "\r\n.\r\n";
    return Code(msg.length(),msg);
}

Code Pop3::Dele(std::string data) {
    data+=4;
    data[4]='0';
    data[data.length()-2]=0;
    int msg_id=0; //atol(buf);

    if (state!=POP3_STATE_TRANSACTION || (unsigned int)msg_id>list.size()) {
        return Code(POP3_NEGATIVE,"Delete");
    }
    //list.erase();
    return Code(POP3_POSITIVE,"+OK Delete");
}

Code Pop3::Last(std::string data) {
    if (state!=POP3_STATE_TRANSACTION) {
        return Code(POP3_NEGATIVE,"Last");
    }
    std::string msg = "+OK ";
    //msg += lastMsg;
    return Code(POP3_POSITIVE,msg);
}

Code Pop3::Stat(std::string data) {
    if (state!=POP3_STATE_TRANSACTION) {
        return Code(POP3_NEGATIVE,"Can't get stats");
    }
    return Code(POP3_STATE_RESPONSE,"Stats");
}

Code Pop3::List(std::string data) {
    if (state!=POP3_STATE_TRANSACTION) {
        return Code(POP3_NEGATIVE,"No List!");
    }

    std::string listStr = "+OK \r\n";
    for (unsigned int i=0; i < list.size(); i++) {
        listStr += i+1;
        listStr += ' ';
        //listStr += list[i].cLength;
        listStr += "\r\n";
    }
    listStr += ".\r\n";
    return Code(POP3_POSITIVE,listStr);
}

Code Pop3::Quit(std::string data) {
    if (state==POP3_STATE_TRANSACTION)
        state=POP3_STATE_UPDATE;
    UpdateMails();
    return Code(POP3_POSITIVE,"Goodbye");
}

Code Pop3::Noop(std::string data) {
    return Code(POP3_NEGATIVE,"No Op");
}

Code Pop3::Rset(std::string data) {
    if (state!=POP3_STATE_TRANSACTION) {
        return Code(POP3_NEGATIVE,"Reset");
    }
    for (unsigned int i=0; i < list.size(); i++) {
        //m_pPop3MessageList[i].Reset();
        //printf("Message %d: %ld %s\n",i+1,m_pPop3MessageList[i].GetSize(), m_pPop3MessageList[i].GetPath());
    }
    return Code(POP3_NEGATIVE,"End Reset");
}
