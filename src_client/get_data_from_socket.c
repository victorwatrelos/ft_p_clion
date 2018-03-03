#include "get_data_from_socket.h"
#include "send_data.h"

static int			get_port(void)
{
    static int port = 30000;

	++port;
	return (port);
}

static int 			send_port(int sockfd, int server_data_port)
{
	char arg[128];
	char *port_str;

	if ((port_str = ft_itoa(server_data_port)) == NULL)
	{
		return (-1);
	}
	ft_strcpy(arg, "\"PORT\" \"");
	ft_strcat(arg, port_str);
	ft_strcat(arg, "\"\n");
    free(port_str);
	if (send_data(sockfd, arg, ft_strlen(arg)) == 0)
		return(-1);
	return (0);
}

static int			init_server(int sockfd, int server_socket)
{
	struct sockaddr_in  this;
	int 				server_data_port;
    int 				i;

	//TODO If fail try an other port
	i = 0;
	while (++i)
	{
		printf("Trying on port %d\n", server_data_port);
		server_data_port = get_port();
		ft_bzero(&this, sizeof(this));
		this.sin_family = AF_INET;
		this.sin_port = htons(server_data_port);
		this.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(sockfd, (struct sockaddr *)&this, sizeof(this)) >= 0)
			break;
		if (i < 4)
			continue ;
		printf("Data server could not init the connection\n");
		return (-1);
	}
	printf("Listen\n");
	if (listen(sockfd, 1) < 0)
	{
		printf("Data server could not listen\n");
		return (-1);
	}
    return (send_port(server_socket, server_data_port));
}

static char			*read_all_data_from_socket(int fd, int *size)
{
	char	*res;
	size_t	idx;
	int		ret;
	size_t	res_size;

	if ((res = malloc(BUFF_SIZE_SOCKET * sizeof(char))) == NULL)
		return (NULL);
	printf("HERE\n");
	idx = 0;
	res_size = BUFF_SIZE_SOCKET;
	while ((ret = recv(fd, res + idx, res_size - idx, 0)) > 0)
	{
		printf("loop\n");
		idx += ret;
		if (idx == res_size)
		{
			if ((res = reallocf(res, res_size + BUFF_SIZE_SOCKET)) == NULL)
				return (NULL);
			res_size += BUFF_SIZE_SOCKET;
		}
		printf("END LOOP\n");
	}
	printf("END\n");
	if (ret < 0)
	{
		free(res);
		return (NULL);
	}
	printf("SUCCESS\n");
	*size = idx;
	return (res);
}

char *get_data_from_socket(int *size, int server_socket)
{
	
	int                 sockfd_client;
	socklen_t           lg;
	struct sockaddr_in  that;
	char				*res;
	int					sockfd_server;

	if ((sockfd_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return (NULL);
	if (init_server(sockfd_server, server_socket) < 0)
		return (NULL);
    printf("Beforce accept\n");
	lg = sizeof(that);
	sockfd_client = accept(sockfd_server, (struct sockaddr *)(&that), &lg);
	printf("After accept\n");
	if (sockfd_client < 0)
	{
		printf("Fail to accept data connection\n");
		return (NULL);
	}
    printf("Accepted data connection from server\n");
	res = read_all_data_from_socket(sockfd_client, size);
	close(sockfd_client);
	close(sockfd_server);
	return (res);
}
