#pragma once

//Base
#include <glad.h>
#include <glfw3.h>
#include <iostream>

//数学库
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

    //按键输入(声明在这里是为了混一个namespace)
	extern void processInput(GLFWwindow* window, int key, int scancode, int action, int mode);//输入回调事件

	extern void mouse_callback(GLFWwindow* window, double xpos, double ypos);


	class RE_Main
	{
	protected:
		//单例 请在子类中完成 _getInstance 为其赋子类对象
		static RE_Main* _instance;
		RE_Main();

		//三大运行
		virtual bool reInit();//初始化


		//运行游戏 不应被重写
		bool reUpdate(); //检测是否结束，并依次执行 Logic bfrender render 这个方法在继承中不会被重写

		//游戏逻辑相关
		virtual void gameLogic(); //允许被重写

		//游戏逻辑全部运行结束，这一帧渲染内容已经确定，或应在该函数中做最终确定
		//准备渲染相关的变量
		virtual void beforeRender();//允许被重写

		//进行渲染
		virtual void render();//允许被重写


		//帧末 收尾工作 flag 置回
		virtual void frameOver();//允许被重写

		virtual void relese();//释放资源

		//常量
		bool flInit = true;//初始化是否成功
		bool onGame = true;//游戏是否需要继续运行下去

		

		//变量

		GLFWwindow* window = nullptr;

	public:

		//屏幕分辨率（不做 set get 过渡封装）
		unsigned int SCR_WIDTH = 800;
		unsigned int SCR_HEIGHT = 600;

		//时差相关
		double dt;//距离上一帧运行结束经过的时间差


		/// <summary>
		/// <para>记录当前时间作为lastTime</para> 
		/// <para>reUpdate中会在每次gameLogic之后记录LastTime</para> 
		/// <para>请不要在其它地方修改LastTime</para> 
		/// </summary>
		double _LastTime;

		//清屏颜色
		float clearColor[4] = { 0.2f, 0.2f, 0.3f, 1.0f };


		//游戏物体

		std::unordered_set<RENode*> nodes;

		std::unordered_set<InputEvnet*> ipt;//输入事件接口

		std::unordered_set<MouseEvnet*> mos;//输入事件接口

		//待实现：#####################################

		//在游戏中动态删除物体




		//静态调用##########################################################
		
		static RE_Main* getInstance();


		/// <summary>
		/// <para>控制台打印输出 </para> 
		/// </summary>
		/// <param name="s">输出信息</param>
		/// <param name="gl">(giveline)是否需要加首尾引启</param>
		static void logMessage(string s,bool gl = false); 


		static void resetMat4(glm::mat4 & r);//重置为单位矩阵


		//开始运行 入口点  不应被重写
		void run();

		//Set
		void setScreenWH(int w, int h);//设置屏幕尺寸



		

		/// <summary>
		/// <para>刷新时差dt(不会改变lastTime)</para> 
		/// <para>RE_Main中每次进入gameLogic时会自动刷新一次时差</para>
		/// <para>但由于gameLogic本身运行亦需要消耗时间</para> 
		/// <para>如果您觉得之前运行了某些耗时过长的Logic导致时差已不准确，不妨调用此函数</para>
		/// </summary>
		void refreshDt();

		
		/// <summary>
		/// 结束游戏运行
		/// </summary>
		void _GameOver();


		void _InitFail();//出现某些错误，初始化失败，游戏无法运行

		bool isInitFail();


        

	};

	

}



//Randy OpenGL Engine (REngine)
//-----------------------------------------------------------------
/*

Start at 2021/07/05 14:31
power by OpenGL 3.3




*/








