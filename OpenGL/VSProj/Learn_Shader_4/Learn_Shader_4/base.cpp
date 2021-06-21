#include <glad.h>
#include <glfw3.h>
#include <iostream>

#include "Shader.h"

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



    //Triangle

    Shader ourShader("baseVShader.vs", "baseFShader.fs");






    //############################################################################################################
    //���������������(Vertex Array Object, VAO) �� VAO��ϳ�VBO�Ķ������ݣ��Լ��������붥�����Ե�����
    //��Ⱦ����ʱֻ��Ҫ�󶨽��VAO����������Ⱦ���ɣ������ظ��������붥������

    //����һ��VAO����
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);//����һ�����󻺳� ����һ������ ID �Ƕ�һ�޶��ģ����Ҵ�1��ʼ
    //ע����������洴��VBOʱ�õ���glGenBuffers��ͬ

    glBindVertexArray(VAO);//��VAO��֮�����õ�VBO���ݺ������������ûᱻ��¼�����VAO��



    //����һ����������(Vertex Data),������������(Vertex)
    //ÿ������ֻ��λ����Ϣ
    //����λ�� �淶���豸����(NDC)��

    // ģ�Ϳռ� ---�� ����ռ� ---���۲�ռ� ---�� ��βü��ռ� ---�� NDC ---�� ��Ļ�ռ� 

    /*float vertices[] = {
    -0.5f, -0.5f, 0.5f,
     0.5f, -0.5f, 0.5f,
     0.0f, 0.5f, 0.5f
    };*/

    float vertices[] = {
        // λ��              // ��ɫ
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
    };


    unsigned int idxs[] = {
        0,1,2
    };

    //����һ�����㻺�����(Vertex Buffer Objects, VBO) ���ڹ����Դ�
    unsigned int VBO;
    glGenBuffers(1, &VBO); //����һ�����󻺳� �����ɶ�����������󶨵��� uint ����ID

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    unsigned int EBO;
    glGenBuffers(1, &EBO);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//�󶨻������¼������
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idxs), idxs, GL_STATIC_DRAW);

    //���ö�������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));

    //ÿ��������Ϣ����   vec3 Pos + vec3 colorRGB (6��float)
    //����pos  0�ţ�3�� float������Ҫ��׼�������� 6��float����ʼƫ��0
    //����colorRGB  1�ţ�3�� float������Ҫ��׼�������� 6��float����ʼƫ�� 3��float����


    glEnableVertexAttribArray(0);//���� 0 ��ID�Ķ�������
    glEnableVertexAttribArray(1);//���� 0 ��ID�Ķ�������
    




    //��Ⱦǰ



    //������Ⱦ
    while (!glfwWindowShouldClose(window)) //ÿ�μ���Ƿ�Ҫ�󴰿��˳�
    {
        //processInput(window); //��������¼�
        glfwPollEvents(); //��鴥���¼���������Ӧ����  ���ǵ�framebuffer_size_callback�ᱻ����

        //��Ⱦ�¼�

        glClearColor(0.2f, 0.2f, 0.3f, 1.0f); //����������ɫ
        glClear(GL_COLOR_BUFFER_BIT); //��պ�����ɫ������ --- ���ΪglClearColor���õ�������ɫ


        //��Ⱦ������
        //float timeValue = glfwGetTime(); //��ȡ��Ϸ��������
        //float greenValue = (sin(timeValue) / 2.0f) + 0.5f; //sin��
        //static int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //��ѯuniform ���� ourColor �ĵ�ַ����
        //δ�ҵ�����-1

        ourShader.use(); //ע�⣡���������ѯ��ַ��������Ҫ UseProgram �����ñ����� UseProgram֮��

        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); //����ourColor ����
        //��ַ����
        //����ourColor �Ĳ�������ΪourColor��һ��vec4����Ҫ4��float����
        //glUniform4f ע������ "4f" ��Ҫ4��float �� Ҳ������ "fv" һ������

        glBindVertexArray(VAO);//��VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);//��Ⱦ������



        glfwSwapBuffers(window); //˫����ʵ�
        
    }

    //
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


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


