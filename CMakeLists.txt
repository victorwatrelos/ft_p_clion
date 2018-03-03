cmake_minimum_required(VERSION 3.9)
project(ft_p C)

set(CMAKE_C_STANDARD 99)
add_executable(server
        src_server/test_path.c
        src_server/main_server.c
        src_server/cmd_pwd_serv.c
        src_server/cmd_put_serv.c
        src_server/cmd_get_serv.c
        src_server/cmd_cd_serv.c
        src_server/catch_signal.c
        src_server/auth_client.c
        src_server/cmd_quit_serv.c
        src_server/read_loop.c
        src_server/includes
        src_server/cmd_ls.c
        src_server/launch_server.c
        src_common/send_data.c
        src_common/recv_data.c
        src_common/str_utils.c
        src_common/includes
        src_common/get_args.c
        )
target_include_directories(server PRIVATE src_server/includes)
target_include_directories(server PRIVATE src_common/includes)
target_include_directories(server PRIVATE src_common/includes)
target_include_directories(server PRIVATE lib)
target_link_libraries(server ${CMAKE_SOURCE_DIR}/lib/libft_core.a)

add_executable(client
        src_client/main_client.c
        src_client/get_param.c
        src_client/catch_client_sig.c
        src_client/loop_cmd.c
        src_client/cmd_get.c
        src_client/cmd_put.c
        src_client/cmd_pwd.c
        src_client/cmd_quit.c
        src_client/cmd_cd.c
        src_client/connect_to_server.c
        src_client/get_data_from_socket.c
        src_client/cmd_ls.c
        src_common/send_data.c
        src_common/recv_data.c
        src_common/str_utils.c
        src_common/includes
        src_common/get_args.c
        )
SET_TARGET_PROPERTIES(client PROPERTIES LINKER_LANGUAGE C)
target_include_directories(client PRIVATE src_client/includes)
target_include_directories(client PRIVATE src_common/includes)
target_include_directories(client PRIVATE src_common/includes)
target_include_directories(client PRIVATE lib)
target_link_libraries(client ${CMAKE_SOURCE_DIR}/lib/libft_core.a)
