/*********************************************************************************
 *      Copyright:  (C) 2024 linuxer<linuxer@email.com>
 *                  All rights reserved.
 *
 *       Filename:  socket.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(14/03/24)
 *         Author:  linuxer <linuxer@email.com>
 *      ChangeLog:  1, Release initial version on "14/03/24 11:16:25"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/tcp.h>
#include "project.h"

int	client_connect(int fd,struct argument *argp)
{
	struct sockaddr_in		servaddr;
	socklen_t				len = sizeof(servaddr);
	int						rv = 0;

	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(argp->port);
	inet_aton(argp->ip,&servaddr.sin_addr);

	if( (rv = connect(fd,(struct sockaddr *)&servaddr,len)) < 0 )
	{
		printf("Client connect failure:%s\n",strerror(errno));
		return -1;
	}

	return 0;
}

int server_connect(int argc,char *argv[])
{
	int					fd = 0;
	int					on = 1;
	struct sockaddr_in  servaddr;

	fd = socket(AF_INET,SOCK_STREAM,0);
	if( fd < 0 )
	{
		printf("Server create socket fd failure:%s\n",strerror(errno));
		return -1;
	}

	argp = parameter_analysis(argc,argv);
	if( !argp )
	{
		printf("Parameter analysis failure:%s\n",strerror(errno));
		return -1;
	}

	setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(argp->port);
	if( !argp->ip )
	{
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		inet_pton(AF_INET,argp->ip,&servaddr.sin_addr);
	}

	if( bind(fd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 )
	{
		printf("Server bind the TCP socket failure:%s\n",strerror(errno));
		return -1;
	}

	if( listen(fd,128) < 0 )
	{
		printf("Server listen the TCP socket failure:%s\n",strerror(errno));
		return -1;
	}

	return fd;
}

int net_status(int fd)
{
	struct tcp_info		info;
	socklen_t			len = sizeof(info);

	getsockopt(fd,IPPROTO_TCP,TCP_INFO,&info,&len);
	if( info.tcpi_state == TCP_ESTABLISHED )
	{
		printf("\n");
		return 1;
	}
	else
	{
		printf("\n");
		return 0;
	}
}


