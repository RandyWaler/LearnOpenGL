#include <glad.h>
#include <glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //响应windows 动态调整窗口大小的函数

void processInput(GLFWwindow* window, int key, int scancode, int action, int mode);//输入回调事件

int main()
{
    //初始化
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //MAJOR-主版本设置为3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //MINOR-次版本设置为3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //使用 OPENGL Core-profile 核心模式
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Mac OS X 系统

    //创建窗口

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //设置视口大小
    glViewport(0, 0, 800, 600);

    //注册回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, processInput);



    //Triangle


    //定义一个顶点数据(Vertex Data),包含三个顶点(Vertex)
    //每个顶点只有位置信息
    //坐标位于 规范化设备坐标(NDC)下

    // 模型空间 ---》 世界空间 ---》观察空间 ---》 齐次裁剪空间 ---》 NDC ---》 屏幕空间 

    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    //创建一个顶点缓冲对象(Vertex Buffer Objects, VBO) 用于管理显存
    unsigned int VBO; 
    glGenBuffers(1, &VBO); //创建一个对象缓冲

    glBindBuffer(GL_ARRAY_BUFFER, VBO);//将这个对象缓冲绑定为 GL_ARRAY_BUFFER （顶点缓存对象类型）

    //将顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //缓存内存类型，数据尺寸，数据对象，发送模式

    //GL_STATIC_DRAW ：数据不会或几乎不会改变
    //GL_DYNAMIC_DRAW：数据会被改变很多 (确保显卡把数据放在能够高速写入的内存部分)
    //GL_STREAM_DRAW ：数据每次绘制时都会改变 (确保显卡把数据放在能够高速写入的内存部分)











    //启动渲染
    while (!glfwWindowShouldClose(window)) //每次检查是否要求窗口退出
    {
        //processInput(window); //检测输入事件
        glfwPollEvents(); //检查触发事件，调用响应函数  我们的framebuffer_size_callback会被调用

        //渲染事件

        glClearColor(0.2f, 0.2f, 0.3f, 1.0f); //设置清屏颜色
        glClear(GL_COLOR_BUFFER_BIT); //清空后置颜色缓冲区 --- 填充为glClearColor设置的清屏颜色

        glfwSwapBuffers(window); //双缓冲呈递
        
    }

    glfwTerminate();//退出时释放资源

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); //重定义窗口大小
}

void processInput(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //检测摁下 Esc
        glfwSetWindowShouldClose(window, true); //关闭窗口
}



