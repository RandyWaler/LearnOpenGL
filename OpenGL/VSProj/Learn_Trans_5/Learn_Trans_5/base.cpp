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





//相机
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

glm::vec3 cameraRo = glm::vec3(0.0f, 0.0f, 0.0f); //Heading(Y) --- Pitch(X) --- Bank(Z) 轴旋转

glm::mat4 view = glm::mat4(1.0f);//观察矩阵（相机Trans矩阵的逆矩阵）

glm::mat4 cmaTrans = glm::mat4(1.0f);;//相机的Transfom矩阵

glm::mat4 cmaRo = glm::mat4(1.0f);;//相机的旋转矩阵

double cmaMoveSpeed = 5.0f;//相机每秒的移动速度

double cmaRoSpeed = 480.0f;//相机每单位像素运动的旋转速度

void cmaInit();//相机初始化

void cmaUpdate();//生成相机的Transfomr矩阵 和 观察矩阵 


//MainCon 引擎总控制

void framebuffer_size_callback(GLFWwindow* window, int width, int height); //响应windows 动态调整窗口大小的函数

void processInput(GLFWwindow* window, int key, int scancode, int action, int mode);//输入回调事件

inline void resetMat4(glm::mat4& r);//重置为单位矩阵

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;


double lastTime;

double dt;//时间差值

void MainInit();

