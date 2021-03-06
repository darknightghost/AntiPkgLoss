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
#include <unistd.h>
#include <errno.h>
#include "common/common/common.h"

int main(int argc, char* argv[])
{
	//Load config
	//Initialize log module
	//Run as daemon
	if(daemon(1, 0) != 0) {
		return errno;
	}

	//Call service module

	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	return 0;
}
