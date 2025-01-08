#pragma once

#include "pch.h"

class IView
{
public:
    virtual const char* getID();
    virtual const char* getText();
    bool m_bChecked = false;
    virtual ImTextureID getIcon();
    virtual void Render();// Método para renderizar la vista
};

extern std::vector<IView*> Views;