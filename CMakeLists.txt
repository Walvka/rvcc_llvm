cmake_minimum_required(VERSION 3.13.4)

# We need to support C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check basic standard libraries, headers, flags
include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckLibraryExists)
include(CheckCXXCompilerFlag)

# We need to use LLVM
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  project(chilang)

  # Add LLVM and standard configurations
  find_package(LLVM REQUIRED CONFIG)
  message("Found LLVM ${LLVM_PACKAGE_VERSION}, build type ${LLVM_BUILD_TYPE}")
  list(APPEND CMAKE_MODULE_PATH ${LLVM_DIR})

  include(DetermineGCCCompatible)
  include(AddLLVM)
  include(HandleLLVMOptions)

  include_directories("${LLVM_BINARY_DIR}/include" "${LLVM_INCLUDE_DIR}")
  link_directories("${LLVM_LIBRARY_DIR}")

  # Use LLVM
  add_definitions(${LLVM_DEFINITIONS})
  include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
  llvm_map_components_to_libnames(llvm_libs Core)

  set(CHILANG_BUILT_STANDALONE 1)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed.")
endif()

set(CHILANG_VERSION_STRING "0.1")

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/include/chilang/Basic/Version.inc.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/chilang/Basic/Version.inc
)

include(AddChilang)

include_directories(BEFORE
  ${CMAKE_CURRENT_BINARY_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(LLVM_COMPILER_IS_GCC_COMPATIBLE)
  if(NOT LLVM_ENABLE_RTTI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
  endif()
  if(NOT LLVM_ENABLE_EH)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
  endif()
endif()

add_subdirectory(lib)
add_subdirectory(tools)