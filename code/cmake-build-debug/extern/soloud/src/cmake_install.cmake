# Install script for directory: C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/code")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/cmake-build-debug/extern/soloud/src/soloud.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/soloud" TYPE FILE FILES
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_audiosource.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_bassboostfilter.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_biquadresonantfilter.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_bus.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_c.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_dcremovalfilter.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_echofilter.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_error.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_fader.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_fft.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_fftfilter.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_file.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_file_hack_off.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_file_hack_on.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_filter.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_flangerfilter.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_internal.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_lofifilter.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_monotone.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_openmpt.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_queue.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_robotizefilter.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_sfxr.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_speech.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_tedsid.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_thread.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_vic.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_vizsn.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_wav.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_waveshaperfilter.h"
    "C:/Users/julia/Documents/GitHub/05-gl3-kalb/code/extern/soloud/src/../include/soloud_wavstream.h"
    )
endif()

