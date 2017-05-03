cmake_minimum_required(VERSION 3.1.0)

project(ParticleSimulator)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(UNIX OR MINGW)
    set(CMAKE_CXX_FLAGS "-g -gdwarf-2 -O3 -static-libstdc++ -Wall")
endif()

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Don't build GLFW docs" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Don't build GLFW examples" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Don't build GLFW tests" FORCE)
add_subdirectory(externals/glfw)
include_directories(externals/glfw/include)

# GLEW
set(BUILD_UTILS OFF CACHE BOOL "Don't build GLEW utility programs" FORCE)
add_subdirectory(externals/glew/build/cmake)
include_directories(externals/glew/include)

# GLM
include_directories(externals/glm)

# STB
include_directories(externals/stb)

# Headerize
add_subdirectory(externals/Headerize)
macro(ProcessWithHeaderize headervar sourcevar inputfile outdir)
    get_filename_component(basefile ${inputfile} NAME)
    set(outputfile ${outdir}/${basefile})
    set_source_files_properties(${outputfile} PROPERTIES HEADER_ONLY TRUE)
    add_custom_command(
        OUTPUT ${outputfile}.hpp ${outputfile}.cpp
        COMMAND Headerize ${inputfile} ${outputfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${inputfile}
        COMMENT "Generating headers for ${inputfile}"
    )
    list(APPEND ${headervar} ${outputfile}.hpp)
    list(APPEND ${sourcevar} ${outputfile}.cpp)
endmacro()

# Vulkan
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        find_library(Vulkan_LIBRARY vulkan-1 PATHS "$ENV{VULKAN_SDK}/Bin")
        find_program(Vulkan_VALIDATOR glslangValidator PATHS "$ENV{VULKAN_SDK}/Bin")
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        find_library(Vulkan_LIBRARY vulkan-1 PATHS "$ENV{VULKAN_SDK}/Bin32")
        find_program(Vulkan_VALIDATOR glslangValidator PATHS "$ENV{VULKAN_SDK}/Bin32")
    endif()
else()
    find_library(Vulkan_LIBRARY vulkan-1 PATHS "$ENV{VULKAN_SDK}/lib")
    find_program(Vulkan_VALIDATOR glslangValidator PATHS "$ENV{VULKAN_SDK}/bin")
endif()

find_path(VULKAN_INCLUDE_DIR NAMES vulkan/vulkan.h HINTS "$ENV{VULKAN_SDK}/include")
include_directories(${VULKAN_INCLUDE_DIR})

# Shader compilation
macro(ProcessWithValidator outvar inputfile outdir)
    get_filename_component(basefile ${inputfile} NAME)
    set(outputfile ${outdir}/${basefile})
    add_custom_command(
        OUTPUT ${outputfile}.spv
        COMMAND ${Vulkan_VALIDATOR} -V ${inputfile} -o ${outputfile}.spv
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${inputfile}
        COMMENT "Compiling shader: ${inputfile}"
    )
    list(APPEND ${outvar} ${outputfile}.spv)
endmacro()

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

# Target
set(SRCS
        src/main.cpp
        src/GraphicsPipeline.cpp
        src/Renderer.cpp
        src/Sampler.cpp
        src/Shader.cpp
        src/StorageBuffer.cpp
        src/Texture.cpp
        src/Window.cpp
    )

set(HEADERS
        src/GraphicsPipeline.hpp
        src/Renderer.hpp
        src/Sampler.hpp
        src/Shader.hpp
        src/StorageBuffer.hpp
        src/Texture.hpp
        src/Window.hpp
    )

# GLSL shaders to compile to SPIR-V.
set(SHADERS
        assets/default.vert
        assets/default.frag
    )

foreach(item ${SHADERS})
    ProcessWithValidator(VULKAN_SPV ${item} ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

set_property(SOURCE ${SRCS} APPEND PROPERTY OBJECT_DEPENDS ${VULKAN_SPV})

# Embedded sources
set(EMBEDDED_INPUT
        assets/Particle.png
        ${VULKAN_SPV}
    )

foreach(item ${EMBEDDED_INPUT})
    ProcessWithHeaderize(EMBEDDED_HEADER EMBEDDED_SOURCE ${item} ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

set_property(SOURCE ${SRCS} APPEND PROPERTY OBJECT_DEPENDS ${EMBEDDED_HEADER})

create_directory_groups(${SRCS} ${HEADERS})

add_executable(ParticleSimulator ${SRCS} ${HEADERS} ${EMBEDDED_SOURCE})
target_link_libraries(ParticleSimulator glew glfw ${Vulkan_LIBRARY})
set_property(TARGET ParticleSimulator PROPERTY CXX_STANDARD 11)
set_property(TARGET ParticleSimulator PROPERTY CXX_STANDARD_REQUIRED ON)
