#include "datasource.h"
#include <stdio.h>
#include <win32.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
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

static struct DataPage* s_today_page = NULL;
static struct DataPage* s_cur_page = NULL;
static const buffer_t* s_path = NULL;

static buffer_t* get_recode_file_name(int y, int m, int d)
{
	buffer_t* buff = buffer_new_with_string_length(s_path->data, s_path->len + 20);
	buffer_appendf(buff, "wo_%04d-%02d-%02d.rcd", y, m, d);	
	return buff;
}

static size_t str_lines_count(const char* str)
{
	size_t szbuf = strlen(str);
	if (szbuf == 0)
		return 0;

	size_t lines = 1;
	size_t istart = 0;
	size_t iend = 0;

	while (iend < szbuf) {
		if (*(str + iend++) == '\n') {
			if (iend - istart > 0) {
				istart = iend;
				lines++;
			}			
		}
	}
	if (istart == iend)
		lines--;
	return lines;
}

static struct DataPage* load_data_to_data_page(const char* buffer)
{
	size_t lines = str_lines_count(buffer);
	size_t szbuf = strlen(buffer);

	size_t stsize = sizeof(struct DataPage) + lines * sizeof(char*) + szbuf + 1;
	struct DataPage* page = (struct DataPage*)malloc(stsize);
	page->lines = 0;
	page->prev = page->next = NULL;

	char* content = (char*)(page + 1) + lines * sizeof(char*);
	memcpy(content, buffer, szbuf);
	*(content + szbuf) = '\0';

	char** ctxindex = (char**)(page + 1);
	char* lastline = content;
	int tl = 0;
	for (int i = 0; i < szbuf; ++i) {
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
	return page;
}

static struct DataPage* load_data_page(int y, int m, int d)
{
	buffer_t* filename = get_recode_file_name(y, m, d);
	FILE* file = fs_open(filename->data, FS_OPEN_READ);
	buffer_free(filename);
	if (file == NULL) {
		return NULL;
	}
	size_t fsize = fs_fsize(file);

	char* buffer = (char*)_alloca(fsize + 1);
	fread(buffer, 1, fsize, file);
	*(buffer + fsize) = '\0';

	struct DataPage* page = load_data_to_data_page(buffer);
	page->year = y;
	page->month = m;
	page->day = d;

	fs_close(file);
	return page;
}

static int get_data_count(struct DataPage* page)
{
	if (page == NULL)
		return 0;
	return page->lines;
}

static const char* get_data_index(struct DataPage* page, int index)
{
	int count = get_data_count(page);
	if (index < 0 || index >= count)
		return NULL;
	return *((char **)(page + 1) + index);
}

static int get_extra_size_of_data_page(struct DataPage* page)
{
	int idxsize = page->lines * sizeof(char*);
	int strsize = 0;
	for (int i = 0; i < page->lines; ++i)
	{
		strsize += strlen(*((char**)(page + 1) + i)) + 1;
	}
	return idxsize + strsize;
}

static struct DataPage* combine_data_page(struct DataPage* page1, struct DataPage* page2)
{
	int page1size = get_extra_size_of_data_page(page1);
	int page2size = get_extra_size_of_data_page(page2);

	struct DataPage* npage = (struct DataPage*)malloc(page1size + page2size + sizeof(struct DataPage));
	npage->year = page1->year;
	npage->month = page1->month;
	npage->day = page1->day;
	npage->lines = page1->lines + page2->lines;
	npage->prev = page1->prev;
	npage->next = page1->next;

	char* content = (char*)(npage + 1) + npage->lines * sizeof(char*);
	char** ctxindex = (char**)(npage + 1);
	
	for (int i = 0; i < page1->lines; ++i)
	{
		*(ctxindex++) = content;
		const char* d = get_data_index(page1, i);
		memcpy(content, d, strlen(d));
		content += strlen(d);
		*content = '\0';
		content++;
	}
	for (int i = 0; i < page2->lines; ++i)
	{
		*(ctxindex++) = content;
		const char* d = get_data_index(page2, i);
		memcpy(content, d, strlen(d));
		content += strlen(d);
		*content = '\0';
		content++;
	}
	free(page1); free(page2);
	return npage;
}

void dsrc_init_data_source()
{
	buffer_t* cur = get_current_path();
	buffer_append(cur, "\\rc\\");
	s_path = cur;

	if (fs_exists(s_path->data) != 0)	{
		fs_mkdir(s_path->data, 0);
	}
	dsrc_load_today_data();
}

void dsrc_load_today_data()
{
	if (s_today_page == NULL) {
		int y, m, d = 0;
		get_date_today(&y, &m, &d);
		s_cur_page = s_today_page = load_data_page(y, m, d);
	} else {
		s_cur_page = s_today_page;
	}
}

void dsrc_add_current_data(const char* data)
{
	struct DataPage* page = load_data_to_data_page(data);

	int y, m, d = 0;
	get_date_today(&y, &m, &d);

	if (s_cur_page == NULL)	{		
		s_today_page = s_cur_page = page;		
		s_cur_page->year = y;
		s_cur_page->month = m;
		s_cur_page->day = d;
	} else {
		s_cur_page = combine_data_page(s_cur_page, page);
		if (s_cur_page->year == y && s_cur_page->month == m && s_cur_page->day == d)
			s_today_page = s_cur_page;
	}
}

void dsrc_load_preday_data()
{
	if (s_cur_page == NULL)	{
		dsrc_load_today_data();
	}
	if (s_cur_page != NULL) {
		if (s_cur_page->prev == NULL) {
			int y, m, d = 0;
			get_date_preday(s_cur_page->year, s_cur_page->month, s_cur_page->day, &y, &m, &d);
			s_cur_page = s_cur_page->prev = load_data_page(y, m, d);
		} else {
			s_cur_page = s_cur_page->prev;
		}
	}
}

void dsrc_load_nextday_data()
{
	if (s_cur_page == NULL) {
		dsrc_load_today_data();
	}
	if (s_cur_page != NULL) {
		if (s_cur_page->next == NULL) {
			int y, m, d = 0;
			get_date_nextday(s_cur_page->year, s_cur_page->month, s_cur_page->day, &y, &m, &d);
			s_cur_page = s_cur_page->next = load_data_page(y, m, d);
		}
		else {
			s_cur_page = s_cur_page->next;
		}
	}
}

int dsrc_get_data_count()
{
	return get_data_count(s_cur_page);
}

const char* dsrc_get_data_index(int index)
{
	return get_data_index(s_cur_page, index);
}

void dsrc_save_cur_data()
{
	int lines = 0;
	if ((lines = dsrc_get_data_count()) == 0)
		return;

	buffer_t * filename = get_recode_file_name(s_cur_page->year, s_cur_page->month, s_cur_page->day);
	FILE* file = fs_open(filename->data, FS_OPEN_WRITE);
	buffer_free(filename);

	for (int i = 0; i < lines; ++i)	{
		const char* ctx = dsrc_get_data_index(i);
		size_t len = strlen(ctx);
		if (len > 0) {
			fs_fnwrite(file, ctx, len);
		}
	}

	fs_close(file);
}

void dsrc_destroy_data_source()
{
	if (s_today_page == NULL) return;

	struct DataPage* p = s_today_page->prev;
	struct DataPage* n = s_today_page->next;

	while (p != NULL) {
		struct DataPage* t = p->prev;
		free(p);
		p = t;
	}
	while (n != NULL) {
		struct DataPage* t = n->next;
		free(n);
		n = t;
	}
	free(s_today_page);
	s_today_page = NULL;
	s_cur_page = NULL;
}
