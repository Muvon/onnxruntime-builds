/*
 * musl has no execinfo.h and Alpine removed libexecinfo in 3.17. ONNX Runtime
 * includes the header on every non-Android POSIX target, so the build needs
 * one. These functions report an empty backtrace.
 */
#ifndef _EXECINFO_H
#define _EXECINFO_H
#include <stddef.h>
static inline int backtrace(void **buffer, int size) { (void)buffer; (void)size; return 0; }
static inline char **backtrace_symbols(void *const *buffer, int size) { (void)buffer; (void)size; return NULL; }
static inline void backtrace_symbols_fd(void *const *buffer, int size, int fd) { (void)buffer; (void)size; (void)fd; }
#endif
