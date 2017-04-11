cmake_minimum_required(VERSION 3.1.0)

project(ParticleSimulator)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "-g -gdwarf-2 -O3 -static-libstdc++ -Wall")
endif()

# GLEW
set(BUILD_UTILS OFF CACHE BOOL "Don't build GLEW utility programs" FORCE)
add_subdirectory(externals/glewcmake/build/cmake)
include_directories(externals/glewcmake/include)

# GLM
include_directories(externals/glm)

# STB
include_directories(externals/stb)

# Vulkan
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        find_library(Vulkan_LIBRARY vulkan-1 PATHS "$ENV{VULKAN_SDK}/Bin")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        find_library(Vulkan_LIBRARY vulkan-1 PATHS "$ENV{VULKAN_SDK}/Bin32")
    endif()
else()
    find_library(Vulkan_LIBRARY vulkan-1 PATHS "$ENV{VULKAN_SDK}/lib")
endif()

find_path(VULKAN_INCLUDE_DIR NAMES vulkan/vulkan.h HINTS "$ENV{VULKAN_SDK}/include")
include_directories(${VULKAN_INCLUDE_DIR})

# This function should be passed a list of all files in a target. It will automatically generate
# file groups following the directory hierarchy, so that the layout of the files in IDEs matches the
# one in the filesystem.
function(create_directory_groups)
    # Place any files that aren't in the source list in a separate group so that they don't get in
    # the way.
    source_group("Other Files" REGULAR_EXPRESSION ".")

    foreach(file_name ${ARGV})
        get_filename_component(dir_name "${file_name}" PATH)
        # Group names use '\' as a separator even though the entire rest of CMake uses '/'...
        string(REPLACE "/" "\\" group_name "${dir_name}")
        source_group("${group_name}" FILES "${file_name}")
    endforeach()
endfunction()

# Copy assets
set(ASSETS

    )

add_custom_target(AssetTarget ALL)

function(CopyAsset inputfile outdir)
    set(outputfile ${outdir}/${inputfile})
    
    add_custom_command(
        TARGET AssetTarget
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${inputfile} ${outputfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Copying ${inputfile}"
    )
endfunction()

foreach(item ${ASSETS})
    CopyAsset(${item} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endforeach()

set(SRCS
        src/main.cpp
    )

set(HEADERS
    )

create_directory_groups(${SRCS} ${HEADERS})

add_executable(Testbench ${SRCS} ${HEADERS})
target_link_libraries(Testbench glew ${Vulkan_LIBRARY})
set_property(TARGET Testbench PROPERTY CXX_STANDARD 11)
set_property(TARGET Testbench PROPERTY CXX_STANDARD_REQUIRED ON)
