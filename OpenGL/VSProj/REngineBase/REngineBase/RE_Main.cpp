#include "RE_Main.h"

using namespace std;
using namespace RE;

//static初始化
//bool RE_Main::onGame = true;
RE_Main* RE_Main::_instance = nullptr;



RE::RE_Main::RE_Main()
{
}

bool RE_Main::reInit()
{
    
    //OpenGL Base

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //MAJOR-主版本设置为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //MINOR-次版本设置为3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //使用 OPENGL Core-profile 核心模式
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Mac OS X 系统

    //创建窗口

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        logMessage("Failed to create GLFW window", true);
        return false;
    }
    glfwMakeContextCurrent(window);

    //初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        logMessage("Failed to initialize GLAD", true);
        return false;
    }

    //设置视口大小
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    //注册回调函数
    glfwSetKeyCallback(window, processInput);
    glfwSetCursorPosCallback(window, mouse_callback);//注册回调函数



    //Our BaseInit



    //开启背面剔除

    glEnable(GL_DEPTH_TEST);//启用深度测试

    glCullFace(GL_BACK); //设置剔除面（默认是背面）
    glEnable(GL_CULL_FACE); //激活剔除

    return true;

}

bool RE_Main::reUpdate()
{


    if(!RE_Main::onGame) return false; //游戏逻辑层已要求游戏退出
    
    if (glfwWindowShouldClose(window)) { //win32 要求游戏退出（例如任务管理器，或者用户关闭窗口）
        _GameOver();
        return false;
    }
    
    gameLogic();
    beforeRender();
    _LastTime = glfwGetTime();
    render();
    
    return true;

}


//游戏逻辑相关
void RE::RE_Main::gameLogic()
{
    glfwPollEvents(); //检查触发输入事件，调用响应函数 
    refreshDt();//刷新时差


    for (auto n : nodes) n->onGameLogic();//调用所有节点的 gameLogic 逻辑


}


//游戏逻辑全部运行结束
//这一帧渲染内容已经确定，或应在该函数中做最终确定
//准备渲染相关的变量
void RE::RE_Main::beforeRender()
{
    for (auto n : nodes) n->onBeforeRender();//调用所有节点的 onBeforeRender 逻辑


}


//进行渲染
void RE::RE_Main::render()
{
    //清屏
    glClearColor(clearColor[0],clearColor[1],clearColor[2],clearColor[3]); //设置清屏颜色（状态设置）
    glClear(GL_COLOR_BUFFER_BIT); //清空后置颜色缓冲区 --- 填充为glClearColor设置的清屏颜色


    for (auto n : nodes) n->onRender();//调用所有节点的 onRender 逻辑

    //渲染注册的对象

    glfwSwapBuffers(window); //双缓冲呈递
}

void RE::RE_Main::frameOver()
{
    for (auto n : nodes) n->onFrameOver();//调用所有节点的 onFrameOver 逻辑
}

void RE::RE_Main::relese()
{

    //释放所有注册的节点

    for (auto n : nodes) {
        delete n;
    }

    delete _instance;

    glfwTerminate();//退出时释放资源
    

}

RE_Main* RE::RE_Main::getInstance()
{
    return _instance;
}

void RE::RE_Main::logMessage(std::string s,bool gl)
{
	if (gl) cout << "\n-------------------------\n" << endl;
	cout << s << endl;
	if(gl) cout << "\n-------------------------" << endl;


}

void RE::RE_Main::resetMat4(glm::mat4 & r)
{
    r[0][0] = 1.0f; r[0][1] = 0.0f; r[0][2] = 0.0f; r[0][3] = 0.0f;
    r[1][0] = 0.0f; r[1][1] = 1.0f; r[1][2] = 0.0f; r[1][3] = 0.0f;
    r[2][0] = 0.0f; r[2][1] = 0.0f; r[2][2] = 1.0f; r[2][3] = 0.0f;
    r[3][0] = 0.0f; r[3][1] = 0.0f; r[3][2] = 0.0f; r[3][3] = 1.0f;
}
void RE_Main::run()
{

	if (!reInit()) { //初始化失败
		logMessage("Error!!! REngine Init Fail\nProgram exited",true);
        _GameOver();
	}
    dt = 0.0f;
    _LastTime = glfwGetTime();
	while (reUpdate());

	relese();
}

void RE::RE_Main::setScreenWH(int w, int h)
{
    if (w <= 0 || h <= 0) {
        logMessage("Error!!! setScreenWH get bad param",true);
        return;
    }

    SCR_HEIGHT = w;
    SCR_HEIGHT = h;
}

void RE::RE_Main::refreshDt()
{
    dt = glfwGetTime() - _LastTime;
}

void RE::RE_Main::_GameOver()
{
    onGame = false;
    logMessage("GameOver");
}

void RE::RE_Main::_InitFail()
{
    flInit = false;
}

bool RE::RE_Main::isInitFail()
{
    return flInit;
}


//键盘输入事件
void RE::processInput(GLFWwindow* window, int key, int scancode, int action, int mode) 
{
    for (auto i : RE_Main::getInstance()->ipt) { //调用所有注册的键盘输入事件
        i->onInput(window); 
    }

}

void RE::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    for (auto i : RE_Main::getInstance()->mos) {
        i->onMouse(window, xpos, ypos);
    }
}
