//Base
#include <glad.h>
#include <glfw3.h>
#include <iostream>


//��ѧ��
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shader�ļ�����
#include "Shader.h"

//ͼƬ����
#define STB_IMAGE_IMPLEMENTATION //ֻ��.h���뱻���õĴ��벿��
#include "stb_image.h"

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



    //��Ⱦ����

    //���� shader

    Shader ourShader("TexShader.vs", "TexShader.fs");
    
    //GL �����������

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);//��Ϊһ��2D����


    //��������Ļ���&�˲���ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //ST(XY) ����ΪREPEAT �ظ�����ģʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //�Ŵ�&��С������Ϊ�����˲�ģʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //ͨ�� stb_image ��������

    //��������
    int width, height, nrChannels;
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //��һ����������Ŀ�� ָ��Ϊ 2D���� ��֮ǰ�󶨵�1D/3D������Ӱ�죩��
        //          �Ӷ��������ý�������֮ǰ�󶨵�2D������� texture��Ϊ�丽��������

        //�ڶ�������MipMap���ɼ���
        //��������������洢��ʽ  RGB ��λ��ɫ
        //��������������
        //������������Ϊ0����ʷ��������
        //���߲���������Դ�ĸ�ʽ
        //�ڰ˲������������� GL_UNSIGNED_BYTE ����Ϊchar(byte)����
        //�ھŲ�����ͼ������
        glGenerateMipmap(GL_TEXTURE_2D);//MipMap ����ͨ�����ӵڶ������������ glGenerateMipmap ����
    }
    else std::cout << "Failed to load texture" << std::endl;

    stbi_image_free(data);//�����������Լ�MipMap��ͼ������data���Ա��ͷŵ�


    //����Ц��
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);//��Ϊһ��2D����


    //��������Ļ���&�˲���ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //ST(XY) ����ΪREPEAT �ظ�����ģʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //�Ŵ�&��С������Ϊ�����˲�ģʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //ͨ�� stb_image ��������

    //����Ц��
    stbi_set_flip_vertically_on_load(true);//��תy�ᣬƥ��GL��������
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //��һ����������Ŀ�� ָ��Ϊ 2D���� ��֮ǰ�󶨵�1D/3D������Ӱ�죩��
        //          �Ӷ��������ý�������֮ǰ�󶨵�2D������� texture��Ϊ�丽��������

        //�ڶ�������MipMap���ɼ���
        //��������������洢��ʽ  GL_RGBA .png ��͸��ͨ��
        //��������������
        //������������Ϊ0����ʷ��������
        //���߲���������Դ�ĸ�ʽ
        //�ڰ˲������������� GL_UNSIGNED_BYTE ����Ϊchar(byte)����
        //�ھŲ�����ͼ������
        glGenerateMipmap(GL_TEXTURE_2D);//MipMap ����ͨ�����ӵڶ������������ glGenerateMipmap ����
    }
    else std::cout << "Failed to load texture" << std::endl;

    stbi_image_free(data);//�����������Լ�MipMap��ͼ������data���Ա��ͷŵ�



     //����һ��VAO����
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);//����һ�����󻺳� ����һ������ ID �Ƕ�һ�޶��ģ����Ҵ�1��ʼ
    //ע����������洴��VBOʱ�õ���glGenBuffers��ͬ

    glBindVertexArray(VAO);//��VAO��֮�����õ�VBO���ݺ������������ûᱻ��¼�����VAO��


    float vertices[] = {
        //     ---- λ�� ----       ---- ��ɫ ----     - �������� -
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����0
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����1
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����2
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����3
    };

    unsigned int idxs[] = {
        0,1,3,2
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //λ������
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//��ɫ����
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));//UV����

    //ÿ��������Ϣ����   vec3 Pos + vec3 colorRGB + vec2 UV (8��float)
    //����pos  0�ţ�3�� float������Ҫ��׼�������� 8 ��float����ʼƫ��0
    //����colorRGB  1�ţ�3�� float������Ҫ��׼�������� 8 ��float����ʼƫ�� 3��float����
    //���� UV 2�ţ�2�� float������Ҫ��׼�������� 8 ��float����ʼƫ�� 6��float����


    glEnableVertexAttribArray(0);//���� 0 ��ID�Ķ�������
    glEnableVertexAttribArray(1);//���� 1 ��ID�Ķ�������
    glEnableVertexAttribArray(2);//���� 2 ��ID�Ķ�������


    
    


    

    //��ַ��1�����󣬲���Ҫת��(T)����(���ϰ���������ҳ˵Ŀ�����),�����ǵı任����ת���� GL ���ܵ���ʽ


    //��Ⱦ����ǰ

    ourShader.use(); // ��Ҫ����������uniform����֮ǰ������ɫ������
    
    //�������� uniform ���� ����֤��������Ԫ�ı�Ŷ�Ӧ
    ourShader.setInt("ourTexture", 0); //��������Ҫ�� .fs ��ɫ�������� uniform ����ʱһ��
    ourShader.setInt("ourTexture2", 1); 

    


    //������Ⱦ
    while (!glfwWindowShouldClose(window)) //ÿ�μ���Ƿ�Ҫ�󴰿��˳�
    {
        //processInput(window); //��������¼�
        glfwPollEvents(); //��鴥���¼���������Ӧ����  ���ǵ�framebuffer_size_callback�ᱻ����

        //��Ⱦ�¼�

        glClearColor(0.2f, 0.2f, 0.3f, 1.0f); //����������ɫ
        glClear(GL_COLOR_BUFFER_BIT); //��պ�����ɫ������ --- ���ΪglClearColor���õ�������ɫ


        //��Ⱦ����

        

        glActiveTexture(GL_TEXTURE0);// �ڰ�����֮ǰ�ȼ�������Ԫ��0������ԪĬ�ϱ����
        glBindTexture(GL_TEXTURE_2D, texture);

        glActiveTexture(GL_TEXTURE1); //����1������Ԫ��������
        glBindTexture(GL_TEXTURE_2D, texture2);

       
        ourShader.use();//��ѯ����Ҫ��use֮�󣬵��������Ա�����use֮��



        //Ϊ�˶�̬�任�ͱ��붯̬���� Trasnform ��̬����Uniform����

        //�任########################################################################################

        glm::mat4 trans = glm::mat4(1.0f); //��ѡ�ȳ�ʼ��Ϊ��λ����(��λ������ת������)

        trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));//�ٽ�����ת�� (0,0) Ϊ���ĵ�����ϵ��
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f)); //������ϵ����ԭ����ת(��(0.5,0)��ת)
        trans = glm::translate(trans, glm::vec3(-0.5f, 0.0f, 0.0f));// ת���� (0.5,0) Ϊԭ���������ϵ
        trans = glm::rotate(trans, (float)glfwGetTime()*3, glm::vec3(0.0f, 0.0f, 1.0f)); //����(��ԭ��/��������Ϊ����)��ת
        trans = glm::scale(trans, glm::vec3(0.3, 0.3, 0.3)); //ԭλ����ԭ��/��������Ϊ���ģ�����

        //����ĸ�ֵ GLM���Զ���������ˣ�ԭ��������¾��󣩣����صĽ����һ�������˶���任�ı任����

        //����GLʹ���������ҳˣ���������ı任˳����Ƿ����Ч��    
        // trans = A; trans = B; trans = C;
        //�Ӷ� trans = ABC
        // trans x vec = ABC x vec   ��˳��C->B->A��

        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform"); //����Uniform���Ե�ַ
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));//����Uniform����

       
        //�������ӵ���Ⱦ

        glBindVertexArray(VAO);//��VAO
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0); 
        //��Ⱦ���Ǵ�

        //ע�⣡������ ʹ�� EBO ��Ҫ�� glDrawElements �������� VBO �������� glDrawArrays
        


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



