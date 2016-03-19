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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common.h"
#include "map.h"

#define	grand_parent(n) ((n)->p_parent->p_parent)

#define	uncle(n) { \
		prbtree_node_t __tmp_ret; \
		if((n)->p_parent == grand_parent(n)->p_left) { \
			__tmp_ret = grand_parent(n)->p_right; \
		} else { \
			__tmp_ret = grand_parent(n)->p_left; \
		} \
		__tmp_ret; \
	}

#define sibling(n) { \
		prbtree_node_t __tmp_ret; \
		if((n)->p_parent->p_left == (n)) { \
			__tmp_ret = (n)->p_parent->p_right; \
		} else { \
			__tmp_ret = (n)->p_parent->p_left; \
		} \
		__tmp_ret; \
	}

#define color(x) ((x) == NULL || (x)->color == TREE_COLOR_BLACK \
                  ? TREE_COLOR_BLACK \
                  : TREE_COLOR_RED)

static	bool	left_rotate(pmap_t p_map, prbtree_node_t p_node);
static	bool	right_rotate(pmap_t p_map, prbtree_node_t p_node);


bool map_insert(pmap_t p_map, char* key, void* value)
{
	prbtree_node_t p_new_node;
	prbtree_node_t p_insert_node;
	prbtree_node_t p_z;
	prbtree_node_t p_y;
	int cmp_result;

	//Allocate new node
	p_new_node = malloc(sizeof(rbtree_node_t));

	if(p_new_node == NULL) {
		return false;
	}

	p_new_node->key = malloc(strlen(key) + 1);

	if(p_new_node->key == NULL) {
		free(p_new_node);
		return false;
	}

	strcpy(p_new_node->key, key);
	p_new_node->value = value;
	p_new_node->p_left = NULL;
	p_new_node->p_right = NULL;

	//Instert new node
	if(*p_map == NULL) {
		*p_map = p_new_node;
		p_new_node->p_parent = NULL;
		p_new_node->color = TREE_COLOR_BLACK;
		return true;

	} else {
		//Find where to insert & insert
		p_insert_node = *p_map;

		while(true) {
			cmp_result = strcmp(key, p_insert_node->key);

			if(cmp_result < 0) {
				if(p_insert_node->p_left == NULL) {
					p_insert_node->p_left = p_new_node;
					p_new_node->p_parent = p_insert_node;
					break;
				}

				p_insert_node = p_insert_node->p_left;

			} else if(cmp_result == 0) {
				return false;

			} else {
				if(p_insert_node->p_right == NULL) {
					p_insert_node->p_right = p_new_node;
					p_new_node->p_parent = p_insert_node;
					break;
				}

				p_insert_node = p_insert_node->p_right;

			}
		}

		p_new_node->color = TREE_COLOR_RED;

		//Fix tree
		p_z = p_new_node;

		while(p_z->p_parent != NULL
		      && p_z->p_parent->color == TREE_COLOR_RED) {
			if(p_z->p_parent == grand_parent(p_z)->p_left) {
				p_y = grand_parent(p_z)->p_right;

				if(p_y != NULL && p_y->color == TREE_COLOR_RED) {
					p_z->p_parent->color = TREE_COLOR_BLACK;
					p_y->color = TREE_COLOR_BLACK;
					grand_parent(p_z)->color = TREE_COLOR_RED;
					p_z = grand_parent(p_z);

				} else {
					if(p_z == p_z->p_parent->p_right) {
						p_z = p_z->p_parent;
						left_rotate(p_map, p_z);

					}
				}

				if(p_z->p_parent != NULL && grand_parent(p_z) != NULL) {
					p_z->p_parent->color = TREE_COLOR_BLACK;
					grand_parent(p_z)->color = TREE_COLOR_RED;
					right_rotate(p_map, grand_parent(p_z));
				}

			} else {
				p_y = grand_parent(p_z)->p_left;

				if(p_y != NULL && p_y->color == TREE_COLOR_RED) {
					p_z->p_parent->color = TREE_COLOR_BLACK;
					p_y->color = TREE_COLOR_BLACK;
					grand_parent(p_z)->color = TREE_COLOR_RED;
					p_z = grand_parent(p_z);

				} else {
					if(p_z == p_z->p_parent->p_left) {
						p_z = p_z->p_parent;
						right_rotate(p_map, p_z);

					}
				}

				if(p_z->p_parent != NULL && grand_parent(p_z) != NULL) {
					p_z->p_parent->color = TREE_COLOR_BLACK;
					grand_parent(p_z)->color = TREE_COLOR_RED;
					left_rotate(p_map, grand_parent(p_z));
				}

			}
		}

		(*p_map)->color = TREE_COLOR_BLACK;
	}

	return true;
}

