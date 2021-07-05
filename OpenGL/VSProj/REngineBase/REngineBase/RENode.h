#pragma once

//Base
#include <glad.h>
#include <glfw3.h>
#include <iostream>
#include <unordered_set>

//数学库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace RE {

	//节点类 
	//Transform 父子关系
	//对接 RE_Main 中的 Logic beforeRender render relese 
	class RENode 
	{
	protected:

		//Transform相关

		glm::vec3 pos{0.0f,0.0f,0.0f}; //位置
		glm::mat4 rotation = glm::mat4(1.0f); //旋转
		glm::vec3 scal{ 1.0f,1.0f,1.0f };//缩放

		glm::mat4 trans = glm::mat4(1.0f);//Transform矩阵

		glm::mat4 world = glm::mat4(1.0f);//世界矩阵（叠加了父物体的Transfrom）

		RENode* parent=nullptr;//父物体

		std::unordered_set<RENode*> childs;//子物体

		bool change = false;//Transfrom是否被改变过

	public:

		//构造函数
		RENode(glm::vec3 p = glm::vec3(0.0f,0.0f,0.0f),glm::mat4 ro = glm::mat4(1.0f),glm::vec3 sc = glm::vec3(1.0f,1.0f,1.0f));

		//析构甘薯

		virtual ~RENode();

		//set 设置相关

		void setlocalPosition(glm::vec3 p);
		void setlocalRotation(glm::mat4 ro);
		void setlocalScale(glm::vec3 sc);

		//get 相关

		glm::vec3 getlocalPosition();
		glm::mat4 getlocalRotation();
		glm::vec3 getlocalScale();


		//Transform运动

		void move(glm::vec3 p);//移动
		void rotate(glm::mat4 ro);//旋转
		void scale(glm::vec3);//缩放

		//Transform相关

		/// <summary>
		/// 修正Transfrom矩阵为当前记录的pos rotation scale
		/// </summary>
		/// <param name="ptrans">累加父链的Transform</param>
		/// <param name="cp">父链是否有被修改过</param>
		virtual void setTransform(glm::mat4 ptrans,bool cp);


		void addChild(RENode* c);

		void removeChild(RENode* c);

		//待实现：#####################################

        //添加删除子物体，不影响子物体的世界位置


		//对接 RE_Main 调用

		virtual void onGameLogic();
		virtual void onBeforeRender();
		virtual void onRender();


	};
}

