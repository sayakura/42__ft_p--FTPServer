/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qpeng <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 12:30:05 by qpeng             #+#    #+#             */
/*   Updated: 2019/02/26 12:30:06 by qpeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include "common.h"

int
	send_file(int server_sock, int file_size, int fd)
{
	char	*data;
	int		b_sent;

	b_sent = 0;
	data = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	while ((b_sent != file_size) &&\
		(b_sent += send(server_sock, data, file_size, 0)) && (b_sent > 0))
	{
		printf("%s» Sending file....%d / %d%s\r", YELLOW, b_sent,\
												file_size, RESET);
		fflush(stdout);
	}
	munmap(data, file_size);
	if (b_sent == file_size)
		printf("\n%s» Successfully sent %d bytes.%s\n", GREEN, file_size,\
		RESET);
	return (b_sent == file_size ? 1 : 0);
}

int
	receive_file(int client_socket, int file_size, int received_file_fd)
{
	int		remain_data;
	int		len;
	char	buffer[BUFSIZ];

	remain_data = file_size;
	while (remain_data > 0 && \
		((len = recv(client_socket, buffer, BUFSIZ, 0)) > 0))
	{
		remain_data -= (write(received_file_fd, buffer, len));
		printf("%s» Receiving file....%d / %d%s\r", YELLOW,\
			file_size - remain_data,\
			file_size, RESET);
		fflush(stdout);
	}
	close(received_file_fd);
	if (!remain_data)
		printf("\n%s» Successfully received %d bytes.%s\n", GREEN,\
													file_size, RESET);
	return (remain_data == 0 ? 1 : 0);
}

char
	get_char(void)
{
	char buff[1];

	read(0, buff, 1);
	return (buff[0]);
}

char
	*get_input(void)
{
	int			i;
	static char	buf[101];

	i = 0;
	ft_bzero(buf, sizeof(buf));
	write(1, "> ", 2);
	while (i <= 100 && ((buf[i++] = get_char()) != '\n'))
		;
	if (i >= 100)
	{
		ft_bzero(buf, sizeof(buf));
		printf("%s» Can't read more than 100 char characters.%s\n", RED, RESET);
		get_input();
	}
	buf[i - 1] = '\0';
	return (buf);
}

int
	ip_version(const char *src)
{
	char buf[16];

	if (inet_pton(AF_INET, src, buf))
		return (4);
	else if (inet_pton(AF_INET6, src, buf))
		return (6);
	return (-1);
}
