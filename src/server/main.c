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
#include <string.h>
#include "common/common/common.h"

bool check(prbtree_node_t p_node)
{
	if(p_node->p_parent == NULL && p_node->color != TREE_COLOR_BLACK) {
		return false;
	}

	if(p_node->color == TREE_COLOR_RED) {
		if(p_node->p_left != NULL) {
			if(p_node->p_right == NULL) {
				return false;
			}

			if(p_node->p_left->color == TREE_COLOR_RED) {
				return false;
			}

			if(!check(p_node->p_left)) {
				return false;
			}

			if(strcmp(p_node->p_left->key, p_node->key) >= 0) {
				return false;
			}
		}

		if(p_node->p_right != NULL) {
			if(p_node->p_left == NULL) {
				return  false;
			}

			if(p_node->p_right->color == TREE_COLOR_RED) {
				return false;
			}

			if(!check(p_node->p_right)) {
				return false;
			}

			if(strcmp(p_node->p_right->key, p_node->key) < 0) {
				return false;
			}
		}
	}

	if(p_node->color == TREE_COLOR_BLACK) {
		if(p_node->p_left == NULL && p_node->p_right != NULL) {
			if(p_node->p_right->color != TREE_COLOR_RED
			   || p_node->p_right->p_left != NULL
			   || p_node->p_right->p_right != NULL) {
				return false;
			}
		}

		if(p_node->p_right == NULL && p_node->p_left != NULL) {
			if(p_node->p_left->color != TREE_COLOR_RED
			   || p_node->p_left->p_left != NULL
			   || p_node->p_left->p_right != NULL) {
				return false;
			}
		}

	}

	return true;
}

int main(int argc, char* argv)
{
	map_t map = NULL;
	map_insert(&map , "qqqq", "asdasdasda");
	map_insert(&map , "www", "asdasdasda");
	map_insert(&map , "wwwwefwefa", "asdasdasda");
	map_insert(&map , "asdadc", "a54da654d");
	map_insert(&map , "hdrh4164", "sf54df15");
	map_insert(&map , "j2132516541ghj", "tg2h2");
	map_insert(&map , "16541615", "a5414651465");
	map_insert(&map , "1651t6hbdrth", "th5d1rh6");
	map_insert(&map , "rs5g12s651", "ser41g6s514");

	if(!check(map)) {
		printf("Check failed\n");

	} else {
		printf("check Succeeded!\n");
	}

	printf("%s\n", map_get(&map, "j2132516541ghj"));

	return 0;
}
