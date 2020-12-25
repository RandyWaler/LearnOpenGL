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



