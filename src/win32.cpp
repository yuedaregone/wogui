#include "win32.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <windows.h>
#include <time.h>

const char * GetCurrentPath()
{
	char path[256] = {0};
	GetModuleFileName(NULL, path, 256);	
	return path;
}

bool FileExist(const char* path)
{
	return access(path, 0) == 0;
}

bool NewDirectory(const char * path)
{
	if (FileExist(path))
		return true;
	return CreateDirectory(path, NULL);
}

void GetDateToday(int* y, int* m, int* d)
{
	time_t t = time(NULL);
	struct tm* st = localtime(&t);
	*y = st->tm_year;
	*m = st->tm_mon;
	*d = st->tm_mday;
}



