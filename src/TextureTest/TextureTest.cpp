#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../include/LearnOpenGLHeader.h"
#include "../../include/Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb_image.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//生成VAO/VBO/EBO
void GenTextureTestData(unsigned int* VAO, unsigned int* VBO, unsigned int* EBO)
{
    //定义一个顶点数组，位置+颜色+纹理坐标
    float vertices[] = {
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    //定义VEO，也就是顶点绘制顺序
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    //生成VAO/VBO/EBO
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);

    //绑定VAO/VBO/EBO
    glBindVertexArray(*VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //设置顶点位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //设置顶点颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    //设置顶点绘制顺序
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

//生成纹理
void GenTexture(unsigned int* TextureIdx, char const *filename, int count)
{
    //生成纹理引用
    glGenTextures(1, TextureIdx);
    glBindTexture(GL_TEXTURE_2D, *TextureIdx); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //因为OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部。
    //很幸运，stb_image.h能够在图像加载时帮助我们翻转y轴，只需要在加载任何图像前加入以下语句
    stbi_set_flip_vertically_on_load(true);

    //读取纹理、绑定纹理
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (0 == count)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, *TextureIdx);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, *TextureIdx);
    }
    
    //释放原始纹理数据
    stbi_image_free(data);
}

//鼠标滚轮滚动的设置纹理显隐
double textureTestScroll = 0;

//监听滚轮
void OnTextureTestScroll(GLFWwindow* window, double x, double y)
{
    textureTestScroll += y / 10;
    if (textureTestScroll > 1)
    {
        textureTestScroll = 1;
    }
    if (textureTestScroll < 0)
    {
        textureTestScroll = 0;
    }
}



//绘制移动三角形
int DrawTextureTest()
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
            "src/TextureTest/TextureTest.vsh",
            "src/TextureTest/TextureTest.fsh");

    //生成VAO/VBO/EBO
    unsigned int VAO, VBO, EBO;
    GenTextureTestData(&VAO, &VBO, &EBO);
    
    //生成纹理1、纹理2
    unsigned int TextureIdx1, TextureIdx2;
    GenTexture(&TextureIdx1, "src/TextureTest/TextureTest1.jpg", 0);
    GenTexture(&TextureIdx2, "src/TextureTest/TextureTest2.png", 1);

    //使用的着色器程序
    shader.Use();
    //设置Shader的纹理，2个纹理都要设置
    shader.SetInt("texture1", 0);
    shader.SetInt("texture2", 1);
    
    //监听鼠标滚轮
    glfwSetScrollCallback(window, OnTextureTestScroll);

    //渲染循环
    while(!glfwWindowShouldClose(window))
    {
        //监听按键是否按下
        processInput(window);
        //清屏
        GlClear();
        
        //每帧设置透明度值
        shader.SetFloat("textureVal", (float)textureTestScroll);

        //必须每帧绑定下
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureIdx1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, TextureIdx2);

        //按照三角形绘制顶点
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
        glfwSwapBuffers(window);
        //检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
        glfwPollEvents();
    }

    //释放shader相关
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    shader.Delete();

    //释放资源、退出程序
    glfwTerminate();
    return 0;
}