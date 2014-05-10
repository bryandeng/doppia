# This is a CMake build file, for more information consult:
# http://en.wikipedia.org/wiki/CMake
# and
# http://www.cmake.org/Wiki/CMake
# http://www.cmake.org/cmake/help/syntax.html
# http://www.cmake.org/Wiki/CMake_Useful_Variables
# http://www.cmake.org/cmake/help/cmake-2-8-docs.html

# This file is intended to be included by other cmake files (see src/applications/*/CMakeLists.txt)

# ----------------------------------------------------------------------

site_name(HOSTNAME)

# Use "gcc -v -Q -march=native -O3 test.c -o test" to see which options the compiler actualy uses
# using -march=native will include all sse options available on the given machine (-msse -msse2 -msse3 -mssse3 -msse4.1 -msse4.2, etc...)
# also using -march=native will imply -mtune=native
# Thus the optimization flags below should work great on all machines
# (O3 is already added by CMAKE_CXX_FLAGS_RELEASE)
set(OPT_CXX_FLAGS "-fopenmp -ffast-math -funroll-loops -march=native")
#set(OPT_CXX_FLAGS "-fopenmp -ffast-math -funroll-loops -march=native -freciprocal-math -funsafe-math-optimizations -fassociative-math -ffinite-math-only -fcx-limited-range")  # cheap -Ofast copy
#set(OPT_CXX_FLAGS "-ffast-math -funroll-loops -march=native") # disabled OpenMp, just for testing
#set(OPT_CXX_FLAGS "-fopenmp -ffast-math -funroll-loops") # disable native compilation, so we can profile with valgrind/callgrind

# enable link time optimization
# http://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
#set(OPT_CXX_FLAGS "${OPT_CXX_FLAGS} -flto")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -flto")
#set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -flto")

#option(CUDA_HOST_SHARED_FLAGS OFF) # cuda/nvcc uses a separate set of options than gcc

# ----------------------------------------------------------------------
# Boyuan Deng's machine: Dell Precision M4600 "Caspar"
# ----------------------------------------------------------------------
if(${HOSTNAME} STREQUAL  "Caspar")
  message(STATUS "Using Boyuan Deng's Dell Precision M4600 optimization options")

  set(CMAKE_CXX_COMPILER "/usr/bin/g++-4.7")
  set(CMAKE_C_COMPILER "/usr/bin/gcc-4.7")

  # add local compiled opencv trunk in the pkg-config paths
  #set(PKG_CONFIG_PATH ${PKG_CONFIG_PATH}:/usr/local/lib/pkgconfig)
  set(PKG_CONFIG_PATH /usr/local/lib/pkgconfig)

  set(opencv_INCLUDE_DIRS "/usr/local/include/")
  set(opencv_LIBRARY_DIRS "/usr/local/lib/")

  option(USE_GPU "Should the GPU be used ?" TRUE)
  set(CUDA_BUILD_CUBIN OFF)
  set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} -arch=sm_21)

  # faster malloc, and a good profiler via http://google-perftools.googlecode.com
  set(google_perftools_LIBS tcmalloc profiler)
endif ()


# ----------------------------------------------------------------------
# enable compilation for shared libraries
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} -Xcompiler -fpic)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  # enable cuda debug information, to use with cuda-dbg
  set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} -g -G)
else()
# FIXME disabled only for testing
  set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} -O3 --use_fast_math) # speed up host and device code
endif()

# ----------------------------------------------------------------------
# set default compilation flags and default build

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g") # add debug information, even in release mode
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -DNDEBUG -DBOOST_DISABLE_ASSERTS ${OPT_CXX_FLAGS}")
#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

set(opencv_LIBRARIES
    opencv_core opencv_imgproc opencv_highgui opencv_ml
    opencv_video opencv_features2d
    opencv_calib3d
    #opencv_objdetect opencv_contrib
    opencv_legacy opencv_flann opencv_gpu
   )

if(USE_GPU)
  add_definitions(-DUSE_GPU)
endif(USE_GPU)


# set default cmake build type (None Debug Release RelWithDebInfo MinSizeRel)
if( NOT CMAKE_BUILD_TYPE )
   set( CMAKE_BUILD_TYPE "Release" )
endif()

# ----------------------------------------------------------------------
