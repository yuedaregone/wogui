#pragma once

const char* GetCurrentPath();

bool FileExist(const char* path);

bool NewDirectory(const char* path);

void GetDateToday(int* y, int* m, int* d);

