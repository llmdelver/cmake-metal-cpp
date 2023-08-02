
add_executable(acl main.cpp platform/platform.cpp metal/add_wrapper.cpp)

add_subdirectory(platform)

message(${PLATFORM_COMPILE_DEFINITION})
target_compile_definitions(acl PUBLIC ${PLATFORM_COMPILE_DEFINITION})

if(APPLE)
    find_library(FOUNDATION_LIBRARY Foundation)
    find_library(METAL_LIBRARY Metal)
    find_library(QUARTZCORE_LIBRARY QuartzCore)

    set(PLATFORM_LIB ${FOUNDATION_LIBRARY} ${METAL_LIBRARY} ${QUARTZCORE_LIBRARY})
endif()

target_link_libraries(acl PRIVATE ${PLATFORM_LIB})
target_include_directories(acl SYSTEM PRIVATE ${PLATFORM_HEADER} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(APPLE)
    file(GLOB metal_files ${CMAKE_CURRENT_SOURCE_DIR}/metal/*.metal)
    message(STATUS ${metal_files})
    set(metal_air "")

    foreach(_file ${metal_files})
        get_filename_component(base_name ${_file} NAME_WLE)
        set(dst_file ${CMAKE_BINARY_DIR}/${base_name}.air)
        add_custom_command(
            OUTPUT ${dst_file}
            COMMAND xcrun -sdk macosx metal -c ${_file} -o ${dst_file}
            DEPENDS ${_file}
            VERBATIM USES_TERMINAL
        )
        list(APPEND metal_air ${dst_file})
    endforeach()

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/compute.metalar
        COMMAND xcrun -sdk macosx metal-ar -r ${CMAKE_BINARY_DIR}/compute.metalar ${metal_air}
        DEPENDS ${metal_air})

    add_custom_command(OUTPUT compute.metallib
        COMMAND xcrun -sdk macosx metallib ${CMAKE_BINARY_DIR}/compute.metalar -o compute.metallib
        DEPENDS ${CMAKE_BINARY_DIR}/compute.metalar
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

    add_custom_target(run ALL DEPENDS compute.metallib)
    install(FILES ${CMAKE_BINARY_DIR}/compute.metallib DESTINATION bin)
endif()

install(TARGETS acl DESTINATION bin)