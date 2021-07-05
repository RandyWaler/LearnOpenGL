#pragma once

//Base
#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include <unordered_set>

//��ѧ��
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace RE {

	//�ڵ��� 
	//Transform ���ӹ�ϵ
	//�Խ� RE_Main �е� Logic beforeRender render relese 
	class RENode 
	{
	protected:

		//Transform���

		glm::vec3 pos{0.0f,0.0f,0.0f}; //λ��
		glm::mat4 rotation = glm::mat4(1.0f); //��ת
		glm::vec3 scal{ 1.0f,1.0f,1.0f };//����

		glm::mat4 trans = glm::mat4(1.0f);//Transform����

		glm::mat4 world = glm::mat4(1.0f);//������󣨵����˸������Transfrom��

		RENode* parent=nullptr;//������

		std::unordered_set<RENode*> childs;//������

		bool change = false;//Transfrom�Ƿ񱻸ı��

	public:

		//���캯��
		RENode(glm::vec3 p = glm::vec3(0.0f,0.0f,0.0f),glm::mat4 ro = glm::mat4(1.0f),glm::vec3 sc = glm::vec3(1.0f,1.0f,1.0f));

		//��������

		virtual ~RENode();

		//set �������

		void setlocalPosition(glm::vec3 p);
		void setlocalRotation(glm::mat4 ro);
		void setlocalScale(glm::vec3 sc);

		//get ���

		glm::vec3 getlocalPosition();
		glm::mat4 getlocalRotation();
		glm::vec3 getlocalScale();


		//Transform�˶�

		void move(glm::vec3 p);//�ƶ�
		void rotate(glm::mat4 ro);//��ת
		void scale(glm::vec3);//����

		//Transform���

		/// <summary>
		/// ����Transfrom����Ϊ��ǰ��¼��pos rotation scale
		/// </summary>
		/// <param name="ptrans">�ۼӸ�����Transform</param>
		/// <param name="cp">�����Ƿ��б��޸Ĺ�</param>
		virtual void setTransform(glm::mat4 ptrans,bool cp);


		void addChild(RENode* c);

		void removeChild(RENode* c);

		//��ʵ�֣�#####################################

        //���ɾ�������壬��Ӱ�������������λ��


		//�Խ� RE_Main ����

		virtual void onGameLogic();
		virtual void onBeforeRender();
		virtual void onRender();


	};
}

