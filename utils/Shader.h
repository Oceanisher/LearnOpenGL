#ifndef SHADER_H
#define SHADER_H
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//读取文件内容
std::string Read(const char* path)
{
    std::ifstream file;
    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        file.open(path);
        std::stringstream stream;
        // read file's buffer contents into streams
        stream << file.rdbuf();
        // close file handlers
        file.close();
        // convert stream into string
        return stream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    return NULL;
}

//Shader程序类
//读取两个Shader、创建Program
class Shader
{
public :
    unsigned int Idx;
    Shader(const char* vPath, const char* fPath)
    {
        //读取顶点shader
        std::string vStr = Read(vPath);
        const char* vContent = vStr.c_str();
        //读取片段shader
        std::string fStr = Read(fPath);
        const char* fContent = fStr.c_str();
        
        //Shader处理
        unsigned int vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vContent, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fContent, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        //Program处理
        // shader Program
        Idx = glCreateProgram();
        glAttachShader(Idx, vertex);
        glAttachShader(Idx, fragment);
        glLinkProgram(Idx);
        checkCompileErrors(Idx, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    };
    
    //使用
    void Use()
    {
        glUseProgram(Idx);
    }
    
    //删除
    void Delete()
    {
        glDeleteProgram(Idx);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};