
project(test)

# executable
add_executable(1-daytimetcpsrv 1-daytimetcpsrv.c)
add_executable(1-daytimetcpclient 1-daytimetcpclient.c)

# link library
set(${PROJECT_NAME}_libraries
    core
)

set(${PROJECT_NAME}_names
    1-daytimetcpsrv
    1-daytimetcpclient
)

target_link_libraries(1-daytimetcpsrv ${${PROJECT_NAME}_libraries})
target_link_libraries(1-daytimetcpclient ${${PROJECT_NAME}_libraries})

# install
install (TARGETS ${${PROJECT_NAME}_names} DESTINATION bin)