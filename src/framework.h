#ifndef __FRAMEWORK_H__
#define __FRAMEWORK_H__

int GetFPS();

void InitOpenGL(int width, int height);
void DestroyOpenGL();

void InitGUI(void* window);
void DestroyGUI();

void Render();

#endif
