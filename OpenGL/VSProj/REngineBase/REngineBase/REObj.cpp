#include "REObj.h"

#include "RE_Main.h"

#include "RECamera.h"


using namespace RE;

void RE::REObj::crearBuffer()
{
    //创建一个VAO对象
    glGenVertexArrays(1, &VAO);//创建一个对象缓冲 ，第一个参数 ID 是独一无二的，并且从1开始
    //注意这里和下面创建VBO时用到的glGenBuffers不同

    glBindVertexArray(VAO);//绑定VAO（之后设置的VBO数据和输入属性配置会被记录到这个VAO）

    //创建一个顶点缓冲对象(Vertex Buffer Objects, VBO) 用于管理显存
    glGenBuffers(1, &VBO); //创建一个对象缓冲 ，生成对象的数量，绑定到的 uint 对象ID

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*VBOCount, vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//绑定缓冲对象，录入数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * verticesCount, idxs, GL_STATIC_DRAW);


    
}

RE::REObj::REObj(glm::vec3 p, glm::mat4 ro, glm::vec3 sc):RENode(p,ro,sc)
{
    ourShader = nullptr;
    vertices = nullptr;
    idxs = nullptr;

    flMesh = false;

    vsTransName = string("transform"); //vs Shader 中定义的矩阵名称默认
    vsViewName = string("view");
    vsPersName = string("perspective");

}

RE::REObj::~REObj()
{
    delete ourShader;
    if (flMesh) { //确认搭载了网格
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

        delete []vertices;
        delete []idxs;
    }

    for (auto i : tex2DS) {
        glDeleteTextures(1,&i);
    }
}

void RE::REObj::creatShader(std::string vsShader, std::string fsShader)
{
    ourShader = new Shader(vsShader, fsShader);

}



void RE::REObj::creatBox(REObj* obj)
{
    obj -> flMesh = true;

    obj->primitiveModel = GL_TRIANGLES;
    obj->verticesCount = 36;
    obj->VBOCount = 96;


    obj->vertices = new float[] {
        //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 前右上0
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // 前右下1
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // 前左下2
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // 前左上3
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // 后右上4
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // 后右下5
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,    // 后左下6
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,     // 后左上7

            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,    // 后右上8(4) 为使得右面贴图正确
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // 后右下9(5) 
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // 后左下10(6) 为使得左面贴图正确
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // 后左上11(7)
    };

    obj->idxs = new unsigned int[] {
        0, 3, 1, 1, 3, 2, //正面
            4, 7, 0, 0, 7, 3, //顶面
            3, 11, 2, 2, 11, 10, //左面
            7, 4, 6, 6, 4, 5, //背面
            8, 0, 9, 9, 0, 1, //右面
            1, 2, 5, 5, 2, 6  //底面
    };


    obj->crearBuffer();

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


    //这里先设置一次shader的uniform属性相关

    if (!obj->ourShader) {
        RE_Main::logMessage("Error!!! Creat Base Obj but no Shdaer\nVS Camere Uniform Set Fail", true);
        return;
    }
    obj->ourShader->use();

    obj->ourShader->setMat4(obj->vsTransName, obj->world);
    obj->ourShader->setMat4(obj->vsViewName, RE::mrdView);
    obj->ourShader->setMat4(obj->vsPersName, RE::mrdPerspective);
}

void RE::REObj::onRender()
{
    if (!ourShader || !flMesh) return;//没有Shader 或 没有flMesh 不进行渲染

    ourShader->use();//查询不需要在use之后，但设置属性必须在use之后

    //纹理设置

    int n = tex2DS.size();

    for (int i = 0; i < n; ++i) { //激活&绑定纹理
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, tex2DS[i]);
    }

    //设置矩阵
    
    if (change) {
        ourShader->setMat4(vsTransName, world);// std::cout << "set world" << std::endl;
    }
    if(mrdViewChange) ourShader->setMat4(vsViewName,mrdView);
    if (mrdPersChange) ourShader->setMat4(vsPersName, mrdPerspective);

    glDrawElements(primitiveModel, verticesCount, GL_UNSIGNED_INT, 0);

    //RE_Main::logMessage("on Render");


}
