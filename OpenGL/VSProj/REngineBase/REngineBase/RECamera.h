#pragma once

//Base
#include <glad.h>
#include <glfw3.h>
#include <iostream>

//数学库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RENode.h"

namespace RE { 

	extern bool mrdViewChange;//矩阵是否被修改过
	extern bool mrdPersChange;
	extern glm::mat4 mrdView;//主渲染观察矩阵

	extern glm::mat4 mrdPerspective; //主渲染投影矩阵
	
	class RECamera : public RENode
	{
	protected:
		static RECamera* mainCamera; //渲染用的主摄像机

	public:
		static RECamera* getMainCamera();

		RECamera();
		virtual ~RECamera();

		virtual void onFrameOver() override;

	};
}

