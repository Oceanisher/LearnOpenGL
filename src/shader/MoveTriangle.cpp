#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../include/LearnOpenGLHeader.h"
#include "../../utils/Shader.h"

//生成VAO/VBO
void DrawMoveTriangleData(unsigned int* VAO, unsigned int* VBO)
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

