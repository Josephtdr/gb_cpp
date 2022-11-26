# find_package(SDL2 REQUIRED)

# include_directories(${PROJECT_NAME} 
#     ${SDL2_INCLUDE_DIRS}
# )
include_directories(
    ${MyProject_SOURCE_DIR}/src/inc
    # ${MyProject_SOURCE_DIR}/external/BSlogger/src
)
# link_directories(
#     ${MyProject_SOURCE_DIR}/external/BSlogger
# )

add_executable(${PROJECT_NAME} 
    main.cpp
    registers.cpp
    memoryBus.cpp
    cpu.cpp
    cpu_operations.cpp
    cpu_opcodes.cpp
    gpu.cpp
)

# target_link_libraries(${PROJECT_NAME} bslogger)

# target_link_libraries(${PROJECT_NAME} 
#     ${SDL2_LIBRARIES}
# )