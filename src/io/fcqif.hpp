#ifndef FCQIF_H
#define FCQIF_H

#include <string>
#include <fstream>
#include "cqif.hpp"

class ifcqif: public icqif {
    private:
        std::ifstream ifs;
        bool use_cin;
        char buf;
    public:
        void open(const std::string& file_path);
        void close(void);
    public:
        void pop(void);
        char top(void);
        bool dry(void);
    public:
        ifcqif(const std::string& file_path);
        ifcqif();
        ~ifcqif();
};

class ofcqif: public ocqif {
private:
    std::ofstream ofs;
    bool use_cout;
public:
    void open(const std::string& file_path);
    void close(void);
public:
    void push(char c);
public:
    ofcqif(const std::string& file_path);
    ~ofcqif();
};

#endif // FCQIF_H
