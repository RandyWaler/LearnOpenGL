//Ϊ��ֹ stb_image �ڲ�ͬcpp�����ã��������ظ�����

//�ֽ�������Ҫ�õ���������ͼƬ���صĴ���ȫ��д�����cpp��

//ͼƬ����
#define STB_IMAGE_IMPLEMENTATION //ֻ��.h���뱻���õĴ��벿��
#include "stb_image.h"

#include "REObj.h"
#include "RE_Main.h"

using namespace RE;


void RE::REObj::add2DTexture(std::string uniName, std::string resName,int dataModel, bool reY)
{

    if (!ourShader) { //δ����Shader�ͼ�������
        RE_Main::logMessage("Error!!! no Shader but add 2dTexture", true);
        RE_Main::getInstance()->_InitFail();
        return;
    }

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
    stbi_set_flip_vertically_on_load(reY);//��תy�ᣬƥ��GL��������
    unsigned char* data = stbi_load(resName.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, dataModel, width, height, 0, dataModel, GL_UNSIGNED_BYTE, data);
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
    else {
        RE_Main::logMessage("Fail load 2D Texture" + resName, true);

        RE_Main::getInstance()->_InitFail();

        return;
    }


    stbi_image_free(data);//�����������Լ�MipMap��ͼ������data���Ա��ͷŵ�

    tex2DS.emplace_back(texture);

    ourShader->use();

    // Ŀǰֻ֧��2D����ֱ�����±���� .fs ��ɫ���� Uniform����λ�� 
    ourShader->setInt(uniName, tex2DS.size() - 1);



}