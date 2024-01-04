#ifndef TESTOPENGL_LEARNOPENGLHEADER_H
#define TESTOPENGL_LEARNOPENGLHEADER_H
#endif

#include <GLFW/glfw3.h>

//绘制三角形
int DrawTriangle();
//开启窗口
GLFWwindow* OpenWd();
//回调函数，用来处理窗口大小变化时，重新设置OpenGL的渲染窗口大小
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//输入监听函数，这里监听ESC是否被按下，如果按下，那么退出
void processInput(GLFWwindow *window);
//清屏
void GlClear();