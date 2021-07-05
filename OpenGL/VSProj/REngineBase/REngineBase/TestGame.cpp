#include "TestGame.h"

//数学库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shader文件加载
#include "Shader.h"

//图片加载
#define STB_IMAGE_IMPLEMENTATION //只键.h中入被引用的代码部分
#include "stb_image.h"

using namespace std;
using namespace RE;

//软重写
bool TestGame::reInit()
{
    if (!RE_Main::reInit()) return false;



    RENode* nod = new RENode();

   
    //logMessage("TestGame Init Succeed");
    return true;
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
    if (!onGame) {
        logMessage("Error!!! The game is over");
        return nullptr;
    }
    if (!_instance) _instance = new TestGame();
    return _instance;
}


