#include "win32.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <windows.h>
#include <time.h>

buffer_t* get_current_path()
{
	char path[256] = {0};
	GetModuleFileName(NULL, path, 256);	

	size_t len = strlen(path);
	for (size_t i = len - 1; i > 0; i--)
	{
		if (path[i] == '\\')
		{
			path[i] = '\0';
			break;
		}
	}

	buffer_t* ret = buffer_new();
	buffer_append(ret, path);
	return ret;
}

void get_date_today(int* y, int* m, int* d)
{
	time_t t = time(NULL);
	struct tm* st = localtime(&t);
	*y = st->tm_year + 1900;
	*m = st->tm_mon + 1;
	*d = st->tm_mday;
}



