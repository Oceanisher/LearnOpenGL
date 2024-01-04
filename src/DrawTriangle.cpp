#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/LearnOpenGLHeader.h"

//回调函数，用来处理窗口大小变化时，重新设置OpenGL的渲染窗口大小
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//输入监听函数，这里监听ESC是否被按下，如果按下，那么退出
void processInput(GLFWwindow *window);

//清屏
void GlClear();
//生成VAO/VBO
void GlShaderVao(unsigned int* VAO, unsigned int* VBO);
//构建着色器程序
unsigned int GlShaderProgramProc();
//顶点着色器
unsigned int GlShaderVertex();
//片段着色器
unsigned int GlShaderFragment();
//编译shader
unsigned int GlShaderCompile(const char *vertexShaderSource, int type);
//校验shader是否编译成功
void GlShaderCompileCheck(unsigned int shaderIdx);

//绘制三角形
int DrawTriangle()
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
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //通知GLFW将我们窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);

    //初始化GLAD
    //给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。
    //GLFW给我们的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //告诉OpenGL渲染窗口的尺寸大小，即视口(Viewport)，这样OpenGL才只能知道怎样根据窗口大小显示数据和坐标
    //前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）
    glViewport(0, 0, 800, 600);

    //注册窗口变更监听回调，重新设置OpenGL的渲染窗口大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //生成着色器程序、顶点着色器、片段着色器
    unsigned int programIdx = GlShaderProgramProc();

    //生成VAO/VBO
    unsigned int VAO, VBO;
    GlShaderVao(&VAO, &VBO);

    //渲染循环，阻止窗口退出
    //每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序
    while(!glfwWindowShouldClose(window))
    {
        //输入
        //监听按键是否按下
        processInput(window);

        //清屏
        GlClear();

        //使用的着色器程序
        glUseProgram(programIdx);
        //按照三角形绘制顶点
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
        glfwSwapBuffers(window);
        //检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
        glfwPollEvents();
    }

    //释放shader相关
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(programIdx);

    //释放资源、退出程序
    glfwTerminate();
    return 0;
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

//生成VAO/VBO
void GlShaderVao(unsigned int* VAO, unsigned int* VBO)
{
    //定义一个顶点数组
    float vertices[] = {
            -0.5f, -0.5f, 0.0f, // left  
            0.5f, -0.5f, 0.0f, // right 
            0.0f,  0.5f, 0.0f  // top   
    };

    //生成VAO/VBO
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    //绑定VAO/VBO
    glBindVertexArray(*VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //设置顶点属性值，也就是顶点数组的split方法
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

//构建着色器程序
unsigned int GlShaderProgramProc()
{
    //创建着色器程序
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //创建顶点着色器
    unsigned int vertexIdx = GlShaderVertex();
    //创建片段着色器
    unsigned int fragmentIdx = GlShaderFragment();

    //附加两个着色器到程序上，然后链接，这样才能使用
    glAttachShader(shaderProgram, vertexIdx);
    glAttachShader(shaderProgram, fragmentIdx);
    glLinkProgram(shaderProgram);

    //校验程序是否链接成功
    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //删除着色器对象
    glDeleteShader(vertexIdx);
    glDeleteShader(fragmentIdx);
    return shaderProgram;
}

//顶点着色器
unsigned int GlShaderVertex()
{
    //定义顶点着色器代码
    const char *shaderSource = "#version 330 core\n"
                               "layout (location = 0) in vec3 aPos;\n"
                               "void main()\n"
                               "{\n"
                               "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                               "}\0";
    return GlShaderCompile(shaderSource, GL_VERTEX_SHADER);
}

//片段着色器
unsigned int GlShaderFragment()
{
    //定义片段着色器代码
    const char *shaderSource = "#version 330 core\n"
                               "out vec4 FragColor;\n"
                               "void main()\n"
                               "{\n"
                               "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                               "}\0";
    return GlShaderCompile(shaderSource, GL_FRAGMENT_SHADER);
}

//编译shader
unsigned int GlShaderCompile(const char *vertexShaderSource, int type)
{
    //创建着色器对象
    unsigned int shaderIdx;
    shaderIdx = glCreateShader(type);

    //把shader代码附着到着色器对象上。1代表只有一段shader代码。
    glShaderSource(shaderIdx, 1, &vertexShaderSource, NULL);
    //编译着色器对象
    glCompileShader(shaderIdx);
    //校验是否编译成功
    GlShaderCompileCheck(shaderIdx);
    return shaderIdx;
}

//校验shader是否编译成功
void GlShaderCompileCheck(unsigned int shaderIdx)
{
    int  success;
    char infoLog[512];
    //是否编译成功
    glGetShaderiv(shaderIdx, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        //获取编译日志
        glGetShaderInfoLog(shaderIdx, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}