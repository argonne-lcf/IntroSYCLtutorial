cmake_minimum_required(VERSION 3.23.5...3.27)

project(
        single_source_shortest_pathsssp
        VERSION 0.1
        DESCRIPTION "sssp"
        LANGUAGES CXX
)

# UnoAPI:CMakeLists-language-settings:begin
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# UnoAPI:CMakeLists-language-settings:end

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# UnoAPI:CMakeLists-fetchcontent:begin
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        8.1.1
)
FetchContent_MakeAvailable(fmt)
# UnoAPI:CMakeLists-fetchcontent:end

# Lots of compilation errors without this setting
option(SPDLOG_FMT_EXTERNAL "Use external fmt library instead of bundled" ON)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.14.1
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
        scnlib
        GIT_REPOSITORY https://github.com/eliaskosunen/scnlib.git
        GIT_TAG        v3.0.1
)
FetchContent_MakeAvailable(scnlib)

FetchContent_Declare(
        cli11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG        v2.4.2
)
FetchContent_MakeAvailable(cli11)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(sssp)
