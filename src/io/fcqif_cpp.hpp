#ifndef FCQIF_CPP_H
#define FCQIF_CPP_H

#include <string>
#include <fstream>
#include "cqif.hpp"

class ofcqif_cpp: public ocqif {
private:
    std::ofstream of; 
public:
    void open(const std::string& file_name);
    void close(void);
public:
    void push(char c);
public:
    ofcqif_cpp(const std::string& file_name);
    ~ofcqif_cpp();
};

#endif // FCQIF_CPP_H
