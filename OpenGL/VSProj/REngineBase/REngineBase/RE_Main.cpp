#include "RE_Main.h"

using namespace std;
using namespace RE;

//static初始化
bool RE_Main::onGame = true;
RE_Main* RE_Main::_instance = nullptr;



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




    //Our BaseInit

    dt = 0.0f;
    _LastTime = glfwGetTime();

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
    _LastTime = glfwGetTime();
    beforeRender();
    render();

    return true;

}


//游戏逻辑相关
void RE::RE_Main::gameLogic()
{
    glfwPollEvents(); //检查触发输入事件，调用响应函数 
    refreshDt();//刷新时差

}


//游戏逻辑全部运行结束
//这一帧渲染内容已经确定，或应在该函数中做最终确定
//准备渲染相关的变量
void RE::RE_Main::beforeRender()
{

}


//进行渲染
void RE::RE_Main::render()
{
    //清屏
    glClearColor(clearColor[0],clearColor[1],clearColor[2],clearColor[3]); //设置清屏颜色（状态设置）
    glClear(GL_COLOR_BUFFER_BIT); //清空后置颜色缓冲区 --- 填充为glClearColor设置的清屏颜色

    //渲染注册的对象

    glfwSwapBuffers(window); //双缓冲呈递
}

void RE::RE_Main::relese()
{
    glfwTerminate();//退出时释放资源
    delete _instance;

}



void RE::RE_Main::logMessage(std::string s,bool gl)
{
	if (gl) cout << "\n-------------------------\n" << endl;
	cout << s << endl;
	if(gl) cout << "\n-------------------------" << endl;


}

void RE_Main::run()
{

	if (!reInit()) { //初始化失败
		logMessage("Error!!! REngine Init Fail\n Program exited");
        _GameOver();
	}

	while (reUpdate());

	relese();
}

void RE::RE_Main::setScreenWH(int w, int h)
{
    if (w <= 0 || h <= 0) {
        logMessage("Error!!! setScreenWH get bad param");
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


//键盘输入事件
void RE::processInput(GLFWwindow* window, int key, int scancode, int action, int mode) 
{


}
