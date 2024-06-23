#ifndef TESTOPENGL_LIGHTNORMAL_H
#define TESTOPENGL_LIGHTNORMAL_H

#endif //TESTOPENGL_LIGHTNORMAL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../include/LearnOpenGLHeader.h"
#include "../../include/Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb_image.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//鼠标X轴位置
float MouseXPos;
//鼠标Z轴位置
float MouseYPos;
//鼠标滚轮缩放
float MouseScrollOffset;

//测试矩阵
class LightNormal
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

        //生成光源
        //生成着色器程序
        Shader shaderSource(
                "src/LightNormal/LightNormal.vsh",
                "src/LightNormal/LightNormalSource.fsh");

        
        //生成目标
        //生成着色器程序
        Shader shaderTarget(
                "src/LightNormal/LightNormal.vsh",
                "src/LightNormal/LightNormal.fsh");

        //生成纹理1、纹理2
        unsigned int TextureIdx1, TextureIdx2;
        GenMatrixTexture(&TextureIdx1, "src/LightNormal/diffuse.png", 0);
//        GenMatrixTexture(&TextureIdx1, "src/TextureTest/TextureTest1.jpg", 0);
        GenMatrixTexture(&TextureIdx2, "src/LightNormal/specular.png", 1);

        //生成VAO/VBO/EBO
        unsigned int lightVAO, targetVAO, VBO;
        GenMatrixTestData(&lightVAO, &targetVAO, &VBO);

        //开启深度测试
        glEnable(GL_DEPTH_TEST);

        //鼠标监听
        MouseListener(window);

        //渲染循环
        while(!glfwWindowShouldClose(window))
        {
            //监听按键是否按下
            InputProc(window);
            //清屏、清除深度缓冲
            GlClearAndDepth();

            //必须每帧绑定下
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, TextureIdx1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, TextureIdx2);
            
            //鼠标处理
            MouseProc();

            //渲染光源
            glm::vec3 lightPos = glm::vec3( 1.2f, 1.0f, 2.0f);
            //使用的着色器程序
            shaderSource.Use();
            //设置Shader的纹理，2个纹理都要设置
            shaderSource.SetInt("texture1", 0);
            shaderSource.SetInt("texture2", 1);
            MatrixProc(shaderSource, lightPos, glm::vec3(0.2f), false);
            //按照三角形绘制顶点
            glDrawArrays(GL_TRIANGLES, 0, 36);

            //渲染目标
            glm::vec3 targetPos = glm::vec3( 1.0f,  1.0f,  1.0f);
            //使用的着色器程序
            shaderTarget.Use();
            //设置Shader的纹理，2个纹理都要设置
            shaderTarget.SetVec3("camPos", CamPos.x, CamPos.y, CamPos.z);
            //每帧变换光照颜色
//            glm::vec3 lightColor;
//            lightColor.x = sin(glfwGetTime() * 2.0f);
//            lightColor.y = sin(glfwGetTime() * 0.7f);
//            lightColor.z = sin(glfwGetTime() * 1.3f);
//            glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // 降低影响
//            glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响
            //设置光照属性
            shaderTarget.SetVec3("light.position",  lightPos.x, lightPos.y, lightPos.z);
            shaderTarget.SetVec3("light.ambient", 0.2f, 0.2f, 0.2f);
            shaderTarget.SetVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
            shaderTarget.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
            //设置物体材质
            shaderTarget.SetInt("material.diffuse",  0);
            shaderTarget.SetInt("material.specular", 1);
            shaderTarget.SetFloat("material.shininess", 64.0f);
            MatrixProc(shaderTarget, targetPos, glm::vec3(1.0f), true);
            //按照三角形绘制顶点
            glDrawArrays(GL_TRIANGLES, 0, 36);

            //交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
            glfwSwapBuffers(window);
            //检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
            glfwPollEvents();
        }

        //释放shader相关
        glDeleteVertexArrays(1, &lightVAO);
        glDeleteVertexArrays(1, &targetVAO);
        glDeleteBuffers(1, &VBO);
        shaderSource.Delete();
        shaderTarget.Delete();

        //释放资源、退出程序
        glfwTerminate();
        return 0;
    }

