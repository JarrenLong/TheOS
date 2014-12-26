#ifndef GRAPHICS_H
#define GRAPHICS_H

typedef struct vesaInfo {
    unsigned char  VESASignature[4]     __attribute__ ((packed));
    unsigned short VESAVersion          __attribute__ ((packed));
    unsigned long  OEMStringPtr         __attribute__ ((packed));
    unsigned char  Capabilities[4]      __attribute__ ((packed));
    unsigned long  VideoModePtr         __attribute__ ((packed));
    unsigned short TotalMemory          __attribute__ ((packed));
    unsigned short OemSoftwareRev       __attribute__ ((packed));
    unsigned long  OemVendorNamePtr     __attribute__ ((packed));
    unsigned long  OemProductNamePtr    __attribute__ ((packed));
    unsigned long  OemProductRevPtr     __attribute__ ((packed));
    unsigned char  Reserved[222]        __attribute__ ((packed));
    unsigned char  OemData[256]         __attribute__ ((packed));
} vesaInfo;

typedef struct vesaModeInfo {
    unsigned short ModeAttributes       __attribute__ ((packed));
    unsigned char  WinAAttributes       __attribute__ ((packed));
    unsigned char  WinBAttributes       __attribute__ ((packed));
    unsigned short WinGranularity       __attribute__ ((packed));
    unsigned short WinSize              __attribute__ ((packed));
    unsigned short WinASegment          __attribute__ ((packed));
    unsigned short WinBSegment          __attribute__ ((packed));
    unsigned long  WinFuncPtr           __attribute__ ((packed));
    unsigned short BytesPerScanLine     __attribute__ ((packed));
    unsigned short XResolution          __attribute__ ((packed));
    unsigned short YResolution          __attribute__ ((packed));
    unsigned char  XCharSize            __attribute__ ((packed));
    unsigned char  YCharSize            __attribute__ ((packed));
    unsigned char  NumberOfPlanes       __attribute__ ((packed));
    unsigned char  BitsPerPixel         __attribute__ ((packed));
    unsigned char  NumberOfBanks        __attribute__ ((packed));
    unsigned char  MemoryModel          __attribute__ ((packed));
    unsigned char  BankSize             __attribute__ ((packed));
    unsigned char  NumberOfImagePages   __attribute__ ((packed));
    unsigned char  Reserved_page        __attribute__ ((packed));
    unsigned char  RedMaskSize          __attribute__ ((packed));
    unsigned char  RedMaskPos           __attribute__ ((packed));
    unsigned char  GreenMaskSize        __attribute__ ((packed));
    unsigned char  GreenMaskPos         __attribute__ ((packed));
    unsigned char  BlueMaskSize         __attribute__ ((packed));
    unsigned char  BlueMaskPos          __attribute__ ((packed));
    unsigned char  ReservedMaskSize     __attribute__ ((packed));
    unsigned char  ReservedMaskPos      __attribute__ ((packed));
    unsigned char  DirectColorModeInfo  __attribute__ ((packed));
    unsigned long  PhysBasePtr          __attribute__ ((packed));
    unsigned long  OffScreenMemOffset   __attribute__ ((packed));
    unsigned short OffScreenMemSize     __attribute__ ((packed));
    unsigned char  Reserved[206]        __attribute__ ((packed));
} vesaModeInfo;

struct Point {
public:
    uint64_t position; //XYZW
    uint64_t color; //RBGA
};

class Graphics {
public:
    Graphics() {
        x=y=w=h=256;
    }
    bool CheckVESA() {return true;}
    void GetModeInfo() {}
    bool FindMode() {return true;}
    void SetMode() {}
protected:
    void setBank();
private:
    uint32_t x,y,w,h;
    uint8_t *scrMem;
    vesaInfo head;
    vesaModeInfo modes;
};

