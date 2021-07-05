#include "RE_Main.h"

using namespace std;
using namespace RE;

//static��ʼ��
//bool RE_Main::onGame = true;
RE_Main* RE_Main::_instance = nullptr;



RE::RE_Main::RE_Main()
{
}

bool RE_Main::reInit()
{
    
    //OpenGL Base

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //MAJOR-���汾����Ϊ3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //MINOR-�ΰ汾����Ϊ3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //ʹ�� OPENGL Core-profile ����ģʽ
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Mac OS X ϵͳ

    //��������

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        logMessage("Failed to create GLFW window", true);
        return false;
    }
    glfwMakeContextCurrent(window);

    //��ʼ��glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        logMessage("Failed to initialize GLAD", true);
        return false;
    }

    //�����ӿڴ�С
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    //ע��ص�����
    glfwSetKeyCallback(window, processInput);
    glfwSetCursorPosCallback(window, mouse_callback);//ע��ص�����



    //Our BaseInit



    //���������޳�

    glEnable(GL_DEPTH_TEST);//������Ȳ���

    glCullFace(GL_BACK); //�����޳��棨Ĭ���Ǳ��棩
    glEnable(GL_CULL_FACE); //�����޳�

    return true;

}

bool RE_Main::reUpdate()
{


    if(!RE_Main::onGame) return false; //��Ϸ�߼�����Ҫ����Ϸ�˳�
    
    if (glfwWindowShouldClose(window)) { //win32 Ҫ����Ϸ�˳�����������������������û��رմ��ڣ�
        _GameOver();
        return false;
    }
    
    gameLogic();
    beforeRender();
    _LastTime = glfwGetTime();
    render();
    
    return true;

}


//��Ϸ�߼����
void RE::RE_Main::gameLogic()
{
    glfwPollEvents(); //��鴥�������¼���������Ӧ���� 
    refreshDt();//ˢ��ʱ��


    for (auto n : nodes) n->onGameLogic();//�������нڵ�� gameLogic �߼�


}


//��Ϸ�߼�ȫ�����н���
//��һ֡��Ⱦ�����Ѿ�ȷ������Ӧ�ڸú�����������ȷ��
//׼����Ⱦ��صı���
void RE::RE_Main::beforeRender()
{
    for (auto n : nodes) n->onBeforeRender();//�������нڵ�� onBeforeRender �߼�


}


//������Ⱦ
void RE::RE_Main::render()
{
    //����
    glClearColor(clearColor[0],clearColor[1],clearColor[2],clearColor[3]); //����������ɫ��״̬���ã�
    glClear(GL_COLOR_BUFFER_BIT); //��պ�����ɫ������ --- ���ΪglClearColor���õ�������ɫ


    for (auto n : nodes) n->onRender();//�������нڵ�� onRender �߼�

    //��Ⱦע��Ķ���

    glfwSwapBuffers(window); //˫����ʵ�
}

void RE::RE_Main::frameOver()
{
    for (auto n : nodes) n->onFrameOver();//�������нڵ�� onFrameOver �߼�
}

void RE::RE_Main::relese()
{

    //�ͷ�����ע��Ľڵ�

    for (auto n : nodes) {
        delete n;
    }

    delete _instance;

    glfwTerminate();//�˳�ʱ�ͷ���Դ
    

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

	if (!reInit()) { //��ʼ��ʧ��
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


//���������¼�
void RE::processInput(GLFWwindow* window, int key, int scancode, int action, int mode) 
{
    for (auto i : RE_Main::getInstance()->ipt) { //��������ע��ļ��������¼�
        i->onInput(window); 
    }

}

void RE::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    for (auto i : RE_Main::getInstance()->mos) {
        i->onMouse(window, xpos, ypos);
    }
}
