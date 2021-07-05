#include "TestGame.h"

//��ѧ��
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shader�ļ�����
#include "Shader.h"

//ͼƬ����
#define STB_IMAGE_IMPLEMENTATION //ֻ��.h���뱻���õĴ��벿��
#include "stb_image.h"

using namespace std;
using namespace RE;

//����д
bool TestGame::reInit()
{
    if (!RE_Main::reInit()) return false;



    RENode* nod = new RENode();

   
    //logMessage("TestGame Init Succeed");
    return true;
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
    if (!onGame) {
        logMessage("Error!!! The game is over");
        return nullptr;
    }
    if (!_instance) _instance = new TestGame();
    return _instance;
}


