#ifndef _IFILE_H_
#define _IFILE_H_

#include <iostream>
#include <fstream>

#define READ 1
#define WRITE 2
#define TRUNC 4
#define APPEND 8
#define MAX_STRINGTABLE_SIZE 2048

class IFile {
public:
    IFile();
    IFile(char *filename, int mode);
    virtual ~IFile();

    int Open(char *filename, int mode);
    int GetPos();
    int SetPos(int position);
    char *Read(int bytes);
    int Write(char *buf);
    void Close();

    template <typename T>bool operator << (T) {
        //return Write(T);
        return true;
    }
protected:
    int Length();
private:
    char *filename;
    int mode;
    int pos;
    int length;
    std::fstream file;
    std::string stringTable[MAX_STRINGTABLE_SIZE];
};

#endif // _IFILE_H_
