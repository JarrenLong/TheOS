#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "Device.h"

#define NULL 0
#define ESC			27
#define BACKSPACE '\b'
#define TAB '\t'
#define NEWLINE '\n'
#define RETURN '\r'
#define KESC			1
#define	KF1		0x80
#define	KF2		(KF1 + 1)
#define	KF3		(KF2 + 1)
#define	KF4		(KF3 + 1)
#define	KF5		(KF4 + 1)
#define	KF6		(KF5 + 1)
#define	KF7		(KF6 + 1)
#define	KF8		(KF7 + 1)
#define	KF9		(KF8 + 1)
#define	KF10		(KF9 + 1)
#define	KF11		(KF10 + 1)
#define	KF12		(KF11 + 1)
#define	KINS		0x90
#define	KDEL		(KINS + 1)
#define	KHOME		(KINS + 2)
#define	KEND		(KINS + 3)
#define	KPGUP		(KINS + 4)
#define	KPGDN		(KINS + 5)
#define	KLEFT		(KINS + 6)
#define	KUP		    (KINS + 7)
#define	KDOWN		(KINS + 8)
#define	KRIGHT		(KINS + 9)
#define	KMETA_ALT	0x0200	// Alt is pressed
#define	KMETA_CTRL	0x0400	// Ctrl is pressed
#define	KMETA_SHIFT	0x0800	// Shift is pressed
#define	KMETA_ANY	(KMETA_ALT | KMETA_CTRL | KMETA_SHIFT)
#define	KMETA_CAPS	0x1000	// CapsLock is on
#define	KMETA_NUM	0x2000	// NumLock is on
#define	KMETA_SCRL	0x4000	// ScrollLock is on
#define	KPRNT	( KRT + 1 )
#define	KPAUSE	( KRT + 2 )
#define	KLWIN	( KRT + 3 )
#define	KRWIN	( KRT + 4 )
#define	KMENU	( KRT + 5 )
#define	KRLEFT_CTRL		0x1D
#define	KRLEFT_SHIFT		0x2A
#define	KRCAPS_LOCK		0x3A
#define	KRLEFT_ALT		0x38
#define	KRRIGHT_ALT		0x38	// same as left
#define	KRRIGHT_CTRL		0x1D	// same as left
#define	KRRIGHT_SHIFT		0x36
#define	KRSCROLL_LOCK		0x46
#define	KRNUM_LOCK		0x45
#define	KRDEL			0x53
#define KEYPRESS 0x80
#define MAXKEYBUFFER 64
#define KEYPORT 0x60

static const uint8_t asciiNonSh[] = { NULL, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                      '0', '-', '=', BACKSPACE, TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', NEWLINE, 0,
                                      'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n',
                                      'm', ',', '.', '/', 0, 0, 0, ' ', 0, KF1, KF2, KF3, KF4, KF5, KF6, KF7, KF8, KF9, KF10, 0, 0,
                                      KHOME, KUP, KPGUP,'-', KLEFT, '5', KRIGHT, '+', KEND, KDOWN, KPGDN, KINS, KDEL, 0, 0, 0, KF11, KF12
                                    };

// Shifted scancodes to ASCII:
static const uint8_t asciiShift[] = { NULL, ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(',
                                      ')', '_', '+', BACKSPACE, TAB, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', NEWLINE, 0,
                                      'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N',
                                      'M', '<', '>', '?', 0, 0, 0, ' ', 0, KF1, KF2, KF3, KF4, KF5, KF6, KF7, KF8, KF9, KF10, 0, 0,
                                      KHOME, KUP, KPGUP, '-', KLEFT, '5', KRIGHT, '+', KEND, KDOWN, KPGDN, KINS, KDEL, 0, 0, 0, KF11, KF12
                                    };

class Keyboard : public Device {
public:
    Keyboard();
    ~Keyboard();

    uint32_t Read();
    uint32_t Write();
    void SetLEDS(uint8_t scrollLock, uint8_t numLock, uint8_t capsLock);
    void Flush();
    void ResetComputer();
private:
    uint64_t Handler(Stack stack);
};

#endif // KEYBOARD_H
