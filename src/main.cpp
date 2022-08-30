#include <iostream>
#include "vlex.hpp"

using namespace std;
using namespace veriprohip;

const char* vlex_elem_type_name(vlex_elem_type_t type) {
    switch (type) {
    case SPACE:
        return "SPACE";
    case WORD:
        return "WORD";
    case NUMBER:
        return "NUMBER";
    case OPERATOR:
        return "OPERATOR";
    case TEXT:
        return "TEXT";
    case COMMENT:
        return "COMMENT";
    default:
        return "NONE";
    }
}

int main()
{
    list<vlex_elem> elems;
    vlex("t.v", elems);

    for (list<vlex_elem>::const_iterator it=elems.cbegin(); it!=elems.cend(); it++) {
        cout << vlex_elem_type_name(it->type) << ':' << *it << endl;
    }

    return 0;
}
