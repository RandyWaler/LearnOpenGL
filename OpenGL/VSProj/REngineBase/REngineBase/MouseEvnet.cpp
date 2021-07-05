#include "MouseEvnet.h"

#include "RE_Main.h"

RE::MouseEvnet::MouseEvnet()
{
	RE::RE_Main::getInstance()->mos.insert(this);
}

RE::MouseEvnet::~MouseEvnet()
{
	RE::RE_Main::getInstance()->mos.erase(this);
}
