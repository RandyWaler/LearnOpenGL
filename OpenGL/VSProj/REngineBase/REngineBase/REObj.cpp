#include "REObj.h"

#include "RE_Main.h"

#include "RECamera.h"


using namespace RE;

void RE::REObj::crearBuffer()
{
    //����һ��VAO����
    glGenVertexArrays(1, &VAO);//����һ�����󻺳� ����һ������ ID �Ƕ�һ�޶��ģ����Ҵ�1��ʼ
    //ע����������洴��VBOʱ�õ���glGenBuffers��ͬ

    glBindVertexArray(VAO);//��VAO��֮�����õ�VBO���ݺ������������ûᱻ��¼�����VAO��

    //����һ�����㻺�����(Vertex Buffer Objects, VBO) ���ڹ����Դ�
    glGenBuffers(1, &VBO); //����һ�����󻺳� �����ɶ�����������󶨵��� uint ����ID

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*VBOCount, vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//�󶨻������¼������
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * verticesCount, idxs, GL_STATIC_DRAW);


    
}

RE::REObj::REObj(glm::vec3 p, glm::mat4 ro, glm::vec3 sc):RENode(p,ro,sc)
{
    ourShader = nullptr;
    vertices = nullptr;
    idxs = nullptr;

    flMesh = false;

    vsTransName = string("transform"); //vs Shader �ж���ľ�������Ĭ��
    vsViewName = string("view");
    vsPersName = string("perspective");

}

RE::REObj::~REObj()
{
    delete ourShader;
    if (flMesh) { //ȷ�ϴ���������
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
        //     ---- λ�� ----       ---- ��ɫ ----     - �������� -
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // ǰ����0
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // ǰ����1
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,   // ǰ����2
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // ǰ����3
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // ������4
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // ������5
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,    // ������6
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,     // ������7

            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,    // ������8(4) Ϊʹ��������ͼ��ȷ
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // ������9(5) 
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // ������10(6) Ϊʹ��������ͼ��ȷ
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // ������11(7)
    };

    obj->idxs = new unsigned int[] {
        0, 3, 1, 1, 3, 2, //����
            4, 7, 0, 0, 7, 3, //����
            3, 11, 2, 2, 11, 10, //����
            7, 4, 6, 6, 4, 5, //����
            8, 0, 9, 9, 0, 1, //����
            1, 2, 5, 5, 2, 6  //����
    };


    obj->crearBuffer();

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


    //����������һ��shader��uniform�������

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
    if (!ourShader || !flMesh) return;//û��Shader �� û��flMesh ��������Ⱦ

    ourShader->use();//��ѯ����Ҫ��use֮�󣬵��������Ա�����use֮��

    //��������

    int n = tex2DS.size();

    for (int i = 0; i < n; ++i) { //����&������
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, tex2DS[i]);
    }

    //���þ���
    
    if (change) {
        ourShader->setMat4(vsTransName, world);// std::cout << "set world" << std::endl;
    }
    if(mrdViewChange) ourShader->setMat4(vsViewName,mrdView);
    if (mrdPersChange) ourShader->setMat4(vsPersName, mrdPerspective);

    glDrawElements(primitiveModel, verticesCount, GL_UNSIGNED_INT, 0);

    //RE_Main::logMessage("on Render");


}
