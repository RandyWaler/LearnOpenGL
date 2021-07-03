//Base
#include <glad.h>
#include <glfw3.h>
#include <iostream>


//数学库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shader文件加载
#include "Shader.h"

//图片加载
#define STB_IMAGE_IMPLEMENTATION //只键.h中入被引用的代码部分
#include "stb_image.h"

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



    //渲染对象

    //创建 shader

    Shader ourShader("TexShader.vs", "TexShader.fs");
    
    //GL 创建纹理对象

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);//绑定为一个2D纹理


    //设置纹理的环绕&滤波方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //ST(XY) 设置为REPEAT 重复环绕模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //放大&缩小，设置为线性滤波模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //通过 stb_image 加载纹理

    //加载箱子
    int width, height, nrChannels;
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //第一参数，纹理目标 指定为 2D纹理 （之前绑定的1D/3D纹理不受影响），
        //          从而本次设置将作用于之前绑定的2D纹理对象 texture，为其附加上纹理

        //第二参数，MipMap生成级别
        //第三参数，纹理存储格式  RGB 三位颜色
        //第四五参数，宽高
        //第六参数，总为0的历史遗留设置
        //第七参数，数据源的格式
        //第八参数，数据类型 GL_UNSIGNED_BYTE 储存为char(byte)数组
        //第九参数，图像数据
        glGenerateMipmap(GL_TEXTURE_2D);//MipMap 可以通过增加第二参数，或调用 glGenerateMipmap 生成
    }
    else std::cout << "Failed to load texture" << std::endl;

    stbi_image_free(data);//生成了纹理，以及MipMap后，图像数据data可以被释放掉


    //加载笑脸
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);//绑定为一个2D纹理


    //设置纹理的环绕&滤波方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //ST(XY) 设置为REPEAT 重复环绕模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //放大&缩小，设置为线性滤波模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //通过 stb_image 加载纹理

    //加载笑脸
    stbi_set_flip_vertically_on_load(true);//翻转y轴，匹配GL纹理坐标
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        //第一参数，纹理目标 指定为 2D纹理 （之前绑定的1D/3D纹理不受影响），
        //          从而本次设置将作用于之前绑定的2D纹理对象 texture，为其附加上纹理

        //第二参数，MipMap生成级别
        //第三参数，纹理存储格式  GL_RGBA .png 带透明通道
        //第四五参数，宽高
        //第六参数，总为0的历史遗留设置
        //第七参数，数据源的格式
        //第八参数，数据类型 GL_UNSIGNED_BYTE 储存为char(byte)数组
        //第九参数，图像数据
        glGenerateMipmap(GL_TEXTURE_2D);//MipMap 可以通过增加第二参数，或调用 glGenerateMipmap 生成
    }
    else std::cout << "Failed to load texture" << std::endl;

    stbi_image_free(data);//生成了纹理，以及MipMap后，图像数据data可以被释放掉



     //创建一个VAO对象
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);//创建一个对象缓冲 ，第一个参数 ID 是独一无二的，并且从1开始
    //注意这里和下面创建VBO时用到的glGenBuffers不同

    glBindVertexArray(VAO);//绑定VAO（之后设置的VBO数据和输入属性配置会被记录到这个VAO）


    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上0
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下1
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下2
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上3
    };

    unsigned int idxs[] = {
        0,1,3,2
    };

    //创建一个顶点缓冲对象(Vertex Buffer Objects, VBO) 用于管理显存
    unsigned int VBO;
    glGenBuffers(1, &VBO); //创建一个对象缓冲 ，生成对象的数量，绑定到的 uint 对象ID

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    unsigned int EBO;
    glGenBuffers(1, &EBO);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//绑定缓冲对象，录入数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idxs), idxs, GL_STATIC_DRAW);

    //配置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //位置属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//颜色属性
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));//UV属性

    //每个顶点信息包括   vec3 Pos + vec3 colorRGB + vec2 UV (8个float)
    //对于pos  0号，3个 float，不需要标准化，步长 8 个float，起始偏移0
    //对于colorRGB  1号，3个 float，不需要标准化，步长 8 个float，起始偏移 3个float长度
    //对于 UV 2号，2个 float，不需要标准化，步长 8 个float，起始偏移 6个float长度


    glEnableVertexAttribArray(0);//启用 0 号ID的顶点属性
    glEnableVertexAttribArray(1);//启用 1 号ID的顶点属性
    glEnableVertexAttribArray(2);//启用 2 号ID的顶点属性


    
    


    

    //地址，1个矩阵，不需要转置(T)处理(针对习惯行向量右乘的开发者),将我们的变换矩阵转换到 GL 接受的形式


    //渲染启动前

    ourShader.use(); // 不要忘记在设置uniform变量之前激活着色器程序！
    
    //设置纹理 uniform 属性 ，保证其与纹理单元的编号对应
    ourShader.setInt("ourTexture", 0); //这里名称要与 .fs 着色器中声明 uniform 属性时一致
    ourShader.setInt("ourTexture2", 1); 

    


    //启动渲染
    while (!glfwWindowShouldClose(window)) //每次检查是否要求窗口退出
    {
        //processInput(window); //检测输入事件
        glfwPollEvents(); //检查触发事件，调用响应函数  我们的framebuffer_size_callback会被调用

        //渲染事件

        glClearColor(0.2f, 0.2f, 0.3f, 1.0f); //设置清屏颜色
        glClear(GL_COLOR_BUFFER_BIT); //清空后置颜色缓冲区 --- 填充为glClearColor设置的清屏颜色


        //渲染箱子

        

        glActiveTexture(GL_TEXTURE0);// 在绑定纹理之前先激活纹理单元（0号纹理单元默认被激活）
        glBindTexture(GL_TEXTURE_2D, texture);

        glActiveTexture(GL_TEXTURE1); //激活1号纹理单元并绑定纹理
        glBindTexture(GL_TEXTURE_2D, texture2);

       
        ourShader.use();//查询不需要在use之后，但设置属性必须在use之后



        //为了动态变换就必须动态计算 Trasnform 动态设置Uniform属性

        //变换########################################################################################

        glm::mat4 trans = glm::mat4(1.0f); //必选先初始化为单位矩阵(无位移无旋转无缩放)

        trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));//再将坐标转回 (0,0) 为中心的坐标系下
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f)); //新坐标系下绕原点旋转(绕(0.5,0)旋转)
        trans = glm::translate(trans, glm::vec3(-0.5f, 0.0f, 0.0f));// 转入以 (0.5,0) 为原点的新坐标系
        trans = glm::rotate(trans, (float)glfwGetTime()*3, glm::vec3(0.0f, 0.0f, 1.0f)); //自旋(以原点/自身轴心为中心)旋转
        trans = glm::scale(trans, glm::vec3(0.3, 0.3, 0.3)); //原位（以原点/自身轴心为中心）缩放

        //这里的赋值 GLM会自动将矩阵相乘（原矩阵左乘新矩阵），返回的结果是一个包括了多个变换的变换矩阵

        //由于GL使用列向量右乘，因此真正的变换顺序会是反序的效果    
        // trans = A; trans = B; trans = C;
        //从而 trans = ABC
        // trans x vec = ABC x vec   （顺序C->B->A）

        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform"); //查找Uniform属性地址
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));//设置Uniform属性

       
        //进行箱子的渲染

        glBindVertexArray(VAO);//绑定VAO
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0); 
        //渲染三角带

        //注意！！！！ 使用 EBO 需要用 glDrawElements 方法，而 VBO 绘制则是 glDrawArrays
        


        glfwSwapBuffers(window); //双缓冲呈递
        
    }

    //
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


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



