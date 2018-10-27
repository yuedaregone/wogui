#include "framework.h"
#include <GL\glew.h>
#include <imgui.h>
#include <imgui_impl_opengl2.h>
#include <imgui_impl_win32.h>
#include <time.h>

int GetFPS()
{
	return 30;
}

void InitOpenGL(int width, int height)
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

void DestroyOpenGL()
{

}

void InitGUI(void* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplOpenGL2_Init();

	ImGui::StyleColorsDark();	
}

void DestroyGUI()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void Render()
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

	
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::NewFrame();	

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello!", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		
		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);
		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
	ImGui::Render();

	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}