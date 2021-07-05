#pragma once
//Base
#include <glad.h>
#include <glfw3.h>
#include <iostream>

namespace RE {

	class InputEvnet
	{
	public:

		virtual void onInput(GLFWwindow* window) = 0; //½Ó¿Ú

		InputEvnet();

		virtual ~InputEvnet();


	};

}

