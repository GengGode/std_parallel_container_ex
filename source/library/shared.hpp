#pragma once
#if defined(_WIN32)
    #ifdef std_parallel_container_ex_EXPORTS
        #define std_parallel_container_ex_API __declspec(dllexport)
    #else
        #define std_parallel_container_ex_API __declspec(dllimport)
    #endif
#else
    #ifdef std_parallel_container_ex_EXPORTS
        #define std_parallel_container_ex_API __attribute__((visibility("default")))
    #else
        #define std_parallel_container_ex_API
    #endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    std_parallel_container_ex_API const char* get_version();
    std_parallel_container_ex_API int test();

#ifdef __cplusplus
}
#endif

#undef std_parallel_container_ex_API