#endif // GRAPHICS_H
/*
int CheckPresence() {
    __dpmi_regs r;
    long dosbuf;
    int c;

    // use the conventional memory transfer buffer
    dosbuf = __tb & 0xFFFFF;

    // initialize the buffer to zero
    for (c=0; c<sizeof(VESA_INFO); c++)
        _farpokeb(_dos_ds, dosbuf+c, 0);

    dosmemput("VBE2", 4, dosbuf);

    // call the VESA function
    r.x.ax = 0x4F00;
    r.x.di = dosbuf & 0xF;
    r.x.es = (dosbuf>>4) & 0xFFFF;
    __dpmi_int(0x10, &r);

    // quit if there was an error
    if (r.h.ah)
        return -1;

    // copy the resulting data into our structure
    dosmemget(dosbuf, sizeof(VESA_INFO), &vesa_info);

    // check that we got the right magic marker value
    if (strncmp(vesa_info.VESASignature, "VESA", 4) != 0)
        return -1;

    // it worked!
    return 0;
}

int get_mode_info(int mode) {
    __dpmi_regs r;
    long dosbuf;
    int c;

    // use the conventional memory transfer buffer
    dosbuf = __tb & 0xFFFFF;

    // initialize the buffer to zero
    for (c=0; c<sizeof(MODE_INFO); c++)
        _farpokeb(_dos_ds, dosbuf+c, 0);

    // call the VESA function
    r.x.ax = 0x4F01;
    r.x.di = dosbuf & 0xF;
    r.x.es = (dosbuf>>4) & 0xFFFF;
    r.x.cx = mode;
    __dpmi_int(0x10, &r);

    // quit if there was an error
    if (r.h.ah)
        return -1;

    // copy the resulting data into our structure
    dosmemget(dosbuf, sizeof(MODE_INFO), &mode_info);

    // it worked!
    return 0;
}

int find_vesa_mode(int w, int h) {
    int mode_list[256];
    int number_of_modes;
    long mode_ptr;
    int c;

    // check that the VESA driver exists, and get information about it
    if (get_vesa_info() != 0)
        return 0;

    // convert the mode list pointer from seg:offset to a linear address
    mode_ptr = ((vesa_info.VideoModePtr & 0xFFFF0000) >> 12) +
               (vesa_info.VideoModePtr & 0xFFFF);

    number_of_modes = 0;

    // read the list of available modes
    while (_farpeekw(_dos_ds, mode_ptr) != 0xFFFF) {
        mode_list[number_of_modes] = _farpeekw(_dos_ds, mode_ptr);
        number_of_modes++;
        mode_ptr += 2;
    }

    // scan through the list of modes looking for the one that we want
    for (c=0; c<number_of_modes; c++) {

        // get information about this mode
        if (get_mode_info(mode_list[c]) != 0)
            continue;

        // check the flags field to make sure this is a color graphics mode,
         * and that it is supported by the current hardware
        if ((mode_info.ModeAttributes & 0x19) != 0x19)
            continue;

        // check that this mode is the right size
        if ((mode_info.XResolution != w) || (mode_info.YResolution != h))
            continue;

        // check that there is only one color plane
        if (mode_info.NumberOfPlanes != 1)
            continue;

        // check that it is a packed-pixel mode (other values are used for
         * different memory layouts, eg. 6 for a truecolor resolution)
        if (mode_info.MemoryModel != 4)
            continue;

        // check that this is an 8-bit (256 color) mode
        if (mode_info.BitsPerPixel != 8)
            continue;

        // if it passed all those checks, this must be the mode we want!
        return mode_list[c];
    }

    // oh dear, there was no mode matching the one we wanted!
    return 0;
}

int set_vesa_mode(int w, int h) {
    __dpmi_regs r;
    int mode_number;

    // find the number for this mode
    mode_number = find_vesa_mode(w, h);
    if (!mode_number)
        return -1;

    // call the VESA mode set function
    r.x.ax = 0x4F02;
    r.x.bx = mode_number;
    __dpmi_int(0x10, &r);
    if (r.h.ah)
        return -1;

    // it worked!
    return 0;
}

void set_vesa_bank(int bank_number) {
    __dpmi_regs r;

    r.x.ax = 0x4F05;
    r.x.bx = 0;
    r.x.dx = bank_number;
    __dpmi_int(0x10, &r);
}

void putpixel_vesa_640x480(int x, int y, int color) {
    int address = y*640+x;
    int bank_size = mode_info.WinGranularity*1024;
    int bank_number = address/bank_size;
    int bank_offset = address%bank_size;

    set_vesa_bank(bank_number);

    _farpokeb(_dos_ds, 0xA0000+bank_offset, color);
}

void copy_to_vesa_screen(char *memory_buffer, int screen_size) {
    int bank_size = mode_info.WinSize*1024;
    int bank_granularity = mode_info.WinGranularity*1024;
    int bank_number = 0;
    int todo = screen_size;
    int copy_size;

    while (todo > 0) {
        // select the appropriate bank
        set_vesa_bank(bank_number);

        // how much can we copy in one go?
        if (todo > bank_size)
            copy_size = bank_size;
        else
            copy_size = todo;

        // copy a bank of data to the screen
        dosmemput(memory_buffer, copy_size, 0xA0000);

        // move on to the next bank of data
        todo -= copy_size;
        memory_buffer += copy_size;
        bank_number += bank_size/bank_granularity;
    }
}
*/
