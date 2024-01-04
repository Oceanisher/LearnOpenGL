#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/LearnOpenGLHeader.h"

//开启窗口
GLFWwindow* OpenWd()
{
    //初始化
    glfwInit();
    //代表使用的OpenGL版本是3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //代表使用核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //MAC系统使用此行
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //创建一个800*600的窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        return nullptr;
    }
    //通知GLFW将我们窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);

    //初始化GLAD
    //给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。
    //GLFW给我们的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    //告诉OpenGL渲染窗口的尺寸大小，即视口(Viewport)，这样OpenGL才只能知道怎样根据窗口大小显示数据和坐标
    //前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）
    glViewport(0, 0, 800, 600);

    //注册窗口变更监听回调，重新设置OpenGL的渲染窗口大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

//回调函数，用来处理窗口大小变化时，重新设置OpenGL的渲染窗口大小
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//输入监听函数，这里监听ESC是否被按下，如果按下，那么退出
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//清屏
void GlClear()
{
    //glClearColor函数是一个状态设置函数
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //glClear函数则是一个状态使用的函数，它使用了当前的状态来获取应该清除为的颜色
    glClear(GL_COLOR_BUFFER_BIT);
}