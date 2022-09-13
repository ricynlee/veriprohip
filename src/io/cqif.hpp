#ifndef CQIF_H
#define CQIF_H

// input/output char queue interface definition

class icqif {
    public:
        virtual void pop(void)=0; // move to next char
        virtual char top(void)=0; // peek current char
        virtual bool dry(void)=0; // queue's now empty
    public:
        virtual ~icqif() {}
};

class ocqif {
    public:
        virtual void push(char c)=0; // write char
    public:
        virtual ~ocqif() {}
};

#endif // CQIF_H
