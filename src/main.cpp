#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include "GL\glew.h"
#include "GL\wglew.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl2.h"

#define WINDOW_NAME "WOGUI"
#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 300

struct SBmpInfo
{
	BITMAPINFO	m_BitmapInfo;
	RGBQUAD		m_bmiColors[2];	// 为BITMAPINFO的m_bmiColors补充两个元素空间
};

LRESULT CALLBACK WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

//#define CONSOLE_DEBUG
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);	
	UNREFERENCED_PARAMETER(nCmdShow);

#ifdef CONSOLE_DEBUG
	start_console();
#endif	

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProcHandler;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, NULL);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = WINDOW_NAME;
	wcex.hIconSm = LoadIcon(hInstance, NULL);

	if (!RegisterClassEx(&wcex))
	{
		printf("RegisterClassEx %s Error: %d\n", WINDOW_NAME, GetLastError());
		return -1;
	}	

	UINT width = WINDOW_WIDTH;
	UINT height = WINDOW_HEIGHT;
	
	DWORD style = WS_EX_LAYERED | WS_EX_TOOLWINDOW;
	HWND hwnd = CreateWindowEx(style, WINDOW_NAME, WINDOW_NAME, WS_VISIBLE | WS_SYSMENU,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
		return -1;	

	UINT* bgBuff = (UINT*)malloc(width * height * sizeof(UINT));
	memset(bgBuff, 0, width * height * sizeof(UINT));

	HDC dc = ::GetDC(hwnd);
	HDC layerDc = ::CreateCompatibleDC(dc);
	HBITMAP bitmap = ::CreateCompatibleBitmap(dc, width, height);

	SBmpInfo* sbmpInfo = (SBmpInfo*)malloc(sizeof(SBmpInfo));
	BITMAPINFO& bmpInfo = sbmpInfo->m_BitmapInfo;
	bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
	bmpInfo.bmiHeader.biWidth = width;
	bmpInfo.bmiHeader.biHeight = height;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_BITFIELDS;

	*(UINT*)(bmpInfo.bmiColors + 0) = 0xFF0000;
	*(UINT*)(bmpInfo.bmiColors + 1) = 0x00FF00;
	*(UINT*)(bmpInfo.bmiColors + 2) = 0x0000FF;

	::SelectObject(layerDc, bitmap);

	DWORD dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;// | PFD_DRAW_TO_BITMAP ;
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = dwFlags;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int pixelFormat = ChoosePixelFormat(dc, &pfd);
	if (pixelFormat == 0) {
		return -1;
	}
	BOOL bResult = SetPixelFormat(dc, pixelFormat, &pfd);
	if (bResult == FALSE) {
		return -1;
	}
	HGLRC glrc = wglCreateContext(dc);
	if (!glrc) {
		return -1;
	}
	if (FALSE == wglMakeCurrent(dc, glrc))
		return -1;

	//glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) { //init glew
		fprintf(stderr, "Failed to init glew!");
		return -1;
	}
	int attribList[] =
	{
		WGL_DRAW_TO_PBUFFER_ARB, TRUE,      // allow rendering to the pbuffer
		WGL_SUPPORT_OPENGL_ARB,  TRUE,      // associate with OpenGL
		WGL_DOUBLE_BUFFER_ARB,   FALSE,     // single buffered
		WGL_RED_BITS_ARB,   8,              // minimum 8-bits for red channel
		WGL_GREEN_BITS_ARB, 8,              // minimum 8-bits for green channel
		WGL_BLUE_BITS_ARB, 8,              // minimum 8-bits for blue channel
		WGL_ALPHA_BITS_ARB, 8,              // minimum 8-bits for alpha channel
		WGL_DEPTH_BITS_ARB, 16,             // minimum 16-bits for depth buffer
		0
	};
	int format = 0;
	UINT matchingFormats = 0;

	if (!wglChoosePixelFormatARB(dc, attribList, 0, 1, &format, &matchingFormats))
	{
		return -1;
	}

	HPBUFFERARB hpBuffer = wglCreatePbufferARB(dc, format, width, height, 0);
	if (!hpBuffer)
	{
		return -1;
	}

	HDC pbufferDC = wglGetPbufferDCARB(hpBuffer);
	if (!pbufferDC)
	{
		return FALSE;
	}

	HGLRC pbufferGLRC = wglCreateContext(pbufferDC);
	if (!pbufferGLRC)
	{
		return FALSE;
	}

	wglMakeCurrent(dc, 0);
	wglMakeCurrent(pbufferDC, pbufferGLRC);
	
	glEnable(GL_ALPHA_TEST);
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glClearColor(0, 0, 0, 0.5);
	
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplOpenGL2_Init();

	ImGui::StyleColorsDark();
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//TODO: Update And Render
			//memset(bgBuff, 0, width * height * sizeof(UINT));

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			

			glBegin(GL_TRIANGLES);
			
			glColor4f(0.2, 0.5, 0.2, 0.5);
			glVertex3f(-1.0, -1.0, 0.0);
			glVertex3f(1.0, -1.0, 0.0);
			glVertex3f(0.0, 1.0, 0.0);

			glEnd();

			glBegin(GL_TRIANGLES);

			glColor4f(0.7, 0.3, 0.1, 0.5);
			glVertex3f(-1.0, -1.0, 0.0);
			glVertex3f(1.0, -1.0, 0.0);
			glVertex3f(1.0, 1.0, 0.0);

			glEnd();	


			ImGui_ImplOpenGL2_NewFrame();
			ImGui_ImplWin32_NewFrame();

			ImGui::NewFrame();
			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);

			// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
				ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
				ImGui::Checkbox("Another Window", &show_another_window);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}
			ImGui::Render();
			
			ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

			
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			glReadPixels(0, 0, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, bgBuff);
					
			::SetDIBitsToDevice(layerDc,
				0, 0, width, height,
				0, 0, 0, height,
				bgBuff, &sbmpInfo->m_BitmapInfo, DIB_RGB_COLORS);

			BLENDFUNCTION blend;
			blend.BlendOp = AC_SRC_OVER;
			blend.BlendFlags = 0;
			blend.AlphaFormat = 1;
			blend.SourceConstantAlpha = 255;

			RECT rect;
			GetWindowRect(hwnd, &rect);

			POINT p = { rect.left, rect.top };
			SIZE s = { (LONG)width, (LONG)height };

			POINT m_originPos = { 0, 0 };
			::UpdateLayeredWindow(hwnd, dc, &p, &s, layerDc, &m_originPos, 0, &blend, ULW_ALPHA);
		}
	}

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(glrc);

	ReleaseDC(hwnd, dc);
	DeleteObject(bitmap);
	ReleaseDC(hwnd, layerDc);

	free(bgBuff);
	free(sbmpInfo);
	 	
#ifdef CONSOLE_DEBUG
	stop_console();
#endif	
	return 0;
}
