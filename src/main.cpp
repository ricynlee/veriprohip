#include <iostream>
#include "vprep.hpp"
#include <conio.h>

#include "trace.hpp"

using namespace std;
using namespace veriprohip;

//const char* vlex_elem_type_name(vlex_elem_type_t type) {
//    switch (type) {
//    case SPACE:
//        return "SPACE";
//    case WORD:
//        return "WORD";
//    case NUMBER:
//        return "NUMBER";
//    case OPERATOR:
//        return "OPERATOR";
//    case TEXT:
//        return "TEXT";
//    case COMMENT:
//        return "COMMENT";
//    default:
//        return "NONE";
//    }
//}

int main()
{
    map<string, string> predefs;
    list<string> incdirs(1, "d:\\");

    vprep_icqif v("d:\\tmp.v", predefs, incdirs);
    while (!v.dry()) {
        cout << v.top();
        v.pop();
    }
    cout.flush();

    getch(); // wait for input
    return 0;
}