void* map_get(pmap_t p_map, char* key)
{
	prbtree_node_t p_node;
	int cmp_result;
	p_node = *p_map;

	while(p_node != NULL) {
		cmp_result = strcmp(key, p_node->key);

		if(cmp_result == 0) {
			return p_node->value;

		} else if(cmp_result < 0) {
			p_node = p_node->p_left;

		} else {
			p_node = p_node->p_right;
		}
	}

	return NULL;
}

bool map_set(pmap_t p_map, char* key, char* value)
{
	prbtree_node_t p_node;
	int cmp_result;

	p_node = *p_map;

	while(p_node != NULL) {
		cmp_result = strcmp(key, p_node->key);

		if(cmp_result == 0) {
			p_node->value = value;
			return true;

		} else if(cmp_result < 0) {
			p_node = p_node->p_left;

		} else {
			p_node = p_node->p_right;
		}
	}

	return false;
}

void* map_remove(pmap_t p_map, char* key)
{
	prbtree_node_t p_node;
	prbtree_node_t p_x;
	prbtree_node_t p_y;
	prbtree_node_t p_w;
	int cmp_result;
	void* ret;

	//Search for node
	p_node = *p_map;

	while(true) {
		if(p_node == NULL) {
			return NULL;
		}

		cmp_result = strcmp(key, p_node->key);

		if(cmp_result == 0) {
			break;

		} else if(cmp_result < 0) {
			p_node = p_node->p_left;

		} else {
			p_node = p_node->p_right;
		}
	}

	ret = p_node->value;
	free(p_node->key);

	//Remove node
	if(p_node->p_left == NULL || p_node->p_right == NULL) {
		p_y = p_node;

	} else {
		p_y = p_node->p_left;

		while(p_y->p_right != NULL) {
			p_y = p_y->p_right;
		}
	}

	if(p_y->p_left != NULL) {
		p_x = p_y->p_left;

	} else if(p_y->p_right != NULL) {
		p_x = p_y->p_right;

	}

	if(p_x != NULL) {
		p_x->p_parent = p_y->p_parent;
	}

	if(p_y->p_parent == NULL) {
		*p_map = p_x;

	} else if(p_y == p_y->p_parent->p_left) {
		p_y->p_parent->p_left = p_x;

	} else {
		p_y->p_parent->p_right = p_x;
	}

	if(p_y != p_node) {
		p_node->key = p_y->key;
		p_node->value = p_y->value;
	}

	if(p_y->color == TREE_COLOR_BLACK && p_x == NULL) {
		p_x = p_y->p_parent;
		/*if(p_y->p_parent->p_left != NULL) {
			p_x = p_y->p_parent->p_left;

		} else {
			p_x = p_y->p_parent->p_right;
		}*/

	} else if(p_y->color == TREE_COLOR_BLACK) {

		//Fix the tree
		while(p_x != *p_map && p_x->color == TREE_COLOR_BLACK) {
			if(p_x == p_x->p_parent->p_left) {
				p_w = p_x->p_parent->p_right;

				if(p_w->color == TREE_COLOR_RED) {
					p_w->color = TREE_COLOR_BLACK;
					p_x->p_parent->color = TREE_COLOR_RED;
					left_rotate(p_map, p_x->p_parent);
					p_w = p_x->p_parent->p_right;
				}

				if(p_w->p_left->color == TREE_COLOR_BLACK
				   && p_w->p_right->color == TREE_COLOR_BLACK) {
					p_w->color == TREE_COLOR_RED;
					p_x = p_x->p_parent;

				} else if(p_w->p_right->color == TREE_COLOR_BLACK) {
					p_w->p_left->color = TREE_COLOR_BLACK;
					p_w->color = TREE_COLOR_RED;
					right_rotate(p_map, p_w);
					p_w = p_x->p_parent->p_right;
				}

				p_w->color = p_x->p_parent->color;
				p_x->p_parent->color = TREE_COLOR_BLACK;
				p_w->p_right->color = TREE_COLOR_BLACK;
				left_rotate(p_map, p_x->p_parent);
				p_x = *p_map;

			} else {
				p_w = p_x->p_parent->p_left;

				if(p_w->color == TREE_COLOR_RED) {
					p_w->color = TREE_COLOR_BLACK;
					p_x->p_parent->color = TREE_COLOR_RED;
					right_rotate(p_map, p_x->p_parent);
					p_w = p_x->p_parent->p_left;
				}

				if(p_w->p_right->color == TREE_COLOR_BLACK
				   && p_w->p_left->color == TREE_COLOR_BLACK) {
					p_w->color == TREE_COLOR_RED;
					p_x = p_x->p_parent;

				} else if(p_w->p_left->color == TREE_COLOR_BLACK) {
					p_w->p_right->color = TREE_COLOR_BLACK;
					p_w->color = TREE_COLOR_RED;
					left_rotate(p_map, p_w);
					p_w = p_x->p_parent->p_left;
				}

				p_w->color = p_x->p_parent->color;
				p_x->p_parent->color = TREE_COLOR_BLACK;
				p_w->p_left->color = TREE_COLOR_BLACK;
				right_rotate(p_map, p_x->p_parent);
				p_x = *p_map;
			}
		}

	}

	free(p_y);

	return ret;
}

