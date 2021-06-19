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



    //Triangle


    //������ɫ��###########################################################################
    
    
    //_C����ַ���Ӳ����

    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";


    //����������ɫ������
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // GL_VERTEX_SHADER ������ɫ������

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //��Դ��
    glCompileShader(vertexShader);//���붥����ɫ������


    //��ⶥ����ɫ����������,����������������Ϣ
    int  success;
    char infoLog[512] = {0};
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "\n---------------------------------------------" << std::endl;
        std::cout << "��ERROR�� VERTEX_SHADER_COMPILATION_FAILED\n" << infoLog << std::endl;
        std::cout << "\n---------------------------------------------" << std::endl;
    }else std::cout << "VERTEX_SHADER_COMPILATION_SUCCESS" << std::endl;



    //Ƭ��Ԫ��ɫ��###########################################################################

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(0.7f, 0.2f, 1.0f, 1.0f);\n"
        "}\n";

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "\n---------------------------------------------" << std::endl;
        std::cout << "��ERROR�� FRAGMENT_SHADER_COMPILATION_FAILED\n" << infoLog << std::endl;
        std::cout << "\n---------------------------------------------" << std::endl;
    }
    else std::cout << "FRAGMENT_SHADER_COMPILATION_SUCCESS" << std::endl;


    //������ɫ��������󣨲��ʶ���#############################################################################
    
    //�������ɫ���ϲ����ӣ��γ�һ������Ⱦ����ϵ

    unsigned int shaderProgram; //��������
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader); //װ����ɫ��
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);//��������

    //����Ƿ����ӳɹ�
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);


    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "\n---------------------------------------------" << std::endl;
        std::cout << "��ERROR�� SHADER_PROGRAM_LINK_FAILED\n" << infoLog << std::endl;
        std::cout << "\n---------------------------------------------" << std::endl;

    }
    else std::cout << "SHADER_PROGRAM_LINK_SUCCESS" << std::endl;


    //���Ӻϳɺ���ɫ�����Ա�ɾ��
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    


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

    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    //����һ�����㻺�����(Vertex Buffer Objects, VBO) ���ڹ����Դ�
    unsigned int VBO;
    glGenBuffers(1, &VBO); //����һ�����󻺳� ����һ������ ID �Ƕ�һ�޶��ģ����Ҵ�1��ʼ

    glBindBuffer(GL_ARRAY_BUFFER, VBO);//��������󻺳��Ϊ GL_ARRAY_BUFFER �����㻺��������ͣ�

    //���������ݸ��Ƶ�������ڴ���
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //�����ڴ����ͣ����ݳߴ磬���ݶ��󣬷���ģʽ

    //GL_STATIC_DRAW �����ݲ���򼸺�����ı�
    //GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶ� (ȷ���Կ������ݷ����ܹ�����д����ڴ沿��)
    //GL_STREAM_DRAW ������ÿ�λ���ʱ����ı� (ȷ���Կ������ݷ����ܹ�����д����ڴ沿��)



    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //ָ���������ݵĽ�����ʽ&���ö������ԣ��������붥�����ԣ�#############################################################

    //��һ���������� ��IDΪ0 -- λ��pos  ��3��float����

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //�ɵ���ʱ���󶨵�GL_ARRAY_BUFFER�Ļ�����������ȡ�Ǹ������е����� -- glBindBuffer(GL_ARRAY_BUFFER, VBO); 

    //��һ������������ɫ���е� layout (location = 0) ��������ID��Ӧ������ϣ�����㣬���͵����������
    //�ڶ���������ָ���������Ե����ݴ�С(����)����3�� float����
    //������������ָ���������Եĵ�����������
    //���ĸ��������Ƿ�ϣ�����ݱ���׼��(Normalize)��TRUE��intӳ�䵽[-1,1] uintӳ�䵽[0,1]��FALSE����
    //���������������(Stride)���������Զ�ȡ�����ݲ���
    //������������ƫ��������Ҫ��ȡ�Ķ������ԣ������ݻ����������ʼ��ƫ��λ��


    glEnableVertexAttribArray(0); //���� 0 ��ID�Ķ�������




    //������Ⱦ
    while (!glfwWindowShouldClose(window)) //ÿ�μ���Ƿ�Ҫ�󴰿��˳�
    {
        //��Ӧ�����¼�/��Ϸ�߼�
        glfwPollEvents(); //��鴥���¼���������Ӧ����  ���ǵ�framebuffer_size_callback�ᱻ����

        //��Ⱦ

        //��ʼ��������
        glClearColor(0.2f, 0.2f, 0.3f, 1.0f); //����������ɫ
        glClear(GL_COLOR_BUFFER_BIT); //��պ�����ɫ������ --- ���ΪglClearColor���õ�������ɫ

        //��Ⱦ����
        
        glUseProgram(shaderProgram); //ָ����Ⱦ����(����)
        glBindVertexArray(VAO);//��VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);//��Ⱦ������

        //��һ��������ͼԪ����
        //�ڶ��������������������ʼ����
        //������������������������


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



