#pragma once

#include "RE_Main.h"
#include "REObj.h"

//Base
#include <glad.h>
#include <glfw3.h>
#include <iostream>

class TestGame : public RE::RE_Main
{
protected:
    virtual bool reInit() override;
    virtual void render() override;

public:
    //静态调用##########################################################
    static RE_Main* getInstance();

    //重写

    virtual void gameLogic() override; //允许被重写


    //MyGame

    std::vector<RE::REObj*> boxs;


    RE::REObj* Abox;
    RE::REObj* Bbox;
};

