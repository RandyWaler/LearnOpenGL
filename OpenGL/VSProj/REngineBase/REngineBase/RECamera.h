#pragma once

#include "RENode.h"
#include "InputEvnet.h"
#include "MouseEvnet.h"


//Base
#include <glad.h>
#include <glfw3.h>
#include <iostream>

//数学库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



//注意！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
//主渲染相机初始化要在所有物体前面完成
//保证后续物体初次view Pers 矩阵设置正确


namespace RE { 

	extern bool mrdViewChange;//矩阵是否被修改过
	extern bool mrdPersChange;
	extern glm::mat4 mrdView;//主渲染观察矩阵

	extern glm::mat4 mrdPerspective; //主渲染投影矩阵

	extern bool haveMainCamera;//是否已经有主摄像机了
	


	//漫游摄像机 - 单例 - 必须作为主摄像机使用

	class REFreeCamera : public RENode,InputEvnet,MouseEvnet
	{
	protected:

		//用父类的 pos
		//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);

		//相机欧拉角
		glm::vec3 cameraRo = glm::vec3(0.0f, 0.0f, 0.0f); //Heading(Y) --- Pitch(X) --- Bank(Z) 轴旋转


		glm::mat4 pitch;//temp

		//观察矩阵（相机Trans矩阵的逆矩阵）
		//glm::mat4 view = glm::mat4(1.0f); 用公开的mrdView

		//用父类的Transform矩阵
		//glm::mat4 cmaTrans = glm::mat4(1.0f);;//相机的Transfom矩阵

		//用父类的旋转矩阵
		//glm::mat4 cmaRo = glm::mat4(1.0f);;//相机的旋转矩阵

		static REFreeCamera* mainCamera; //渲染用的主摄像机


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

		//不做过渡封装
		double cmaMoveSpeed = 3.0f;//相机每秒的移动速度

		double cmaRoSpeed = 100.0f;//相机每单位像素运动的旋转速度

		float cmaAngle = 45.0f;

		float cmaNearPlane = 0.1f;

		float cmaFarPlane = 100.0f;


		static REFreeCamera* getMainCamera();

		REFreeCamera();
		virtual ~REFreeCamera();

		virtual void onFrameOver() override;

		virtual void cmaInit();//相机初始化 将当前参数 设置到view矩阵 Pers矩阵
		//注意！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
        //主渲染相机初始化要在所有物体前面完成
        //保证后续物体初次view Pers 矩阵设置正确


		//Evnet
		virtual void onInput(GLFWwindow* window);

		virtual void onMouse(GLFWwindow* window, double xpos, double ypos);

		//

		virtual void onBeforeRender() override; //这里采用硬重写模式，改用欧拉角设置旋转矩阵

	};
}

