#include "datasource.h"
#include <stdio.h>
#include <win32.h>
#include <stdlib.h>
#include <string.h>
#include "clib/fs.h"
#include "clib/buffer.h"
#define MAX_LINE_CHARACTER 2048

struct DataPage
{
	int year;
	int month;
	int day;
	int lines;
	DataPage* prev;
	DataPage* next;
};

static struct  DataPage* s_page = NULL;
static const buffer_t* s_path = NULL;

static buffer_t* get_recode_file_name(int y, int m, int d)
{
	buffer_t* buff = buffer_new_with_string_length(s_path->data, s_path->len + 20);
	buffer_appendf(buff, "wo_%04d-%02d-%02d.rcd", y, m, d);	
	return buff;
}

static buffer_t* get_today_recode_file_name()
{
	int y, m, d = 0;
	get_date_today(&y, &m, &d);
	return get_recode_file_name(y, m, d);
}

static void init_data_page(DataPage* page)
{
	if (page == NULL) return;
	page->year = page->month = page->day = page->lines = 0;
	page->prev = page->next = NULL;
}

static struct DataPage* read_data_page(const char* filename)
{
	 FILE* file = fs_open(filename, FS_OPEN_READ);
	 size_t fsize = fs_fsize(file);
	 size_t lines = fs_flines(file);

	 size_t stsize = sizeof(struct DataPage) + lines * sizeof(const char*) + fsize + 1;
	 struct DataPage* page = (struct DataPage*)malloc(stsize);
	 init_data_page(page);

	 char* content = (char*)(page + 1) + lines * sizeof(const char*);	
	 fread(content, 1, fsize, file);
	 *(content + fsize) = '\0';
	 
	 char** ctxindex = (char**)(page + 1);
	 char* lastline = content;
	 int tl = 0;
	 for (int i = 0; i < fsize; ++i) {
		 if (*(content + i) == '\n') {
			 *(content + i) = '\0';
			 if (i > 0 && *(content + i - 1) == '\r') {
				 *(content + i - 1) = '\0';
			 }
			 if (strlen(lastline) > 0 && *lastline != '#') {
				 *(ctxindex + tl++) = lastline;
				 ++page->lines;
			 }
			 lastline = content + i + 1;
		 }
	 }
	 if (strlen(lastline) > 0 && *lastline != '#') {
		 *(ctxindex + tl++) = lastline;
		 ++page->lines;
	 }
	 fs_close(file);
	 return page;
}

void init_data_source()
{
	buffer_t* cur = get_current_path();
	buffer_append(cur, "\\rc\\");
	s_path = cur;

	if (fs_exists(s_path->data) != 0)
	{
		fs_mkdir(s_path->data, 0);
	}
	
	buffer_t* filename = get_today_recode_file_name();
	
	s_page = read_data_page(filename->data);
}

int get_data_count()
{
	return s_page->lines;
}

const char* get_data_index(int index)
{
	int count = get_data_count();
	if (index < 0 || index >= count)
		return NULL;
	return *((char **)(s_page + 1) + index);
}

void save()
{
}

void destroy_data_source()
{
}
