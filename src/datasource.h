#pragma once

void dsrc_init_data_source();

void dsrc_load_today_data();

void dsrc_load_preday_data();

void dsrc_load_nextday_data();

int dsrc_get_data_count();

const char* dsrc_get_data_index(int index);

void dsrc_save_cur_data();

void dsrc_destroy_data_source();
