#include "RECamera.h"
#include "RE_Main.h"

#include<string>

using namespace std;
using namespace RE;


bool RE::mrdViewChange = false;
bool RE::mrdPersChange = false;
bool RE::haveMainCamera = false;

glm::mat4 RE::mrdView = glm::mat4(1.0f);

glm::mat4 RE::mrdPerspective = glm::mat4(1.0f);



REFreeCamera* REFreeCamera::mainCamera = nullptr;

REFreeCamera* RE::REFreeCamera::getMainCamera()
{
	if (!mainCamera&&!haveMainCamera) {
		mainCamera = new REFreeCamera();
		haveMainCamera = true;
	}
	return mainCamera;
}

RE::REFreeCamera::REFreeCamera()
{
	lastX = RE_Main::getInstance()->SCR_WIDTH / 2;
	lastY = RE_Main::getInstance()->SCR_HEIGHT / 2;
}

RE::REFreeCamera::~REFreeCamera()
{
}

void RE::REFreeCamera::onFrameOver()
{
	RENode::onFrameOver();

	mrdViewChange = false;
	mrdPersChange = false;
}

void RE::REFreeCamera::cmaInit() //相机初始化
{
	change = false;

	RE_Main::resetMat4(trans);

	trans = glm::translate(trans, pos); //T移动


	RE_Main::resetMat4(rotation);

	//Heading（Y）
	rotation = glm::rotate(rotation, glm::radians(cameraRo.y), glm::vec3(0.0f, 1.0f, 0.0f));

	RE_Main::resetMat4(pitch); //这里glm是行向量右乘，因此Heading旋转后，PitchX转轴是第一行向量不是第一列

	pitch = glm::rotate(pitch, glm::radians(cameraRo.x), glm::vec3(rotation[0][0], rotation[0][1], rotation[0][2])); //Pitch

	rotation = pitch * rotation;

	trans *= rotation;
	
	//Bank旋转暂时忽略

    //view Pers矩阵设置

	mrdView = glm::transpose(rotation); //正交阵直接转置即可

	mrdView = glm::translate(mrdView, pos); //T移动

	RE_Main::resetMat4(mrdPerspective);

	mrdPerspective = glm::perspective(glm::radians(cmaAngle), (float)RE_Main::getInstance()->SCR_WIDTH / (float)RE_Main::getInstance()->SCR_HEIGHT, cmaNearPlane, cmaFarPlane);

}


//接口函数
void RE::REFreeCamera::onInput(GLFWwindow* window) 
{

	//RE_Main::logMessage("onInput");

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { //检测摁下 Esc
		RE_Main::getInstance()->_GameOver(); //结束游戏
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) flag_W = true;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) flag_W = false;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) flag_S = true;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) flag_S = false;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) flag_A = true;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) flag_A = false;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) flag_D = true;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) flag_D = false;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) flag_Q = true;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE) flag_Q = false;

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) flag_E = true;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) flag_E = false;


	mrdViewChange = true; //置flag 观察矩阵变化

}

void RE::REFreeCamera::onMouse(GLFWwindow* window, double xpos, double ypos)
{

	//RE_Main::logMessage("onMouse");
	nmX = xpos;
	nmY = ypos;

	mrdViewChange = true; //置flag 观察矩阵变化
}


float dtro;

void RE::REFreeCamera::onBeforeRender()
{
	//RE_Main::logMessage("onBeforeRender");

	

	RE_Main::getInstance()->refreshDt();
	auto dt = RE::RE_Main::getInstance()->dt;

	//RE_Main::logMessage(std::to_string(pos.z));

	//相机运动
	if (flag_W)//向前移动(本地空间)
		pos += glm::vec3(trans[2][0] * cmaMoveSpeed * dt, trans[2][1] * cmaMoveSpeed * dt, trans[2][2] * cmaMoveSpeed * dt);
	else if (flag_S)//向后移动(本地空间)
		pos -= glm::vec3(trans[2][0] * cmaMoveSpeed * dt, trans[2][1] * cmaMoveSpeed * dt, trans[2][2] * cmaMoveSpeed * dt);
	if (flag_A)//向左移动(本地空间)
		pos += glm::vec3(trans[0][0] * cmaMoveSpeed * dt, trans[0][1] * cmaMoveSpeed * dt, trans[0][2] * cmaMoveSpeed * dt);
	else if (flag_D)//向右移动(本地空间)
		pos -= glm::vec3(trans[0][0] * cmaMoveSpeed * dt, trans[0][1] * cmaMoveSpeed * dt, trans[0][2] * cmaMoveSpeed * dt);
	if (flag_Q)//向上移动(世界空间上下)
		pos.y += cmaMoveSpeed * dt;
	else if (flag_E)//向下移动(世界空间上下)
		pos.y -= cmaMoveSpeed * dt;

	//相机旋转


	dtro = glm::abs(nmY - lastY);
	if (dtro > 30) dtro = 30;
	if (dtro > 2) {
		if (nmY > lastY) mouseVDir = -dtro;//向下移动了
		else if (nmY < lastY) mouseVDir = dtro;//向上移动了
	}
	else mouseVDir = 0;

	dtro = glm::abs(nmX - lastX);
	if (dtro > 30) dtro = 30;
	if (dtro > 2) {
		if (nmX > lastX) mouseHDir = dtro;//向右移动了
		else if (nmX < lastX) mouseHDir = -dtro;
	}
	else mouseHDir = 0;

	lastY = nmY;
	lastX = nmX;


	//上下旋转Pitch
	RE_Main::getInstance()->refreshDt();
	dt = RE::RE_Main::getInstance()->dt;

	dtro = roSmooth * mouseVDir * dt * cmaRoSpeed;
	cameraRo.x += dtro;


	dtro = roSmooth * mouseHDir * dt * cmaRoSpeed;
	cameraRo.y -= dtro;


	if (cameraRo.x > 85.0f) cameraRo.x = 85.0f;
	else if (cameraRo.x < -85.0f) cameraRo.x = -85.0f;




	//cout << "\n" <<dt <<" ----------------------------" << endl;


	RE_Main::resetMat4(trans);

	trans = glm::translate(trans, pos); //T移动


	RE_Main::resetMat4(rotation);

	//Heading（Y）
	rotation = glm::rotate(rotation, glm::radians(cameraRo.y), glm::vec3(0.0f, 1.0f, 0.0f));

	RE_Main::resetMat4(pitch); //这里glm是行向量右乘，因此Heading旋转后，PitchX转轴是第一行向量不是第一列

	pitch = glm::rotate(pitch, glm::radians(cameraRo.x), glm::vec3(rotation[0][0], rotation[0][1], rotation[0][2])); //Pitch

	rotation = pitch * rotation;

	trans *= rotation;

	//Bank旋转暂时忽略

	//view Pers矩阵设置

	//RE_Main::resetMat4(mrdView);

	mrdView = glm::transpose(rotation); //正交阵直接转置即可

	mrdView = glm::translate(mrdView, pos); //T移动

	
}
