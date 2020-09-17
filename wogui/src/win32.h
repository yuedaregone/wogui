#pragma once
#include "clib/buffer.h"

buffer_t* get_current_path();

void get_date_today(int* y, int* m, int* d);

void get_date_preday(int cy, int cm, int cd, int* y, int* m, int* d);

void get_date_nextday(int cy, int cm, int cd, int* y, int* m, int* d);

