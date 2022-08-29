#ifndef VLEX_H
#define VLEX_H

#include <string>
#include <list>

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

#if defined(VLEX_HANDLING_SWITCH_SUPPORT)
    typedef enum {
        BY_FILE,
        BY_CODE,
    } vlex_handling_t;
    extern int vlex(const std::string& verilog, std::list<vlex_elem>& elems, vlex_handling_t handling=BY_FILE);
#else
    extern int vlex(const std::string& verilog, std::list<vlex_elem>& elems);
#endif
}

#endif // VLEX_H
