#include "String.h"

class EMailAddress {
    String User
    String Domain;
public:
    EMailAddress(String EmailAddress) {
        Address = EmailAddress;
    }
    bool SetAddress(String EmailAddress) {

	int i=0;
	for(int i=0;i<EmailAddress.length() && EmailAddress[i]!='@';i++) {
		User +=EmailAddress[i];
	}
	i++;
	for(i;i<EmailAddress.length();i++) {
		Domain +=EmailAddress[i];
	}
        return true;
    }
    String GetAddress() {
	String Address;
	Address += User;
	Address += '@';
	Address += Domain;
        return Address;
    }
    String GetDomain() {
        return Domain;
    }
    String GetUser() {
        return User;
    }
};
