#include "Console.h"

#include "X86.h"

VGA::VGA() {
    vMem = (unsigned short*)0xB8000;
    cx=cy=0;
}

VGA::~VGA() {
    vMem = (unsigned short*)0xB8000;
    cx=cy=0;
}

void VGA::Clear() {
    unsigned char attributeByte = (0 << 4) | (15 & 0x0F);
    unsigned short space = 0x20 | (attributeByte << 8);
    int i;
    for (i = 0; i < 80*25; i++) {
        vMem[i] = space;
    }
    cx=cy=0;
    Update();
}

void VGA::Get(char *&input) {
    char c = '\0';
    char *tmp = 0;
    while (c != '\n') {
        *tmp = getch();
        tmp++;
    }
    input = tmp;
}

void VGA::Put(char *output) {
    int i = 0;
    while (output[i]) {
        putch(output[i++]);
    }
}

void VGA::putch(char c) {
    unsigned short *location;
    if (c == 0x08 && cx) {
        cx--;
    } else if (c == 0x09) {
        cx = (cx+8) & ~(8-1);
    } else if (c == '\r') {
        cx = 0;
    } else if (c == '\n') {
        cx = 0;
        cy++;
    } else if (c >= ' ') {
        location = vMem + (cy*80 + cx);
        *location = c | ((0 << 4) | (0xF & 0x0F) << 8);
        cx++;
    }
    if (cx >= 80) {
        cx = 0;
        cy++;
    }
    Scroll();
    Update();
}

char VGA::getch() {
    unsigned int scancode;
    char retchar;
    while (1) {
        /*
        while ( ( inportb( 0x64 ) & 1 ) == false )
            ;
        scancode = inportb(0x60);
        if ( scancode & KEYPRESS ) {
            scancode &= 0x7F;
        }
        if ( scancode == KRLEFT_SHIFT || scancode == KRRIGHT_SHIFT ) {
            retchar = asciiShift[ scancode ];
        } else {
            retchar = asciiNonSh[ scancode ];
        }
        return retchar;
        */
    }
}

void VGA::Update() {
    unsigned short pos = cy * 80 + cx;
    /*
    outportb(0x3D4, 14);
    outportb(0x3D5, pos >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, pos);
    */
}

void VGA::Scroll() {
    unsigned char attributeByte = (0 << 4) | (15 & 0x0F);
    unsigned short space = 0x20 | (attributeByte << 8);
    if (cy >= 25) {
        int i;
        for (i = 0*80; i < 24*80; i++) {
            vMem[i] = vMem[i+80];
        }
        for (i = 24*80; i < 25*80; i++) {
            vMem[i] = space;
        }
        cy = 24;
    }
}

