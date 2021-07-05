#include "RECamera.h"
#include "RE_Main.h"
using namespace std;
using namespace RE;


bool RE::mrdViewChange = false;
bool RE::mrdPersChange = false;

//glm::mat4 RE::mrdView = glm::mat4(1.0f);
//
//glm::mat4 RE::mrdPerspective = glm::mat4(1.0f);

glm::mat4 RE::mrdView = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

glm::mat4 RE::mrdPerspective = glm::perspective(glm::radians(45.0f), (float)800/ (float)600, 0.1f, 100.0f);


RECamera* RECamera::mainCamera = nullptr;

RECamera* RE::RECamera::getMainCamera()
{
	if (!mainCamera) {
		mainCamera = new RECamera();

		//mainCamera Init
		mrdView = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

		mrdPerspective = glm::perspective(glm::radians(45.0f), (float)RE_Main::getInstance()->SCR_WIDTH / (float)RE_Main::getInstance()->SCR_HEIGHT, 0.1f, 100.0f);


	}
	return mainCamera;
}

RE::RECamera::RECamera()
{
}

RE::RECamera::~RECamera()
{
}

void RE::RECamera::onFrameOver()
{
	RENode::onFrameOver();

	mrdViewChange = false;
	mrdPersChange = false;
}
