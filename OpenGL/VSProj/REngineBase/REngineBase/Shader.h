#pragma once

#include <glad.h> // ����glad����ȡ���еı���OpenGLͷ�ļ�

//C++ sting �ļ��� �����
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//��ѧ��
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    // ����ID
    unsigned int ID; //shaderProgrm ID

    // ��������ȡ��������ɫ��
    Shader(const std::string vertexPath, const std::string fragmentPath); //��������Ҫ��ȡ�Ķ�����ɫ��&Ƭ��Ԫ��ɫ��
    ~Shader();
    // ʹ��/�������
    void use();
    // uniform���ߺ���
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, glm::mat4 value) const;

};

