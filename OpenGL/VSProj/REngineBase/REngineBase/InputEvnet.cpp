#include "InputEvnet.h"

#include "RE_Main.h"


RE::InputEvnet::InputEvnet()
{

	RE::RE_Main::getInstance()->ipt.insert(this);
}

RE::InputEvnet::~InputEvnet()
{
	RE::RE_Main::getInstance()->ipt.erase(this);

}
