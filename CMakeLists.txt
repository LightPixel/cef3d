# Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

# OVERVIEW
#
# CMake is a cross-platform open-source build system that can generate project
# files in many different formats. It can be downloaded from
# http://www.cmake.org or installed via a platform package manager.
#
# CMake-generated project formats that have been tested with this CEF binary
# distribution include:
#
# Linux:      Ninja, Unix Makefiles
# Mac OS X:   Ninja, Xcode 5+
# Windows:    Ninja, Visual Studio 2010+
#
# Ninja is a cross-platform open-source tool for running fast builds using
# pre-installed platform toolchains (GNU, clang, Xcode or MSVC). It can be
# downloaded from http://martine.github.io/ninja/ or installed via a platform
# package manager.
#
# CMAKE STRUCTURE
#
# This CEF binary distribution includes the following CMake files:
#
# CMakeLists.txt              Bootstrap that sets up the CMake environment and
#                             loads the other CMake files.
# macros.cmake                Helper macros for building a generic CEF-based
#                             application.
# libcef_dll/CMakeLists.txt   Defines the libcef_dll_wrapper target.
# cefclient/CMakeLists.txt    Defines the cefclient target.
# cefsimple/CMakeLists.txt    Defines the cefsimple target.
#
# Existing CMake projects that use this binary distribution without changing the
# directory structure can include the existing "libcef_dll/CMakeLists.txt" file
# with minimal or no changes.
#
# BUILD REQUIREMENTS
#
# The below requirements must be met to build this CEF binary distribution.
#
# - CMake version 2.8.12.1 or newer.
#
# - Linux requirements:
#   Currently supported distributions include Debian Wheezy, Ubuntu Precise, and
#   related. Newer versions will likely also work but may not have been tested.
#   Required packages include:
#     build-essential
#     libgtk2.0-dev     (required by the cefclient target only)
#     libgtkglext1-dev  (required by the cefclient target only)
#
# - Mac OS X requirements:
#   Xcode 5 or newer building on Mac OS X 10.7 (Lion) or newer. The Xcode
#   command-line tools must also be installed.
#
# - Windows requirements:
#   Visual Studio 2010 or newer building on Windows XP SP3 or newer. 64-bit
#   version of Windows 7 or newer recommended.
#
# BUILD EXAMPLES
#
# The below commands will generate project files and create a Debug build of all
# CEF targets using CMake and the platform toolchain.
#
# Start by creating and entering the CMake build output directory:
# > cd path/to/cef_binary_*
# > mkdir build && cd build
#
# To perform a Linux build using a 32-bit CEF binary distribution on a 32-bit
# Linux platform or a 64-bit CEF binary distribution on a 64-bit Linux platform:
#   Using Unix Makefiles:
#     > cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
#     > make -j4 cefclient cefsimple
#
#   Using Ninja:
#     > cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug ..
#     > ninja cefclient cefsimple
#
# To perform a Mac OS X build using a 32-bit CEF binary distribution:
#   Using the Xcode IDE:
#     > cmake -G "Xcode" -DPROJECT_ARCH="i386" ..
#     Open build\cef.xcodeproj in Xcode and select Product > Build.
#
#   Using Ninja:
#     > cmake -G "Ninja" -DPROJECT_ARCH="i386" -DCMAKE_BUILD_TYPE=Debug ..
#     > ninja cefclient cefsimple
#
# To perform a Mac OS X build using a 64-bit CEF binary distribution:
#   Using the Xcode IDE:
#     > cmake -G "Xcode" -DPROJECT_ARCH="x86_64" ..
#     Open build\cef.xcodeproj in Xcode and select Product > Build.
#
#   Using Ninja:
#     > cmake -G "Ninja" -DPROJECT_ARCH="x86_64" -DCMAKE_BUILD_TYPE=Debug ..
#     > ninja cefclient cefsimple
#
# To perform a Windows build using a 32-bit CEF binary distribution:
#   Using the Visual Studio 2013 IDE:
#     > cmake -G "Visual Studio 12" ..
#     Open build\cef.sln in Visual Studio and select Build > Build Solution.
#
#   Using Ninja with Visual Studio 2013 command-line tools:
#     (this path may be different depending on your Visual Studio installation)
#     > "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\vcvars32.bat"
#     > cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug ..
#     > ninja cefclient cefsimple
#
# To perform a Windows build using a 64-bit CEF binary distribution:
#   Using the Visual Studio 2013 IDE:
#     > cmake -G "Visual Studio 12 Win64" ..
#     Open build\cef.sln in Visual Studio and select Build > Build Solution.
#
#   Using Ninja with Visual Studio 2013 command-line tools:
#     (this path may be different depending on your Visual Studio installation)
#     > "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin\amd64\vcvars64.bat"
#     > cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Debug ..
#     > ninja cefclient cefsimple

