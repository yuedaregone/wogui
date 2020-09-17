#include "searchview.h"
#include <imgui.h>
#include <windows.h>
#include "win32.h"
#include <vector>

typedef struct _search_result {

}SearchResult;

char* pathBuffer = NULL;
char* searchContent = NULL;
std::vector<SearchResult> result;

void search_init()
{
	pathBuffer = new char[MAX_PATH * 2];
	memset(pathBuffer, 0, sizeof(pathBuffer));

	searchContent = new char[1024];
	memset(searchContent, 0, 1024);
}


void start_search(const char* path, const char* target) {

}


void search_update()
{
	ImGui::NewFrame();
	{
		ImGui::Begin("!", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
		
		ImGui::InputText("Input the path:", pathBuffer, MAX_PATH, 0, NULL, NULL);

		if (ImGui::Button("Browser")) {
			result.clear();
			char* result = get_choose_folder(pathBuffer);
			if (result != NULL) {
				pathBuffer = result;
			}
		}

		ImGui::InputText("Target content:", searchContent, 1024, 0, NULL, NULL);
		if (ImGui::Button("Search")) {
			result.clear();
			start_search(pathBuffer, searchContent);
		}

		if (result.size() > 0) {

		}
		
		ImGui::End();
	}
	ImGui::Render();
}

void search_destroy()
{
}
