
project(test)

# executable
add_executable(tcpsrv tcpsrv.c)
add_executable(tcpclient tcpclient.c)

# link library
set(${PROJECT_NAME}_libraries
    core
)

set(${PROJECT_NAME}_names
    tcpsrv
    tcpclient
)

target_link_libraries(tcpsrv ${${PROJECT_NAME}_libraries})
target_link_libraries(tcpclient ${${PROJECT_NAME}_libraries})

# install
install (TARGETS ${${PROJECT_NAME}_names} DESTINATION bin)