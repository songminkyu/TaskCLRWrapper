/*
  Copyright 2015-2021 Adobe
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef STLAB_CONFIG_HPP
#define STLAB_CONFIG_HPP

// Configuration generated from CMake

/* #undef STLAB_USE_BOOST_CPP17_SHIMS */
#define STLAB_NO_STD_COROUTINES

#define STLAB_THREADS(X) (STLAB_THREADS_##X())
#define STLAB_THREADS_WIN32() 1
#define STLAB_THREADS_PTHREAD() 0
#define STLAB_THREADS_PTHREAD_EMSCRIPTEN() 0
#define STLAB_THREADS_PTHREAD_APPLE() 0
#define STLAB_THREADS_NONE() 0

#define STLAB_TASK_SYSTEM(X) (STLAB_TASK_SYSTEM_##X())
#define STLAB_TASK_SYSTEM_PORTABLE() 0
#define STLAB_TASK_SYSTEM_LIBDISPATCH() 0
#define STLAB_TASK_SYSTEM_EMSCRIPTEN() 0
#define STLAB_TASK_SYSTEM_WINDOWS() 1

#define STLAB_MAIN_EXECUTOR(X) (STLAB_MAIN_EXECUTOR_##X())
#define STLAB_MAIN_EXECUTOR_LIBDISPATCH() 0
#define STLAB_MAIN_EXECUTOR_EMSCRIPTEN() 0
#define STLAB_MAIN_EXECUTOR_QT() 1
#define STLAB_MAIN_EXECUTOR_NONE() 0

// Various functionality macros

#define STLAB_FEATURE_PRIVATE_OBJC_ARC() 0
#define STLAB_FEATURE(X) (STLAB_FEATURE_PRIVATE_##X())

#define STLAB_CPP_VERSION(X) (STLAB_CPP_VERSION_PRIVATE() == (X))
#define STLAB_CPP_VERSION_LESS_THAN(X) (STLAB_CPP_VERSION_PRIVATE() < (X))
#define STLAB_CPP_VERSION_AT_LEAST(X) (STLAB_CPP_VERSION_PRIVATE() >= (X))

#if __APPLE__

    #if defined(__has_feature)
        #if __has_feature(objc_arc)
            #undef STLAB_FEATURE_PRIVATE_OBJC_ARC
            #define STLAB_FEATURE_PRIVATE_OBJC_ARC() 1
        #endif
    #endif


#elif _MSC_VER

    #if _MSVC_LANG == 201103L
        #define STLAB_CPP_VERSION_PRIVATE() 11
    #elif _MSVC_LANG == 201402L
        #define STLAB_CPP_VERSION_PRIVATE() 14
    #elif _MSC_FULL_VER >= 191225830 && _MSVC_LANG == 201703L
        #define STLAB_CPP_VERSION_PRIVATE() 17
    #elif _MSVC_LANG == 202002L
        #define STLAB_CPP_VERSION_PRIVATE() 20
    #else
        #pragma message("Unknown version of C++, assuming C++20.")
        #define STLAB_CPP_VERSION_PRIVATE() 20
    #endif

#endif

#if !defined(STLAB_CPP_VERSION_PRIVATE)
    #if __cplusplus == 201103L
        #define STLAB_CPP_VERSION_PRIVATE() 11
    #elif __cplusplus == 201402L
        #define STLAB_CPP_VERSION_PRIVATE() 14
    #elif __cplusplus == 201703L
        #define STLAB_CPP_VERSION_PRIVATE() 17
	#elif __cplusplus == 202002L
        #define STLAB_CPP_VERSION_PRIVATE() 20
    #else
        //#warning Unknown version of C++, assuming C++23.
        #define STLAB_CPP_VERSION_PRIVATE() 23
    #endif
#endif

// STLAB_NODISCARD macro

#if __cplusplus < 201703L
#define STLAB_NODISCARD()
#else
#define STLAB_NODISCARD() [[nodiscard]]
#endif

#endif // STLAB_CONFIG_HPP
