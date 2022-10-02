#include <string>
#include "term.hpp"
#include "trace.hpp"

using namespace std;

static trace_thresh_t th = TRACE_ALL;

void set_trace_thresh(trace_thresh_t trace_th) {
    th = trace_th;
}

void trace_info(const string str) {
    printf_color(FC_GREEN, BC_DEFAULT, "%s\n", str.c_str());
}

void trace_info(const char* const str) {
    printf_color(FC_GREEN, BC_DEFAULT, "%s\n", str);
}

void trace_warn(const string str) {
    printf_color(FC_YELLOW, BC_DEFAULT, "%s\n", str.c_str());
}

void trace_warn(const char* const str) {
    printf_color(FC_YELLOW, BC_DEFAULT, "%s\n", str);
}

void trace_error(const string str) {
    printf_color(FC_RED, BC_DEFAULT, "%s\n", str.c_str());
}

void trace_error(const char* const str) {
    printf_color(FC_RED, BC_DEFAULT, "%s\n", str);
}

void trace_fatal(const string str) {
    printf_color(FC_DARKRED, BC_WHITE, "%s\n", str.c_str());
}

void trace_fatal(const char* const str) {
    printf_color(FC_DARKRED, BC_WHITE, "%s\n", str);
}
