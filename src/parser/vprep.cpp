#include <string>
#include <cctype>
#include <utility>
#include <map>
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
            return '\0';
        }
    }

    bool vprep_icqif::dry(void) {
        return (
            q.empty() && (
                ifstk.size()==0 || (
                    ifstk.size()==1 && ifstk.top().dry()
                )
            )
        );
    }

    void vprep_icqif::parse(void) { // process `
        if (ifstk.top().top()!='`') {
            q.push_back(ifstk.top().top());
            ifstk.top().pop();
            return;
        }

        auto str_into_q = [&](const string& s) {
            for (string::const_iterator cit=s.cbegin(); cit!=s.cend(); cit++) {
                q.push_back(*cit);
            }
        };

        vlex_elem elem;

        vlex_derive(ifstk.top(), elem);
        str_into_q(elem);
        elem.erase(0,1); // skip initial `

        if (elem=="define") {
            do {
                vlex_derive(ifstk.top(), elem);
                str_into_q(elem);
            } while (elem.type==SPACE || elem.type==COMMENT);

            if (elem.type!=WORD) {
                throw "Illegal use of `define";
                return;
            }

            defs[elem] = "";
            map<string, string>::iterator it = defs.find(elem);

            do {
                vlex_derive(ifstk.top(), elem);
                str_into_q(elem);
            } while (elem.type==SPACE || elem.type==COMMENT);

            it->second = elem;

            do {
                vlex_derive(ifstk.top(), elem);
                str_into_q(elem);
                if (elem.find_first_of("\r\n")==string::npos) { // not reaching newline
                    it->second.append(elem);
                } else {
                    break;
                }
            } while (elem.type!=NONE);

            q.clear();
        } else if (elem=="ifdef" || elem=="ifndef") {
            bool ndef = (elem[2]=='n');

            do {
                vlex_derive(ifstk.top(), elem);
                str_into_q(elem);
            } while (elem.type==SPACE || elem.type==COMMENT);

            if (elem.type!=WORD) {
                throw "Illegal use of `ifdef/`ifndef";
                return;
            }

            if (defs.count(elem)==ndef) {
                // if-expr does not hold, find matching `else/`endif
                int ifcnt = 0;
                do {
                    vlex_derive(ifstk.top(), elem);
                    str_into_q(elem);
                    if (elem=="`ifdef" || elem=="`ifndef") {
                        ifcnt++;
                    } else if (elem=="`endif") {
                        if (ifcnt) {
                            ifcnt--;
                        } else {
                            break;
                        }
                    } else if (elem=="`else") {
                        if (ifcnt==0) {
                            break;
                        }
                    }
                } while (elem.type!=NONE);
            }

            q.clear();
        } else if (elem=="else") {
            // find matching `endif
            int ifcnt = 0;
            do {
                vlex_derive(ifstk.top(), elem);
                str_into_q(elem);
                if (elem=="`ifdef" || elem=="`ifndef") {
                    ifcnt++;
                } else if (elem=="`endif") {
                    if (ifcnt) {
                        ifcnt--;
                    } else {
                        break;
                    }
                }
            } while (elem.type!=NONE);

            q.clear();
        } else if (elem=="endif") {
            q.clear();
        } else if (elem=="include") {
            do {
                vlex_derive(ifstk.top(), elem);
                str_into_q(elem);
            } while (elem.type==SPACE || elem.type==COMMENT);

            if (elem.type!=TEXT) {
                throw "Illegal use of `include";
                return;
            }

            string hdr_name(elem.substr(1u, elem.length()-2)); // strip open/closed quotes
            string& hdr_path = hdr_name;
            bool found = false;

            auto file_exists = [&](const string& path)->bool {
                return ifstream(path.c_str()).good();
            };

            for (list<string>::const_iterator cit = dirs.cbegin(); cit!=dirs.cend(); cit++) {
                if (file_exists(*cit + hdr_name)) {
                    hdr_path.assign(*cit + hdr_name);
                    found = true;
                    ifstk.emplace(hdr_path);
                }
            }

            if (!found) {
                throw "Cannot local included file";
                return;
            }

            q.clear();
        } else if (defs.count(elem)) {
            q.clear();
            str_into_q(defs[elem]);
        }
    }

    void vprep_icqif::pop(void) {
        if (!q.empty()) {
            q.pop_front();
        }

        if(ifstk.size()>1 && ifstk.top().dry()) {
            ifstk.pop();
        }

        while (q.empty() && !ifstk.top().dry()) {
            parse(); // process `, if any
        }
    }

    void vprep_icqif::quit(void) {
        defs.clear();
        dirs.clear();
        while (ifstk.size()) {
            ifstk.pop();
        }
        q.clear();
    }

    void vprep_icqif::prep(const string& vfile) {
        ifstk.emplace(vfile);
        pop();
    }

    void vprep_icqif::prep(
        const string& vfile,
        const map<string, string>& predefs,
        const list<string>& incdirs
    ) {
        for (map<string, string>::const_iterator it=predefs.cbegin(); it!=predefs.cend(); it++) {
            defs[it->first] = it->second;
        }

        for (list<string>::const_iterator it=incdirs.cbegin(); it!=incdirs.cend(); it++) {
            dirs.emplace_back(*it);
        }

        ifstk.emplace(vfile);
        pop();
    }

    vprep_icqif::vprep_icqif() {
        // nothing to do
    }

    vprep_icqif::vprep_icqif(const string& vfile) {
        prep(vfile);
    }

    vprep_icqif::vprep_icqif(
        const string& vfile,
        const map<string, string>& predefs,
        const list<string>& incdirs
    ) {
        prep(vfile, predefs, incdirs);
    }

    vprep_icqif::~vprep_icqif() {
        quit();
    }
}
