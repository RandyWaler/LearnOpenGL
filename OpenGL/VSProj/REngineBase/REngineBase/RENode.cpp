#include "RENode.h"
#include "RE_Main.h"

using namespace glm;
using namespace RE;


RE::RENode::RENode(glm::vec3 p, glm::mat4 ro, glm::vec3 sc) : pos{ p }, rotation{ ro }, scal{sc}
{
	//RE_Main::logMessage("Creat Node",true);

	RE_Main::getInstance()->nodes.insert(this);

	RE_Main::resetMat4(trans);

	trans = translate(trans, pos);
	trans = trans * rotation; //先变换在右侧，先旋转后移动
	trans = glm::scale(trans, scal);//向前面叠加scale

	world = trans;//更新世界矩阵

}

RE::RENode::~RENode()
{
	//RE_Main::logMessage("Delete Node",true);

}

void RE::RENode::setlocalPosition(glm::vec3 p)
{
	pos = p;
	change = true;
}

void RE::RENode::setlocalRotation(glm::mat4 ro)
{
	rotation = ro;
	change = true;
}

void RE::RENode::setlocalScale(glm::vec3 sc)
{
	scal = sc;
	change = true;
}

glm::vec3 RE::RENode::getlocalPosition()
{
	return pos;
}

glm::mat4 RE::RENode::getlocalRotation()
{
	return rotation;
}

glm::vec3 RE::RENode::getlocalScale()
{
	return scal;
}

void RE::RENode::move(glm::vec3 p)
{
	pos += p;
	change - true;
}

void RE::RENode::rotate(glm::mat4 ro)
{
	rotation = ro * rotation;
	change - true;
}

void RE::RENode::scale(glm::vec3 sc)
{
	scal *= sc;
	change - true;
}


void RE::RENode::setTransform(glm::mat4 ptrans, bool cp)
{
	if (change) { //当前节点的Transfrom有被修改过
		RE_Main::resetMat4(trans);

		trans = translate(trans, pos);
		trans = trans * rotation; //先变换在右侧，先旋转后移动
		trans = glm::scale(trans, scal);//向前面叠加scale
	}

	if(cp||change) //父节点或者自己有修改都要更新世界矩阵
	    world = trans * ptrans;//更新世界矩阵

	for (auto c : childs) c->setTransform(world, cp || change);//启用子物体的更新
	
	
}

void RE::RENode::addChild(RENode* c)
{
	if (childs.count(c)) {
		RE_Main::logMessage("Warning!!! re add Child");//重复添加子物体
		return;
	}

	childs.insert(c);
	c->change = true;
	c->parent = this;

}

void RE::RENode::removeChild(RENode* c)
{
	if (!childs.count(c)) {
		RE_Main::logMessage("Warning!!! no Child");//企图删除不存在的子物体
		return;
	}

	childs.erase(c);
	c->change = true;
	c->parent = nullptr;
}

void RE::RENode::onGameLogic(){} //nothing

void RE::RENode::onBeforeRender()
{
	if (!this->parent) { //从没有父物体的节点引导更新Transfrom矩阵
		setTransform(glm::mat4(1.0f), false);
	}

}

void RE::RENode::onRender(){} //nothing

void RE::RENode::onFrameOver()
{
	change = false;
}
