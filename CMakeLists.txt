cmake_minimum_required(VERSION 3.13.4)
project(MangaNeko VERSION 0.0.0)

#Set C++ to 17, dragon with C++ 14 requires boost.
set(CMAKE_CXX_STANDARD 17)

#Add source files
file(GLOB_RECURSE mnSourceFiles
    "source/*.h"
    "source/*.hpp"
    "source/*.c"
    "source/*.cc"
    "source/*.cpp"
    "config/*" # Just to be able to browse config files in IDEs
)

if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/dependencies/sources")
    #Copy 7zip source code
    file(COPY libraries/bit7z/third_party/7-zip DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/dependencies/sources")
    
    #Build Linux 7zip so
    execute_process(
        COMMAND make -f ../../cmpl_gcc.mak
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/dependencies/sources/7-zip/CPP/7zip/Bundles/Format7zF"
    )

    #Copy the 7zip shared object
    file(COPY "${CMAKE_CURRENT_BINARY_DIR}/dependencies/sources/7-zip/CPP/7zip/Bundles/Format7zF/b/g/7z.so" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/dependencies/)
endif()

add_executable(MangaNeko ${mnSourceFiles})

#Add and link libraries
add_subdirectory(libraries/drogon)
add_subdirectory(libraries/bit7z)
target_link_libraries(MangaNeko PRIVATE drogon PRIVATE bit7z64)

#Specify targets output directory
set_target_properties( MangaNeko
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/debug/lib"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/debug/lib"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/debug"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/release/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/release/lib"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/release"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/bin/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/bin/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/bin"
    
)

#Copy resources

#SSL Certificates
file(COPY config DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin)

#Respect source code folder structure
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#set_property(TARGET MangaNeko PROPERTY FOLDER "MangaNeko")

#Source Code
file(GLOB_RECURSE mnSource
    "source/*.h"
    "source/*.hpp"
    "source/*.c"
    "source/*.cc"
    "source/*.cpp"
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/source" PREFIX "Source" FILES ${mnSource})

#Config files
file (GLOB mnConfig
    "config/*"
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/config/" PREFIX "Configuration Files" FILES ${mnConfig})
