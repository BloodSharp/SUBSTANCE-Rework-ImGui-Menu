#include "pch.h"

AimbotView::AimbotView()
{
    this->m_bChecked = true;
   /* this->myHeader = new  HeaderBar();*/
   
    //Hader
    this->myHeader.Size = ImVec2(800, 40);
    this->myHeader.LeftLabelText = "Left Animated";
    this->myHeader.RightLabelText = "Right Static";
    
  
  //Aimbot_Cheat
    this->Aimbot_Cheat.ID = "func.aimbot";
    this->Aimbot_Cheat.Title = "Aimbot";
    this->Aimbot_Cheat.Size = ImVec2(200, 100);
    this->Aimbot_Cheat.Description = "Automatically assists the player in aiming and firing at their targets.";
//  widget.CheckedChanged = []() { std::cout << "CheckedChanged!\n"; };
//  widget.ButtonClicked = []() { std::cout << "IconButton Pressed!\n"; };
//
//  // En tu bucle de render:
//  widget.Render();
    
    
    this->currentOption = Aimbot_Cheat.ID;
    
    Widgets = { &Aimbot_Cheat  };
     
    for (FunctionWidget* view : Widgets)
    {
		view->CheckedChanged = [this, view]()
			{
				view->BorderOffset = 0.0f;
			};
        
        view->ButtonClicked = [this, view]()    
		{
                if (this->currentOption == view->ID)
                    return;
                
			this->currentOption = view->ID;
            this->myHeader.LeftLabelText = "destroyer & substance ~& cd " + std::string("Combat") + "/" + this->currentOption;
            this->myHeader.ResetAnimationTimer();
		};
    }
    
    
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


void Aimbot() {

    ImGui::Text("Vista de controles de Aimbot");

}


void AimbotView::Render()
{
    ImVec2 windowSize = ImGui::GetIO().DisplaySize;

    float leftSectionWidth = 220.0f;
    ImGui::BeginChild("LeftSection", ImVec2(leftSectionWidth, 0));

    float topMargin = 10.0f;
    float bottomMargin = 10.0f;

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + topMargin);

    for (FunctionWidget* view : Widgets) {
        view->BorderOffset += 0.005f;
        view->Animating = !(view->BorderOffset >= 1.0f);
        view->Render();
    }
   /* foreach(var widget in Widgets)
    {
        widget.BorderOffset += 0.005f;
        widget.Animating = !(widget.BorderOffset >= 1.0f);
        widget.Render();
    }*/
    

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
    myHeader.Size = ImVec2(adjustedWidth - 200, myHeader.Size.y);
    //headerBar.Render(ImGui::GetIO().DeltaTime);
    double dt = ImGui::GetIO().DeltaTime; // deltaTime

    myHeader.Render(dt);
    
    
    if (currentOption == Aimbot_Cheat.ID)
    {
        Aimbot();
    }
    /*
    else if (currentOption == Triggerbot_Cheat.ID)
    {
        Trigger();
    }
    */

    ImGui::EndChild();
}
