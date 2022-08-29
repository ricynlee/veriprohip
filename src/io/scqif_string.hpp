#ifndef SCQIF_STRING_H
#define SCQIF_STRING_H

#include <string>
#include <cstddef>
#include "cqif.hpp"

class iscqif_string: public icqif, public std::string {
    private:
        size_t readoffset;
    public:
        void pop(void);
        char top(void);
        bool dry(void);
    public:
        iscqif_string();
        iscqif_string(const std::string& s);
    // TODO: do some overloading for content protection or auto rewinding
};

class oscqif_string: public ocqif, public std::string {
    public:
        void push(char c);
    // TODO: do some overloading for content protection or auto rewinding
};

#endif // SCQIF_STRING_H
