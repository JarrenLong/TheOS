#define IDS_OK_MSG              "220 OK"
#define IDS_UNKNOWN_MSG         "550 Unknown Command"
#define IDS_WELCOME_MSG         "220 Welcome to XmailServer"
#define IDS_MSG_SYNTAX_ERROR    "500 Syntax error, command unrecognized"
#define IDS_INCORRECT_PARAMETER "501 Syntax error in parameters or arguments"
#define IDS_CMD_NOT_IMPLEMENTED "502 Command not implemented"
#define IDS_BAD_SEQUENCE        "503 Bad sequence of commands"
#define IDS_PARAM_NOT_IMPLEMENTED "504 Command parameter not implemented"
#define IDS_SYSTEM_STATUS       "211 System status, or system help reply"
#define IDS_HELP_MSG            "214 Help message"
#define IDS_WELCOME             "220 %s Service ready"
#define IDS_STRING112           "221 %s Service closing transmission channel"
#define IDS_STRING113           "421 %s Service not available, closing transmission channel"
#define IDS_STRING114           "250 OK, Action completed"
#define IDS_STRING115           "251 User not local; will forward to %s"
#define IDS_STRING116           "450 Requested mail action not taken: mailbox unavailable"
#define IDS_STRING117           "550 Requested action not taken: mailbox unavailable"
#define IDS_STRING118           "451 Requested action aborted: error in processing"
#define IDS_STRING119           "551 User not local; please try <forward-path>"
#define IDS_STRING120           "452 Requested action not taken: insufficient system storage"
#define IDS_STRING121           "552 Requested mail action aborted: exceeded storage allocation"
#define IDS_STRING122           "553 Requested action not taken: mailbox name not allowed"
#define IDS_STRING123           "354 Start mail input; end with <CRLF>.<CRLF>"
#define IDS_STRING124           "554 Transaction failed"
/*
#define IDS_OK_MSG                      101
#define IDS_UNKNOWN_MSG                 102
#define IDS_WELCOME_MSG                 103
#define IDS_MSG_SYNTAX_ERROR            104
#define IDS_INCORRECT_PARAMETER         105
#define IDS_CMD_NOT_IMPLEMENTED         106
#define IDS_BAD_SEQUENCE                107
#define IDS_PARAM_NOT_IMPLEMENTED       108
#define IDS_SYSTEM_STATUS               109
#define IDS_HELP_MSG                    110
#define IDS_WELCOME                     111
#define IDS_STRING112                   112
#define IDS_STRING113                   113
#define IDS_STRING114                   114
#define IDS_STRING115                   115
#define IDS_STRING116                   116
#define IDS_STRING117                   117
#define IDS_STRING118                   118
#define IDS_STRING119                   119
#define IDS_STRING120                   120
#define IDS_STRING121                   121
#define IDS_STRING122                   122
#define IDS_STRING123                   123
#define IDS_STRING124                   124
#define IDS_APP_TITLE                   125
*/
#define POP3_NEGATIVE 0
#define POP3_POSITIVE 1
#define POP3_STATE_AUTHORIZATION 2
#define POP3_STATE_TRANSACTION 3
#define POP3_STATE_UPDATE 4
#define POP3_STAT_RESPONSE 5
