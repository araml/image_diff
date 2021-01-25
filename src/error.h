#ifndef ERROR_H
#define ERROR_H

#include <ostream>
#include <sstream>
#include <assert.h>
#include <iostream>

#if __ANDROID__
#include <android/log.h>
#define printf(...) __android_log_print(ANDROID_LOG_INFO, "TWS", __VA_ARGS__)
#endif

#define LOG(...) log_w(__FILE__, __LINE__, __VA_ARGS__)

#define ASSERT_EQ(val1, val2, ...) \
       log_wassert((val1) == (val2), __FILE__, __LINE__, \
                   "\n", #val1, ":", (val1), "\n", "But it should be:", (val2))

#if !defined(_WIN32) && (__cplusplus > 201703L)
template <typename head, typename... Args>
void log_t(std::ostringstream &err, const head h, const Args &... args) {
    err << h << " ";
    if constexpr (sizeof...(args) > 0) {
        log_t(err, args...);
    } else {
        fprintf(stderr, "%s\n", err.str().c_str());
    }
}

#else
template <typename head>
void log_t(std::ostringstream &err, const head h) {
    err << h << " ";
    fprintf(stderr, "%s\n", err.str().c_str());
}

template <typename head, typename... Args>
void log_t(std::ostringstream &err, const head h, const Args &... args) {
    err << h << " ";
    log_t(err, args...);
}
#endif

template <typename... Args>
void log_w(const char *file, int line, const Args &... args) {
    std::ostringstream err;
    err << file << "(" << line << ") ";
    log_t(err, args...);
}

template <typename... Args>
void log_wassert(bool cond, const char *file, int line, const Args &... args) {
    if (!cond) {
        log_w(file, line, args...);
        assert(cond);
    }
}

// this assert lets you insert a breakpoint in code.
#define ASSERT(expr)                          \
        if (expr) {}                          \
        else {                                \
                LOG(#expr);                   \
                asm (".intel_syntax   \n\t"   \
                     "int 3           \n\t"); \
        }

#define GL_ERROR_CHECK(f)                                                      \
    while (glGetError() != GL_NO_ERROR) {}                                     \
    f;                                                                         \
    if (glGetError() != GL_NO_ERROR) {                                         \
        LOG(glGetError());                                                     \
    }

#endif // ERROR_H
