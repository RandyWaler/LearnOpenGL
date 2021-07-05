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
    //��̬����##########################################################
    static RE_Main* getInstance();

    //��д

    virtual void gameLogic() override; //������д


    //MyGame

    std::vector<RE::REObj*> boxs;


    RE::REObj* Abox;
    RE::REObj* Bbox;
};

