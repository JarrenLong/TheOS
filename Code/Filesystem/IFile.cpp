#include "IFile.h"

IFile::IFile() {
    pos = length = mode = 0;
}

IFile::IFile(char *filename, int mode) {
    this->filename = filename;
    this->mode = mode;
    pos = 0;
    length = Open(filename,READ);
}

IFile::~IFile() {
    Close();
}

int IFile::Open(char *filename, int mode) {
    switch (mode) {
    case READ:
        file.open(filename,std::ios::in);
        break;
    case WRITE:
        file.open(filename,std::ios::out);
        break;
    case TRUNC:
        file.open(filename,std::ios::trunc);
        break;
    case APPEND:
        file.open(filename,std::ios::app);
        break;
    default:
        return -1;
        break;
    }
    if (file.is_open()) {
        pos = file.tellg();
        return Length();
    }
    return -1;
}

int IFile::Length() {
    if (file.is_open()) {
        file.seekp(std::ios_base::end);
        length = file.tellg();
        return length;
    }
    return -1;
}

int IFile::GetPos() {
    return pos;
}

int IFile::SetPos(int position) {
    pos = position;
    if (file.is_open()) {
        file.seekp(std::ios_base::end);
    }
    return pos;
}

void IFile::Close() {
    if (file.is_open()) {
        file.close();
    }
    delete &file;
    pos = length = 0;
}

char *IFile::Read(int bytes) {
    char *buf = new char[bytes];
    if (file.is_open()) {
        if (mode!=READ) {
            Close();
            Open(filename,READ);
        }
        file.read((char*)buf, bytes);
    }
    return buf;
}

int IFile::Write(char *buf) {
    int bytes = 0;
    if (file.is_open()) {
        if (mode!=WRITE) {
            Close();
            Open(filename,WRITE);
        }
        file.write((char*)buf, sizeof(buf));
    }
    return bytes;
}
