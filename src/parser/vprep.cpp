#include <string>
#include <cctype>
#include <utility>
#include <map>
#include <deque>
#include <list>
#include "vlex.hpp"
#include "vprep.hpp"
#include "fcqif.hpp"
#include <windows.h>

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
                    ifstk.size()==1 && ifstk.back().dry()
                )
            )
        );
    }

    void vprep_icqif::parse(void) { // process `
        if (ifstk.back().top()!='`') {
            q.push_back(ifstk.back().top());
            ifstk.back().pop();
            return;
        }

        auto str_into_q = [&](const string& s) {
            for (string::const_iterator cit=s.cbegin(); cit!=s.cend(); cit++) {
                q.push_back(*cit);
            }
        };

        vlex_elem elem;

        vlex_derive(ifstk.back(), elem);
        str_into_q(elem);
        elem.erase(0,1); // skip initial `

        if (elem=="define") {
            do {
                vlex_derive(ifstk.back(), elem);
                str_into_q(elem);
            } while (elem.type==SPACE || elem.type==COMMENT);

            if (elem.type!=WORD) {
                throw "Illegal use of `define";
                return;
            }

            defs[elem] = "";
            map<string, string>::iterator it = defs.find(elem);

            do {
                vlex_derive(ifstk.back(), elem);
                str_into_q(elem);
            } while (elem.type==SPACE || elem.type==COMMENT);

            it->second = elem;

            do {
                vlex_derive(ifstk.back(), elem);
                str_into_q(elem);
                if (elem.find_first_of("\r\n")==string::npos) { // not reaching newline
                    it->second.append(elem);
                } else {
                    break;
                }
            } while (elem.type!=NONE);

            q.clear();
        } else if (elem=="ifdef" || elem=="ifndef") {
            unsigned ndef = (elem[2]=='n');

            do {
                vlex_derive(ifstk.back(), elem);
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
                    vlex_derive(ifstk.back(), elem);
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
                vlex_derive(ifstk.back(), elem);
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
                vlex_derive(ifstk.back(), elem);
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
                    break;
                }
            }

            if (!found) {
                throw "Cannot locate included file";
                return;
            } else {
                for (deque<ifcqif_with_path>::const_iterator cit=ifstk.cbegin(); cit!=ifstk.cend(); cit++) {
                    if (cit->get_file_path()==hdr_path) { // loop inclusion
                        throw "Loop inclusion";
                        return;
                    }
                }
            }

            ifstk.emplace_back(hdr_path);
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

        if(ifstk.size()>1 && ifstk.back().dry()) {
            ifstk.pop_back();
        }

        while (q.empty() && !ifstk.back().dry()) {
            parse(); // process `, if any
        }
    }

    void vprep_icqif::quit(void) {
        defs.clear();
        dirs.clear();
        while (ifstk.size()) {
            ifstk.pop_back();
        }
        q.clear();
    }

    void vprep_icqif::prep(const string& vfile) {
        ifstk.emplace_back(vfile);
        pop();
    }

    void vprep_icqif::prep(
        const string& vfile,
        const map<string, string>& predefs,
        const list<string>& incdirs
    ) {
        for (map<string, string>::const_iterator cit=predefs.cbegin(); cit!=predefs.cend(); cit++) {
            defs[cit->first] = cit->second;
        }

        for (list<string>::const_iterator cit=incdirs.cbegin(); cit!=incdirs.cend(); cit++) {
            dirs.emplace_back(*cit);
            if (cit->back()!='\\') { // add ending '\\' if none
                dirs.back().push_back('\\');
            }
        }

        ifstk.emplace_back(vfile);
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

    void ifcqif_with_path::open(const string& file_path) {
        char path[MAX_PATH];
        GetFullPathName(file_path.c_str(), MAX_PATH, path, NULL);
        abs_path.assign(path);
        ifcqif::open(abs_path);
    }

    const string& ifcqif_with_path::get_file_path(void) const {
        return abs_path;
    }

    ifcqif_with_path::ifcqif_with_path(const string& file_path):abs_path(), ifcqif() {
        open(file_path);
    }
}
