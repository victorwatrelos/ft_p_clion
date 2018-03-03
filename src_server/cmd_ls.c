#include <arpa/inet.h>
#include "cmd_ls_serv.h"
#include "get_args.h"

static int	run_cmd(char *cmd, char **args, int output_fd)
{
	int				pid;
	int				status;
	struct rusage	usage;

	if ((pid = fork()) < 0)
		return (0);
	else if (pid == 0)
	{
        dup2(output_fd, 1);
		dup2(output_fd, 2);
		execv(cmd, args);
		printf("Execv fail for cmd: %s\n", cmd);
		exit(1);
	}
	else
		wait4(pid, &status, 0, &usage);
	return (1);
}

static int	get_client_port(int fd)
{
	char	buff[4096];
	char	**args;
	int		nb_args;
	int		port;

	buff[4095] = '\0';
	printf("GET_CLIENT_PORT\n");
	if (recv_cmd(fd, buff, 4095) < 0)
		return (-1);
    printf("Received buff: %s\n", buff);
	if (get_args(buff, &args) < 2)
		return (-1);
	printf("After");
	if (ft_strcmp("PORT", args[0]) != 0)
		return (-1);
	printf("THEN");
	if ((port = ft_atoi(args[1])) <= 1024 || port > 65536)
		return (-1);
	printf("FINISH   %d\n", port);
	return port;
}

static int			init_client(int sockfd, int port, struct in_addr *ip_addr)
{
	struct sockaddr_in  dest;
	struct hostent		*server;

	ft_memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = ip_addr->s_addr;
	dest.sin_port = htons(port);
	dest.sin_family = AF_INET;
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(ip_addr->s_addr), str, INET_ADDRSTRLEN);
	printf("Client address IP: %s\n", str); // prints "192.0.2.33"

	if (connect(sockfd, (struct sockaddr *)&dest, sizeof(struct sockaddr_in)) < 0)
		return (0);
	printf("Socket to client data server SUCCESS\n");
	fflush(0);
	return (1);
}

static int	get_client_fd(int fd, t_serv_fs *serv_fs)
{
	int			port;
	int			sockfd;

	printf("Trying to get client port\n");
	if ((port = get_client_port(fd)) < 0)
		return (-1);
	printf("Port received: %d\n", port);
	fflush(0);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("Socket creation fail\n");
		return (0);
	}
	if (!init_client(sockfd, port, serv_fs->client_addr))
	{
		printf("Socket initialization fail\n");
		return (0);
	}
	printf("Creating socket to client for data transmission on port %d: SUCCESS", port);
	fflush(0);
	return (sockfd);
}


int			cmd_ls_serv(int fd, t_serv_fs *serv_fs, char **args, int nb_args)
{
	char			*path;
	t_string		resp;
	int				client_fd;

	if (nb_args < 2)
		return (0);
	printf("verifying ls path: %s\n", args[nb_args - 1]);
	if (!test_ls_path(args[nb_args - 1]))
	{
		printf("Path invalid\n");
		return (0);
	}
	if ((client_fd = get_client_fd(fd, serv_fs)) < 0)
	{
		printf("Error receiving client data socket\n");
		return (0);
	}
    run_cmd("/bin/ls", args, client_fd);
    close(client_fd);
	return (1);
}
