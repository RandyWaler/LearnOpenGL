#pragma once

//Base
#include <glad.h>
#include <glfw3.h>
#include <iostream>

//��ѧ��
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RENode.h"

namespace RE { 

	extern bool mrdViewChange;//�����Ƿ��޸Ĺ�
	extern bool mrdPersChange;
	extern glm::mat4 mrdView;//����Ⱦ�۲����

	extern glm::mat4 mrdPerspective; //����ȾͶӰ����
	
	class RECamera : public RENode
	{
	protected:
		static RECamera* mainCamera; //��Ⱦ�õ��������

	public:
		static RECamera* getMainCamera();

		RECamera();
		virtual ~RECamera();

		virtual void onFrameOver() override;

	};
}

