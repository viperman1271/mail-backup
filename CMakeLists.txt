cmake_minimum_required(VERSION 3.10)

project(mail-backup)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
foreach(configuration_type IN ITEMS Debug Release)
    foreach(architecture IN ITEMS x86_64 x86)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${configuration_type} ${CMAKE_BINARY_DIR}/bin/${configuration_type}/${architecture})
    endforeach()
endforeach()

find_package(CURL REQUIRED)

add_executable(mail-backup main.cpp imap.cpp)
target_include_directories(mail-backup PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(mail-backup PRIVATE CURL::libcurl)
