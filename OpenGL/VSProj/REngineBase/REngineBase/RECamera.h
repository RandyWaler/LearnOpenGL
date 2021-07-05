#pragma once

#include "RENode.h"
#include "InputEvnet.h"
#include "MouseEvnet.h"


//Base
#include <glad.h>
#include <glfw3.h>
#include <iostream>

//��ѧ��
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



//ע�⣡��������������������������������������������������������������������������
//����Ⱦ�����ʼ��Ҫ����������ǰ�����
//��֤�����������view Pers ����������ȷ


namespace RE { 

	extern bool mrdViewChange;//�����Ƿ��޸Ĺ�
	extern bool mrdPersChange;
	extern glm::mat4 mrdView;//����Ⱦ�۲����

	extern glm::mat4 mrdPerspective; //����ȾͶӰ����

	extern bool haveMainCamera;//�Ƿ��Ѿ������������
	


	//��������� - ���� - ������Ϊ�������ʹ��

	class REFreeCamera : public RENode,InputEvnet,MouseEvnet
	{
	protected:

		//�ø���� pos
		//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);

		//���ŷ����
		glm::vec3 cameraRo = glm::vec3(0.0f, 0.0f, 0.0f); //Heading(Y) --- Pitch(X) --- Bank(Z) ����ת


		glm::mat4 pitch;//temp

		//�۲�������Trans����������
		//glm::mat4 view = glm::mat4(1.0f); �ù�����mrdView

		//�ø����Transform����
		//glm::mat4 cmaTrans = glm::mat4(1.0f);;//�����Transfom����

		//�ø������ת����
		//glm::mat4 cmaRo = glm::mat4(1.0f);;//�������ת����

		static REFreeCamera* mainCamera; //��Ⱦ�õ��������


		// -- Input Key

		bool flag_W = false;
		bool flag_S = false;
		bool flag_A = false;
		bool flag_D = false;
		bool flag_Q = false;
		bool flag_E = false;

		// --  Mouse

		float lastX , lastY ;
		float nmX = lastX;
		float nmY = lastY;

		float roSmooth = 0.3f;


		float mouseHDir = 0;
		float mouseVDir = 0;

	public:

		//�������ɷ�װ
		double cmaMoveSpeed = 3.0f;//���ÿ����ƶ��ٶ�

		double cmaRoSpeed = 100.0f;//���ÿ��λ�����˶�����ת�ٶ�

		float cmaAngle = 45.0f;

		float cmaNearPlane = 0.1f;

		float cmaFarPlane = 100.0f;


		static REFreeCamera* getMainCamera();

		REFreeCamera();
		virtual ~REFreeCamera();

		virtual void onFrameOver() override;

		virtual void cmaInit();//�����ʼ�� ����ǰ���� ���õ�view���� Pers����
		//ע�⣡��������������������������������������������������������������������������
        //����Ⱦ�����ʼ��Ҫ����������ǰ�����
        //��֤�����������view Pers ����������ȷ


		//Evnet
		virtual void onInput(GLFWwindow* window);

		virtual void onMouse(GLFWwindow* window, double xpos, double ypos);

		//

		virtual void onBeforeRender() override; //�������Ӳ��дģʽ������ŷ����������ת����

	};
}

