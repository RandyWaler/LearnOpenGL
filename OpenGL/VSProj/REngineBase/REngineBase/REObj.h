#pragma once

#include <vector>

#include "RENode.h"
#include "Shader.h"

namespace RE {

	//包含网格 着色器 一个可渲染出的对象
	class REObj : public RENode
	{
	protected:

		//网格相关
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;

		float *vertices; //VBO 顶点
		unsigned int *idxs; //EBO索引

		bool flMesh;//是否搭载了Mesh网格


		int primitiveModel; //图元模式
		//GL_TRIANGLES 单个三角型(逆时针卷绕为正面)
		//GL_TRIANGLE_STRIP 三角带 一逆一顺为正面的模式
		//GL_TRIANGLE_FAN 三角扇，每两点与始心点逆时针卷入为正

		int verticesCount;//顶点数量

		int VBOCount;//VBO缓冲顶点数量

		//Shader相关
		Shader* ourShader;


		void crearBuffer();//网格&所有数组已设置好 创建VAO VBO EBO

		std::vector<unsigned int> tex2DS; //2D纹理(引擎目前只支撑2D纹理，通过 vec idx 直接计算纹理单元)


	public:

		std::string vsTransName;//vs Shader 中世界矩阵的名字
		std::string vsViewName;//vs Shader 中观察矩阵的名字
		std::string vsPersName;//vs Shader 中投影矩阵的名字

		REObj(glm::vec3 p = glm::vec3(0.0f, 0.0f, 0.0f), glm::mat4 ro = glm::mat4(1.0f), glm::vec3 sc = glm::vec3(1.0f, 1.0f, 1.0f));
		virtual ~REObj();

		void creatShader(std::string vsShader, std::string fsShader);

		void add2DTexture(std::string uniName, std::string resName,int dataModel=GL_RGB,bool reY = false); //实现于 iamge.cpp中


		//静态调用#######################################################

		static void creatBox(REObj* obj);//搭载一个BoxMesh


		//重写父类

		virtual void onRender() override;//空函数 建议子类硬重写


	};

}

