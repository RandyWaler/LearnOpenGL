#include "RE_Main.h"

using namespace std;
using namespace RE;

//static��ʼ��
bool RE_Main::onGame = true;
RE_Main* RE_Main::_instance = nullptr;



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




    //Our BaseInit

    dt = 0.0f;
    _LastTime = glfwGetTime();

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
    _LastTime = glfwGetTime();
    beforeRender();
    render();

    return true;

}


//��Ϸ�߼����
void RE::RE_Main::gameLogic()
{
    glfwPollEvents(); //��鴥�������¼���������Ӧ���� 
    refreshDt();//ˢ��ʱ��

}


//��Ϸ�߼�ȫ�����н���
//��һ֡��Ⱦ�����Ѿ�ȷ������Ӧ�ڸú�����������ȷ��
//׼����Ⱦ��صı���
void RE::RE_Main::beforeRender()
{

}


//������Ⱦ
void RE::RE_Main::render()
{
    //����
    glClearColor(clearColor[0],clearColor[1],clearColor[2],clearColor[3]); //����������ɫ��״̬���ã�
    glClear(GL_COLOR_BUFFER_BIT); //��պ�����ɫ������ --- ���ΪglClearColor���õ�������ɫ

    //��Ⱦע��Ķ���

    glfwSwapBuffers(window); //˫����ʵ�
}

void RE::RE_Main::relese()
{
    glfwTerminate();//�˳�ʱ�ͷ���Դ
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

	if (!reInit()) { //��ʼ��ʧ��
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


//���������¼�
void RE::processInput(GLFWwindow* window, int key, int scancode, int action, int mode) 
{


}
