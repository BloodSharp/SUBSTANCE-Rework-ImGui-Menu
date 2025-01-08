#include "pch.h"

const char* IView::getID()
{
    return "";
}

const char* IView::getText()
{
    return "";
}

ImTextureID IView::getIcon()
{
    return 0;
}

void IView::Render()
{
}



class VisualsView : public IView
{
public:
    const char* getID() override
    {
        return "Visuals##TAB";
    }
    const char* getText() override
    {
        return "Visuals";
    }
    ImTextureID virtual getIcon()
    {
        return 0;
    }
    void Render() override
    {

    }
};

class WaypointsView : public IView
{
public:
    const char* getID() override
    {
        return "Waypoints##TAB";
    }
    const char* getText() override
    {
        return "Waypoints";
    }
    ImTextureID virtual getIcon()
    {
        return 0;
    }
    void Render() override
    {

    }
};

class MiscellaneousView : public IView
{
public:
    const char* getID() override
    {
        return "Miscellaneous##TAB";
    }
    const char* getText() override
    {
        return "Miscellaneous";
    }
    ImTextureID virtual getIcon()
    {
        return 0;
    }
    void Render() override
    {

    }
};

std::vector<IView*> Views = { new AimbotView(), new VisualsView(), new WaypointsView(), new MiscellaneousView() };
