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





//���
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

glm::vec3 cameraRo = glm::vec3(0.0f, 0.0f, 0.0f); //Heading(Y) --- Pitch(X) --- Bank(Z) ����ת

glm::mat4 view = glm::mat4(1.0f);//�۲�������Trans����������

glm::mat4 cmaTrans = glm::mat4(1.0f);;//�����Transfom����

glm::mat4 cmaRo = glm::mat4(1.0f);;//�������ת����

double cmaMoveSpeed = 5.0f;//���ÿ����ƶ��ٶ�

double cmaRoSpeed = 480.0f;//���ÿ��λ�����˶�����ת�ٶ�

void cmaInit();//�����ʼ��

void cmaUpdate();//���������Transfomr���� �� �۲���� 


//MainCon �����ܿ���

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //��Ӧwindows ��̬�������ڴ�С�ĺ���

void processInput(GLFWwindow* window, int key, int scancode, int action, int mode);//����ص��¼�

inline void resetMat4(glm::mat4& r);//����Ϊ��λ����

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;


double lastTime;

double dt;//ʱ���ֵ

void MainInit();

void MainUpdate();//Ӧ�ó���׶��������У����ڴ���ÿ֡��һЩͨ�ò���������

void mouse_callback(GLFWwindow* window, double xpos, double ypos);



// -- MainCon Key

bool flag_W = false;
bool flag_S = false;
bool flag_A = false;
bool flag_D = false;
bool flag_Q = false;
bool flag_E = false;

// -- MainCon Mouse

float lastX = SCR_WIDTH/2, lastY = SCR_HEIGHT/2;
float nmX = lastX;
float nmY = lastY;


