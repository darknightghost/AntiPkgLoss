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
#include <assert.h>
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

#ifdef	DEBUG
	#define	CHECK_TREE(map)	tree_check((map))
#endif
#ifndef	DEBUG
	#define	CHECK_TREE(map)
#endif

static	bool	left_rotate(pmap_t p_map, prbtree_node_t p_node);
static	bool	right_rotate(pmap_t p_map, prbtree_node_t p_node);
static	void	remove_rebalance(pmap_t p_map, prbtree_node_t p_node);
static	void	tree_check(prbtree_node_t p_node);
static	void	check_node_balance(prbtree_node_t p_node,
                                   int* p_black_num, int* p_max_black_num,
                                   bool* p_end);


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
	UNREFERENCED_PARAMER(remove_rebalance);
	return ret;
}

void map_destroy(pmap_t p_map)
{
	if((*p_map)->p_left != NULL) {
		map_destroy(&((*p_map)->p_left));
	}

	if((*p_map)->p_right != NULL) {
		map_destroy(&((*p_map)->p_right));
	}

	free((*p_map)->key);
	free(*p_map);
	*p_map = NULL;
	return;
}

void map_balance_chk(pmap_t p_map)
{
	int black_num;
	int max_black_num;
	bool end;

	black_num = 0;
	max_black_num = 0;
	end = false;

	check_node_balance(*p_map, &black_num, &max_black_num, &end);
	return;
}

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

	} else if(p_node == p_y->p_parent->p_left) {
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

	} else if(p_node == p_y->p_parent->p_left) {
		p_y->p_parent->p_left = p_y;

	} else {
		p_y->p_parent->p_right = p_y;
	}

	CHECK_TREE(p_y);
	return true;
}

void tree_check(prbtree_node_t p_node)
{
	if(p_node->p_left != NULL) {
		assert(p_node->p_left->p_parent == p_node);
	}

	if(p_node->p_right != NULL) {
		assert(p_node->p_right->p_parent == p_node);
	}
}

void remove_rebalance(pmap_t p_map, prbtree_node_t p_node)
{
	prbtree_node_t p_sibling;
	prbtree_node_t p_parent;

	while(p_node->color == TREE_COLOR_BLACK && p_node->p_parent != NULL) {
		p_parent = p_node->p_parent;

		if(p_node == p_parent->p_left) {
			p_sibling = p_parent->p_right;

			if(p_sibling->color == TREE_COLOR_RED) {
				//Case 1:sibling node is red
				p_parent->color = TREE_COLOR_RED;
				p_sibling->color = TREE_COLOR_BLACK;
				left_rotate(p_map, p_parent);
				p_sibling = p_parent->p_right;
			}

			CHECK_TREE(*p_map);

			if(color(p_sibling->p_left) == TREE_COLOR_BLACK
			   && color(p_sibling->p_right) == TREE_COLOR_BLACK) {
				//Case 2:sibling is black,Its childres are black
				p_sibling->color = TREE_COLOR_RED;
				p_node = p_parent;
				CHECK_TREE(*p_map);
				continue;
			}

			if(color(p_sibling->p_right) == TREE_COLOR_BLACK) {
				//Case 3:right child of sibling is black,lefi is red
				p_sibling->p_left->color = TREE_COLOR_BLACK;
				p_sibling->color = TREE_COLOR_RED;
				right_rotate(p_map, p_sibling);
				p_sibling = p_parent->p_right;
			}

			CHECK_TREE(*p_map);
			//Case 4:right child of sibling is red
			p_sibling->p_right->color = TREE_COLOR_BLACK;
			p_sibling->color = p_parent->color;
			p_parent->color = TREE_COLOR_BLACK;
			CHECK_TREE(*p_map);
			left_rotate(p_map, p_parent);
			CHECK_TREE(*p_map);
			break;

		} else {
			p_sibling = p_parent->p_left;

			if(p_sibling->color == TREE_COLOR_RED) {
				//Case 1:sibling node is red
				p_parent->color = TREE_COLOR_RED;
				p_sibling->color = TREE_COLOR_BLACK;
				right_rotate(p_map, p_parent);
				p_sibling = p_parent->p_left;
				CHECK_TREE(*p_map);
			}

			if(color(p_sibling->p_right) == TREE_COLOR_BLACK
			   && color(p_sibling->p_left) == TREE_COLOR_BLACK) {
				//Case 2:sibling is black,Its childres are black
				p_sibling->color = TREE_COLOR_RED;
				p_node = p_parent;
				CHECK_TREE(*p_map);
				continue;
			}

			if(color(p_sibling->p_left) == TREE_COLOR_BLACK) {
				//Case 3:left child of sibling is black,lefi is red
				p_sibling->p_right->color = TREE_COLOR_BLACK;
				p_sibling->color = TREE_COLOR_RED;
				left_rotate(p_map, p_sibling);
				p_sibling = p_parent->p_left;
				CHECK_TREE(*p_map);
			}

			//Case 4:left child of sibling is red
			p_sibling->p_left->color = TREE_COLOR_BLACK;
			p_sibling->color = p_parent->color;
			p_parent->color = TREE_COLOR_BLACK;
			CHECK_TREE(*p_map);
			right_rotate(p_map, p_parent);
			CHECK_TREE(*p_map);
			break;
		}
	}
}

void check_node_balance(prbtree_node_t p_node,
                        int* p_black_num, int* p_max_black_num,
                        bool * p_end)
{
	if(p_node->color == TREE_COLOR_BLACK) {
		(*p_black_num)++;
	}

	if(p_node->p_parent == NULL) {
		assert(p_node->color == TREE_COLOR_BLACK);
	}

	if(p_node->p_left == NULL) {
		if(*p_end == false) {
			*p_end = true;
			*p_max_black_num = *p_black_num;

		} else {
			assert(*p_max_black_num == *p_black_num);
		}

	} else {
		if(p_node->color == TREE_COLOR_RED) {
			assert(p_node->p_left->color == TREE_COLOR_BLACK);
		}

		assert(p_node->p_left->p_parent == p_node);
		check_node_balance(p_node->p_left,
		                   p_black_num, p_max_black_num,
		                   p_end);
	}

	if(p_node->p_right == NULL) {
		if(*p_end == false) {
			*p_end = true;
			*p_max_black_num = *p_black_num;

		} else {
			assert(*p_max_black_num == *p_black_num);
		}

	} else {
		if(p_node->color == TREE_COLOR_RED) {
			assert(p_node->p_right->color == TREE_COLOR_BLACK);
		}

		assert(p_node->p_right->p_parent == p_node);
		check_node_balance(p_node->p_right,
		                   p_black_num, p_max_black_num,
		                   p_end);
	}

	if(p_node->color == TREE_COLOR_BLACK) {
		(*p_black_num)--;
	}

	return;
}


