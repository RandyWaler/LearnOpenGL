#pragma once

//Base
#include <glad.h>
#include <glfw3.h>
#include <iostream>

using std::string;
class GLFWwindow;

namespace RE {

	


    //��������
	extern void processInput(GLFWwindow* window, int key, int scancode, int action, int mode);//����ص��¼�


	



	class RE_Main
	{
	protected:
		//���� ������������� _getInstance Ϊ�丳�������
		static RE_Main* _instance;

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

		virtual void relese();//�ͷ���Դ

		//����

		static bool onGame;//��Ϸ�Ƿ���Ҫ����������ȥ

		unsigned int SCR_WIDTH = 800;
		unsigned int SCR_HEIGHT = 600;

		//����

		GLFWwindow* window = nullptr;

	public:

		//ʱ�����
		double dt;//������һ����Ϸ�߼����н���������ʱ���


		/// <summary>
		/// <para>��¼��ǰʱ����ΪlastTime</para> 
		/// <para>reUpdate�л���ÿ��gameLogic֮���¼LastTime</para> 
		/// <para>�벻Ҫ�������ط��޸�LastTime</para> 
		/// </summary>
		double _LastTime;

		//������ɫ
		float clearColor[4] = { 0.2f, 0.2f, 0.3f, 1.0f };

		//��̬����##########################################################

		/// <summary>
		/// <para>����̨��ӡ��� </para> 
		/// </summary>
		/// <param name="s">�����Ϣ</param>
		/// <param name="gl">(giveline)�Ƿ���Ҫ����β����</param>
		static void logMessage(string s,bool gl = false); 


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
        

	};

	

}



//Randy OpenGL Engine (REngine)
//-----------------------------------------------------------------
/*

Start at 2021/07/05 14:31
power by OpenGL 3.3




*/








