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

void get_date_preday(int cy, int cm, int cd, int * y, int * m, int * d)
{
	struct tm st;
	memset(&st, 0, sizeof(st));
	st.tm_year = cy - 1900;
	st.tm_mon = cm - 1;
	st.tm_mday = cd;
	time_t t = mktime(&st);
	t -= 3600 * 12;
	struct tm* rt = localtime(&t);
	*y = rt->tm_year + 1900;
	*m = rt->tm_mon + 1;
	*d = rt->tm_mday;
}

void get_date_nextday(int cy, int cm, int cd, int * y, int * m, int * d)
{
	struct tm st;
	memset(&st, 0, sizeof(st));
	st.tm_year = cy - 1900;
	st.tm_mon = cm - 1;
	st.tm_mday = cd;
	time_t t = mktime(&st);
	t += 3600 * 12;
	struct tm* rt = localtime(&t);
	*y = rt->tm_year + 1900;
	*m = rt->tm_mon + 1;
	*d = rt->tm_mday;
}



