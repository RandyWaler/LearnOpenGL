#pragma once

#include <glad.h> // 包含glad来获取所有的必须OpenGL头文件

//C++ sting 文件流 输出流
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // 程序ID
    unsigned int ID; //shaderProgrm ID

    // 构造器读取并构建着色器
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath); //参数：需要读取的顶点着色器&片段元着色器
    // 使用/激活程序
    void use();
    // uniform工具函数
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;


};