int mouseHDir = 0;
int mouseVDir = 0;


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
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    //ע��ص�����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, processInput);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//��׽��꣨��겻��ʾ��

    glfwSetCursorPosCallback(window, mouse_callback);//ע��ص�����
    

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



    //����
    //float vertices[] = {
    //    //     ---- λ�� ----       ---- ��ɫ ----     - �������� -
    //         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����0
    //         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����1
    //        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����2
    //        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����3
    //};

    //unsigned int idxs[] = {
    //    0,1,3,2
    //};


    /*
    
          7--------4
         /|       /|
        / |      / |
       /  |     /  |
      3---|----0   |
      |  6     |   5
      | /      |  / 
      |/       | /  
      2________1/
    */


    //������
    float vertices[] = {
        //     ---- λ�� ----       ---- ��ɫ ----     - �������� -
             0.5f,  0.5f, 0.5f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ǰ����0
             0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // ǰ����1
            -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // ǰ����2
            -0.5f,  0.5f, 0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // ǰ����3
             0.5f,  0.5f,  -0.5f,   0.0f, 0.0f, 0.0f,  1.0f, 0.0f,    // ������4
             0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,    // ������5
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f,    // ������6
            -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,     // ������7

            0.5f,  0.5f,  -0.5f,   0.0f, 0.0f, 0.0f,  0.0f, 1.0f,    // ������8(4) Ϊʹ��������ͼ��ȷ
            0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,    // ������9(5) 
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f,    // ������10(6) Ϊʹ��������ͼ��ȷ
            -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,    // ������11(7)

    };

    unsigned int idxs[] = {
        0,3,1,1,3,2, //����
        4,7,0,0,7,3, //����
        3,11,2,2,11,10, //����
        7,4,6,6,4,5, //����
        8,0,9,9,0,1, //����
        1,2,5,5,2,6  //����
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


    
    
    unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform"); //����Uniform���Ե�ַ
    unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view"); //����Uniform���Ե�ַ
    unsigned int perspectiveLoc = glGetUniformLocation(ourShader.ID, "perspective"); //����Uniform���Ե�ַ

    glm::mat4 trans_ro;//��ת����
    
    glm::mat4 perspective = glm::mat4(1.0f); //!!!!!!!!! �������õ�λ�����ʼ��

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f)); //��ѡ�ȳ�ʼ��Ϊ��λ����(��λ������ת������) 
    perspective = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


    glm::vec3 cubePositions[] = { //���������
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

    /*glm::mat4 dw = glm::mat4(1.0f);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << dw[i][j] << "\t";
        }
        std::cout << std::endl;
    }*/



    //��Ⱦ����ǰ

    ourShader.use(); // ��Ҫ����������uniform����֮ǰ������ɫ������
    
    //�������� uniform ���� ����֤��������Ԫ�ı�Ŷ�Ӧ
    ourShader.setInt("ourTexture", 0); //��������Ҫ�� .fs ��ɫ�������� uniform ����ʱһ��
    ourShader.setInt("ourTexture2", 1); 

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);//����Uniform����

    glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, glm::value_ptr(perspective));//����Uniform����

    glEnable(GL_DEPTH_TEST);//������Ȳ���


    //���������޳�
    glCullFace(GL_BACK); //�����޳��棨Ĭ���Ǳ��棩
    glEnable(GL_CULL_FACE); //�����޳�

    //GL_BACK �޳�����
    //GL_FRONT �޳�����
    //GL_FRONT_AND_BACK ���汳�涼�޳�


    glm::mat4 trans;


    MainInit();
    cmaInit();
    


    //������Ⱦ
    while (!glfwWindowShouldClose(window)) //ÿ�μ���Ƿ�Ҫ�󴰿��˳�
    {
        glfwPollEvents(); //��鴥���¼���������Ӧ����  ���ǵ�framebuffer_size_callback�ᱻ����
        MainUpdate();
        //processInput(window); //��������¼�
        

        //��������ȵȵ�


        cmaUpdate();


        //��Ⱦ�¼�����Ⱦ��ʼǰ����һ֡Ҫ��Ⱦ��ȫ�������Ѿ�ȫ��ȷ����

        glClearColor(0.2f, 0.2f, 0.3f, 1.0f); //����������ɫ
        //glClear(GL_COLOR_BUFFER_BIT); //��պ�����ɫ������ --- ���ΪglClearColor���õ�������ɫ

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//ÿ֡�����ɫ���� ��������Ȼ��� ��|��������mask��

        //��Ⱦ����

        

        glActiveTexture(GL_TEXTURE0);// �ڰ�����֮ǰ�ȼ�������Ԫ��0������ԪĬ�ϱ����
        glBindTexture(GL_TEXTURE_2D, texture);

        glActiveTexture(GL_TEXTURE1); //����1������Ԫ��������
        glBindTexture(GL_TEXTURE_2D, texture2);

       
        ourShader.use();//��ѯ����Ҫ��use֮�󣬵��������Ա�����use֮��
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));//��������Ĺ۲����


        //Ϊ�˶�̬�任�ͱ��붯̬���� Trasnform ��̬����Uniform����

        //�任########################################################################################

        //resetMat4(trans_ro);

        //trans_ro = glm::rotate(trans_ro, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));; //��ѡ�ȳ�ʼ��Ϊ��λ����(��λ������ת������)
        

        //��ת����
        //trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));//�ٽ�����ת�� (0,0) Ϊ���ĵ�����ϵ��
        //trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f)); //������ϵ����ԭ����ת(��(0.5,0)��ת)
        //trans = glm::translate(trans, glm::vec3(-0.5f, 0.0f, 0.0f));// ת���� (0.5,0) Ϊԭ���������ϵ
        //trans = glm::rotate(trans, (float)glfwGetTime()*3, glm::vec3(0.0f, 0.0f, 1.0f)); //����(��ԭ��/��������Ϊ����)��ת
        //trans = glm::scale(trans, glm::vec3(0.3, 0.3, 0.3)); //ԭλ����ԭ��/��������Ϊ���ģ�����

        //��ת����(������)

        //trans_ro = glm::rotate(trans_ro, (float)glfwGetTime()/4.0f, glm::vec3(0.0f, 0.0f, 1.0f));//Bank
        //trans_ro = glm::rotate(trans_ro, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));//Pitch
        //trans_ro = glm::rotate(trans_ro, (float)glfwGetTime()/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));//Heading


        //����ĸ�ֵ GLM���Զ���������ˣ�ԭ��������¾��󣩣����صĽ����һ�������˶���任�ı任����

        //����GLʹ���������ҳˣ���������ı任˳����Ƿ����Ч��    
        // trans = A; trans = B; trans = C;
        //�Ӷ� trans = ABC
        // trans x vec = ABC x vec   ��˳��C->B->A��

        
        
        //��ַ��1�����󣬲���Ҫת��(T)����(���ϰ���������ҳ˵Ŀ�����),�����ǵı任����ת���� GL ���ܵ���ʽ
        
        

       
        //�������ӵ���Ⱦ

        glBindVertexArray(VAO);//��VAO
        //glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0); 
        //��Ⱦ���Ǵ�

        for (int i = 0; i < 10;++i) {

            resetMat4(trans_ro);
            trans_ro = glm::rotate(trans_ro, (float)glfwGetTime()*2/(i+1.0f), glm::vec3(0.0f, 0.0f, 1.0f));//Bank
            trans_ro = glm::rotate(trans_ro, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));//Pitch
            trans_ro = glm::rotate(trans_ro, (float)glfwGetTime()/(i+1.0f), glm::vec3(0.0f, 1.0f, 0.0f));//Heading

            resetMat4(trans);
            trans = glm::translate(trans, cubePositions[i]);
            //trans = trans_ro; //���������������Ӧ������ˣ������Ǹ�ֵ

            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans*trans_ro));//����Uniform����
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //��Ⱦ����������
            //GL_TRIANGLES ����������(��ʱ�����Ϊ����)

            //GL_TRIANGLE_STRIP ���Ǵ� һ��һ˳Ϊ�����ģʽ

            //GL_TRIANGLE_FAN �����ȣ�ÿ������ʼ�ĵ���ʱ�����Ϊ��
        }


        

        

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
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height); //�ض��崰�ڴ�С
}




