#ifndef TRACE_H
#define TRACE_H

// not thread-safe

#include <string>

typedef enum {
    TRACE_TH_INFO,
    TRACE_TH_WARN,
    TRACE_TH_ERROR,
    TRACE_TH_FATAL,
    TRACE_SUPPRESS,
    TRACE_ALL = TRACE_TH_INFO,
    TRACE_NONE = TRACE_SUPPRESS,
} trace_thresh_t;

extern void set_trace_thresh(trace_thresh_t trace_th);

extern void trace_info(const std::string str);
extern void trace_info(const char* const str);

extern void trace_warn(const std::string str);
extern void trace_warn(const char* const str);

extern void trace_error(const std::string str);
extern void trace_error(const char* const str);

extern void trace_fatal(const std::string str);
extern void trace_fatal(const char* const str);

#endif // TRACE_H
