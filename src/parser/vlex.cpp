#include <cctype>
#include <list>
#include <string>
#include "vlex.hpp"
#include "cqif.hpp"
#include "fcqif.hpp"

namespace veriprohip {
    using namespace std;

    int vlex_derive(icqif& verilog, vlex_elem& elem) { // derive next elem from icqif
        int status = (-1);

        enum {
            VLEX_STATE_IDLE,
            VLEX_STATE_SPACE,
            VLEX_STATE_SPACE_END,
            VLEX_STATE_NUMBER_GOT_LEADING_DIGITS,
            VLEX_STATE_NUMBER_GOT_APOSTROPHE,
            VLEX_STATE_NUMBER_GOT_RADIX_B,
            VLEX_STATE_NUMBER_GOT_RADIX_O,
            VLEX_STATE_NUMBER_GOT_RADIX_D,
            VLEX_STATE_NUMBER_GOT_RADIX_H,
            VLEX_STATE_NUMBER_END,
            VLEX_STATE_WORD,
            VLEX_STATE_WORD_END,
            VLEX_STATE_TEXT_GOT_OPEN_QUOTATION,
            VLEX_STATE_TEXT_GOT_ESCAPE,
            VLEX_STATE_TEXT_END,
            VLEX_STATE_UNDETERMINED_SLASH,
            VLEX_STATE_COMMENT_LINE,
            VLEX_STATE_COMMENT_BLOCK,
            VLEX_STATE_COMMENT_BLOCK_GOT_POSSIBLE_FINISH,
            VLEX_STATE_COMMENT_END,
            VLEX_STATE_OPERATOR_GOT_LT,
            VLEX_STATE_OPERATOR_GOT_GT,
            VLEX_STATE_OPERATOR_GOT_AND,
            VLEX_STATE_OPERATOR_GOT_OR,
            VLEX_STATE_OPERATOR_GOT_XOR,
            VLEX_STATE_OPERATOR_GOT_EQ,
            VLEX_STATE_OPERATOR_GOT_EQS,
            VLEX_STATE_OPERATOR_END,
            VLEX_STATE_END,
            VLEX_STATE_ERROR,
        } fsm = VLEX_STATE_IDLE;

        elem.clear();

        auto take = [&]() { elem.string::push_back(verilog.top()); verilog.pop(); };

        while (true) {
            if (fsm == VLEX_STATE_IDLE) {
                if (verilog.dry()) {
                    fsm = VLEX_STATE_END;
                    continue;
                }

                if (isspace(verilog.top())) {
                    take();
                    fsm = VLEX_STATE_SPACE;
                    continue;
                } else if (isdigit(verilog.top())) {
                    take();
                    fsm = VLEX_STATE_NUMBER_GOT_LEADING_DIGITS;
                    continue;
                } else if (verilog.top()=='\'') {
                    take();
                    fsm = VLEX_STATE_NUMBER_GOT_APOSTROPHE;
                    continue;
                } else if (isalpha(verilog.top()) || verilog.top()=='_' || verilog.top()=='$' || verilog.top()=='`') {
                    take();
                    fsm = VLEX_STATE_WORD;
                    continue;
                } else if (verilog.top()=='\"') {
                    take();
                    fsm = VLEX_STATE_TEXT_GOT_OPEN_QUOTATION;
                    continue;
                } else if (verilog.top()=='/') {
                    take();
                    fsm = VLEX_STATE_UNDETERMINED_SLASH;
                    continue;
                } else if (verilog.top()=='<') { // < <= <<
                    take();
                    fsm = VLEX_STATE_OPERATOR_GOT_LT;
                    continue;
                } else if (verilog.top()=='>') { // > >= >>
                    take();
                    fsm = VLEX_STATE_OPERATOR_GOT_GT;
                    continue;
                } else if (verilog.top()=='&') { // & &&
                    take();
                    fsm = VLEX_STATE_OPERATOR_GOT_AND;
                    continue;
                } else if (verilog.top()=='|') { // | ||
                    take();
                    fsm = VLEX_STATE_OPERATOR_GOT_OR;
                    continue;
                } else if (verilog.top()=='^') { // ^ ^~
                    take();
                    fsm = VLEX_STATE_OPERATOR_GOT_XOR;
                    continue;
                } else if (verilog.top()=='!' || verilog.top()=='=') { // != == === !==
                    take();
                    fsm = VLEX_STATE_OPERATOR_GOT_EQ;
                    continue;
                } else if (ispunct(verilog.top())) { // other operators
                    take();
                    fsm = VLEX_STATE_OPERATOR_END;
                    continue;
                } else {
                    fsm = VLEX_STATE_ERROR;
                    throw "Illegal character";
                    continue;
                }
            } else if (fsm == VLEX_STATE_NUMBER_GOT_LEADING_DIGITS) {
                if (isdigit(verilog.top()) || verilog.top()=='_') {
                    // TODO: what about 10_000_?
                    take();
                    continue;
                } else if (verilog.top()=='\'') {
                    take();
                    fsm = VLEX_STATE_NUMBER_GOT_APOSTROPHE;
                    continue;
                } else if (isalnum(verilog.top())) {
                    fsm = VLEX_STATE_ERROR;
                    throw "Illegal character in inferred number";
                    continue;
                } else {
                    fsm = VLEX_STATE_NUMBER_END;
                    continue;
                }
            } else if (fsm == VLEX_STATE_NUMBER_GOT_APOSTROPHE) {
                if (tolower(verilog.top())=='b') {
                    take();
                    fsm = VLEX_STATE_NUMBER_GOT_RADIX_B;
                    continue;
                } else if (tolower(verilog.top())=='o') {
                    take();
                    fsm = VLEX_STATE_NUMBER_GOT_RADIX_O;
                    continue;
                } else if (tolower(verilog.top())=='d') {
                    take();
                    fsm = VLEX_STATE_NUMBER_GOT_RADIX_D;
                    continue;
                } else if (tolower(verilog.top())=='h') {
                    take();
                    fsm = VLEX_STATE_NUMBER_GOT_RADIX_H;
                    continue;
                } else {
                    fsm = VLEX_STATE_ERROR;
                    throw "Illegal radix mark character in inferred number";
                    continue;
                }
            } else if (fsm == VLEX_STATE_NUMBER_GOT_RADIX_B) {
                if (verilog.top()=='0' || verilog.top()=='1' || verilog.top()=='_' || tolower(verilog.top())=='x' || tolower(verilog.top())=='z') {
                    // TODO: what about 10_000_?
                    take();
                    continue;
                } else if (isalnum(verilog.top()) || verilog.top()=='\'') {
                    fsm = VLEX_STATE_ERROR;
                    throw "Illegal character in inferred binary number";
                    continue;
                } else {
                    fsm = VLEX_STATE_NUMBER_END;
                    continue;
                }
            } else if (fsm == VLEX_STATE_NUMBER_GOT_RADIX_O) {
                // TODO: not implemented
                fsm = VLEX_STATE_NUMBER_END;
                throw "No support of inferred octal number";
                continue;
            } else if (fsm == VLEX_STATE_NUMBER_GOT_RADIX_D) {
                if (isdigit(verilog.top()) || verilog.top()=='_' || tolower(verilog.top())=='x' || tolower(verilog.top())=='z') {
                    // TODO: what about 10_000_?
                    take();
                    continue;
                } else if (isalpha(verilog.top()) || verilog.top()=='\'') {
                    fsm = VLEX_STATE_ERROR;
                    throw "Illegal character in inferred decimal number";
                    continue;
                } else {
                    fsm = VLEX_STATE_NUMBER_END;
                    continue;
                }
            } else if (fsm == VLEX_STATE_NUMBER_GOT_RADIX_H) {
                if (isxdigit(verilog.top()) || verilog.top()=='_' || tolower(verilog.top())=='x' || tolower(verilog.top())=='z') {
                    // TODO: what about 10_000_?
                    take();
                    continue;
                } else if (isalpha(verilog.top()) || verilog.top()=='\'') {
                    fsm = VLEX_STATE_ERROR;
                    throw "Illegal character in inferred hexadecimal number";
                    continue;
                } else {
                    fsm = VLEX_STATE_NUMBER_END;
                    continue;
                }
            } else if (fsm == VLEX_STATE_NUMBER_END) {
                elem.type = NUMBER;
                fsm = VLEX_STATE_IDLE;
                status = 0; // mark success
                break;
            } else if (fsm == VLEX_STATE_WORD) {
                if (isalnum(verilog.top()) || verilog.top()=='_') {
                    take();
                    continue;
                } else {
                    fsm = VLEX_STATE_WORD_END;
                    continue;
                }
            } else if (fsm == VLEX_STATE_WORD_END) {
                elem.type = WORD;
                fsm = VLEX_STATE_IDLE;
                status = 0; // mark success
                break;
            } else if (fsm == VLEX_STATE_TEXT_GOT_OPEN_QUOTATION) {
                if (verilog.top()=='\\') {
                    take();
                    fsm = VLEX_STATE_TEXT_GOT_ESCAPE;
                    continue;
                } else if (verilog.top()=='\r' || verilog.top()=='\n') {
                    fsm = VLEX_STATE_ERROR;
                    throw "Line end in inferred quoted text";
                    continue;
                } else if (verilog.top()=='\"') { // closed quotation
                    take();
                    fsm = VLEX_STATE_TEXT_END;
                    continue;
                } else {
                    take();
                    continue;
                }
            } else if (fsm == VLEX_STATE_TEXT_GOT_ESCAPE) {
                if (verilog.top()=='\r' || verilog.top()=='\n') {
                    fsm = VLEX_STATE_ERROR;
                    throw "Line end in escape sequence of inferred quoted text";
                    continue;
                } else {
                    // TODO: escape sequence validity check
                    take();
                    fsm = VLEX_STATE_TEXT_GOT_OPEN_QUOTATION;
                    continue;
                }
            } else if (fsm == VLEX_STATE_TEXT_END) {
                elem.type = TEXT;
                fsm = VLEX_STATE_IDLE;
                status = 0; // mark success
                break;
            } else if (fsm == VLEX_STATE_SPACE) {
                if (isspace(verilog.top())) {
                    take();
                    continue;
                } else {
                    fsm = VLEX_STATE_SPACE_END;
                    continue;
                }
            } else if (fsm == VLEX_STATE_SPACE_END) {
                elem.type = SPACE;
                fsm = VLEX_STATE_IDLE;
                status = 0; // mark success
                break;
            } else if (fsm == VLEX_STATE_UNDETERMINED_SLASH) {
                if (verilog.top()=='/') { // line comment
                    take();
                    fsm = VLEX_STATE_COMMENT_LINE;
                    continue;
                } else if (verilog.top()=='*') { // block comment
                    take();
                    fsm = VLEX_STATE_COMMENT_BLOCK;
                    continue;
                } else { // operator division /
                    fsm = VLEX_STATE_OPERATOR_END;
                    continue;
                }
            } else if (fsm == VLEX_STATE_COMMENT_LINE) {
                if (verilog.top()=='\r' || verilog.top()=='\n') {
                    fsm = VLEX_STATE_COMMENT_END;
                    continue;
                } else {
                    take();
                    continue;
                }
            } else if (fsm == VLEX_STATE_COMMENT_BLOCK) {
                if (verilog.top()=='*') {
                    take();
                    fsm = VLEX_STATE_COMMENT_BLOCK_GOT_POSSIBLE_FINISH;
                    continue;
                } else {
                    take();
                    continue;
                }
            } else if (fsm == VLEX_STATE_COMMENT_BLOCK_GOT_POSSIBLE_FINISH) {
                if (verilog.top()=='*') {
                    take();
                    continue;
                } else if (verilog.top()=='/') {
                    take();
                    fsm = VLEX_STATE_COMMENT_END;
                    continue;
                } else { // not actual finish of block comment
                    take();
                    fsm = VLEX_STATE_COMMENT_BLOCK;
                    continue;
                }
            } else if (fsm == VLEX_STATE_COMMENT_END) {
                elem.type = COMMENT;
                fsm = VLEX_STATE_IDLE;
                status = 0; // mark success
                break;
            } else if (fsm == VLEX_STATE_OPERATOR_GOT_LT) {
                if (verilog.top()=='=') { // <=
                    take();
                    fsm = VLEX_STATE_OPERATOR_END;
                    continue;
                } else { // <
                    fsm = VLEX_STATE_OPERATOR_END;
                    continue;
                }
            } else if (fsm == VLEX_STATE_OPERATOR_GOT_GT) {
                if (verilog.top()=='=') { // >=
                    take();
                    fsm = VLEX_STATE_OPERATOR_END;
                    continue;
                } else { // >
                    fsm = VLEX_STATE_OPERATOR_END;
                    continue;
                }
            } else if (fsm == VLEX_STATE_OPERATOR_GOT_AND) {
                if (verilog.top()=='&') {
                    take();
                    fsm = VLEX_STATE_OPERATOR_END;
                    continue;
                } else {
                    fsm = VLEX_STATE_OPERATOR_END;
                    continue;
                }
            } else if (fsm == VLEX_STATE_OPERATOR_GOT_OR) {
                if (verilog.top()=='|') {
                    take();
                    fsm = VLEX_STATE_OPERATOR_END;
                    continue;
                } else {
                    fsm = VLEX_STATE_OPERATOR_END;
                    continue;
                }
            } else if (fsm == VLEX_STATE_OPERATOR_GOT_XOR) {
                if (verilog.top()=='~') {
                    take();
                    fsm = VLEX_STATE_OPERATOR_END;
                    continue;
                } else {
                    fsm = VLEX_STATE_OPERATOR_END;
                    continue;
                }
            } else if (fsm == VLEX_STATE_OPERATOR_GOT_EQ) {
                if (verilog.top()=='=') {
                    take();
                    fsm = VLEX_STATE_OPERATOR_GOT_EQS;
                    continue;
                } else {
                    fsm = VLEX_STATE_OPERATOR_END;
                    continue;
                }
            } else if (fsm == VLEX_STATE_OPERATOR_GOT_EQS) {
                if (verilog.top()=='=') {
                    take();
                    fsm = VLEX_STATE_OPERATOR_END;
                    continue;
                } else {
                    fsm = VLEX_STATE_OPERATOR_END;
                    continue;
                }
            } else if (fsm == VLEX_STATE_OPERATOR_END) {
                elem.type = OPERATOR;
                fsm = VLEX_STATE_IDLE;
                status = 0; // mark success
                break;
            } else if (fsm == VLEX_STATE_END) {
                status = 0; // mark success
                break;
            } else /* VLEX_STATE_ERROR */ {
                status = (-1);
                break;
            }
        }

        return status;
    }
}
