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

void RE::REFreeCamera::cmaInit() //�����ʼ��
{
	change = false;

	RE_Main::resetMat4(trans);

	trans = glm::translate(trans, pos); //T�ƶ�


	RE_Main::resetMat4(rotation);

	//Heading��Y��
	rotation = glm::rotate(rotation, glm::radians(cameraRo.y), glm::vec3(0.0f, 1.0f, 0.0f));

	RE_Main::resetMat4(pitch); //����glm���������ҳˣ����Heading��ת��PitchXת���ǵ�һ���������ǵ�һ��

	pitch = glm::rotate(pitch, glm::radians(cameraRo.x), glm::vec3(rotation[0][0], rotation[0][1], rotation[0][2])); //Pitch

	rotation = pitch * rotation;

	trans *= rotation;
	
	//Bank��ת��ʱ����

    //view Pers��������

	mrdView = glm::transpose(rotation); //������ֱ��ת�ü���

	mrdView = glm::translate(mrdView, pos); //T�ƶ�

	RE_Main::resetMat4(mrdPerspective);

	mrdPerspective = glm::perspective(glm::radians(cmaAngle), (float)RE_Main::getInstance()->SCR_WIDTH / (float)RE_Main::getInstance()->SCR_HEIGHT, cmaNearPlane, cmaFarPlane);

}


//�ӿں���
void RE::REFreeCamera::onInput(GLFWwindow* window) 
{

	//RE_Main::logMessage("onInput");

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { //������� Esc
		RE_Main::getInstance()->_GameOver(); //������Ϸ
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


	mrdViewChange = true; //��flag �۲����仯

}

void RE::REFreeCamera::onMouse(GLFWwindow* window, double xpos, double ypos)
{

	//RE_Main::logMessage("onMouse");
	nmX = xpos;
	nmY = ypos;

	mrdViewChange = true; //��flag �۲����仯
}


float dtro;

void RE::REFreeCamera::onBeforeRender()
{
	//RE_Main::logMessage("onBeforeRender");

	

	RE_Main::getInstance()->refreshDt();
	auto dt = RE::RE_Main::getInstance()->dt;

	//RE_Main::logMessage(std::to_string(pos.z));

	//����˶�
	if (flag_W)//��ǰ�ƶ�(���ؿռ�)
		pos += glm::vec3(trans[2][0] * cmaMoveSpeed * dt, trans[2][1] * cmaMoveSpeed * dt, trans[2][2] * cmaMoveSpeed * dt);
	else if (flag_S)//����ƶ�(���ؿռ�)
		pos -= glm::vec3(trans[2][0] * cmaMoveSpeed * dt, trans[2][1] * cmaMoveSpeed * dt, trans[2][2] * cmaMoveSpeed * dt);
	if (flag_A)//�����ƶ�(���ؿռ�)
		pos += glm::vec3(trans[0][0] * cmaMoveSpeed * dt, trans[0][1] * cmaMoveSpeed * dt, trans[0][2] * cmaMoveSpeed * dt);
	else if (flag_D)//�����ƶ�(���ؿռ�)
		pos -= glm::vec3(trans[0][0] * cmaMoveSpeed * dt, trans[0][1] * cmaMoveSpeed * dt, trans[0][2] * cmaMoveSpeed * dt);
	if (flag_Q)//�����ƶ�(����ռ�����)
		pos.y += cmaMoveSpeed * dt;
	else if (flag_E)//�����ƶ�(����ռ�����)
		pos.y -= cmaMoveSpeed * dt;

	//�����ת


	dtro = glm::abs(nmY - lastY);
	if (dtro > 30) dtro = 30;
	if (dtro > 2) {
		if (nmY > lastY) mouseVDir = -dtro;//�����ƶ���
		else if (nmY < lastY) mouseVDir = dtro;//�����ƶ���
	}
	else mouseVDir = 0;

	dtro = glm::abs(nmX - lastX);
	if (dtro > 30) dtro = 30;
	if (dtro > 2) {
		if (nmX > lastX) mouseHDir = dtro;//�����ƶ���
		else if (nmX < lastX) mouseHDir = -dtro;
	}
	else mouseHDir = 0;

	lastY = nmY;
	lastX = nmX;


	//������תPitch
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

	trans = glm::translate(trans, pos); //T�ƶ�


	RE_Main::resetMat4(rotation);

	//Heading��Y��
	rotation = glm::rotate(rotation, glm::radians(cameraRo.y), glm::vec3(0.0f, 1.0f, 0.0f));

	RE_Main::resetMat4(pitch); //����glm���������ҳˣ����Heading��ת��PitchXת���ǵ�һ���������ǵ�һ��

	pitch = glm::rotate(pitch, glm::radians(cameraRo.x), glm::vec3(rotation[0][0], rotation[0][1], rotation[0][2])); //Pitch

	rotation = pitch * rotation;

	trans *= rotation;

	//Bank��ת��ʱ����

	//view Pers��������

	//RE_Main::resetMat4(mrdView);

	mrdView = glm::transpose(rotation); //������ֱ��ת�ü���

	mrdView = glm::translate(mrdView, pos); //T�ƶ�

	
}
