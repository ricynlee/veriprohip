#include <string>
#include "term.hpp"
#include "trace.hpp"

using namespace std;

static trace_thresh_t th = TRACE_ALL;

void set_trace_thresh(trace_thresh_t trace_th) {
    th = trace_th;
}

void trace_info(const char* const str) {
    err_printf_color(FC_GREEN, BC_DEFAULT, "[INFO] %s\n", str);
}

void trace_warn(const char* const str) {
    err_printf_color(FC_YELLOW, BC_DEFAULT, "[WARNING] %s\n", str);
}

void trace_error(const char* const str) {
    err_printf_color(FC_RED, BC_DEFAULT, "[ERROR] %s\n", str);
}

void trace_fatal(const char* const str) {
    err_printf_color(FC_DARKRED, BC_WHITE, "[FATAL] %s\n", str);
}

/***********************************************************************************************/
void trace_info(const string str) {
    trace_info(str.c_str());
}

void trace_warn(const string str) {
    trace_warn(str.c_str());
}

void trace_error(const string str) {
    trace_error(str.c_str());
}

void trace_fatal(const string str) {
    trace_fatal(str.c_str());
}
