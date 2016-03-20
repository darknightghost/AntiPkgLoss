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


int main(int argc, char* argv[])
{
	map_t map = NULL;
	map_insert(&map , "qqqq", "asdasdasda");
	CHK_MAP_BALANCE(&map);
	map_insert(&map , "www", "asdasdasda");
	CHK_MAP_BALANCE(&map);
	map_insert(&map , "wwwwefwefa", "asdasdasda");
	CHK_MAP_BALANCE(&map);
	map_insert(&map , "asdadc", "a54da654d");
	CHK_MAP_BALANCE(&map);
	map_insert(&map , "hdrh4164", "sf54df15");
	CHK_MAP_BALANCE(&map);
	map_insert(&map , "j2132516541ghj", "tg2h2");
	CHK_MAP_BALANCE(&map);
	map_insert(&map , "16541615", "a5414651465");
	CHK_MAP_BALANCE(&map);
	map_insert(&map , "1651t6hbdrth", "th5d1rh6");
	CHK_MAP_BALANCE(&map);
	map_insert(&map , "rs5g12s651", "ser41g6s514");
	CHK_MAP_BALANCE(&map);

	map_remove(&map , "qqqq");
	CHK_MAP_BALANCE(&map);
	map_remove(&map , "www");
	CHK_MAP_BALANCE(&map);
	map_remove(&map , "wwwwefwefa");
	CHK_MAP_BALANCE(&map);
	map_remove(&map , "asdadc");
	CHK_MAP_BALANCE(&map);
	map_remove(&map , "hdrh4164");
	CHK_MAP_BALANCE(&map);
	map_remove(&map , "j2132516541ghj");
	CHK_MAP_BALANCE(&map);
	map_remove(&map , "16541615");
	CHK_MAP_BALANCE(&map);
	map_remove(&map , "1651t6hbdrth");
	CHK_MAP_BALANCE(&map);
	map_remove(&map , "rs5g12s651");
	CHK_MAP_BALANCE(&map);

	if(map_get(&map, "wwwwefwefa") == NULL) {
		printf("removed\n");
	}

	UNREFERENCED_PARAMER(argc);
	UNREFERENCED_PARAMER(argv);

	return 0;
}
