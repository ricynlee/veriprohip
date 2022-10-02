#ifndef VPREP_H
#define VPREP_H

#include <map>
#include <list>
#include <string>
#include <deque>
#include "cqif.hpp"
#include "fcqif.hpp"

namespace veriprohip {
    class ifcqif_with_path: public ifcqif {
    private:
        std::string abs_path;
    public:
        void open(const std::string& file_path);
        ifcqif_with_path(const std::string& file_path);
        const std::string& get_file_path(void) const;
    };

    class vprep_icqif: public icqif {
    private:
        std::map<std::string, std::string> defs;
        std::list<std::string> dirs;
        std::deque<ifcqif_with_path> ifstk;
        std::deque<char> q;
    public:
        void prep( // prepare for preprocessing
            const std::string& vfile,
            const std::map<std::string, std::string>& predefs,
            const std::list<std::string>& incdirs
        );
        void prep(const std::string& vfile);
        void parse(void); // process `
        void quit(void);
    public:
        vprep_icqif(
            const std::string& vfile,
            const std::map<std::string, std::string>& predefs,
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
