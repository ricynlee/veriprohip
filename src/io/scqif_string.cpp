#include <string>
#include "scqif.hpp"

using namespace std;

void iscqif_string::pop(void) {
    if (readoffset<string::length()) {
        readoffset++;
    }
}

char iscqif_string::top(void) {
    if (readoffset<string::length()) {
        return string::operator[](readoffset);
    } else {
        return '\0';
    }
}

bool iscqif_string::dry(void) {
    return (bool)(readoffset>=string::length());
}

iscqif_string::iscqif_string():string() {
    readoffset = 0;
}

iscqif_string::iscqif_string(const string& s):string(s) {
    readoffset = 0;
}

void oscqif_string::push(char c) {
    // TODO: implementation
}

