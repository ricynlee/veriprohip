#ifndef SCQIF_CHARARRAY_H
#define SCQIF_CHARARRAY_H

#include "cqif.hpp"
#include <cstddef>

// chararray is subject to external modification

class iscqif_chararray: public icqif {
    private:
        const char* chararray;
        size_t readoffset;
    public:
        void pop(void);
        char top(void);
        bool dry(void);
    public:
        void rewind(void);
        void bind_chararray(const char* ptr);
    public:
        iscqif_chararray(const char* p=NULL);
};

class oscqif_chararray: public ocqif {
    private:
        char* chararray;
        size_t capacity;
        size_t writeoffset;
    public:
        void push(char c);
    public:
        void rewind(void);
        void bind_chararray(char* ptr, size_t cap);
    public:
        oscqif_chararray(char* ptr=NULL);
};

#endif // SCQIF_CHARARRAY_H
