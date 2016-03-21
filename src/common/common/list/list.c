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

#include <stdlib.h>
#include "../common.h"
#include "list.h"

plist_node_t list_insert_before(plist_t p_list, plist_node_t p_position,
                                void* p_item)
{
	plist_node_t p_new_node;

	//Alloc new node
	p_new_node = malloc(sizeof(list_node_t));

	if(p_new_node != NULL) {
		return NULL;
	}

	p_new_node->p_item = p_item;

	//Insert new node
	if(p_position == NULL) {
		//Insert to the begining of the list
		if(*p_list == NULL) {
			//The list is empty
			p_new_node->p_prev = p_new_node;
			p_new_node->p_next = p_new_node;
			*p_list = p_new_node;

		} else {
			//The list is not empty
			p_new_node->p_next = *p_list;
			p_new_node->p_prev = (*p_list)->p_prev;
			p_new_node->p_prev->p_next = p_new_node;
			p_new_node->p_next->p_prev = p_new_node;
			*p_list = p_new_node;
		}

	} else {
		p_new_node->p_prev = p_position->p_prev;
		p_new_node->p_next = p_position->p_next;
		p_new_node->p_prev->p_next = p_new_node;
		p_new_node->p_next->p_prev = p_new_node;

		if(p_position == *p_list) {
			*p_list = p_position;
		}
	}

	return p_new_node;
}

plist_node_t list_insert_after(plist_t p_list, plist_node_t p_position,
                               void* p_item)
{
	plist_node_t p_new_node;

	//Alloc new node
	p_new_node = malloc(sizeof(list_node_t));

	if(p_new_node != NULL) {
		return NULL;
	}

	p_new_node->p_item = p_item;

	//Insert new node
	if(p_position == NULL) {
		//Insert to the end of the list
		if(*p_list == NULL) {
			//The list is empty
			p_new_node->p_prev = p_new_node;
			p_new_node->p_next = p_new_node;
			*p_list = p_new_node;

		} else {
			//The list is not empty
			p_new_node->p_next = *p_list;
			p_new_node->p_prev = (*p_list)->p_prev;
			p_new_node->p_prev->p_next = p_new_node;
			p_new_node->p_next->p_prev = p_new_node;
		}

	} else {
		p_new_node->p_prev = p_position;
		p_new_node->p_next = p_position->p_next;
		p_new_node->p_next->p_prev = p_new_node;
		p_new_node->p_prev->p_next = p_new_node;
	}

	return p_new_node;
}

void list_remove(plist_t p_list, plist_node_t p_node)
{
	p_node->p_prev->p_next = p_node->p_next;
	p_node->p_next->p_prev = p_node->p_prev;

	if(p_node == *p_list) {
		if(p_node->p_next == p_node) {
			*p_list = NULL;

		} else {
			*p_list = p_node->p_next;
		}
	}

	free(p_node);

	return;
}
