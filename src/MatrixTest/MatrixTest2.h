#ifndef TESTOPENGL_MATRIXTEST2_H
#define TESTOPENGL_MATRIXTEST2_H

#endif //TESTOPENGL_MATRIXTEST2_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../include/LearnOpenGLHeader.h"
#include "../../include/Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb_image.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//测试矩阵
class MatrixTest2
{
public:
    //绘制
    int Draw()
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
                "src/MatrixTest/MatrixTest2.vsh",
                "src/MatrixTest/MatrixTest2.fsh");

        //生成VAO/VBO/EBO
        unsigned int VAO, VBO;
        GenMatrixTestData(&VAO, &VBO);

        //生成纹理1、纹理2
        unsigned int TextureIdx1, TextureIdx2;
        GenMatrixTexture(&TextureIdx1, "src/TextureTest/TextureTest1.jpg", 0);
        GenMatrixTexture(&TextureIdx2, "src/TextureTest/TextureTest2.png", 1);

        //使用的着色器程序
        shader.Use();
        //设置Shader的纹理，2个纹理都要设置
        shader.SetInt("texture1", 0);
        shader.SetInt("texture2", 1);

        //开启深度测试
        glEnable(GL_DEPTH_TEST);

        //定义正方体的10个位置
        glm::vec3 cubePositions[] = {
                glm::vec3( 0.0f,  0.0f,  0.0f),
                glm::vec3( 2.0f,  5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.0f, -12.3f),
                glm::vec3( 2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f,  3.0f, -7.5f),
                glm::vec3( 1.3f, -2.0f, -2.5f),
                glm::vec3( 1.5f,  2.0f, -2.5f),
                glm::vec3( 1.5f,  0.2f, -1.5f),
                glm::vec3(-1.3f,  1.0f, -1.5f)
        };

        //观察空间-观察矩阵
        glm::mat4 view = glm::mat4(1.0f);;
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        //齐次裁剪空间-投影矩阵
        glm::mat4 projection = glm::mat4(1.0f);;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        
        //渲染循环
        while(!glfwWindowShouldClose(window))
        {
            //监听按键是否按下
            processInput(window);
            //清屏、清除深度缓冲
            GlClearAndDepth();
            
            //必须每帧绑定下
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TextureIdx1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, TextureIdx2);

            //渲染10个同样的正方体
            for (unsigned int i = 0 ; i < 10 ; i++) {

                glm::mat4 model = glm::mat4(1.0f);
                //移动
                model = glm::translate(model, cubePositions[i]);
                //旋转
                model = glm::rotate(model, (float)glfwGetTime() * glm::radians((i + 1) * 20.0f), glm::vec3(0.5f, 1.0f, 0.0f));
                
                shader.SetMatrix("model", model);
                shader.SetMatrix("view", view);
                shader.SetMatrix("projection", projection);

                //按照三角形绘制顶点
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

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

private:
    //生成VAO/VBO
    void GenMatrixTestData(unsigned int* VAO, unsigned int* VBO)
    {
        //定义一个顶点数组，位置+纹理坐标
        float vertices[] = {
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        //生成VAO/VBO
        glGenVertexArrays(1, VAO);
        glGenBuffers(1, VBO);

        //绑定VAO/VBO
        glBindVertexArray(*VAO);
        glBindBuffer(GL_ARRAY_BUFFER, *VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //设置顶点位置
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        //设置顶点纹理
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    //生成纹理
    void GenMatrixTexture(unsigned int* TextureIdx, char const *filename, int count)
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
};