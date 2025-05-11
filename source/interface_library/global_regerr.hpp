#pragma once
#include <list>
#include <memory>
#include <source_location>
#include <string>
#include <utility>
#include <vector>

#if __has_include(<spdlog/spdlog.h>)
    #include <spdlog/spdlog.h>
    #define HAS_SPDLOG 1
#endif
namespace
{
    template <size_t n> struct source_location
    {
        constexpr source_location(const char (&path)[n], size_t line, size_t col)
        {
            std::copy_n(path, n, this->path);
            this->line = line;
            this->col = col;
        }
        char path[n];
        size_t line;
        size_t col;
    };
    template <size_t n> struct error_message
    {
        constexpr error_message(const char (&msg)[n]) { std::copy_n(msg, n, this->msg); }
        char msg[n];
    };

    class error_invoker
    {
    public:
        struct location
        {
            std::string path;
            size_t line;
            size_t col;
            std::string error_msg;
        };
        inline static std::vector<location> locations = { { "default", 0, 0, "error" } };
        error_invoker(const std::string& path, size_t line, size_t col, const std::string& error_msg) { locations.emplace_back(location{ path, line, col, error_msg }); }
    };

    template <typename proxyer, source_location location, error_message message> class error_proxy
    {
    public:
        inline static proxyer proxy = { location.path, location.line, location.col, message.msg };
        template <typename err_fun, typename... Args> static auto callback(err_fun& f, Args&&... args) { return f(args...); }
    };

    inline const char* error_code_info(int error_code)
    {
        if (error_code < 0 || static_cast<size_t>(error_code) >= ::error_invoker::locations.size())
            return "未知错误";
        return ::error_invoker::locations[error_code].error_msg.c_str();
    }
    inline int error_impl(const char* sz)
    {
        int index = 0;
        for (auto& [_, __, ___, message] : error_invoker::locations)
        {
            if (message == sz)
                break;
            index++;
        }
        return index;
    }

#define reg_err(msg) \
    ::error_proxy<::error_invoker, ::source_location(__FILE__, std::source_location::current().line(), std::source_location::current().column()), ::error_message(msg)>::callback(::error_impl, msg)

#if defined(HAS_SPDLOG)
    #undef HAS_SPDLOG
    #define code_err(msg, ...)                             \
        [&](int code) {                                    \
            spdlog::error(msg __VA_OPT__(, ) __VA_ARGS__); \
            return code;                                   \
        }(reg_err(msg))
    #define false_err(msg, ...)                            \
        [&]() {                                            \
            spdlog::error(msg __VA_OPT__(, ) __VA_ARGS__); \
            return false;                                  \
        }()
#else
    #define code_err(msg, ...) reg_err(msg)
    #define false_err(msg, ...) reg_err(msg)
#endif
} // namespace
