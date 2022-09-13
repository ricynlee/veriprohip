#include <string>
#include <cctype>
#include <utility>
#include <list>
#include <stack>
#include "vlex.hpp"
#include "vprep.hpp"
#include "fcqif.hpp"

namespace veriprohip {
    using namespace std;

    char vprep_icqif::top(void) {
        if (!q.empty()) {
            return q.front();
        } else {
            return ifs.top().top();
        }
    }

    bool vprep_icqif::dry(void) {
        return ((!working) || (q.empty() && ifs.size()==1 && ifs.top().dry()));
    }

    void vprep_icqif::pop(void) {
        if (!working) {
            return;
        }

        if (!q.empty()) {
            q.pop_front();
            return;
        }

        if (ifs.top().dry()) {
            ifs.pop();
        }

        if (ifs.top().top()=='`') {
            auto s2q = [&](const string& s) {
                for (string::const_iterator it=s.cbegin(); it!=s.cend(); it++) {
                    q.push_back(*it);
                }
            };

            vlex_elem elem;

            vlex_trivial(ifs.top(), elem);
            s2q(elem);
            if (elem=="`define") {
                do {
                    vlex_trivial(ifs.top(), elem);
                    s2q(elem);
                } while (elem.type==SPACE || elem.type==COMMENT);

                if (elem.type!=WORD) {
                    defs.emplace_back(pair<string, string>(elem, ""));
                    return;
                }

                do {
                    vlex_trivial(ifs.top(), elem);
                    s2q(elem);
                } while (elem.type==SPACE || elem.type==COMMENT);

                defs.back().second = elem;

                do {
                    vlex_trivial(ifs.top(), elem);
                    s2q(elem);
                    if (elem.find_first_of("\r\n")==string::npos) { // not reaching newline
                        defs.back().second.append(elem);
                    } else {
                        break;
                    }
                } while (true);

                q.clear();
            }
        } else {
            ifs.top().pop();
        }
    }

    void vprep_icqif::quit(void) {
        working = false;
        defs.clear();
        dirs.clear();
        while (ifs.size()) {
            ifs.pop();
        }
        q.clear();
    }

    void vprep_icqif::preproc(const string& vfile) {
        ifs.emplace(vfile);
    }

    void vprep_icqif::preproc(
        const string& vfile,
        const list<pair<string, string> >& predefs,
        const list<string>& incdirs
    ) {
        ifs.emplace(vfile);

        for (list<pair<string, string> >::const_iterator it=predefs.cbegin(); it!=predefs.cend(); it++) {
            defs.emplace_back(*it);
        }

        for (list<string>::const_iterator it=incdirs.cbegin(); it!=incdirs.cend(); it++) {
            dirs.emplace_back(*it);
        }
    }

    vprep_icqif::vprep_icqif():working(false) {
        // nothing to do
    }

    vprep_icqif::vprep_icqif(const string& vfile):working(false) {
        preproc(vfile);
    }

    vprep_icqif::vprep_icqif(
        const string& vfile,
        const list<pair<string, string> >& predefs,
        const list<string>& incdirs
    ):working(false) {
        preproc(vfile, predefs, incdirs);
    }

    vprep_icqif::~vprep_icqif() {
        quit();
    }
}
