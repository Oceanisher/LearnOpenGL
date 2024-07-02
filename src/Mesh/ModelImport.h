//
// Created by 羡鱼 on 2024/7/2.
//

#ifndef TESTOPENGL_MODELIMPORT_H
#define TESTOPENGL_MODELIMPORT_H

#endif //TESTOPENGL_MODELIMPORT_H

#include "../../include/MeshInfo.h"
#include "../../include/ModelInfo.h"
#include "../../include/LearnOpenGLHeader.h"
#include <iostream>
#include <filesystem>
using namespace std::filesystem;

//鼠标X轴位置
float MouseXPos;
//鼠标Z轴位置
float MouseYPos;
//鼠标滚轮缩放
float MouseScrollOffset;

//测试模型导入
class ModelImport
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

        //开启深度测试
        glEnable(GL_DEPTH_TEST);

        //鼠标监听
        MouseListener(window);

        //因为OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部。
        //很幸运，stb_image.h能够在图像加载时帮助我们翻转y轴，只需要在加载任何图像前加入以下语句
        stbi_set_flip_vertically_on_load(true);

        //使用的shader
        Shader ourShader("src/Mesh/ModelImporter.vsh", "src/Mesh/ModelImporter.fsh");

        //导入模型
        Model ourModel(filesystem::path("resources/objects/backpack/backpack.obj"));

        //渲染循环
        while(!glfwWindowShouldClose(window))
        {
            //监听按键是否按下
            InputProc(window);
            //清屏、清除深度缓冲
            GlClearAndDepth();

            //鼠标处理
            MouseProc();
            
            //处理shader
            ourShader.Use();

            //齐次裁剪空间-投影矩阵
            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(MouseFovScroll += MouseScrollOffset), 800.0f / 600.0f, 0.1f, 100.0f);
            MouseScrollOffset = 0.0f;

            //观察空间-观察矩阵
            glm::mat4 view = glm::lookAt(CamPos, CamPos + CamStep, CamUp);

            //模型矩阵
            glm::mat4 model = glm::mat4(1.0f);
            //移动
            model = glm::translate(model, glm::vec3(0.0f));

            ourShader.SetMatrix("model", model);
            ourShader.SetMatrix("view", view);
            ourShader.SetMatrix("projection", projection);

            ourModel.Draw(ourShader);

            //交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
            glfwSwapBuffers(window);
            //检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）
            glfwPollEvents();
        }

        ourShader.Delete();

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
        glfwSetCursorPosCallback(window, &ModelImport::MouseCallback);
        glfwSetScrollCallback(window, &ModelImport::MouseScrollCallback);
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
