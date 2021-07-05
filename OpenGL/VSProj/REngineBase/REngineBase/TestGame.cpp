#include "TestGame.h"

//��ѧ��
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shader�ļ�����
#include "Shader.h"

//REngine

#include "REObj.h"
#include "RECamera.h"


using namespace std;
using namespace RE;


glm::vec3 cubePositions[] = { //�ܶ������ ���ǵ�λ�ò�ͬ
glm::vec3(0.0f,  0.0f,  0.0f),
glm::vec3(2.0f,  5.0f, -15.0f),
glm::vec3(-1.5f, -2.2f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3(2.4f, -0.4f, -3.5f),
glm::vec3(-1.7f,  3.0f, -7.5f),
glm::vec3(1.3f, -2.0f, -2.5f),
glm::vec3(1.5f,  2.0f, -2.5f),
glm::vec3(1.5f,  0.2f, -1.5f),
glm::vec3(-1.3f,  1.0f, -1.5f)
};


//����д
bool TestGame::reInit()
{
    if (!RE_Main::reInit()) return false;


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//��׽��꣨��겻��ʾ��

    //����ŷ�������������
    REFreeCamera::getMainCamera()->setlocalPosition(0.0f, 0.0f, -3.0f);
    REFreeCamera::getMainCamera()->cmaInit();

    //ע�������ʼ��Ҫ����������ǰ�棬��֤view Pers ����������ȷ


    //����������
    int n = sizeof(cubePositions) / sizeof(glm::vec3);

    REObj* obj;

    for (int i = 0; i < n; ++i) { //�����������

        REObj* obj = new REObj(cubePositions[i]); //��λ�����빹�캯������

        obj->creatShader("TexShader.vs", "TexShader.fs");

        REObj::creatBox(obj);

        obj->add2DTexture("ourTexture", "container.jpg");
        obj->add2DTexture("ourTexture2", "awesomeface.png", GL_RGBA, true);
    }


    
   
    //logMessage("TestGame Init Succeed");
    return flInit;
}

//Ӳ��д�������ø�render
void TestGame::render()
{
    //����
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]); //����������ɫ��״̬���ã�
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //��պ�����ɫ������ --- ���ΪglClearColor���õ�������ɫ


    for (auto n : nodes) n->onRender();

    //��Ⱦע��Ķ���

    glfwSwapBuffers(window); //˫����ʵ�
}

RE_Main* TestGame::getInstance()
{
    
    if (!_instance) _instance = new TestGame();

    return _instance;
}


