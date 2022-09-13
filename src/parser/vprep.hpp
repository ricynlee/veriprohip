#ifndef VPREP_H
#define VPREP_H

#include <utility>
#include <list>
#include <string>
#include <stack>
#include <deque>
#include "cqif.hpp"
#include "fcqif.hpp"

namespace veriprohip {
    class vprep_icqif: public icqif {
    private:
        std::list<std::pair<std::string, std::string> > defs;
        std::list<std::string> dirs;
        std::stack<ifcqif> ifs;
        std::deque<char> q;
        bool working;
    public:
        void preproc(
            const std::string& vfile,
            const std::list<std::pair<std::string, std::string> >& predefs,
            const std::list<std::string>& incdirs
        );
        void preproc(const std::string& vfile);
        void quit(void);
    public:
        vprep_icqif(
            const std::string& vfile,
            const std::list<std::pair<std::string, std::string> >& predefs,
            const std::list<std::string>& incdirs
        );
        vprep_icqif(const std::string& vfile);
        vprep_icqif();
        ~vprep_icqif();
    public:
        void pop(void);
        char top(void);
        bool dry(void);
    };
}

#endif // VPREP_H
