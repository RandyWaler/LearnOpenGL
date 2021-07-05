#pragma once

//Base
#include <glad.h>
#include <glfw3.h>
#include <iostream>

//��ѧ��
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//
#include "RENode.h"
#include "InputEvnet.h"
#include "MouseEvnet.h"

using std::string;
class GLFWwindow;

namespace RE {

    //��������(������������Ϊ�˻�һ��namespace)
	extern void processInput(GLFWwindow* window, int key, int scancode, int action, int mode);//����ص��¼�

	extern void mouse_callback(GLFWwindow* window, double xpos, double ypos);


	class RE_Main
	{
	protected:
		//���� ������������� _getInstance Ϊ�丳�������
		static RE_Main* _instance;
		RE_Main();

		//��������
		virtual bool reInit();//��ʼ��


		//������Ϸ ��Ӧ����д
		bool reUpdate(); //����Ƿ������������ִ�� Logic bfrender render ��������ڼ̳��в��ᱻ��д

		//��Ϸ�߼����
		virtual void gameLogic(); //������д

		//��Ϸ�߼�ȫ�����н�������һ֡��Ⱦ�����Ѿ�ȷ������Ӧ�ڸú�����������ȷ��
		//׼����Ⱦ��صı���
		virtual void beforeRender();//������д

		//������Ⱦ
		virtual void render();//������д


		//֡ĩ ��β���� flag �û�
		virtual void frameOver();//������д

		virtual void relese();//�ͷ���Դ

		//����
		bool flInit = true;//��ʼ���Ƿ�ɹ�
		bool onGame = true;//��Ϸ�Ƿ���Ҫ����������ȥ

		

		//����

		GLFWwindow* window = nullptr;

	public:

		//��Ļ�ֱ��ʣ����� set get ���ɷ�װ��
		unsigned int SCR_WIDTH = 800;
		unsigned int SCR_HEIGHT = 600;

		//ʱ�����
		double dt;//������һ֡���н���������ʱ���


		/// <summary>
		/// <para>��¼��ǰʱ����ΪlastTime</para> 
		/// <para>reUpdate�л���ÿ��gameLogic֮���¼LastTime</para> 
		/// <para>�벻Ҫ�������ط��޸�LastTime</para> 
		/// </summary>
		double _LastTime;

		//������ɫ
		float clearColor[4] = { 0.2f, 0.2f, 0.3f, 1.0f };


		//��Ϸ����

		std::unordered_set<RENode*> nodes;

		std::unordered_set<InputEvnet*> ipt;//�����¼��ӿ�

		std::unordered_set<MouseEvnet*> mos;//�����¼��ӿ�

		//��ʵ�֣�#####################################

		//����Ϸ�ж�̬ɾ������




		//��̬����##########################################################
		
		static RE_Main* getInstance();


		/// <summary>
		/// <para>����̨��ӡ��� </para> 
		/// </summary>
		/// <param name="s">�����Ϣ</param>
		/// <param name="gl">(giveline)�Ƿ���Ҫ����β����</param>
		static void logMessage(string s,bool gl = false); 


		static void resetMat4(glm::mat4 & r);//����Ϊ��λ����


		//��ʼ���� ��ڵ�  ��Ӧ����д
		void run();

		//Set
		void setScreenWH(int w, int h);//������Ļ�ߴ�



		

		/// <summary>
		/// <para>ˢ��ʱ��dt(����ı�lastTime)</para> 
		/// <para>RE_Main��ÿ�ν���gameLogicʱ���Զ�ˢ��һ��ʱ��</para>
		/// <para>������gameLogic������������Ҫ����ʱ��</para> 
		/// <para>���������֮ǰ������ĳЩ��ʱ������Logic����ʱ���Ѳ�׼ȷ���������ô˺���</para>
		/// </summary>
		void refreshDt();

		
		/// <summary>
		/// ������Ϸ����
		/// </summary>
		void _GameOver();


		void _InitFail();//����ĳЩ���󣬳�ʼ��ʧ�ܣ���Ϸ�޷�����

		bool isInitFail();


        

	};

	

}



//Randy OpenGL Engine (REngine)
//-----------------------------------------------------------------
/*

Start at 2021/07/05 14:31
power by OpenGL 3.3




*/








