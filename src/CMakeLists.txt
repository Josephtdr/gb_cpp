find_package(SDL2 REQUIRED)

include_directories(
    ${MyProject_SOURCE_DIR}/src/inc
    ${SDL2_INCLUDE_DIRS}
    # ${MyProject_SOURCE_DIR}/external/BSlogger/src
)
# link_directories(
#     ${MyProject_SOURCE_DIR}/external/BSlogger
# )

add_executable(${PROJECT_NAME} 
    main.cpp
    emulator.cpp
    registers.cpp
    memoryBus.cpp
    cpu.cpp
    cpu_operations.cpp
    cpu_opcodes.cpp
    ppu.cpp
    apu.cpp
    channels.cpp
    bitfuncs.cpp
    platform.cpp
)

# add_compile_options ("-Odebug --debug -g")
# add_compile_options ("-O2 -Ofast")

# target_link_libraries(${PROJECT_NAME} bslogger)

target_link_libraries(${PROJECT_NAME} 
    ${SDL2_LIBRARIES}
)