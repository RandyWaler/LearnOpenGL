#include "TestGame.h"

//数学库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shader文件加载
#include "Shader.h"

//REngine

#include "REObj.h"
#include "RECamera.h"


using namespace std;
using namespace RE;


glm::vec3 cubePositions[] = { //很多的箱子 它们的位置不同
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


//软重写
bool TestGame::reInit()
{
    if (!RE_Main::reInit()) return false;


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//捕捉鼠标（鼠标不显示）

    //启用欧拉角漫游摄像机
    REFreeCamera::getMainCamera()->setlocalPosition(0.0f, 0.0f, -3.0f);
    REFreeCamera::getMainCamera()->cmaInit();

    //注意相机初始化要在所有物体前面，保证view Pers 矩阵设置正确


    //创建箱子们
    int n = sizeof(cubePositions) / sizeof(glm::vec3);

    REObj* obj;

    for (int i = 0; i < n; ++i) { //创建多个箱子

        REObj* obj = new REObj(cubePositions[i]); //将位置填入构造函数即可

        obj->creatShader("TexShader.vs", "TexShader.fs");

        REObj::creatBox(obj);

        obj->add2DTexture("ourTexture", "container.jpg");
        obj->add2DTexture("ourTexture2", "awesomeface.png", GL_RGBA, true);
    }


    
   
    //logMessage("TestGame Init Succeed");
    return flInit;
}

//硬重写，不调用父render
void TestGame::render()
{
    //清屏
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]); //设置清屏颜色（状态设置）
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清空后置颜色缓冲区 --- 填充为glClearColor设置的清屏颜色


    for (auto n : nodes) n->onRender();

    //渲染注册的对象

    glfwSwapBuffers(window); //双缓冲呈递
}

RE_Main* TestGame::getInstance()
{
    
    if (!_instance) _instance = new TestGame();

    return _instance;
}


