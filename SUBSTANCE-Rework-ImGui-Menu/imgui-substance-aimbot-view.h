#pragma once

#include "pch.h"

class AimbotView : public IView
{
public:
    AimbotView();
    const char* getID() override;
    const char* getText() override;
    ImTextureID virtual getIcon();
    void Render() override;
    HeaderBar myHeader;
    FunctionWidget Aimbot_Cheat;
    std::vector<FunctionWidget*> Widgets;
    std::string currentOption;
};

