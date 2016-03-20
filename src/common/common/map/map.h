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
#include "../types.h"

#define	TREE_COLOR_BLACK		0
#define	TREE_COLOR_RED			1

#ifdef	DEBUG
	#define	CHK_MAP_BALANCE(x)	map_balance_chk((x))
#endif	//	DEBUG

#ifndef	DEBUG
	#define	CHK_MAP_BALANCE(x)
#endif	//!	DEBUG

typedef	struct	_rbtree_node {
	struct	_rbtree_node*	p_parent;		//Parent node
	struct	_rbtree_node*	p_left;			//Left child
	struct	_rbtree_node*	p_right;		//Right child
	unsigned long			color;			//Red or black
	char*					key;			//Key
	void*					value;			//Value
} rbtree_node_t, *prbtree_node_t, *rbtree_t, *map_t, **pmap_t;

#define	map_init(map)	((map) = NULL)
bool	map_insert(pmap_t p_map, char* key, void* value);
void*	map_get(pmap_t p_map, char* key);
bool	map_set(pmap_t p_map, char* key, char* value);
void*	map_remove(pmap_t p_map, char* key);
void	map_destroy(pmap_t p_map);
void	map_balance_chk(pmap_t p_map);