void MainUpdate();//应用程序阶段最先运行，用于处理每帧的一些通用参数甚设置

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
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    //注册回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, processInput);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//捕捉鼠标（鼠标不显示）

    glfwSetCursorPosCallback(window, mouse_callback);//注册回调函数
    

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



    //方块
    //float vertices[] = {
    //    //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
    //         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上0
    //         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下1
    //        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下2
    //        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上3
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


    //立方体
    float vertices[] = {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
             0.5f,  0.5f, 0.5f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 前右上0
             0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // 前右下1
            -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // 前左下2
            -0.5f,  0.5f, 0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // 前左上3
             0.5f,  0.5f,  -0.5f,   0.0f, 0.0f, 0.0f,  1.0f, 0.0f,    // 后右上4
             0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,    // 后右下5
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f,    // 后左下6
            -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,     // 后左上7

            0.5f,  0.5f,  -0.5f,   0.0f, 0.0f, 0.0f,  0.0f, 1.0f,    // 后右上8(4) 为使得右面贴图正确
            0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,    // 后右下9(5) 
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f,    // 后左下10(6) 为使得左面贴图正确
            -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,    // 后左上11(7)

    };

    unsigned int idxs[] = {
        0,3,1,1,3,2, //正面
        4,7,0,0,7,3, //顶面
        3,11,2,2,11,10, //左面
        7,4,6,6,4,5, //背面
        8,0,9,9,0,1, //右面
        1,2,5,5,2,6  //底面
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


    
    
    unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform"); //查找Uniform属性地址
    unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view"); //查找Uniform属性地址
    unsigned int perspectiveLoc = glGetUniformLocation(ourShader.ID, "perspective"); //查找Uniform属性地址

    glm::mat4 trans_ro;//旋转矩阵
    
    glm::mat4 perspective = glm::mat4(1.0f); //!!!!!!!!! 必须先用单位矩阵初始化

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f)); //必选先初始化为单位矩阵(无位移无旋转无缩放) 
    perspective = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


    glm::vec3 cubePositions[] = { //更多的箱子
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



    //渲染启动前

    ourShader.use(); // 不要忘记在设置uniform变量之前激活着色器程序！
    
    //设置纹理 uniform 属性 ，保证其与纹理单元的编号对应
    ourShader.setInt("ourTexture", 0); //这里名称要与 .fs 着色器中声明 uniform 属性时一致
    ourShader.setInt("ourTexture2", 1); 

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);//设置Uniform属性

    glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, glm::value_ptr(perspective));//设置Uniform属性

    glEnable(GL_DEPTH_TEST);//启用深度测试


    //开启背面剔除
    glCullFace(GL_BACK); //设置剔除面（默认是背面）
    glEnable(GL_CULL_FACE); //激活剔除

    //GL_BACK 剔除背面
    //GL_FRONT 剔除正面
    //GL_FRONT_AND_BACK 正面背面都剔除


    glm::mat4 trans;


    MainInit();
    cmaInit();
    


    //启动渲染
    while (!glfwWindowShouldClose(window)) //每次检查是否要求窗口退出
    {
        glfwPollEvents(); //检查触发事件，调用响应函数  我们的framebuffer_size_callback会被调用
        MainUpdate();
        //processInput(window); //检测输入事件
        

        //动画物理等等等


        cmaUpdate();


        //渲染事件（渲染开始前，这一帧要渲染的全部内容已经全部确定）

        glClearColor(0.2f, 0.2f, 0.3f, 1.0f); //设置清屏颜色
        //glClear(GL_COLOR_BUFFER_BIT); //清空后置颜色缓冲区 --- 填充为glClearColor设置的清屏颜色

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//每帧清空颜色缓冲 并重置深度缓冲 （|与操作组合mask）

        //渲染箱子

        

        glActiveTexture(GL_TEXTURE0);// 在绑定纹理之前先激活纹理单元（0号纹理单元默认被激活）
        glBindTexture(GL_TEXTURE_2D, texture);

        glActiveTexture(GL_TEXTURE1); //激活1号纹理单元并绑定纹理
        glBindTexture(GL_TEXTURE_2D, texture2);

       
        ourShader.use();//查询不需要在use之后，但设置属性必须在use之后
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));//设置相机的观察矩阵


        //为了动态变换就必须动态计算 Trasnform 动态设置Uniform属性

        //变换########################################################################################

        //resetMat4(trans_ro);

        //trans_ro = glm::rotate(trans_ro, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));; //必选先初始化为单位矩阵(无位移无旋转无缩放)
        

        //旋转方块
        //trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));//再将坐标转回 (0,0) 为中心的坐标系下
        //trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f)); //新坐标系下绕原点旋转(绕(0.5,0)旋转)
        //trans = glm::translate(trans, glm::vec3(-0.5f, 0.0f, 0.0f));// 转入以 (0.5,0) 为原点的新坐标系
        //trans = glm::rotate(trans, (float)glfwGetTime()*3, glm::vec3(0.0f, 0.0f, 1.0f)); //自旋(以原点/自身轴心为中心)旋转
        //trans = glm::scale(trans, glm::vec3(0.3, 0.3, 0.3)); //原位（以原点/自身轴心为中心）缩放

        //旋转箱子(立方体)

        //trans_ro = glm::rotate(trans_ro, (float)glfwGetTime()/4.0f, glm::vec3(0.0f, 0.0f, 1.0f));//Bank
        //trans_ro = glm::rotate(trans_ro, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));//Pitch
        //trans_ro = glm::rotate(trans_ro, (float)glfwGetTime()/2.0f, glm::vec3(0.0f, 1.0f, 0.0f));//Heading


        //这里的赋值 GLM会自动将矩阵相乘（原矩阵左乘新矩阵），返回的结果是一个包括了多个变换的变换矩阵

        //由于GL使用列向量右乘，因此真正的变换顺序会是反序的效果    
        // trans = A; trans = B; trans = C;
        //从而 trans = ABC
        // trans x vec = ABC x vec   （顺序C->B->A）

        
        
        //地址，1个矩阵，不需要转置(T)处理(针对习惯行向量右乘的开发者),将我们的变换矩阵转换到 GL 接受的形式
        
        

       
        //进行箱子的渲染

        glBindVertexArray(VAO);//绑定VAO
        //glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0); 
        //渲染三角带

        for (int i = 0; i < 10;++i) {

            resetMat4(trans_ro);
            trans_ro = glm::rotate(trans_ro, (float)glfwGetTime()*2/(i+1.0f), glm::vec3(0.0f, 0.0f, 1.0f));//Bank
            trans_ro = glm::rotate(trans_ro, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));//Pitch
            trans_ro = glm::rotate(trans_ro, (float)glfwGetTime()/(i+1.0f), glm::vec3(0.0f, 1.0f, 0.0f));//Heading

            resetMat4(trans);
            trans = glm::translate(trans, cubePositions[i]);
            //trans = trans_ro; //这里矩阵与矩阵组合应该用相乘，而不是赋值

            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans*trans_ro));//设置Uniform属性
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //渲染单个三角型
            //GL_TRIANGLES 单个三角型(逆时针卷绕为正面)

            //GL_TRIANGLE_STRIP 三角带 一逆一顺为正面的模式

            //GL_TRIANGLE_FAN 三角扇，每两点与始心点逆时针卷入为正
        }


        

        

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
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0, 0, width, height); //重定义窗口大小
}




