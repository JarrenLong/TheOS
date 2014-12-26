#ifndef FILESYSTEM_H
#define FILESYSTEM_H

//#include "../Code.h"
#include "../Filters/Filter.h"

#include <string>
#include <vector>

#define FLAG_READ  1
#define FLAG_WRITE 2
#define FLAG_EXEC 4
#define FLAG_BEG 8
#define FLAG_CUR 16
#define FLAG_END 32
#define FLAG_FILE 64
#define FLAG_DIR 128

class Entity {
public:
    bool doesExist();
    bool GetFilters();
    bool GetType();
    bool GetACL();
    bool SetACL();
    bool GetPath();
    bool GetName();
    bool GetExtension();
    long GetSize();
    bool Flush();
protected:
    bool Create();
    bool Open();
    bool Close();
    bool Read();
    bool Write();
    bool Seek();
    bool Tell();
private:
    bool entityType;
    char acl[3]; //POSIX-style permissions
    long size;
    std::string path;
    std::string name;
    std::string extension;
    std::vector<std::string> filterList;
    //Code lastError;
};

class FileSystem {
public:
    FileSystem();
    virtual ~FileSystem();

    bool Init();
    bool Kill();
    bool RegisterFilter();
    bool UnregisterFilter();
    bool GetFilter();
    bool AddRootPath();
    bool RemoveRootPath();
    bool GetRootPath();
    bool Flush();
    bool Delete();
    bool Copy();
    bool Move();
    bool Rename();
    bool EntityExists();
    bool GetEntityInfo();
    bool GetVersion();
private:
    std::vector<Filter*> filterList;
    std::vector<std::string> rootPathList;
    //Code lastError;
};

#endif // FILESYSTEM_H
