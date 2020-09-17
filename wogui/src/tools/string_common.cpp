#include "string_common.h"
#include <windows.h>

std::string getPath(std::string p)
{
	return p.substr(0, p.find_last_of("\\"));
}

std::string getTotalName(std::string p)
{
	return p.substr(p.find_last_of("/") + 1, p.length());
}

std::string getOnlyNameFromTotalName(std::string name)
{
	return name.substr(0, name.find_last_of("."));
}

std::string getHouzhui(std::string p)
{
	return p.substr(p.find_last_of("."), p.length());
}

void findAllFile(std::vector<std::string>& vStrFile, std::string lpPath)
{
	std::string szFind;
	std::string szFile;

	WIN32_FIND_DATA FindFileData;
	szFind = lpPath;
	szFind = szFind + "\\*.*";

	HANDLE hFind = ::FindFirstFile(szFind.c_str(), &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind) return;
	while (::FindNextFile(hFind, &FindFileData))
	{
		if (strcmp((char*)FindFileData.cFileName, ".") == 0 || strcmp((char*)FindFileData.cFileName, "..") == 0)
		{
			continue;
		}
		szFile = lpPath;
		szFile = szFile + "\\";
		szFile = szFile + FindFileData.cFileName;

		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			findAllFile(vStrFile, szFile);
		}
		else
		{
			std::string houzhui = getHouzhui(FindFileData.cFileName);
			if (strcmp(houzhui.c_str(), ".mzt") == 0)
			{				
				vStrFile.push_back(szFile);
			}
		}
	}
	FindClose(hFind);
}

int getFileLength(FILE* fp)
{
	int temp = ftell(fp);
	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	fseek(fp, temp, SEEK_SET);
	return len;
}

int findStrIndexFromContentAndIndex(const char* content, int startIndex, const char* str)
{
	int len = strlen(content);
	int sLen = strlen(str);
	int index = -1;
	int j = 0;
	for (int i = startIndex; i < len; ++i)
	{
		if (content[i] == str[j])
		{
			++j;
			if (j == sLen)
			{
				index = i + 1;
				break;
			}
		}
		else
		{
			j = 0;
		}
	}
	return index;
}

std::string getCurPath()
{
	char buff[512] = {0};
	GetModuleFileName(NULL, buff, 512);
	return getPath(buff);
}