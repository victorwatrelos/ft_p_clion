#include "cmd_ls.h"
#include "get_data_from_socket.h"

static int	get_response(int sockfd, t_param *param)
{
	int 	size;
	void    *data;

	if ((data = get_data_from_socket(&size, sockfd)) == NULL)
	{
		printf("Get data from socket fail\n");
		return (-1);
	}
	//TODO Make a display response function
	write(1, data, size);
	write(1, "\n", 1);
    return (0);
}

int			cmd_ls(int sockfd, char *line, uint32_t cmd, t_param *param)
{
	char			*cmd_to_serv;
	char			*cmd_args[2];

	printf("LS line: %s\n", line);
	(void)cmd;
	if (!get_param(line, cmd_args + 1))
		cmd_args[1] = ".";
	cmd_args[0] = CMD_LS;
	if ((cmd_to_serv = create_str_from_args(cmd_args, 2)) == NULL)
	{
		ft_putstr("Malloc fail\n");
		exit(1);
	}
	printf("Cmd: |%s|\n", cmd_to_serv);
	if (!send_data(sockfd, cmd_to_serv, ft_strlen(cmd_to_serv)))
	{
		param->to_deco = 1;
		printf(SEND_CMD_FAIL "\n");
		free(cmd_to_serv);
		return (0);
	}
    printf("LS get response\n");
	if (get_response(sockfd, param) < 0)
		return (0);
	return (1);
}
