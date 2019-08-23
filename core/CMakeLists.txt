
project(core)

# add source file
set(${PROJECT_NAME}_sources
    uns_error.c
    uns_socket.c
)

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_sources})