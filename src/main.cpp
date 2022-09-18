#include <iostream>
#include "vprep.hpp"
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
    vprep_icqif v("d:\\tmp.v");
    while (!v.dry()) {
        cout << v.top();
        v.pop();
    }
    cout.flush();
    return 0;
}
