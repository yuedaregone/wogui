#include "datasource.h"
#include <stdio.h>
#include <win32.h>

struct DataLine
{
	int progress;
	char* data;
};

struct DataPage
{
	int year;
	int month;
	int day;
	int count;
	DataLine* lines;
	DataPage* prev;
	DataPage* next;
};

static struct  DataPage* s_page = NULL;
static const char* s_path = NULL;

static const char* GetFileName(int y, int m, int d)
{

}

static const char* GetTodayFileName()
{
	int y, m, d = 0;
	GetDateToday(&y, &m, &d);
	return GetFileName(y, m, d);
}

void InitDataSource()
{
	s_path = GetCurrentPath();
	
	
}

void Save()
{
}

void DestroyDataSource()
{
}
