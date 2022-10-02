#include <string>
#include "fcqif.hpp"
#include <fstream>
#include <iostream>

using namespace std;

void ofcqif::open(const string& file_path) {
    if (use_cout || ofs.is_open()) { // already open
        return;
    }

    if (file_path.length()==0) {
        use_cout = true;
        return;
    }

    do {
        // file already exists - do not overwrite
        ifstream tif(file_path);
        if (tif.is_open())
            return;
    } while (0);

    ofs.open(file_path, ofstream::out);
}

void ofcqif::close(void) {
    if (use_cout) {
        use_cout = false;
        return;
    }

    if (!ofs.is_open()) {
        return;
    }

    ofs.close();
}

void ofcqif::push(char c) {
    if (use_cout) {
        cout.write(&c, 1u);
    } else if (ofs.is_open()) {
        ofs.write(&c, 1u);
    }
}

ofcqif::ofcqif(const std::string& file_path): use_cout(false) {
    open(file_path);
}

ofcqif::~ofcqif() {
    close();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ifcqif::open(const string& file_path) {
    if (use_cin || ifs.is_open()) { // already open
        return;
    }

    if (file_path.length()==0) {
        use_cin = true;
        return;
    }

    ifs.open(file_path);
    if (!dry()) {
        pop();
    }
}

void ifcqif::close(void) {
    if (use_cin) {
        use_cin = false;
        return;
    }

    if (!ifs.is_open()) {
        return;
    }

    ifs.close();
}

void ifcqif::pop(void) {
    if (use_cin) {
        cin.read(&buf, 1u);
    } else if (ifs.is_open() && !ifs.eof()) {
        ifs.read(&buf, 1u);
    } else {
        buf = '\0';
    }
}

char ifcqif::top(void) {
    return buf;
}

bool ifcqif::dry(void) {
    return (!use_cin && (!ifs.is_open() || ifs.eof()));
}

ifcqif::ifcqif(const string& file_path): use_cin(false) {
    open(file_path);
}

ifcqif::ifcqif(): use_cin(false) {
    // nothing to do
}

ifcqif::~ifcqif() {
    close();
}
