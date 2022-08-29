#ifndef VPREP_H
#define VPREP_H

#include <utility>
#include <list>
#include <string>
#include "cqif.hpp"

namespace veriprohip {
    int vprep(icqif& verilog, const std::list<std::pair<std::string, std::string> >& predefs, const std::list<std::string>& searchdirs);
}

#endif // VPREP_H