void	map_destroy(pmap_t p_map);

bool left_rotate(pmap_t p_map, prbtree_node_t p_node)
{
	prbtree_node_t p_temp;
	prbtree_node_t p_y;

	if(p_node->p_right == NULL) {
		return false;
	}

	p_y = p_node->p_right;
	p_node->p_right = p_y->p_left;

	if(p_node->p_right != NULL) {
		p_node->p_right->p_parent = p_node;
	}

	p_y->p_parent = p_node;
	p_y->p_left = p_node;

	p_temp = p_node->p_parent;
	p_node->p_parent = p_y;
	p_y->p_parent = p_temp;

	if(p_y->p_parent == NULL) {
		*p_map = p_y;

	} else if(p_node = p_y->p_parent->p_left) {
		p_y->p_parent->p_left = p_y;

	} else {
		p_y->p_parent->p_right = p_y;
	}

	return true;
}

bool right_rotate(pmap_t p_map, prbtree_node_t p_node)
{
	prbtree_node_t p_temp;
	prbtree_node_t p_y;

	if(p_node->p_left == NULL) {
		return false;
	}

	p_y = p_node->p_left;
	p_node->p_left = p_y->p_right;

	if(p_node->p_left != NULL) {
		p_node->p_left->p_parent = p_node;
	}

	p_y->p_parent = p_node;
	p_y->p_right = p_node;

	p_temp = p_node->p_parent;
	p_node->p_parent = p_y;
	p_y->p_parent = p_temp;

	if(p_y->p_parent == NULL) {
		*p_map = p_y;

	} else if(p_node = p_y->p_parent->p_left) {
		p_y->p_parent->p_left = p_y;

	} else {
		p_y->p_parent->p_right = p_y;
	}

	return true;
}