void processInput(GLFWwindow* window, int key, int scancode, int action, int mode)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //检测摁下 Esc
        glfwSetWindowShouldClose(window, true); //关闭窗口

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

void cmaInit() //生成相机的Transfom矩阵
{
    cmaTrans = glm::translate(cmaTrans, cameraPos); //T移动

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
    //相机运动
    if (flag_W)//向前移动
        cameraPos += glm::vec3(cmaTrans[2][0] * cmaMoveSpeed * dt, cmaTrans[2][1] * cmaMoveSpeed * dt, cmaTrans[2][2] * cmaMoveSpeed * dt);
    else if (flag_S)//向后移动
        cameraPos -= glm::vec3(cmaTrans[2][0] * cmaMoveSpeed * dt, cmaTrans[2][1] * cmaMoveSpeed * dt, cmaTrans[2][2] * cmaMoveSpeed * dt);
    if (flag_A)//向左移动
        cameraPos += glm::vec3(cmaTrans[0][0] * cmaMoveSpeed * dt, cmaTrans[0][1] * cmaMoveSpeed * dt, cmaTrans[0][2] * cmaMoveSpeed * dt);
    else if (flag_D)//向右移动
        cameraPos -= glm::vec3(cmaTrans[0][0] * cmaMoveSpeed * dt, cmaTrans[0][1] * cmaMoveSpeed * dt, cmaTrans[0][2] * cmaMoveSpeed * dt);
    if (flag_Q)//向上移动
        cameraPos.y += cmaMoveSpeed * dt;
    else if (flag_E)//向下移动
        cameraPos.y -= cmaMoveSpeed * dt;

    //相机旋转

    if (nmY > lastY) mouseVDir = -1;//向下移动了
    else if (nmY < lastY) mouseVDir = 1;//向上移动了
    else mouseVDir = 0;
    
    

    if (nmX > lastX) mouseHDir = 1;//向右移动了
    else if (nmX < lastX) mouseHDir = -1;
    else mouseHDir = 0;

    lastY = nmY;
    lastX = nmX;


    //上下旋转Pitch

    
    dtro = mouseVDir * dt * cmaRoSpeed;
    glm::clamp(dtro, -maxdt, maxdt);
    cameraRo.x += dtro;

    //cameraRo.x += 45.0f*dt;

    //cameraRo.x = glm::sin(glfwGetTime()) * 30.0f;

    //std::cout << glm::sin(glfwGetTime()) << std::endl;

    //左右旋转Heading

    dtro = mouseHDir * dt * cmaRoSpeed;
    glm::clamp(dtro, -maxdt, maxdt);
    cameraRo.y -= dtro;


    if (cameraRo.x > 85.0f) cameraRo.x = 85.0f;
    else if (cameraRo.x < -85.0f) cameraRo.x = -85.0f;


    //生成当前的相机Transfrom矩阵

    resetMat4(cmaTrans);
    cmaTrans = glm::translate(cmaTrans, cameraPos); //T移动


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

    
    
    
    
    // 《3D游戏数学》 大部分博主原来的想法  --- 必错！！！

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



    //生成View观察矩阵
    resetMat4(view);

    view = glm::transpose(cmaRo);

    view = glm::translate(view, cameraPos); //T移动

    //view = view*glm::translate(glm::mat4(1.0f), cameraPos);


}

void MainInit()
{
    lastTime = glfwGetTime();
}

void MainUpdate()
{
    dt = glfwGetTime() - lastTime; //计算时差
    lastTime = glfwGetTime();

    //std::cout << dt << std::endl;



}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    //注意这里Y是反向的，从顶-->底 增大

    nmX = xpos;
    nmY = ypos;

    

   // std::cout << mouseVDir << std::endl;

}



