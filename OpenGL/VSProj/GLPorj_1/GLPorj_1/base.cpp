#include <glad.h>
#include <glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //��Ӧwindows ��̬�������ڴ�С�ĺ���

void processInput(GLFWwindow* window, int key, int scancode, int action, int mode);//����ص��¼�

int main()
{
    //��ʼ��
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //MAJOR-���汾����Ϊ3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //MINOR-�ΰ汾����Ϊ3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //ʹ�� OPENGL Core-profile ����ģʽ
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Mac OS X ϵͳ

    //��������

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //��ʼ��glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //�����ӿڴ�С
    glViewport(0, 0, 800, 600);

    //ע��ص�����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, processInput);

    //������Ⱦ
    while (!glfwWindowShouldClose(window)) //ÿ�μ���Ƿ�Ҫ�󴰿��˳�
    {
        //processInput(window); //��������¼�
        glfwPollEvents(); //��鴥���¼���������Ӧ����  ���ǵ�framebuffer_size_callback�ᱻ����

        //��Ⱦ�¼�

        glClearColor(0.2f, 0.2f, 0.3f, 1.0f); //����������ɫ
        glClear(GL_COLOR_BUFFER_BIT); //��պ�����ɫ������ --- ���ΪglClearColor���õ�������ɫ

        glfwSwapBuffers(window); //˫����ʵ�
        
    }

    glfwTerminate();//�˳�ʱ�ͷ���Դ

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); //�ض��崰�ڴ�С
}

void processInput(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //������� Esc
        glfwSetWindowShouldClose(window, true); //�رմ���
}



