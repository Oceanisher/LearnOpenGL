#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../include/LearnOpenGLHeader.h"

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
    GLFWwindow* window = OpenWd();
    if (window == nullptr)
    {
        glfwTerminate();
        return -1;
    }

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