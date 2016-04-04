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

static	map_t			address_map;
static	pthread_mutex_t	adress_map_lock;
static	list_t			conn_list;
static	pthread_mutex_t	conn_list_lock;
static	SOCKET			conn_socket;
static	bool			runflag = false;
static	size_t			pkg_size;
static	size_t			max_pkg_size;
static	u32				max_retry;
static	u32				heartbeat;

static	pthread_t		recv_thread;
static	pthread_t		heartbeat_thread;

static	void	set_checksum(ppkg_t p_pkg);
static	bool	checksum(ppkg_t p_pkg);
static	void	send_reply(struct sockaddr_in* p_addr,
                           ppkg_t p_pkg,
                           bool resend);

static	void*	recv_thread_func(void* arg);
static	void*	heartbeat_thread_func(void* arg);

static	void	reply_recv(struct sockaddr_in* addr, char* addr_str,
                           ppkg_t p_pkg);
static	void	resend_recv(struct sockaddr_in* addr, char* addr_str,
                            ppkg_t p_pkg);
static	void	accept_recv(struct sockaddr_in* addr, char* addr_str,
                            ppkg_t p_pkg);
static	void	connect_recv(struct sockaddr_in* addr, char* addr_str,
                             ppkg_t p_pkg);
static	void	data_recv(struct sockaddr_in* addr, char* addr_str,
                          ppkg_t p_pkg);
static	void	heartbeat_recv(struct sockaddr_in* addr, char* addr_str,
                               ppkg_t p_pkg);

bool connection_init(u32 port, size_t _pkg_size,
                     size_t _max_pkg_size, u32 _heartbeat, u32 _max_retry)
{
	void* retval;

	#ifdef	WINDOWS
	WSADATA wsaData;
	#endif	//WINDOWS
	struct sockaddr_in addr;

	#ifdef	WINDOWS

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return false;
	}

	#endif	//WINDOWS

	if(runflag) {
		return true;
	}

	map_init(address_map);
	list_init(conn_list);
	address_map_lock = PTHREAD_MUTEX_INITIALIZER;
	conn_list_lock = PTHREAD_MUTEX_INITIALIZER;
	pkg_size = _pkg_size;
	max_pkg_size = _max_pkg_size;
	heartbeat = _heartbeat;
	max_retry = _max_retry;

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
		closesocket(conn_socket);
		#ifdef	WINDOWS
		WSACleanup();
		#endif	//WINDOWS
		return false;
	}

	//Create threads
	runflag = true;

	//Receive thread
	if(pthread_create(&recv_thread, NULL, recv_thread_func, NULL) != 0) {
		runflag = false;
		closesocket(conn_socket);
		#ifdef	WINDOWS
		WSACleanup();
		#endif	//WINDOWS
		return false;
	}

	//Heartbeat thread
	if(pthread_create(&heartbeat_thread, NULL, heartbeat_thread_func,
	                  NULL) != 0) {
		runflag = false;
		closesocket(conn_socket);
		pthread_join(recv_thread, &retval);
		#ifdef	WINDOWS
		WSACleanup();
		#endif	//WINDOWS
		return false;
	}

	return true;
}

void connection_destroy()
{
	void* retval;

	runflag = false;
	closesocket(conn_socket);
	pthread_join(recv_thread, &retval);
	pthread_join(heartbeat_thread, &retval);
	#ifdef	WINDOWS
	WSACleanup();
	#endif	//WINDOWS
	return;
}

pconnection_t	create_connection(char* address);
pconnection_t	listen_connection();
void			close_connection(pconnection_t p_conn);
size_t			send_data();
size_t			recv_data();

void set_checksum(ppkg_t p_pkg)
{
	u8 sum;
	size_t counted;

	p_pkg->data.head.checksum = 0;

	for(counted = 0; counted < p_pkg->data.head.size; counted++) {
		sum += p_pkg->buf[counted];
	}

	p_pkg->data.head.checksum = 0 - sum;

	return;
}

bool checksum(ppkg_t p_pkg)
{
	u8 sum;
	size_t counted;

	for(counted = 0; counted < p_pkg->data.head.size; counted++) {
		sum += p_pkg->buf[counted];
	}

	if(sum == 0) {
		return true;

	} else {
		return false;
	}
}

void* recv_thread_func(void* arg)
{
	u8* buf;
	ppkg_t p_pkg;
	struct sockaddr_in addr;
	char addr_buf[32];
	pconnection_t p_conn;

	p_pkg = (ppkg_t)buf;
	buf = malloc(max_pkg_size);

	while(runflag) {
		//Receive package
		if(recvfrom(conn_socket, buf, max_pkg_size, 0
		            , (struct sockaddr*)&addr, sizeof(addr)) <= 0) {
			continue;
		}

		//Dispatch package
		//Get address
		sprintf(addr_buf, "%s:%hu", inet_ntoa(addr.sin_addr), addr.sin_port);

		if(p_pkg->data.head.type == PKG_TYPE_REPLY) {
			reply_recv(&addr, addr_buf, p_pkg);

		} else if(p_pkg->head.type == PKG_TYPE_RESEND) {
			resend_recv(&addr, addr_buf, p_pkg);

		} else {
			//Checksum
			if(!checksum(p_pkg)) {
				send_reply(&addr, p_pkg, true);
			}

			switch(p_pkg->data.head.type) {
				case PKG_TYPE_CONNECT:
					connect_recv(&addr, addr_buf, p_pkg);
					break;

				case PKG_TYPE_ACCEPT:
					accept_recv(&addr, addr_buf, p_pkg);
					break;

				case PKG_TYPE_HEARTBEAT:
					heartbeat_recv(&addr, addr_buf, p_pkg);
					break;

				case PKG_TYPE_DATA:
					data_recv(&addr, addr_buf, p_pkg);
					break;
			}
		}
	}

	free(buf);
	UNREFERENCED_PARAMETER(arg);
	return NULL;
}

void reply_recv(struct sockaddr_in* addr, char* addr_str,
                ppkg_t p_pkg);
void resend_recv(struct sockaddr_in* addr, char* addr_str,
                 ppkg_t p_pkg);
void accept_recv(struct sockaddr_in* addr, char* addr_str,
                 ppkg_t p_pkg);
void connect_recv(struct sockaddr_in* addr, char* addr_str,
                  ppkg_t p_pkg);
void data_recv(struct sockaddr_in* addr, char* addr_str,
               ppkg_t p_pkg);
void heartbeat_recv(struct sockaddr_in* addr, char* addr_str,
                    ppkg_t p_pkg);

