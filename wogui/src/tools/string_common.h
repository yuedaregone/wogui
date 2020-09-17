#ifndef __STRING_COMMON__
#define __STRING_COMMON__
#include <string>
#include <vector>

std::string getPath(std::string p);

std::string getTotalName(std::string p);

std::string getOnlyNameFromTotalName(std::string name);

std::string getHouzhui(std::string p);

void findAllFile(std::vector<std::string>& vStrFile, std::string lpPath);

int getFileLength(FILE* fp);

int findStrIndexFromContentAndIndex(const char* content, int startIndex, const char* str);

std::string getCurPath();
#endif