void processInput(GLFWwindow* window, int key, int scancode, int action, int mode)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //������� Esc
        glfwSetWindowShouldClose(window, true); //�رմ���

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) flag_W = true;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) flag_W = false;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) flag_S = true;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) flag_S = false;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) flag_A = true;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) flag_A = false;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) flag_D = true;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) flag_D = false;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) flag_Q = true;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE) flag_Q = false;

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) flag_E = true;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) flag_E = false;

   /* if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        std::cout << "PRESS" << std::endl;
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_REPEAT)
        std::cout << "REPEAT" << std::endl;
    else std::cout << "NOTHING" << std::endl;*/

}

inline void resetMat4(glm::mat4& r)
{
    r[0][0] = 1.0f; r[0][1] = 0.0f; r[0][2] = 0.0f; r[0][3] = 0.0f;
    r[1][0] = 0.0f; r[1][1] = 1.0f; r[1][2] = 0.0f; r[1][3] = 0.0f;
    r[2][0] = 0.0f; r[2][1] = 0.0f; r[2][2] = 1.0f; r[2][3] = 0.0f;
    r[3][0] = 0.0f; r[3][1] = 0.0f; r[3][2] = 0.0f; r[3][3] = 1.0f;
}

void cmaInit() //���������Transfom����
{
    cmaTrans = glm::translate(cmaTrans, cameraPos); //T�ƶ�

    glm::mat4 heading = glm::mat4(1.0f);
    heading = glm::rotate(heading, glm::radians(cameraRo.y), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 pitch = glm::mat4(1.0f);
    pitch = glm::rotate(pitch, glm::radians(cameraRo.x), glm::vec3(heading[0][0], heading[0][1], heading[0][2]));//Pitch



    cmaTrans = glm::rotate(cmaTrans, glm::radians(cameraRo.z), glm::vec3(pitch[0][0], pitch[0][1], pitch[0][2]));//Bank
    cmaTrans = glm::rotate(cmaTrans, glm::radians(cameraRo.x), glm::vec3(heading[0][0], heading[0][1], heading[0][2]));//Pitch
    cmaTrans = glm::rotate(cmaTrans, glm::radians(cameraRo.y), glm::vec3(0.0f, 1.0f, 0.0f));//Heading


}

glm::mat4 pitch = glm::mat4(1.0f);
float dtro;

float maxdt = 10.0f;

void cmaUpdate()
{
    //����˶�
    if (flag_W)//��ǰ�ƶ�
        cameraPos += glm::vec3(cmaTrans[2][0] * cmaMoveSpeed * dt, cmaTrans[2][1] * cmaMoveSpeed * dt, cmaTrans[2][2] * cmaMoveSpeed * dt);
    else if (flag_S)//����ƶ�
        cameraPos -= glm::vec3(cmaTrans[2][0] * cmaMoveSpeed * dt, cmaTrans[2][1] * cmaMoveSpeed * dt, cmaTrans[2][2] * cmaMoveSpeed * dt);
    if (flag_A)//�����ƶ�
        cameraPos += glm::vec3(cmaTrans[0][0] * cmaMoveSpeed * dt, cmaTrans[0][1] * cmaMoveSpeed * dt, cmaTrans[0][2] * cmaMoveSpeed * dt);
    else if (flag_D)//�����ƶ�
        cameraPos -= glm::vec3(cmaTrans[0][0] * cmaMoveSpeed * dt, cmaTrans[0][1] * cmaMoveSpeed * dt, cmaTrans[0][2] * cmaMoveSpeed * dt);
    if (flag_Q)//�����ƶ�
        cameraPos.y += cmaMoveSpeed * dt;
    else if (flag_E)//�����ƶ�
        cameraPos.y -= cmaMoveSpeed * dt;

    //�����ת

    if (nmY > lastY) mouseVDir = -1;//�����ƶ���
    else if (nmY < lastY) mouseVDir = 1;//�����ƶ���
    else mouseVDir = 0;
    
    

    if (nmX > lastX) mouseHDir = 1;//�����ƶ���
    else if (nmX < lastX) mouseHDir = -1;
    else mouseHDir = 0;

    lastY = nmY;
    lastX = nmX;


    //������תPitch

    
    dtro = mouseVDir * dt * cmaRoSpeed;
    glm::clamp(dtro, -maxdt, maxdt);
    cameraRo.x += dtro;

    //cameraRo.x += 45.0f*dt;

    //cameraRo.x = glm::sin(glfwGetTime()) * 30.0f;

    //std::cout << glm::sin(glfwGetTime()) << std::endl;

    //������תHeading

    dtro = mouseHDir * dt * cmaRoSpeed;
    glm::clamp(dtro, -maxdt, maxdt);
    cameraRo.y -= dtro;


    if (cameraRo.x > 85.0f) cameraRo.x = 85.0f;
    else if (cameraRo.x < -85.0f) cameraRo.x = -85.0f;


    //���ɵ�ǰ�����Transfrom����

    resetMat4(cmaTrans);
    cmaTrans = glm::translate(cmaTrans, cameraPos); //T�ƶ�


    //resetMat4(cmaRo);

    //glm::mat4 heading = glm::mat4(1.0f);
    //heading = glm::rotate(heading, glm::radians(cameraRo.y), glm::vec3(0.0f, 1.0f, 0.0f));

    //glm::mat4 pitch = glm::mat4(1.0f);
    //pitch = glm::rotate(pitch, glm::radians(cameraRo.x), glm::vec3(heading[0][0], heading[0][1], heading[0][2]));//Pitch



    //cmaRo = glm::rotate(cmaRo, glm::radians(cameraRo.z), glm::vec3(pitch[0][0], pitch[0][1], pitch[0][2]));//Bank
    //cmaRo = glm::rotate(cmaRo, glm::radians(cameraRo.x), glm::vec3(heading[0][0], heading[0][1], heading[0][2]));//Pitch
    //cmaRo = glm::rotate(cmaRo, glm::radians(cameraRo.y), glm::vec3(0.0f, 1.0f, 0.0f));//Heading


    resetMat4(cmaRo);

    ////Heading
    cmaRo = glm::rotate(cmaRo, glm::radians(cameraRo.y), glm::vec3(0.0f, 1.0f, 0.0f));

    //std::cout << "\n---------------------" << std::endl;

    //std::cout << cameraRo.y << std::endl;

    //for (int ia = 0; ia < 4; ++ia) {
    //    for (int ja = 0; ja < 4; ++ja) {
    //        std::cout << cmaRo[ia][ja] << "\t";
    //    }
    //    std::cout << std::endl;
    //}

    //std::cout << "---------------------\n" << std::endl;


    resetMat4(pitch);

    pitch = glm::rotate(pitch, glm::radians(cameraRo.x), glm::vec3(cmaRo[0][0], cmaRo[0][1], cmaRo[0][2])); //Pitch

    cmaRo = pitch * cmaRo;

    /*glm::mat4 bank = glm::mat4(1.0f);

    bank = glm::rotate(bank, glm::radians(cameraRo.z), glm::vec3(cmaRo[0][2], cmaRo[1][2], cmaRo[2][2]));

    cmaRo = bank * cmaRo;*/


    //std::cout << glm::dot(glm::vec3(cmaRo[0][0], cmaRo[1][0], cmaRo[2][0]), glm::vec3(cmaRo[0][1], cmaRo[1][1], cmaRo[2][1])) << std::endl;
    //std::cout << glm::dot(glm::vec3(cmaRo[0][0], cmaRo[1][0], cmaRo[2][0]), glm::vec3(cmaRo[0][2], cmaRo[1][2], cmaRo[2][2])) << std::endl;
    //std::cout << glm::dot(glm::vec3(cmaRo[0][1], cmaRo[1][1], cmaRo[2][1]), glm::vec3(cmaRo[0][2], cmaRo[1][2], cmaRo[2][2])) << std::endl;

    
    
    
    
    // ��3D��Ϸ��ѧ�� �󲿷ֲ���ԭ�����뷨  --- �ش�����

    //resetMat4(cmaRo);

    //cmaRo = glm::rotate(cmaRo, glm::radians(cameraRo.y), glm::vec3(0.0f, 1.0f, 0.0f));//Heading


    //glm::mat4 pitch = glm::mat4(1.0f);

    //pitch = glm::rotate(pitch, glm::radians(cameraRo.x), glm::vec3(1.0f, 0.0f, 0.0f)); //Pitch

    //cmaRo *= pitch; //* cmaRo;

    //glm::mat4 bank = glm::mat4(1.0f);

    //bank = glm::rotate(bank, glm::radians(cameraRo.z), glm::vec3(0.0f, 0.0f, 1.0f));


    //cmaRo *= bank; //* cmaRo;


    //ERROE

    //resetMat4(cmaRo);

    //cmaRo = glm::rotate(cmaRo, glm::radians(cameraRo.y), glm::vec3(0.0f, 1.0f, 0.0f));//Heading
    //cmaRo = glm::rotate(cmaRo, glm::radians(cameraRo.x), glm::vec3(1.0f, 0.0f, 0.0f));//Pitch
    //cmaRo = glm::rotate(cmaRo, glm::radians(cameraRo.z), glm::vec3(0.0f, 0.0f, 1.0f));//Bank
    
    



    cmaTrans *= cmaRo;



    //����View�۲����
    resetMat4(view);

    view = glm::transpose(cmaRo);

    view = glm::translate(view, cameraPos); //T�ƶ�

    //view = view*glm::translate(glm::mat4(1.0f), cameraPos);


}

void MainInit()
{
    lastTime = glfwGetTime();
}

void MainUpdate()
{
    dt = glfwGetTime() - lastTime; //����ʱ��
    lastTime = glfwGetTime();

    //std::cout << dt << std::endl;



}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //ע������Y�Ƿ���ģ��Ӷ�-->�� ����

    nmX = xpos;
    nmY = ypos;

    

   // std::cout << mouseVDir << std::endl;

}



