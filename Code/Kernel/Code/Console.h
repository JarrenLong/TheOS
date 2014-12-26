#ifndef _VGA_H_
#define _VGA_H_

class VGA {
public:
    VGA();
    ~VGA();

    void Clear();
    void Get(char *&input);
    void Put(char *output);
protected:
    void putch(char c);
    char getch();
    void Update();
    void Scroll();
private:
    unsigned short *vMem;
    unsigned char cx, cy;
};

#endif //_VGA_H_
