#ifndef VLEX_H
#define VLEX_H

#include <string>
#include <list>
#include "cqif.hpp"

namespace veriprohip {
    typedef enum {
        NONE,
        SPACE,
        WORD,
        NUMBER,
        OPERATOR,
        TEXT,
        COMMENT,
    } vlex_elem_type_t;

    class vlex_elem: public std::string {
    public: // exported api
        vlex_elem_type_t type;
    };

    extern int vlex_derive(icqif& verilog, vlex_elem& elem);
}

#endif // VLEX_H
