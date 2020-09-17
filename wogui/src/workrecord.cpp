#include "datasource.h"
#include "workrecord.h"
#include <imgui.h>

void wrd_init()
{
	dsrc_init_data_source();
}

bool ischeck = false;
bool iseditor = false;

char buffer[2048] = { 0 };

void wrd_update()
{
	ImGui::NewFrame();
	{
		static float time = 0.0f;
		static ImVec4 color = ImVec4(0.0f, 0.478f, 0.8f, 1.00f);

		time += ImGui::GetIO().DeltaTime;
		ImGui::Begin("!", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

		int count = dsrc_get_data_count();
		if (count == 0)
		{
			if (ImGui::Button("ADD"))
			{
				iseditor = true;
			}
		}
		else
		{
			for (int i = 0; i < count; ++i)
			{
				const char* word = dsrc_get_data_index(i);
				ImGui::Checkbox(word, &ischeck);
			}
		}

		if (iseditor)
		{
			ImGui::Text("Input:");
			
			ImGui::InputTextMultiline("A", buffer, 2048);

			if (ImGui::Button("OK"))
			{
				dsrc_add_current_data(buffer);
				iseditor = false;
			}
		}

		ImGui::TextColored(color, "This is some useful text.");
		if (time > 5)
		{
			if (int(time + 0.5f) == int(time))
			{
				color = ImVec4(0.0f, 0.478f, 0.8f, 1.00f);
			}
			else
			{
				color = ImVec4(0.855f, 0.227f, 0.0f, 1.00f);
			}

			if (ImGui::Button("Click"))
			{
				time = 0.0f;
				color = ImVec4(0.0f, 0.478f, 0.8f, 1.00f);
			}
		}
		ImGui::End();
	}
	ImGui::Render();
}

void wrd_destroy()
{
	dsrc_destroy_data_source();
}
