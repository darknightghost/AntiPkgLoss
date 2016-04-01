/*
	  Copyright 2016,暗夜幽灵 <darknightghost.cn@gmail.com>

	  This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	  This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	  You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef	WINDOWS
	#include <winsock2.h>
#endif	//WINDOWS

#ifndef	WINDOWS
	#include <sys/socket.h>
#endif	//!	WINDOWS

#include <string.h>
#include <stdlib.h>
#include "../common/common.h"
#include "connection.h"

#ifndef	WINDOWS
	typedef	int				SOCKET;
	#define	closesocket		close
	#define	INVALID_SOCKET	(-1)
	#define	SOCKET_ERROR	(-1)
#endif	//!	WINDOWS

static	map_t	address_map;
static	SOCKET	conn_socket;

static	bool	checksum(ppkg_t p_pkg);
static	void*	recv_thread(void* arg);

bool connection_init(u32 port)
{
	#ifdef	WINDOWS
	WSADATA wsaData;
	#endif	//WINDOWS
	struct sockaddr_in addr;

	#ifdef	WINDOWS

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return false;
	}

	#endif	//WINDOWS

	//Set address
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	//Create socket
	conn_socket = socket(AF_INET, SOCK_DGRAM, 0);

	if(conn_socket == INVALID_SOCKET) {
		#ifdef	WINDOWS
		WSACleanup();
		#endif	//WINDOWS
		return false;
	}

	//Bind socket
	if(bind(conn_socket, (struct sockaddr*)&addr, sizeof(addr))
	   == SOCKET_ERROR) {
		#ifdef	WINDOWS
		WSACleanup();
		#endif	//WINDOWS
		closesocket(conn_socket);
		return false;
	}

	//Create threads
	return true;
}

void connection_destroy()
{
	closesocket(conn_socket);
	#ifdef	WINDOWS
	WSACleanup();
	#endif	//WINDOWS
	return;
}

pconnection_t	create_connection(char* address);
pconnection_t	listen_connection(u16 port);
void			close_connection(pconnection_t p_conn);
size_t			send_data();
size_t			recv_data();

bool checksum(ppkg_t p_pkg)
{
}

void* recv_thread(void* arg)
{
	UNREFERENCED_PARAMETER(arg);
	return NULL;
}
