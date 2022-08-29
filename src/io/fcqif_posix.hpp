#ifndef FCQIF_POSIX_H
#define FCQIF_POSIX_H

#include <string>
#include "cqif.hpp"

class ifcqif_posix: public icqif {
private:
    int  fid;
    bool bufvld;
    char buf;
public:
    void open(const std::string& file_name);
    void close(void);
public:
    void pop(void);
    char top(void);
    bool dry(void);
public:
    ifcqif_posix(void);
    ifcqif_posix(const std::string& file_name);
    ~ifcqif_posix();
};

#endif // FCQIF_POSIX_H