#
# Shared configuration.
#

cmake_minimum_required(VERSION 2.8.12.1)

# Only generate Debug and Release configuration types.
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# Project name.
project(cef)

# Use folders in the resulting project files.
set_property(GLOBAL PROPERTY OS_FOLDERS ON)

# Determine the platform.
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  set(OS_MACOSX 1)
  set(OS_POSIX 1)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  set(OS_LINUX 1)
  set(OS_POSIX 1)
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  set(OS_WINDOWS 1)
endif()

# Determine the project architecture.
if(NOT DEFINED PROJECT_ARCH)
  if(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PROJECT_ARCH "x86_64")
  else()
    set(PROJECT_ARCH "x86")
  endif()

  if(OS_MACOSX)
    # PROJECT_ARCH should be specified on Mac OS X.
    message(WARNING "No PROJECT_ARCH value specified, using ${PROJECT_ARCH}")
  endif()
endif()

if(NOT CMAKE_BUILD_TYPE AND
   (${CMAKE_GENERATOR} STREQUAL "Ninja" OR ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles"))
  # CMAKE_BUILD_TYPE should be specified when using Ninja or Unix Makefiles.
  set(CMAKE_BUILD_TYPE Release)
  message(WARNING "No CMAKE_BUILD_TYPE value selected, using ${CMAKE_BUILD_TYPE}")
endif()

# Include cmake macros.
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}")
include("macros")

# Source include directory.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Allow C++ programs to use stdint.h macros specified in the C99 standard that
# aren't in the C++ standard (e.g. UINT8_MAX, INT64_MIN, etc).
add_definitions(-D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS)


#
# Linux configuration.
#

if(OS_LINUX)
  # Platform-specific compiler/linker flags.
  set(CEF_LIBTYPE                 SHARED)
  set(CEF_COMPILER_FLAGS          "-fno-strict-aliasing -fPIC -fstack-protector -funwind-tables -fvisibility=hidden --param=ssp-buffer-size=4 -pipe -pthread -Wall -Werror -Wno-missing-field-initializers -Wno-unused-parameter")
  set(CEF_C_COMPILER_FLAGS        "-std=c99")
  set(CEF_CXX_COMPILER_FLAGS      "-fno-exceptions -fno-rtti -fno-threadsafe-statics -fvisibility-inlines-hidden -std=gnu++11 -Wsign-compare")
  set(CEF_COMPILER_FLAGS_DEBUG    "-O0 -g")
  set(CEF_COMPILER_FLAGS_RELEASE  "-O2 -fdata-sections -ffunction-sections -fno-ident -DNDEBUG -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2")
  set(CEF_LINKER_FLAGS            "-fPIC -pthread -Wl,--disable-new-dtags -Wl,--fatal-warnings -Wl,-rpath,. -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro")
  set(CEF_LINKER_FLAGS_RELEASE    "-Wl,-O1 -Wl,--as-needed -Wl,--gc-sections")

  include(CheckCCompilerFlag)
  include(CheckCXXCompilerFlag)

  CHECK_C_COMPILER_FLAG(-Wno-unused-local-typedefs COMPILER_SUPPORTS_NO_UNUSED_LOCAL_TYPEDEFS)
  if(COMPILER_SUPPORTS_NO_UNUSED_LOCAL_TYPEDEFS)
    set(CEF_C_COMPILER_FLAGS      "${CEF_C_COMPILER_FLAGS} -Wno-unused-local-typedefs")
  endif()

  # -Wno-literal-suffix             = Don't warn about invalid suffixes on literals
  CHECK_CXX_COMPILER_FLAG(-Wno-literal-suffix COMPILER_SUPPORTS_NO_LITERAL_SUFFIX)
  if(COMPILER_SUPPORTS_NO_LITERAL_SUFFIX)
    set(CEF_CXX_COMPILER_FLAGS    "${CEF_CXX_COMPILER_FLAGS} -Wno-literal-suffix")
  endif()

  # -Wno-narrowing                  = Don't warn about type narrowing
  CHECK_CXX_COMPILER_FLAG(-Wno-narrowing COMPILER_SUPPORTS_NO_NARROWING)
  if(COMPILER_SUPPORTS_NO_NARROWING)
    set(CEF_CXX_COMPILER_FLAGS    "${CEF_CXX_COMPILER_FLAGS} -Wno-narrowing")
  endif()

  if(PROJECT_ARCH STREQUAL "x86_64")
    # 64-bit architecture.
    set(CEF_COMPILER_FLAGS        "${CEF_COMPILER_FLAGS} -m64 -march=x86-64")
    set(CEF_LINKER_FLAGS          "${CEF_LINKER_FLAGS} -m64")
  elseif(PROJECT_ARCH STREQUAL "x86")
    # 32-bit architecture.
    set(CEF_COMPILER_FLAGS        "${CEF_COMPILER_FLAGS} -msse2 -mfpmath=sse -mmmx -m32")
    set(CEF_LINKER_FLAGS          "${CEF_LINKER_FLAGS} -m32")
  endif()

  # Allow the Large File Support (LFS) interface to replace the old interface.
  add_definitions(-D_FILE_OFFSET_BITS=64)

  # Standard libraries.
  set(CEF_STANDARD_LIBS "X11")

  # CEF directory paths.
  set(CEF_RESOURCE_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/Resources")
  set(CEF_BINARY_DIR          "${CMAKE_BINARY_DIR}")
  set(CEF_BINARY_DIR_DEBUG    "${CMAKE_CURRENT_SOURCE_DIR}/precompiled")
  set(CEF_BINARY_DIR_RELEASE  "${CMAKE_CURRENT_SOURCE_DIR}/precompiled")

  # CEF library paths.
  set(CEF_LIB_DEBUG   "${CEF_BINARY_DIR_DEBUG}/libcef.so")
  set(CEF_LIB_RELEASE "${CEF_BINARY_DIR_RELEASE}/libcef.so")

  # List of CEF binary files.
  set(CEF_BINARY_FILES
    chrome-sandbox
    libcef.so
    natives_blob.bin
    snapshot_blob.bin
    )

  # List of CEF resource files.
  set(CEF_RESOURCE_FILES
    cef.pak
    cef_100_percent.pak
    cef_200_percent.pak
    cef_extensions.pak
    devtools_resources.pak
    icudtl.dat
    locales
    )
endif()


#
# Mac OS X configuration.
#

if(OS_MACOSX)
  # Platform-specific compiler/linker flags.
  # See also SET_XCODE_TARGET_PROPERTIES in macros.cmake.
  set(CEF_LIBTYPE                 SHARED)
  set(CEF_COMPILER_FLAGS          "-fno-strict-aliasing -fstack-protector -funwind-tables -fvisibility=hidden -Wall -Wendif-labels -Werror -Wextra -Wnewline-eof -Wno-missing-field-initializers -Wno-unused-parameter")
  set(CEF_C_COMPILER_FLAGS        "-std=c99")
  set(CEF_CXX_COMPILER_FLAGS      "-fno-exceptions -fno-rtti -fno-threadsafe-statics -fobjc-call-cxx-cdtors -fvisibility-inlines-hidden -std=gnu++11 -Wno-narrowing -Wsign-compare")
  set(CEF_COMPILER_FLAGS_DEBUG    "-O0 -g")
  set(CEF_COMPILER_FLAGS_RELEASE  "-O3")
  set(CEF_LINKER_FLAGS            "-Wl,-search_paths_first -Wl,-ObjC -Wl,-pie")
  set(CEF_LINKER_FLAGS_RELEASE    "-Wl,-dead_strip")

  # Standard libraries.
  set(CEF_STANDARD_LIBS "-lpthread" "-framework Cocoa" "-framework AppKit")

  # Find the newest available base SDK.
  execute_process(COMMAND xcode-select --print-path OUTPUT_VARIABLE XCODE_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
  foreach(OS_VERSION 10.10 10.9 10.8 10.7)
    set(SDK "${XCODE_PATH}/Platforms/MacOSX.platform/Developer/SDKs/MacOSX${OS_VERSION}.sdk")
    if(NOT "${CMAKE_OSX_SYSROOT}" AND EXISTS "${SDK}" AND IS_DIRECTORY "${SDK}")
      set(CMAKE_OSX_SYSROOT ${SDK})
    endif()
  endforeach()

  # Target SDK.
  set(CEF_TARGET_SDK               "10.6")
  set(CEF_COMPILER_FLAGS           "${CEF_COMPILER_FLAGS} -mmacosx-version-min=${CEF_TARGET_SDK}")
  set(CMAKE_OSX_DEPLOYMENT_TARGET  ${CEF_TARGET_SDK})

  # Target architecture.
  if(PROJECT_ARCH STREQUAL "x86_64")
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
  else()
    set(CMAKE_OSX_ARCHITECTURES "i386")
  endif()

  # CEF directory paths.
  set(CEF_BINARY_DIR          "${CMAKE_BINARY_DIR}")
  set(CEF_BINARY_DIR_DEBUG    "${CMAKE_CURRENT_SOURCE_DIR}/precompiled")
  set(CEF_BINARY_DIR_RELEASE  "${CMAKE_CURRENT_SOURCE_DIR}/precompiled")

  # CEF library paths.
  set(CEF_LIB_DEBUG   "${CEF_BINARY_DIR_DEBUG}/Chromium Embedded Framework.framework/Chromium Embedded Framework")
  set(CEF_LIB_RELEASE "${CEF_BINARY_DIR_RELEASE}/Chromium Embedded Framework.framework/Chromium Embedded Framework")
endif()


#
# Windows configuration.
#

if(OS_WINDOWS)
  # Consumers who run into LNK4099 warnings can pass /Z7 instead (see issue #385).
  set(CEF_DEBUG_INFO_FLAG "/Zi" CACHE STRING "Optional flag specifying specific /Z flag to use")

  # Platform-specific compiler/linker flags.
  set(CEF_LIBTYPE                 STATIC)
  set(CEF_COMPILER_FLAGS          "/MP /Gy /GR- /W4 /wd\"4100\" /wd\"4127\" /wd\"4244\" /wd\"4481\" /wd\"4512\" /wd\"4701\" /wd\"4702\" /wd\"4996\" ${CEF_DEBUG_INFO_FLAG}")
  set(CEF_COMPILER_FLAGS_DEBUG    "/MDd /RTC1 /Od")
  set(CEF_COMPILER_FLAGS_RELEASE  "/MD /O2 /Ob2 /GF /D NDEBUG /D _NDEBUG")
  set(CEF_LINKER_FLAGS_DEBUG      "/DEBUG")
  set(CEF_EXE_LINKER_FLAGS        "/MANIFEST:NO")
  add_definitions(-DWIN32 -D_WIN32 -D_WINDOWS -DUNICODE -D_UNICODE -DWINVER=0x0602
                  -D_WIN32_WINNT=0x602 -DNOMINMAX -DWIN32_LEAN_AND_MEAN -D_HAS_EXCEPTIONS=0)

  # Standard libraries.
  set(CEF_STANDARD_LIBS "comctl32.lib" "rpcrt4.lib" "shlwapi.lib" "ws2_32.lib")

  # CEF directory paths.
  set(CEF_RESOURCE_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/Resources")
  set(CEF_BINARY_DIR          "${CMAKE_BINARY_DIR}")
  set(CEF_BINARY_DIR_DEBUG    "${CMAKE_CURRENT_SOURCE_DIR}/precompiled")
  set(CEF_BINARY_DIR_RELEASE  "${CMAKE_CURRENT_SOURCE_DIR}/precompiled")

  # CEF library paths.
  set(CEF_LIB_DEBUG   "${CEF_BINARY_DIR_DEBUG}/libcef.lib")
  set(CEF_LIB_RELEASE "${CEF_BINARY_DIR_RELEASE}/libcef.lib")

  # List of CEF binary files.
  set(CEF_BINARY_FILES
    d3dcompiler_43.dll
    d3dcompiler_47.dll
    libcef.dll
    libEGL.dll
    libGLESv2.dll
    natives_blob.bin
    snapshot_blob.bin
    )
  if(PROJECT_ARCH STREQUAL "x86")
    # Only used on 32-bit platforms.
    set(CEF_BINARY_FILES
      ${CEF_BINARY_FILES}
      wow_helper.exe
      )
  endif()

  # List of CEF resource files.
  set(CEF_RESOURCE_FILES
    cef.pak
    cef_100_percent.pak
    cef_200_percent.pak
    cef_extensions.pak
    devtools_resources.pak
    icudtl.dat
    locales
    )

  # Configure use of the sandbox.
  option(USE_SANDBOX "Enable or disable use of the sandbox." OFF)
  if(USE_SANDBOX AND NOT MSVC_VERSION EQUAL 1800)
    # The cef_sandbox.lib static library is currently built with VS2013. It will
    # not link successfully with other VS versions.
    set(USE_SANDBOX OFF)
  endif()

  if(USE_SANDBOX)
    # Definition required by cef_sandbox.lib.
    add_definitions(-DPSAPI_VERSION=1)
    # Definition used by apps to test if the sandbox is enabled.
    add_definitions(-DCEF_USE_SANDBOX)

    # Libraries required by cef_sandbox.lib.
    set(CEF_SANDBOX_STANDARD_LIBS "dbghelp.lib" "psapi.lib")

    # CEF sandbox library paths.
    set(CEF_SANDBOX_LIB_DEBUG "${CEF_BINARY_DIR_DEBUG}/cef_sandbox.lib")
    set(CEF_SANDBOX_LIB_RELEASE "${CEF_BINARY_DIR_RELEASE}/cef_sandbox.lib")
  endif()

  # Configure use of ATL.
  option(USE_ATL "Enable or disable use of ATL." ON)
  if(USE_ATL)
    # Determine if the Visual Studio install supports ATL.
    get_filename_component(VC_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
    get_filename_component(VC_DIR ${VC_BIN_DIR} DIRECTORY)
    if(NOT IS_DIRECTORY "${VC_DIR}/atlmfc")
      set(USE_ATL OFF)
    endif()
  endif()

  if(USE_ATL)
    # Definition used by apps to test if ATL support is enabled.
    add_definitions(-DCEF_USE_ATL)
  endif()
endif()


#
# Post-configuration actions.
#

# Merge compiler/linker flags.
set(CMAKE_C_FLAGS                     "${CEF_COMPILER_FLAGS} ${CEF_C_COMPILER_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG               "${CEF_COMPILER_FLAGS_DEBUG} ${CEF_C_COMPILER_FLAGS_DEBUG}")
set(CMAKE_C_FLAGS_RELEASE             "${CEF_COMPILER_FLAGS_RELEASE} ${CEF_C_COMPILER_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS                   "${CEF_COMPILER_FLAGS} ${CEF_CXX_COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG             "${CEF_COMPILER_FLAGS_DEBUG} ${CEF_CXX_COMPILER_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE           "${CEF_COMPILER_FLAGS_RELEASE} ${CEF_CXX_COMPILER_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS            "${CEF_LINKER_FLAGS} ${CEF_EXE_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG      "${CEF_LINKER_FLAGS_DEBUG} ${CEF_EXE_LINKER_FLAGS_DEBUG}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE    "${CEF_LINKER_FLAGS_RELEASE} ${CEF_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_SHARED_LINKER_FLAGS         "${CEF_LINKER_FLAGS} ${CEF_SHARED_LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG   "${CEF_LINKER_FLAGS_DEBUG} ${CEF_SHARED_LINKER_FLAGS_DEBUG}")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CEF_LINKER_FLAGS_RELEASE} ${CEF_SHARED_LINKER_FLAGS_RELEASE}")


#
# Include target subdirectories.
#

add_subdirectory(libcef_dll)
add_subdirectory(cef3d)
add_subdirectory(CefSubProcess)


#
# Display configuration settings.
#

message(STATUS "*** CONFIGURATION SETTINGS ***")
message(STATUS "Generator:                    ${CMAKE_GENERATOR}")
message(STATUS "Platform:                     ${CMAKE_SYSTEM_NAME}")
message(STATUS "Project architecture:         ${PROJECT_ARCH}")

if(${CMAKE_GENERATOR} STREQUAL "Ninja" OR ${CMAKE_GENERATOR} STREQUAL "Unix Makefiles")
  message(STATUS "Build type:                   ${CMAKE_BUILD_TYPE}")
endif()

if(OS_MACOSX)
  message(STATUS "Base SDK:                     ${CMAKE_OSX_SYSROOT}")
  message(STATUS "Target SDK:                   ${CEF_TARGET_SDK}")
endif()

if(OS_WINDOWS)
  message(STATUS "CEF Windows sandbox:          ${USE_SANDBOX}")
  message(STATUS "Visual Studio ATL support:    ${USE_ATL}")
endif()

set(LIBRARIES ${CEF_STANDARD_LIBS})
if(OS_WINDOWS AND USE_SANDBOX)
  set(LIBRARIES ${LIBRARIES} ${CEF_SANDBOX_STANDARD_LIBS})
endif()
message(STATUS "Standard libraries:           ${LIBRARIES}")

get_directory_property(DEFINITIONS COMPILE_DEFINITIONS)
message(STATUS "Compiler definitions:         ${DEFINITIONS}")

message(STATUS "C_FLAGS:                      ${CMAKE_C_FLAGS}")
message(STATUS "C_FLAGS_DEBUG:                ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "C_FLAGS_RELEASE:              ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "CXX_FLAGS:                    ${CMAKE_CXX_FLAGS}")
message(STATUS "CXX_FLAGS_DEBUG:              ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CXX_FLAGS_RELEASE:            ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "EXE_LINKER_FLAGS:             ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "EXE_LINKER_FLAGS_DEBUG:       ${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
message(STATUS "EXE_LINKER_FLAGS_RELEASE:     ${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
message(STATUS "SHARED_LINKER_FLAGS:          ${CMAKE_SHARED_LINKER_FLAGS}")
message(STATUS "SHARED_LINKER_FLAGS_DEBUG:    ${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
message(STATUS "SHARED_LINKER_FLAGS_RELEASE:  ${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")

if(OS_LINUX OR OS_WINDOWS)
  message(STATUS "CEF Binary files:             ${CEF_BINARY_FILES}")
  message(STATUS "CEF Resource files:           ${CEF_RESOURCE_FILES}")
endif()
