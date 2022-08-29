#ifndef CQIF_H
#define CQIF_H

// input/output char queue interface definition

class icqif {
    public:
        virtual void pop(void) {} // move to next char
        virtual char top(void) {} // peek current char
        virtual bool dry(void) {} // queue's now empty
    public:
        virtual ~icqif() {}
};

class ocqif {
    public:
        virtual void push(char c) {} // write char
    public:
        virtual ~ocqif() {}
};

#endif // CQIF_H