private:
    float MouseLastXPos;
    float MouseLastYPos;
    float MouseSensitive = 0.05f;
    float Pitch = 0.0f;
    float Yaw = 0.0f;
    bool IsMouseFirst = true;
    float MouseFovScroll = 45.0f;
    float CamSpeed = 2.0f;
    float LastTime = 0.0f;
    glm::vec3 CamPos = glm::vec3(0.0f, 0.0f, 3.0f) ;
    glm::vec3 CamStep = glm::vec3(0.0f, 0.0f, -1.0f) ;
    glm::vec3 CamUp = glm::vec3(0.0f, 1.0f, 0.0f) ;

    //按键输入监听
    void InputProc(GLFWwindow *window)
    {
        float deltaTime = glfwGetTime() - LastTime;
        LastTime = glfwGetTime();
        float speed = CamSpeed * deltaTime;
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            CamPos += speed * CamStep;
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            CamPos -= speed * CamStep;
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            CamPos += speed * glm::normalize(glm::cross(CamUp, CamStep));
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            CamPos -= speed * glm::normalize(glm::cross(CamUp, CamStep));
    }

    //生成光源、目标对象VAO/VBO
    void GenMatrixTestData(unsigned int* lightVAO, unsigned int* targetVAO, unsigned int* VBO)
    {
        //定义一个顶点数组，位置+纹理坐标+法向量
        float vertices[] = {
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

                0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
                0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
                0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
                0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
        };
        
        

        //生成光源VAO
        glGenBuffers(1, VBO);
        glGenVertexArrays(1, lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, *VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindVertexArray(*lightVAO);
        //设置顶点位置
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //设置顶点纹理
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //设置法向量
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        //生成目标VAO
        glGenVertexArrays(1, targetVAO);
        glBindVertexArray(*targetVAO);
        glBindBuffer(GL_ARRAY_BUFFER, *VBO);
        //设置顶点位置
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //设置顶点纹理
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //设置法向量
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
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
        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            if (0 == count)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, *TextureIdx);
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, *TextureIdx);
            }
        }

        //释放原始纹理数据
        stbi_image_free(data);
    }

    //矩阵处理
    void MatrixProc(Shader shader, glm::vec3 cubePosition, glm::vec3 cubeScale, bool rot)
    {
        //齐次裁剪空间-投影矩阵
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(MouseFovScroll += MouseScrollOffset), 800.0f / 600.0f, 0.1f, 100.0f);
        MouseScrollOffset = 0.0f;

        //观察空间-观察矩阵
        glm::mat4 view = glm::lookAt(CamPos, CamPos + CamStep, CamUp);

        //模型矩阵
        glm::mat4 model = glm::mat4(1.0f);
        //移动
        model = glm::translate(model, cubePosition);
        //旋转
        if (rot)
        {
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        }
        //缩放
        model = glm::scale(model, cubeScale);

        shader.SetMatrix("model", model);
        shader.SetMatrix("view", view);
        shader.SetMatrix("projection", projection);
    }

    //鼠标监听
    void MouseProc()
    {
        if (IsMouseFirst)
        {
            IsMouseFirst = false;
            MouseLastXPos = MouseXPos;
            MouseLastYPos = MouseYPos;
        }
        else
        {
            //计算帧的鼠标移动
            float xOffset = MouseLastXPos - MouseXPos;
            float yOffset = MouseLastYPos - MouseYPos;
            MouseLastXPos = MouseXPos;
            MouseLastYPos = MouseYPos;
            Pitch += MouseSensitive * yOffset;
            Yaw += MouseSensitive * xOffset;
        }

        //计算摄像机方向
        glm::vec3 front;
        front.y= sin(glm::radians(Pitch));
        front.z = -cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
        front.x = -cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
        CamStep = glm::normalize(front);
    }

    //鼠标监听
    void MouseListener(GLFWwindow *window)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, &LightNormal::MouseCallback);
        glfwSetScrollCallback(window, &LightNormal::MouseScrollCallback);
    }

    //鼠标监听回调
    static void MouseCallback(GLFWwindow* window, double xPos, double yPos)
    {
        MouseXPos = (float)xPos;
        MouseYPos = (float)yPos;
    }

    //鼠标滚轮回调
    static void MouseScrollCallback(GLFWwindow* window, double xPos, double yPos)
    {
        MouseScrollOffset = yPos;
    }
};