#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../include/LearnOpenGLHeader.h"
#include "../../include/Shader.h"

//生成VAO/VBO
void DrawMoveTriangleData(unsigned int* VAO, unsigned int* VBO)
{
    //定义一个顶点数组，位置+颜色
    float vertices[] = {
            -0.5f, -0.5f, 0.0f, 1.0F, 0.0F, 0.0F,
            0.5f, -0.5f, 0.0f, 0.0F, 1.0F, 0.0F,
            0.0f,  0.5f, 0.0f, 0.0F, 0.0F, 1.0F,
    };

    //生成VAO/VBO
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);

    //绑定VAO/VBO
    glBindVertexArray(*VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //设置顶点位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //设置顶点颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

//绘制移动三角形
int DrawMoveTriangle()
{
    //创建窗口
    GLFWwindow* window = OpenWd();
    if (window == nullptr)
    {
        glfwTerminate();
        return -1;
    }
    
    //生成着色器程序
    Shader shader(
            "/Users/oceanisher/Documents/LearnOpenGL/LearnOpenGL/src/shader/MoveTriangle.vsh", 
            "/Users/oceanisher/Documents/LearnOpenGL/LearnOpenGL/src/shader/MoveTriangle.fsh");

    //生成VAO/VBO
    unsigned int VAO, VBO;
    DrawMoveTriangleData(&VAO, &VBO);
    
    //渲染循环
    while(!glfwWindowShouldClose(window))
    {
        //监听按键是否按下
        processInput(window);
        //清屏
        GlClear();
        
        //使用的着色器程序
        shader.Use();
        //循环设置颜色与位置
        double time = glfwGetTime();
        double redColor = cos(time) / 2.0F + 0.5F;
        double greenColor = sin(time) / 2.0F + 0.5F;
        double blueColor = (redColor + greenColor) / 2.0F;
        shader.SetVec3("randomColor", (float)redColor, (float)greenColor, (float)blueColor);
        shader.SetVec3("randomPos", (float)redColor - 0.5F, (float)greenColor - 0.5F, (float)blueColor - 0.5F);
        
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
    shader.Delete();

    //释放资源、退出程序
    glfwTerminate();
    return 0;
}

