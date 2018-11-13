#include "framework.h"
#include <GL\glew.h>
#include <imgui.h>
#include <imgui_impl_opengl2.h>
#include <imgui_impl_win32.h>
#include <time.h>
#include <stdio.h>
#include "datasource.h"

int GetFPS()
{
	return 30;
}

static void InitOpenGL(int width, int height)
{
	glEnable(GL_ALPHA_TEST);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glClearColor(0, 0, 0, 0);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void DestroyOpenGL()
{

}

static void InitGUI(void* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplOpenGL2_Init();

	ImGui::StyleColorsDark();	
	io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 12.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
}

static void DestroyGUI()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool ischeck = false;

static void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/*
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_TRIANGLES);

	glColor4f(0.2f, 0.5f, 0.2f, 0.5f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glEnd();

	glBegin(GL_TRIANGLES);

	glColor4f(0.7f, 0.3f, 0.1f, 0.5f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	glEnd();
	*/

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	

	ImGui::NewFrame();		
	{
		static float time = 0.0f;		
		static ImVec4 color = ImVec4(0.0f, 0.478f, 0.8f, 1.00f);

		time += ImGui::GetIO().DeltaTime;
		ImGui::Begin(TEXT("!"), NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
			
		int count = dsrc_get_data_count();
		for (int i = 0; i < count; ++i)
		{
			const char* word = dsrc_get_data_index(i);
			ImGui::Checkbox(word, &ischeck);
		}

		ImGui::TextColored(color,  "This is some useful text.");

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

	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}


void InitFramework(int width, int height, void* window)
{
	dsrc_init_data_source();

	InitOpenGL(width, height);
	InitGUI(window);
}

void UpdateFramework()
{
	Render();
}

void EndFramework()
{
	DestroyOpenGL();
	DestroyGUI();
}
