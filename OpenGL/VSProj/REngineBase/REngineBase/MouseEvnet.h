#pragma once

//Base
#include <glad.h>
#include <glfw3.h>
#include <iostream>

namespace RE {
	class MouseEvnet
	{
	public:
	    virtual void onMouse(GLFWwindow* window,double xpos,double ypos) = 0; //�ӿ�


		MouseEvnet();
		virtual ~MouseEvnet();

	};
}

