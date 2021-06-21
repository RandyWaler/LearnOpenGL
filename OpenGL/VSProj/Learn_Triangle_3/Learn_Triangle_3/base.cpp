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


    //顶点着色器###########################################################################
    
    
    //_C风格字符串硬编码

    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        //"out vec4 vertexColor; // 为片段着色器指定一个颜色输出\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        //"vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // 把输出变量设置为暗红色\n"
        "}\0";


    //创建顶点着色器对象
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // GL_VERTEX_SHADER 顶点着色器类型

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //绑定源码
    glCompileShader(vertexShader);//编译顶点着色器对象


    //检测顶点着色器编译问题,如果出错输出错误信息
    int  success;
    char infoLog[512] = {0};
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "\n---------------------------------------------" << std::endl;
        std::cout << "【ERROR】 VERTEX_SHADER_COMPILATION_FAILED\n" << infoLog << std::endl;
        std::cout << "\n---------------------------------------------" << std::endl;
    }else std::cout << "VERTEX_SHADER_COMPILATION_SUCCESS" << std::endl;



    //片段元着色器###########################################################################

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        //"in vec4 vertexColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）\n"
        "uniform vec4 ourColor; // 在OpenGL程序代码中设定这个变量\n"
        "void main()\n"
        "{\n"
       // "FragColor = vec4(0.7f, 0.2f, 1.0f, 1.0f);\n"
        "FragColor = ourColor;"
        "}\n";

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "\n---------------------------------------------" << std::endl;
        std::cout << "【ERROR】 FRAGMENT_SHADER_COMPILATION_FAILED\n" << infoLog << std::endl;
        std::cout << "\n---------------------------------------------" << std::endl;
    }
    else std::cout << "FRAGMENT_SHADER_COMPILATION_SUCCESS" << std::endl;


    //创建着色器程序对象（材质对象）#############################################################################
    
    //将多个着色器合并链接，形成一整套渲染的体系

    unsigned int shaderProgram; //创建对象
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader); //装载着色器
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);//进行链接

    //检测是否链接成功
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);


    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "\n---------------------------------------------" << std::endl;
        std::cout << "【ERROR】 SHADER_PROGRAM_LINK_FAILED\n" << infoLog << std::endl;
        std::cout << "\n---------------------------------------------" << std::endl;

    }
    else std::cout << "SHADER_PROGRAM_LINK_SUCCESS" << std::endl;


    
    


    //############################################################################################################
    //创建顶点数组对象(Vertex Array Object, VAO) ， VAO会合成VBO的顶点数据，以及对于输入顶点属性的配置
    //渲染对象时只需要绑定解绑VAO，并调用渲染即可，无需重复配置输入顶点属性

    //创建一个VAO对象
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);//创建一个对象缓冲 ，第一个参数 ID 是独一无二的，并且从1开始
    //注意这里和下面创建VBO时用到的glGenBuffers不同

    glBindVertexArray(VAO);//绑定VAO（之后设置的VBO数据和输入属性配置会被记录到这个VAO）
    


    //定义一个顶点数据(Vertex Data),包含三个顶点(Vertex)
    //每个顶点只有位置信息
    //坐标位于 规范化设备坐标(NDC)下

    // 模型空间 ---》 世界空间 ---》观察空间 ---》 齐次裁剪空间 ---》 NDC ---》 屏幕空间 

    float vertices[] = {
    -0.7f, -0.5f, 0.5f,
     -0.3f, -0.5f, 0.5f,
     -0.5f, 0.0f, 0.5f
    };

    //创建一个顶点缓冲对象(Vertex Buffer Objects, VBO) 用于管理显存
    unsigned int VBO;
    glGenBuffers(1, &VBO); //创建一个对象缓冲 ，生成对象的数量，绑定到的 uint 对象ID

    glBindBuffer(GL_ARRAY_BUFFER, VBO);//将这个对象缓冲绑定为 GL_ARRAY_BUFFER （顶点缓存对象类型）

    //将顶点数据复制到缓冲的内存中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //缓存内存类型，数据尺寸，数据对象，发送模式

    //GL_STATIC_DRAW ：数据不会或几乎不会改变
    //GL_DYNAMIC_DRAW：数据会被改变很多 (确保显卡把数据放在能够高速写入的内存部分)
    //GL_STREAM_DRAW ：数据每次绘制时都会改变 (确保显卡把数据放在能够高速写入的内存部分)


    //指定顶点数据的解析方式&启用顶点属性（配置输入顶点属性）#############################################################

    //有一个顶点属性 ，ID为0 -- 位置pos  由3个float构成

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //由调用时，绑定到GL_ARRAY_BUFFER的缓冲对象决定读取那个缓冲中的数据 -- glBindBuffer(GL_ARRAY_BUFFER, VBO); 

    //第一个参数：与着色器中的 layout (location = 0) 输入属性ID对应，我们希望顶点，发送到这个属性上
    //第二个参数：指定顶点属性的数据大小(数量)，由3个 float构成
    //第三个参数：指定顶点属性的单个数据类型
    //第四个参数：是否希望数据被标准化(Normalize)，TRUE，int映射到[-1,1] uint映射到[0,1]，FALSE不变
    //第五个参数：步长(Stride)，顶点属性读取的数据步长
    //第六个参数：偏移量，需要读取的顶点属性，在数据缓冲中相对起始的偏移位置


    glEnableVertexAttribArray(0); //启用 0 号ID的顶点属性



    //创建另一个黄色，线框长方形的绘制@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    //固定输出黄色的片段元着色器

    const char* fragmentShader_yellowSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(1.0f, 0.85f, 0.0f, 1.0f);\n"
        "}\n";

    unsigned int fragmentShader_yellow;
    fragmentShader_yellow = glCreateShader(GL_FRAGMENT_SHADER);


    glShaderSource(fragmentShader_yellow, 1, &fragmentShader_yellowSource, NULL);
    glCompileShader(fragmentShader_yellow);

    glGetShaderiv(fragmentShader_yellow, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader_yellow, 512, NULL, infoLog);
        std::cout << "\n---------------------------------------------" << std::endl;
        std::cout << "【ERROR】 FRAGMENT_SHADER_YELLOW_COMPILATION_FAILED\n" << infoLog << std::endl;
        std::cout << "\n---------------------------------------------" << std::endl;
    }
    else std::cout << "FRAGMENT_SHADER_YELLOW_COMPILATION_SUCCESS" << std::endl;


    //着色程序
    unsigned int shaderProgram_yellow;
    shaderProgram_yellow = glCreateProgram();

    glAttachShader(shaderProgram_yellow, vertexShader);
    glAttachShader(shaderProgram_yellow, fragmentShader_yellow);
    glLinkProgram(shaderProgram_yellow);

    //检测是否链接成功
    glGetProgramiv(shaderProgram_yellow, GL_LINK_STATUS, &success);


    if (!success) {
        glGetProgramInfoLog(shaderProgram_yellow, 512, NULL, infoLog);
        std::cout << "\n---------------------------------------------" << std::endl;
        std::cout << "【ERROR】 SHADER_PROGRAM_YELLOW_LINK_FAILED\n" << infoLog << std::endl;
        std::cout << "\n---------------------------------------------" << std::endl;

    }
    else std::cout << "SHADER_PROGRAM_YELLOW_LINK_SUCCESS" << std::endl;


    //VAO

    unsigned int VAO2; //创建
    glGenVertexArrays(1, &VAO2);

    glBindVertexArray(VAO2);//绑定


    //顶点&索引

    float vertices2[] = {
        0.8f,0.8f,0.5f, //右上角
        0.8f,0.1f,0.5f, //右下角
        0.1f,0.1f,0.5f, //左下角
        0.1f,0.8f,0.5f, //左上角
    };

    unsigned int indices[] = { // 三角带 一逆一顺 为正面
        2,1,3,0
    };

    //unsigned int indices[] = { // 先顺后逆，反面，会被背面剔除掉
    //   0,1,3,2
    //};

    /*
    
       3--------0
       |        |
       |        |
       |        |
       2--------1
    
    */

    //VBO 保存顶点数据
    unsigned int VBO2;
    glGenBuffers(1, &VBO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2); //绑定缓冲对象，录入数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    //EBO

    unsigned int EBO;
    glGenBuffers(1, &EBO);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//绑定缓冲对象，录入数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //配置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    //启动渲染前

    //链接合成后，着色器可以被删除
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader_yellow);

    //开启背面剔除
    glCullFace(GL_BACK); //设置剔除面（默认是背面）
    glEnable(GL_CULL_FACE); //激活剔除

    //GL_BACK 剔除背面
    //GL_FRONT 剔除正面
    //GL_FRONT_AND_BACK 正面背面都剔除

    //启动渲染
    while (!glfwWindowShouldClose(window)) //每次检查是否要求窗口退出
    {
        //响应输入事件/游戏逻辑
        glfwPollEvents(); //检查触发事件，调用响应函数  我们的framebuffer_size_callback会被调用

        //渲染

        //初始化缓冲区
        glClearColor(0.2f, 0.2f, 0.3f, 1.0f); //设置清屏颜色
        glClear(GL_COLOR_BUFFER_BIT); //清空后置颜色缓冲区 --- 填充为glClearColor设置的清屏颜色

        //渲染对象
        
        //渲染三角型
        float timeValue = glfwGetTime(); //获取游戏运行秒数
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f; //sin震荡
        static int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); 
        //查询uniform 属性 ourColor 的地址索引
        //未找到返回-1

        glUseProgram(shaderProgram); //注意！！！这里查询地址索引不需要 UseProgram 但设置必须在 UseProgram之后

        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); //设置ourColor 属性
        //地址索引
        //构造ourColor 的参数，因为ourColor是一个vec4，需要4个float参数
        //glUniform4f 注意后面的 "4f" 需要4个float ， 也可以用 "fv" 一个向量

        glBindVertexArray(VAO);//绑定VAO
        glDrawArrays(GL_TRIANGLES, 0, 3);//渲染三角型

        //第一个参数：图元类型
        //第二个参数：顶点数组的起始索引
        //第三个参数：绘制三个顶点


        //GL_TRIANGLES 单个三角型(逆时针卷绕为正面)
        
        //GL_TRIANGLE_STRIP 三角带 一逆一顺为正面的模式

        //GL_TRIANGLE_FAN 三角扇，每两点与始心点逆时针卷入为正


        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //绘制为线框



        //渲染矩形
        glUseProgram(shaderProgram_yellow);
        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0); //三角带只需要4个顶点

        //第一个参数：图元类型
        //第二个参数：绘制6个顶点
        //第三个参数：数据类型 (EBO 是 uint 索引)
        //第四个参数：EBO数组的起始索引

       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//取消绘制为线框


        glfwSwapBuffers(window); //双缓冲呈递
        
    }

    //删除缓冲对象

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgram_yellow);

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



