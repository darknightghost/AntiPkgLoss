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

#include <sys/socket.h>
#include "../../common/common.h"

#define	PKG_TYPE_CONNECT	0
#define	PKG_TYPE_REPLY		1
#define	PKG_TYPE_RESEND		2
#define	PKG_TYPE_HEARTBEAT	3
#define	PKG_TYPE_DATA		4

typedef	struct	_connection {
	u32						id;
	u32						pkg_id;
	u8*						buf;
	struct	sockaddr_in		dest_addr;
} connection_t, *pconnection_t;

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
	}
} pkg_t, *ppkg_t;
#pragma pack(pop)

bool			connection_init();
pconnection_t	create_connection(char* address);
pconnection_t	listen_connection(u16 port);
void			close_connection(pconnection_t p_conn);
size_t			send_data();
size_t			recv_data();
