#ifndef FILTER_H
#define FILTER_H

#include <string>

class Filter {
public:
    Filter() {}
    Filter(std::string name, std::string desc) : filterName(name), filterDescription(desc) {}
    virtual ~Filter() {}

    virtual bool Encode(std::string &in, std::string &out) = 0;
    virtual bool Decode(std::string &in, std::string &out) = 0;
    std::string GetName() {
        return filterName;
    }
    std::string GetDescription() {
        return filterDescription;
    }
private:
    std::string filterName;
    std::string filterDescription;
    //Code lastError;
};

#endif // FILTER_H
