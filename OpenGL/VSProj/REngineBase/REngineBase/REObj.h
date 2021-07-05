#pragma once

#include <vector>

#include "RENode.h"
#include "Shader.h"

namespace RE {

	//�������� ��ɫ�� һ������Ⱦ���Ķ���
	class REObj : public RENode
	{
	protected:

		//�������
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;

		float *vertices; //VBO ����
		unsigned int *idxs; //EBO����

		bool flMesh;//�Ƿ������Mesh����


		int primitiveModel; //ͼԪģʽ
		//GL_TRIANGLES ����������(��ʱ�����Ϊ����)
		//GL_TRIANGLE_STRIP ���Ǵ� һ��һ˳Ϊ�����ģʽ
		//GL_TRIANGLE_FAN �����ȣ�ÿ������ʼ�ĵ���ʱ�����Ϊ��

		int verticesCount;//��������

		int VBOCount;//VBO���嶥������

		//Shader���
		Shader* ourShader;


		void crearBuffer();//����&�������������ú� ����VAO VBO EBO

		std::vector<unsigned int> tex2DS; //2D����(����Ŀǰֻ֧��2D����ͨ�� vec idx ֱ�Ӽ�������Ԫ)


	public:

		std::string vsTransName;//vs Shader ��������������
		std::string vsViewName;//vs Shader �й۲���������
		std::string vsPersName;//vs Shader ��ͶӰ���������

		REObj(glm::vec3 p = glm::vec3(0.0f, 0.0f, 0.0f), glm::mat4 ro = glm::mat4(1.0f), glm::vec3 sc = glm::vec3(1.0f, 1.0f, 1.0f));
		virtual ~REObj();

		void creatShader(std::string vsShader, std::string fsShader);

		void add2DTexture(std::string uniName, std::string resName,int dataModel=GL_RGB,bool reY = false); //ʵ���� iamge.cpp��


		//��̬����#######################################################

		static void creatBox(REObj* obj);//����һ��BoxMesh


		//��д����

		virtual void onRender() override;//�պ��� ��������Ӳ��д


	};

}

