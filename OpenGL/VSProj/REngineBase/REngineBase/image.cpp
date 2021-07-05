//为防止 stb_image 在不同cpp中引用，并导致重复定义

//现将所有需要用到这个库进行图片加载的代码全部写在这个cpp中

//图片加载
#define STB_IMAGE_IMPLEMENTATION //只键.h中入被引用的代码部分
#include "stb_image.h"

#include "REObj.h"
#include "RE_Main.h"

using namespace RE;


void RE::REObj::add2DTexture(std::string uniName, std::string resName,int dataModel, bool reY)
{

    if (!ourShader) { //未创建Shader就加载纹理
        RE_Main::logMessage("Error!!! no Shader but add 2dTexture", true);
        RE_Main::getInstance()->_InitFail();
        return;
    }

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
    stbi_set_flip_vertically_on_load(reY);//翻转y轴，匹配GL纹理坐标
    unsigned char* data = stbi_load(resName.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, dataModel, width, height, 0, dataModel, GL_UNSIGNED_BYTE, data);
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
    else {
        RE_Main::logMessage("Fail load 2D Texture" + resName, true);

        RE_Main::getInstance()->_InitFail();

        return;
    }


    stbi_image_free(data);//生成了纹理，以及MipMap后，图像数据data可以被释放掉

    tex2DS.emplace_back(texture);

    ourShader->use();

    // 目前只支持2D纹理，直接以下标计算 .fs 着色器中 Uniform属性位置 
    ourShader->setInt(uniName, tex2DS.size() - 1);



}