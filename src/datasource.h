#pragma once

struct DataPage;

void init_data_source();

const char* get_data_index(int index);

void save();

void destroy_data_source();
