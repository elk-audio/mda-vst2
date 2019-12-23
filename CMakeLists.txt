cmake_minimum_required(VERSION 3.10)
project(mda-vst2)

#############
#  Options  #
#############
set(VST2_SDK_PATH "${PROJECT_SOURCE_DIR}/vstsdk2.4" CACHE STRING "Location of VST 2.x SDK")

#########################
#  Include Directories  #
#########################

set(VST2_INCLUDE_DIRS
    "${VST2_SDK_PATH}" 
    "${VST2_SDK_PATH}/pluginterfaces/vst2.x/"
    "${VST2_SDK_PATH}/public.sdk/source/vst2.x/"
)

##########################
#  Preprocessor Defines  #
##########################

if (UNIX)
    add_definitions( -D__cdecl= )
endif()

###########################
#  Compiler/Linker Flags  #
###########################

# Global flags
set(PROJECT_CXX_FLAGS "-Wall -Wextra -Wno-psabi -std=c++17 -fno-rtti")

# Build optimization flags
set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG -O3 -ffast-math -ftree-vectorize")
set(CMAKE_CXX_FLAGS_RELEASE "${PROJECT_CXX_FLAGS} -DNDEBUG -O3 -ffast-math -ftree-vectorize")
set(CMAKE_CXX_FLAGS_DEBUG   "${PROJECT_CXX_FLAGS} -DDEBUG -O0 -g")


##################
#  VsT SDK       #
##################

set(VST2_SDK_PLUGIN_SOURCES
    "${VST2_SDK_PATH}/public.sdk/source/vst2.x/audioeffectx.cpp"
    "${VST2_SDK_PATH}/public.sdk/source/vst2.x/audioeffect.cpp"
    "${VST2_SDK_PATH}/public.sdk/source/vst2.x/vstplugmain.cpp"
)

set(VST_PLUGINS_DIR ${PROJECT_SOURCE_DIR}/plugins)

function (add_vst2_plugin_target target)
    set(TARGET_SOURCES
        "${VST_PLUGINS_DIR}/${target}.cpp"
        "${VST2_SDK_PLUGIN_SOURCES}"
    )
    message(${TARGET_SOURCES})
    add_library(${target} MODULE ${TARGET_SOURCES})
    target_include_directories(${target} PRIVATE ${VST2_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/${VST_PLUGINS_DIR})
    set_target_properties(${target} PROPERTIES
        # Suppress VST SDK warnings
        COMPILE_FLAGS "-Wno-unused -Wno-multichar -Wno-write-strings -Wno-narrowing"
        # Remove leading 'lib' from generated .so file
        PREFIX ""
    )
endfunction()


###################################
#   Project sub-directories       #
###################################

add_subdirectory(${VST_PLUGINS_DIR})
