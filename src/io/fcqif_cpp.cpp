#include <string>
#include "fcqif_cpp.hpp"
#include <fstream>

using namespace std;

void ofcqif_cpp::open(const string& file_name) {
    if (of.is_open()) {
        return;
    }

    do {
        // file already exists - do not overwrite
        ifstream tif(file_name);
        if (tif.is_open())
            return;
    } while (0);

    of.open(file_name, ofstream::out);
}

void ofcqif_cpp::close(void) {
    if (!of.is_open()) {
        return;
    }

    of.close();
}

void ofcqif_cpp::push(char c) {
    if (of.is_open()) {
        of.write(&c, 1u);
    }
}

ofcqif_cpp::ofcqif_cpp(const std::string& file_name) {
    open(file_name);
}

ofcqif_cpp::~ofcqif_cpp() {
    close();
}
