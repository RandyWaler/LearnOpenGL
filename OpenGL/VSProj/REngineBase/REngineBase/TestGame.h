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
    //¾²Ì¬µ÷ÓÃ##########################################################
    static RE_Main* getInstance();

    //ÖØÐ´

    //MyGame

    std::vector<RE::REObj*> boxs;
};

