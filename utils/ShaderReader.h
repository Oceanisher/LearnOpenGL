#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//从Shader读取文件内容
class ShaderReader
{
public:
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
};