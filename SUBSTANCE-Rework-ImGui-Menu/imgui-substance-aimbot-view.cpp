#include "pch.h"

AimbotView::AimbotView()
{
    this->m_bChecked = true;
}

const char* AimbotView::getID()
{
    return "Aimbot##TAB";
}

const char* AimbotView::getText()
{
    return "Aimbot";
}

ImTextureID AimbotView::getIcon()
{
    return 0;
}

void AimbotView::Render()
{
    ImVec2 windowSize = ImGui::GetIO().DisplaySize;

    float leftSectionWidth = 220.0f;
    ImGui::BeginChild("LeftSection", ImVec2(leftSectionWidth, 0));

    float topMargin = 10.0f;
    float bottomMargin = 10.0f;

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + topMargin);

    /*
    foreach(var widget in Widgets)
    {
        widget.BorderOffset += 0.005f;
        widget.Animating = !(widget.BorderOffset >= 1.0f);
        widget.Render();
    }
    */

    ImGui::Dummy(ImVec2(0, bottomMargin));

    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("RightSection", ImVec2(windowSize.x - leftSectionWidth, 0));

    float marginX = 15.0f; // Sangría horizontal (igual para ambos lados)
    float marginY = 10.0f; // Sangría vertical

    ImVec2 childStartPos = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(0, childStartPos.y + marginY));

    float adjustedWidth = (windowSize.x - leftSectionWidth) - (2 * marginX);
    //headerBar.Size = new ImVec2(adjustedWidth - 200, headerBar.Size.y);

    //headerBar.Render(ImGui::GetIO().DeltaTime);

    /*
    if (currentOption == Aimbot_Cheat.ID)
    {
        Aimbot();
    }
    else if (currentOption == Triggerbot_Cheat.ID)
    {
        Trigger();
    }
    */

    ImGui::EndChild();
}