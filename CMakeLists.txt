cmake_minimum_required(VERSION 3.15)
include(FetchContent)
set(CMAKE_CXX_STANDARD 17)

if(DEFINED ENV{VCPKG_ROOT})
  set(vcpkg_SOURCE_DIR "$ENV{VCPKG_ROOT}")
  set(CMAKE_TOOLCHAIN_FILE
      "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
else()
  message("vcpkg not detected, now installing it")
  FetchContent_Declare(vcpkg GIT_REPOSITORY https://github.com/microsoft/vcpkg)
  FetchContent_GetProperties(vcpkg)
  if(NOT vcpkg_POPULATED)
    FetchContent_Populate(vcpkg)
    set(ENV{VCPKG_ROOT} "${vcpkg_SOURCE_DIR}")
    execute_process(COMMAND "${vcpkg_SOURCE_DIR}/bootstrap-vcpkg.sh")
    set(CMAKE_TOOLCHAIN_FILE
        "${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
  endif()
endif()

project(CSV_FEED LANGUAGES CXX)

execute_process(COMMAND "$ENV{VCPKG_ROOT}/vcpkg" install fmt date)

find_package(date CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(test-1 test-1.cc)
target_link_libraries(test-1 PRIVATE fmt::fmt-header-only date::date)
