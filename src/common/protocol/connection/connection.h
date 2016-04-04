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

#pragma once

#ifdef	WINDOWS
	#include <winsock2.h>
	#include "../pthread/pthread.h"
#endif	//WINDOWS

#ifndef	WINDOWS
	#include <sys/socket.h>
	#include <pthread.h>
#endif	//!	WINDOWS

#include "../../common/common.h"

#define	PKG_TYPE_CONNECT	0
#define	PKG_TYPE_ACCEPT		1
#define	PKG_TYPE_REPLY		2
#define	PKG_TYPE_RESEND		3
#define	PKG_TYPE_HEARTBEAT	4
#define	PKG_TYPE_DATA		5

#pragma pack(push)
#pragma pack(1)
typedef	struct	_pkg_head {
	u32		type;
	u32		id;
	u32		size;
	u32		checksum;
} pkg_head_t, *ppkg_head_t;

typedef	union	_pkg {
	u8			buf[];
	struct {
		pkg_head_t	head;
		u8			data[0];
	} data;
} pkg_t, *ppkg_t;
#pragma pack(pop)

typedef	struct	_send_info {
	u32		pkg_id;
	u64		send_time;
	bool	is_replied;
	ppkg_t	p_pkg;
} send_info_t, psend_info_t;

typedef	struct	_connection {
	char					address[32];
	plist_node				p_node;
	u32						pkg_id;
	bool					is_dead;
	struct	sockaddr_in		dest_addr;
	size_t					max_pkg_size;
	u64						last_heartbeat;
	u32						pkg_count;
	u32						retry;
	list_t					send_info_list;
	pthread_mutex_t			send_info_list_lock;
	list_t					reply_list;
	pthread_mutex_t			reply_list_lock;
} connection_t, *pconnection_t;

bool			connection_init(u32 port, size_t pkg_size, size_t max_pkg_size,
                                u32 heartbeat, u32 max_retry);
void			connection_destroy();
pconnection_t	create_connection(char* address);
pconnection_t	listen_connection();
void			close_connection(pconnection_t p_conn);
size_t			send_data();
size_t			recv_data();
