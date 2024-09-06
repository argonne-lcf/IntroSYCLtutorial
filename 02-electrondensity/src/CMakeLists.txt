# Set the minimum version of cmake
cmake_minimum_required (VERSION 3.8)

# Set the project name
project (loadWF VERSION "0.0" LANGUAGES CXX)

#if (USE_SYCL)
#    add_definitions (-DUSE_SYCL)
#    if (USE_FPGA_EMULATOR)
#        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fintelfpga")
#        add_definitions (-DFPGA_EMULATOR)
#    elseif (USE_FPGA_REPORT)
#        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fintelfpga -fsycl-link -Xshardware")
#        add_definitions (-DFPGA_EMULATOR)
#    elseif (USE_FPGA_BITSTREAM)
#        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fintelfpga -Xshardware")
#    elseif (USE_FPGA_PROFILING)
#        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fintelfpga -Xshardware -Xsprofile")
#    else () #Use CPU emulation by default
#        message("Building for CPU host emulation")
#        add_definitions (-DCPU_HOST)
#    endif ()

    #SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/opt/intel/oneapi/compiler/2024/include/sycl")
    #message(${CMAKE_CXX_FLAGS})
#endif ()


execute_process(COMMAND
  git describe --match=NeVeRmAtCh --always --abbrev=7 --dirty
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_SHA1
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

configure_file( ${PROJECT_SOURCE_DIR}/version.hpp.in
                ${PROJECT_SOURCE_DIR}/version.hpp)

set(CMAKE_CXX_COMPILER "icpx")
#find_program(CMAKE_CXX_COMPILER icpx)
#find_package(IntelSYCL REQUIRED)
#find_package(SYCL)
#find_package(IntelSYCL)
#find_package(SYCL)


if (DEBUG)
     add_definitions(-DPRINTCUBE)
endif()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsycl -Wall")

aux_source_directory (. DIR_SRCS)

include_directories (${PROJECT_SOURCE_DIR})

# Add the executable
add_executable (handleWF.x ${DIR_SRCS})
#add_executable (handleWF.x)


#target_compile_definitions(handleWF.x PRIVATE PRINTCUBE=1)

set (EXECUTABLE_OUTPUT_PATH .)
#set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

# Link the required libraries -lm (math) and -lsycl (SYCL)
target_link_libraries(handleWF.x PRIVATE m sycl)
