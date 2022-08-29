#include <string>
#include "fcqif_posix.hpp"
#include "file.h"

using namespace std;

bool ifcqif_posix::dry(void) {
    return (fid==(-1) || !bufvld);
}

void ifcqif_posix::pop(void) {
    if (dry()) {
        return;
    }

    if (::read(fid, (void*)&buf, 1u)==1) {
        bufvld = true;
    } else {
        bufvld = false;
    }
}

char ifcqif_posix::top(void) {
    if (dry()) {
        return '\0';
    }

    return buf;
}

void ifcqif_posix::open(const string& file_name) {
    if (fid!=(-1)) {
        // a file is already open
        return;
    }

    fid = ::open(file_name.c_str(), O_RDONLY);
    if (fid==(-1)) {
        return;
    }

    bufvld = true; // so pop works
    pop();
}

void ifcqif_posix::close(void) {
    if (fid==(-1)) {
        // no file is open
        return;
    }

    bufvld = false; // prevent any further read - whether successfully closed or not

    if (::close(fid)==0) {
        fid = (-1);
    }
}

ifcqif_posix::ifcqif_posix():fid(-1) {
    // nothing else is done
}

ifcqif_posix::ifcqif_posix(const std::string& file_name):fid(-1) {
    open(file_name);
}

ifcqif_posix::~ifcqif_posix() {
    close();
}
