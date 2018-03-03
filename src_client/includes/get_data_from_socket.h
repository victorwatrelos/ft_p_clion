#ifndef GET_DATA_FROM_SOCKET_H
# define GET_DATA_FROM_SOCKET_H

# include <arpa/inet.h>
# include <sys/socket.h>
# include <libft.h>

# define BUFF_SIZE_SOCKET 4096

char *get_data_from_socket(int *size, int server_socket);

#endif
