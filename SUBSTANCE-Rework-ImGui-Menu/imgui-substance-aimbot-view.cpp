#include "pch.h"

// Aquí asumo que tienes un namespace Core::Instances::Settings con vars
// Ajusta a tu proyecto real:
namespace Core
{
    namespace Instances
    {
        struct SettingsStruct
        {
            bool AIM = false;
            bool Triggerbot = false;
            bool AIM_Visible_Check = false;
            bool AIM_AimAtAll = false;
            bool AIM_StickTarget = false;
            bool AIM_Driver_First = false;
            bool AIM_AutoAim = false;
            bool AIM_Vehicle = false;
            bool AIM_Draw_Fov = false;
            bool AIM_Draw_TargetLine = false;
            int  AIM_Location = 0;
            int  AIM_Type = 0;
            int  AIM_Fov = 5; // ej
            ImVec4 AIM_Fov_Color = ImVec4(1, 0, 0, 1);
            ImVec4 AIM_TargetColor = ImVec4(0, 1, 0, 1);
            float Triggerbot_Interval = 200.0f;
            float Triggerbot_Delay = 50.0f;
        };

        static SettingsStruct Settings;
    }
}


//AimbotView::AimbotView()
//{
//    this->m_bChecked = true;
//   /* this->myHeader = new  HeaderBar();*/
//   
//    //Hader
//    this->myHeader.Size = ImVec2(800, 40);
//    this->myHeader.LeftLabelText = "Left Animated";
//    this->myHeader.RightLabelText = "Right Static";
//    
//  
//  //Aimbot_Cheat
//    this->Aimbot_Cheat.ID = "func.aimbot";
//    this->Aimbot_Cheat.Title = "Aimbot";
//    this->Aimbot_Cheat.Size = ImVec2(200, 100);
//    this->Aimbot_Cheat.Description = "Automatically assists the player in aiming and firing at their targets.";
// 
//    // Triggerbot_Cheat
//    this->Triggerbot_Cheat.ID = "func.triggerbot";
//    this->Triggerbot_Cheat.Title = "Triggerbot";
//    this->Triggerbot_Cheat.Description = "Automatically firing at targets";
//    this->Triggerbot_Cheat.SetChecked(Core::Instances::Settings.Triggerbot);
//    this->Triggerbot_Cheat.Size = ImVec2(200, 100);
//    this->Triggerbot_Cheat.BorderPercent = 0.3f;
//    this->Triggerbot_Cheat.Animating = true;
//    
//    
//    
//    
//    this->currentOption = Aimbot_Cheat.ID;
//    
//    Widgets = { &Aimbot_Cheat  };
//     
//    for (FunctionWidget* view : Widgets)
//    {
//		view->CheckedChanged = [this, view]()
//			{
//				view->BorderOffset = 0.0f;
//			};
//        
//        view->ButtonClicked = [this, view]()    
//		{
//                if (this->currentOption == view->ID)
//                    return;
//                
//			this->currentOption = view->ID;
//            this->myHeader.LeftLabelText = "destroyer & substance ~& cd " + std::string("Combat") + "/" + this->currentOption;
//            this->myHeader.ResetAnimationTimer();
//		};
//    }
//     
//}


// Constructor
AimbotView::AimbotView()
{
    this->m_bChecked = true;

    // CONFIGURACIÓN Header
    this->myHeader.Size = ImVec2(800, 40);
    this->myHeader.LeftLabelText = "destroyer & substance ~& cd combat/killaura";
    this->myHeader.RightLabelText = "-/-";
    this->myHeader.TextRevealDelay = 0.05f;

    // AIMBOT CHEAT
    this->Aimbot_Cheat.ID = "func.aimbot";
    this->Aimbot_Cheat.Title = "Aimbot";
    this->Aimbot_Cheat.Description = "Automatically assists the player in aiming...";
    this->Aimbot_Cheat.SetChecked(Core::Instances::Settings.AIM);
    this->Aimbot_Cheat.Size = ImVec2(200, 100);
    this->Aimbot_Cheat.BorderPercent = 0.3f;
    this->Aimbot_Cheat.Animating = true;

    // TRIGGERBOT CHEAT
    this->Triggerbot_Cheat.ID = "func.triggerbot";
    this->Triggerbot_Cheat.Title = "Triggerbot";
    this->Triggerbot_Cheat.Description = "Automatically firing at targets";
    this->Triggerbot_Cheat.SetChecked(Core::Instances::Settings.Triggerbot);
    this->Triggerbot_Cheat.Size = ImVec2(200, 100);
    this->Triggerbot_Cheat.BorderPercent = 0.3f;
    this->Triggerbot_Cheat.Animating = true;

    // Los metemos en la lista
    Widgets = { &Aimbot_Cheat, &Triggerbot_Cheat };

    // Suscribimos eventos
    for (FunctionWidget* view : Widgets)
    {
        view->CheckedChanged = [this, view]()
            {
                // Lógica “CheckedChanged” (equivalente a tu C#)
                view->BorderOffset = 0.0f;

                // Ej: actualiza Core::Instances::Settings
                if (view->ID == Aimbot_Cheat.ID)
                {
                    Core::Instances::Settings.AIM = view->GetChecked();
                }
                else if (view->ID == Triggerbot_Cheat.ID)
                {
                    Core::Instances::Settings.Triggerbot = view->GetChecked();
                }
            };

        view->ButtonClicked = [this, view]()
            {
                // Equivalente a tu “currentOption = widget.ID”
                if (this->currentOption == view->ID)
                    return;

                this->currentOption = view->ID;
                // Actualizamos label
                this->myHeader.LeftLabelText = "destroyer & substance ~& cd " + std::string("Combat") + "/" + this->currentOption;
                this->myHeader.ResetAnimationTimer();
            };
    }

    // Por defecto: “func.aimbot”
    this->currentOption = Aimbot_Cheat.ID;

    // Ejemplo de widgets (Combo, TrackBar, ColorPicker):
    comboWidget.ID = "combo_1";
    comboWidget.Title = "Select Option";
    comboWidget.Description = "Este widget permite seleccionar una opción.";
    comboWidget.Size = ImVec2(190, 80);
    comboWidget.ComboBoxItems = { "Primero", "Segundo", "Tercero" };
    comboWidget.SelectedIndex = 0;
    comboWidget.SelectedIndexChanged = [this]()
        {
            int idx = comboWidget.SelectedIndex;
            std::cout << "Índice seleccionado: " << idx << std::endl;
        };

    trackBarWidget.Title = "Volumen";
    trackBarWidget.Description = "Ajusta el nivel de volumen.";
    trackBarWidget.Minimum = 0.0f;
    trackBarWidget.Maximum = 100.0f;
    trackBarWidget.Value = 50.0f;
    trackBarWidget.Size = ImVec2(190, 80);
    trackBarWidget.ValueChanged = [this]()
        {
            std::cout << "Nuevo valor TrackBar: " << trackBarWidget.Value << "\n";
        };

    colorPickerWidget.Title = "Selector de Color";
    colorPickerWidget.Description = "Elige un color para personalizar.";
    colorPickerWidget.EnableAlpha = true;
    colorPickerWidget.SelectedColor = ImVec4(1, 0, 0, 1);
    colorPickerWidget.Size = ImVec2(200, 80);
    colorPickerWidget.ColorChanged = [this]()
        {
            std::cout << "Nuevo color: "
                << colorPickerWidget.SelectedColor.x << ", "
                << colorPickerWidget.SelectedColor.y << ", "
                << colorPickerWidget.SelectedColor.z << ", "
                << colorPickerWidget.SelectedColor.w << "\n";
        };

    // EJEMPLO Aimbot Sub-widgets
    AIM_Visible_CheckWidget.ID = "AIM_Visible_Check_check";
    AIM_Visible_CheckWidget.Title = "AIM Visible";
    AIM_Visible_CheckWidget.Description = "Determines if target is visible";
    AIM_Visible_CheckWidget.SetChecked(Core::Instances::Settings.AIM_Visible_Check);
    AIM_Visible_CheckWidget.Size = ImVec2(200, 90);
    AIM_Visible_CheckWidget.CheckedChanged = [this]()
        {
            // Cambiar icono
            if (AIM_Visible_CheckWidget.GetChecked())
            {
                AIM_Visible_CheckWidget.BottomRightIconName = "check";
                AIM_Visible_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
            }
            else
            {
                AIM_Visible_CheckWidget.BottomRightIconName = "uncheck";
                AIM_Visible_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
            }
            // Guardar
            Core::Instances::Settings.AIM_Visible_Check = AIM_Visible_CheckWidget.GetChecked();
        }; 

    // 2) AIM_AimAtAll_CheckWidget
    AIM_AimAtAll_CheckWidget.ID = "AIM_AimAtAll_Check_check";
    AIM_AimAtAll_CheckWidget.Title = "AIM At All";
    AIM_AimAtAll_CheckWidget.Description = "Aimbot for everyone (including same team).";
    AIM_AimAtAll_CheckWidget.Size = ImVec2(200, 90);
    AIM_AimAtAll_CheckWidget.SetChecked(Core::Instances::Settings.AIM_AimAtAll);
    AIM_AimAtAll_CheckWidget.CheckedChanged = [this]()
        {
            if (AIM_AimAtAll_CheckWidget.GetChecked())
            {
                AIM_AimAtAll_CheckWidget.BottomRightIconName = "check";
                AIM_AimAtAll_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
            }
            else
            {
                AIM_AimAtAll_CheckWidget.BottomRightIconName = "uncheck";
                AIM_AimAtAll_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
            }
            Core::Instances::Settings.AIM_AimAtAll = AIM_AimAtAll_CheckWidget.GetChecked();
        };

    // 3) AIM_StickTarget_CheckWidget
    AIM_StickTarget_CheckWidget.ID = "AIM_StickTarget_Check_check";
    AIM_StickTarget_CheckWidget.Title = "Stick Target";
    AIM_StickTarget_CheckWidget.Description = "Maintain lock even if the target moves out temporarily.";
    AIM_StickTarget_CheckWidget.Size = ImVec2(200, 90);
    AIM_StickTarget_CheckWidget.SetChecked(Core::Instances::Settings.AIM_StickTarget);
    AIM_StickTarget_CheckWidget.CheckedChanged = [this]()
        {
            if (AIM_StickTarget_CheckWidget.GetChecked())
            {
                AIM_StickTarget_CheckWidget.BottomRightIconName = "check";
                AIM_StickTarget_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
            }
            else
            {
                AIM_StickTarget_CheckWidget.BottomRightIconName = "uncheck";
                AIM_StickTarget_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
            }
            Core::Instances::Settings.AIM_StickTarget = AIM_StickTarget_CheckWidget.GetChecked();
        };

    // 4) AIM_Location_ComboWidget
    AIM_Location_ComboWidget.ID = "AIM_Location_combo_1";
    AIM_Location_ComboWidget.Title = "AIM Target";
    AIM_Location_ComboWidget.Description = "Where the aimbot should target on the enemy player's model.";
    AIM_Location_ComboWidget.Size = ImVec2(200, 90);
    AIM_Location_ComboWidget.ComboBoxItems = {
        "BONE_HEAD","BONE_NECK","BONE_SPINE2","BONE_SPINE1","BONE_PSEUDO_SPINE"
    };
    AIM_Location_ComboWidget.SelectedIndex = Core::Instances::Settings.AIM_Location;
    AIM_Location_ComboWidget.SelectedIndexChanged = [this]()
        {
            Core::Instances::Settings.AIM_Location = AIM_Location_ComboWidget.SelectedIndex;
        };

    // 5) AIM_Fov_trackBarWidget
    AIM_Fov_trackBarWidget.ID = "AIM_Fov_trackBarWidget";
    AIM_Fov_trackBarWidget.Title = "FOV";
    AIM_Fov_trackBarWidget.Description = "Field of View for targeting.";
    AIM_Fov_trackBarWidget.Size = ImVec2(200, 90);
    AIM_Fov_trackBarWidget.Minimum = 0.0f;
    AIM_Fov_trackBarWidget.Maximum = 10.0f;
    AIM_Fov_trackBarWidget.Value = (float)Core::Instances::Settings.AIM_Fov;
    AIM_Fov_trackBarWidget.ValueChanged = [this]()
        {
            Core::Instances::Settings.AIM_Fov = (int)AIM_Fov_trackBarWidget.Value;
        };

    // 6) AIM_Type_ComboWidget
    AIM_Type_ComboWidget.ID = "AIM_Type_combo_1";
    AIM_Type_ComboWidget.Title = "AIM Type";
    AIM_Type_ComboWidget.Description = "Select Aimbot logic style.";
    AIM_Type_ComboWidget.Size = ImVec2(200, 90);
    AIM_Type_ComboWidget.ComboBoxItems = {
        "Auto",
        "FOV",
        "DISTANCE"
    };
    AIM_Type_ComboWidget.SelectedIndex = Core::Instances::Settings.AIM_Type;
    AIM_Type_ComboWidget.SelectedIndexChanged = [this]()
        {
            Core::Instances::Settings.AIM_Type = AIM_Type_ComboWidget.SelectedIndex;
        };

    // 7) AIM_Driver_First_CheckWidget
    AIM_Driver_First_CheckWidget.ID = "AIM_Driver_First_Check_check";
    AIM_Driver_First_CheckWidget.Title = "Driver First";
    AIM_Driver_First_CheckWidget.Description = "Target enemy drivers first.";
    AIM_Driver_First_CheckWidget.Size = ImVec2(200, 90);
    AIM_Driver_First_CheckWidget.SetChecked(Core::Instances::Settings.AIM_Driver_First);
    AIM_Driver_First_CheckWidget.CheckedChanged = [this]()
        {
            if (AIM_Driver_First_CheckWidget.GetChecked())
            {
                AIM_Driver_First_CheckWidget.BottomRightIconName = "check";
                AIM_Driver_First_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
            }
            else
            {
                AIM_Driver_First_CheckWidget.BottomRightIconName = "uncheck";
                AIM_Driver_First_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
            }
            Core::Instances::Settings.AIM_Driver_First = AIM_Driver_First_CheckWidget.GetChecked();
        };

    // 8) AIM_AutoAim_First_CheckWidget
    AIM_AutoAim_First_CheckWidget.ID = "AIM_AutoAim_First_Check_check";
    AIM_AutoAim_First_CheckWidget.Title = "Auto Aim";
    AIM_AutoAim_First_CheckWidget.Description = "Aimbot is always on auto, no manual activation.";
    AIM_AutoAim_First_CheckWidget.Size = ImVec2(200, 90);
    AIM_AutoAim_First_CheckWidget.SetChecked(Core::Instances::Settings.AIM_AutoAim);
    AIM_AutoAim_First_CheckWidget.CheckedChanged = [this]()
        {
            if (AIM_AutoAim_First_CheckWidget.GetChecked())
            {
                AIM_AutoAim_First_CheckWidget.BottomRightIconName = "check";
                AIM_AutoAim_First_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
            }
            else
            {
                AIM_AutoAim_First_CheckWidget.BottomRightIconName = "uncheck";
                AIM_AutoAim_First_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
            }
            Core::Instances::Settings.AIM_AutoAim = AIM_AutoAim_First_CheckWidget.GetChecked();
        };

    // 9) AIM_Vehicle_First_CheckWidget
    AIM_Vehicle_First_CheckWidget.ID = "AIM_Vehicle_First_Check_check";
    AIM_Vehicle_First_CheckWidget.Title = "Vehicle Aim";
    AIM_Vehicle_First_CheckWidget.Description = "Auto-targeting vehicles.";
    AIM_Vehicle_First_CheckWidget.Size = ImVec2(200, 90);
    AIM_Vehicle_First_CheckWidget.SetChecked(Core::Instances::Settings.AIM_Vehicle);
    AIM_Vehicle_First_CheckWidget.CheckedChanged = [this]()
        {
            if (AIM_Vehicle_First_CheckWidget.GetChecked())
            {
                AIM_Vehicle_First_CheckWidget.BottomRightIconName = "check";
                AIM_Vehicle_First_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
            }
            else
            {
                AIM_Vehicle_First_CheckWidget.BottomRightIconName = "uncheck";
                AIM_Vehicle_First_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
            }
            Core::Instances::Settings.AIM_Vehicle = AIM_Vehicle_First_CheckWidget.GetChecked();
        };

    // 10) AIM_Draw_Fov_CheckWidget
    AIM_Draw_Fov_CheckWidget.ID = "AIM_Draw_Fov_check";
    AIM_Draw_Fov_CheckWidget.Title = "Draw Fov";
    AIM_Draw_Fov_CheckWidget.Description = "Draw the aimbot FOV circle.";
    AIM_Draw_Fov_CheckWidget.Size = ImVec2(200, 90);
    AIM_Draw_Fov_CheckWidget.SetChecked(Core::Instances::Settings.AIM_Draw_Fov);
    AIM_Draw_Fov_CheckWidget.CheckedChanged = [this]()
        {
            if (AIM_Draw_Fov_CheckWidget.GetChecked())
            {
                AIM_Draw_Fov_CheckWidget.BottomRightIconName = "check";
                AIM_Draw_Fov_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
            }
            else
            {
                AIM_Draw_Fov_CheckWidget.BottomRightIconName = "uncheck";
                AIM_Draw_Fov_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
            }
            Core::Instances::Settings.AIM_Draw_Fov = AIM_Draw_Fov_CheckWidget.GetChecked();
        };

    // 11) AIM_Draw_TargetLine_CheckWidget
    AIM_Draw_TargetLine_CheckWidget.ID = "AIM_Draw_TargetLine_check";
    AIM_Draw_TargetLine_CheckWidget.Title = "Target Line";
    AIM_Draw_TargetLine_CheckWidget.Description = "Draw line from your crosshair to target.";
    AIM_Draw_TargetLine_CheckWidget.Size = ImVec2(200, 90);
    AIM_Draw_TargetLine_CheckWidget.SetChecked(Core::Instances::Settings.AIM_Draw_TargetLine);
    AIM_Draw_TargetLine_CheckWidget.CheckedChanged = [this]()
        {
            if (AIM_Draw_TargetLine_CheckWidget.GetChecked())
            {
                AIM_Draw_TargetLine_CheckWidget.BottomRightIconName = "check";
                AIM_Draw_TargetLine_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
            }
            else
            {
                AIM_Draw_TargetLine_CheckWidget.BottomRightIconName = "uncheck";
                AIM_Draw_TargetLine_CheckWidget.BottomRightIconBgColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
            }
            Core::Instances::Settings.AIM_Draw_TargetLine = AIM_Draw_TargetLine_CheckWidget.GetChecked();
        };

    // 12) AIM_Fov_Color_ColorPickerWidget
    AIM_Fov_Color_ColorPickerWidget.ID = "AIM_Fov_Color_ColorPickerWidget";
    AIM_Fov_Color_ColorPickerWidget.Title = "Fov Color";
    AIM_Fov_Color_ColorPickerWidget.Description = "Color for the FOV circle.";
    AIM_Fov_Color_ColorPickerWidget.Size = ImVec2(200, 90);
    AIM_Fov_Color_ColorPickerWidget.EnableAlpha = true;
    AIM_Fov_Color_ColorPickerWidget.SelectedColor = Core::Instances::Settings.AIM_Fov_Color;
    AIM_Fov_Color_ColorPickerWidget.ColorChanged = [this]()
        {
            Core::Instances::Settings.AIM_Fov_Color = AIM_Fov_Color_ColorPickerWidget.SelectedColor;
        };

    // 13) AIM_TargetColor_ColorPickerWidget
    AIM_TargetColor_ColorPickerWidget.ID = "AIM_TargetColor_ColorPickerWidget";
    AIM_TargetColor_ColorPickerWidget.Title = "Target Color";
    AIM_TargetColor_ColorPickerWidget.Description = "Line color to the target.";
    AIM_TargetColor_ColorPickerWidget.Size = ImVec2(200, 90);
    AIM_TargetColor_ColorPickerWidget.EnableAlpha = true;
    AIM_TargetColor_ColorPickerWidget.SelectedColor = Core::Instances::Settings.AIM_TargetColor;
    AIM_TargetColor_ColorPickerWidget.ColorChanged = [this]()
        {
            Core::Instances::Settings.AIM_TargetColor = AIM_TargetColor_ColorPickerWidget.SelectedColor;
        };

    //-----------------------------------
    // T R I G G E R   W I D G E T S
    //-----------------------------------
    Triggerbot_Interval_trackBarWidget.ID = "Triggerbot_Interval_trackBarWidget";
    Triggerbot_Interval_trackBarWidget.Title = "Fire Interval";
    Triggerbot_Interval_trackBarWidget.Description = "Firing duration (ms)";
    Triggerbot_Interval_trackBarWidget.Size = ImVec2(200, 90);
    Triggerbot_Interval_trackBarWidget.Minimum = 0.0f;
    Triggerbot_Interval_trackBarWidget.Maximum = 1000.0f;
    Triggerbot_Interval_trackBarWidget.Value = Core::Instances::Settings.Triggerbot_Interval;
    Triggerbot_Interval_trackBarWidget.ValueChanged = [this]()
        {
            Core::Instances::Settings.Triggerbot_Interval = Triggerbot_Interval_trackBarWidget.Value;
        };

    Triggerbot_Delay_trackBarWidget.ID = "Triggerbot_Delay_trackBarWidget";
    Triggerbot_Delay_trackBarWidget.Title = "Fire Delay";
    Triggerbot_Delay_trackBarWidget.Description = "Delay before firing (ms)";
    Triggerbot_Delay_trackBarWidget.Size = ImVec2(200, 90);
    Triggerbot_Delay_trackBarWidget.Minimum = 0.0f;
    Triggerbot_Delay_trackBarWidget.Maximum = 1000.0f;
    Triggerbot_Delay_trackBarWidget.Value = Core::Instances::Settings.Triggerbot_Delay;
    Triggerbot_Delay_trackBarWidget.ValueChanged = [this]()
        {
            Core::Instances::Settings.Triggerbot_Delay = Triggerbot_Delay_trackBarWidget.Value;
        };

    // etc. (O hazlo en Aimbot() si prefieres)
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


void AimbotView::Aimbot() {
    // Ejemplo de cómo dibujar en 3 columnas
    AIM_Visible_CheckWidget.Render();
    ImGui::SameLine(210);
    AIM_AimAtAll_CheckWidget.Render();
    ImGui::SameLine(420);
    AIM_StickTarget_CheckWidget.Render();

    // Siguiente fila
    AIM_Location_ComboWidget.Render();
    ImGui::SameLine(210);
    AIM_Type_ComboWidget.Render();
    ImGui::SameLine(420);
    AIM_Fov_trackBarWidget.Render();

    // Siguiente fila
    AIM_Driver_First_CheckWidget.Render();
    ImGui::SameLine(210);
    AIM_AutoAim_First_CheckWidget.Render();
    ImGui::SameLine(420);
    AIM_Vehicle_First_CheckWidget.Render();

    // Siguiente fila
    AIM_Draw_Fov_CheckWidget.Render();
    ImGui::SameLine(210);
    AIM_Draw_TargetLine_CheckWidget.Render();
    ImGui::SameLine(420);
    AIM_Fov_Color_ColorPickerWidget.Render();

    // Última fila
    AIM_TargetColor_ColorPickerWidget.Render();
}

void AimbotView::Trigger() {
    // dibujar trackbars
    Triggerbot_Interval_trackBarWidget.Render();
    ImGui::SameLine(210);
    Triggerbot_Delay_trackBarWidget.Render();
}


//void AimbotView::Render()
//{
//    ImVec2 windowSize = ImGui::GetIO().DisplaySize;
//
//    float leftSectionWidth = 220.0f;
//    ImGui::BeginChild("LeftSection", ImVec2(leftSectionWidth, 0));
//
//    float topMargin = 10.0f;
//    float bottomMargin = 10.0f;
//
//    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + topMargin);
//
//    for (FunctionWidget* view : Widgets) {
//        view->BorderOffset += 0.005f;
//        view->Animating = !(view->BorderOffset >= 1.0f);
//        view->Render();
//    }
//   /* foreach(var widget in Widgets)
//    {
//        widget.BorderOffset += 0.005f;
//        widget.Animating = !(widget.BorderOffset >= 1.0f);
//        widget.Render();
//    }*/
//    
//
//    ImGui::Dummy(ImVec2(0, bottomMargin));
//
//    ImGui::EndChild();
//
//    ImGui::SameLine();
//    ImGui::BeginChild("RightSection", ImVec2(windowSize.x - leftSectionWidth, 0));
//
//    float marginX = 15.0f; // Sangría horizontal (igual para ambos lados)
//    float marginY = 10.0f; // Sangría vertical
//
//    ImVec2 childStartPos = ImGui::GetCursorPos();
//    ImGui::SetCursorPos(ImVec2(0, childStartPos.y + marginY));
//
//    float adjustedWidth = (windowSize.x - leftSectionWidth) - (2 * marginX);
//    //headerBar.Size = new ImVec2(adjustedWidth - 200, headerBar.Size.y);
//    myHeader.Size = ImVec2(adjustedWidth - 200, myHeader.Size.y);
//    //headerBar.Render(ImGui::GetIO().DeltaTime);
//    double dt = ImGui::GetIO().DeltaTime; // deltaTime
//
//    myHeader.Render(dt);
//    
//    
//    if (currentOption == Aimbot_Cheat.ID)
//    {
//        Aimbot();
//    }
//    /*
//    else if (currentOption == Triggerbot_Cheat.ID)
//    {
//        Trigger();
//    }
//    */
//
//    ImGui::EndChild();
//}


void AimbotView::Render()
{
    ImVec2 windowSize = ImGui::GetIO().DisplaySize;

    float leftSectionWidth = 220.0f;
    ImGui::BeginChild("LeftSection", ImVec2(leftSectionWidth, 0));

    float topMargin = 10.0f;
    float bottomMargin = 10.0f;

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + topMargin);

    // Dibuja la columna izquierda con FunctionWidgets
    for (FunctionWidget* view : Widgets)
    {
        // Simular animación
        view->BorderOffset += 0.005f;
        view->Animating = !(view->BorderOffset >= 1.0f);

        view->Render();
    }

    ImGui::Dummy(ImVec2(0, bottomMargin));
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("RightSection", ImVec2(windowSize.x - leftSectionWidth, 0));

    float marginX = 15.0f;
    float marginY = 10.0f;

    ImVec2 childStartPos = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(0, childStartPos.y + marginY));

    float adjustedWidth = (windowSize.x - leftSectionWidth) - (2 * marginX);

    // Actualizar tamaño del header
    myHeader.Size = ImVec2(adjustedWidth - 200.0f, myHeader.Size.y);

    // Render header
    double dt = ImGui::GetIO().DeltaTime;
    myHeader.Render(dt);

    // Decidir qué panel de la derecha se dibuja (Aimbot o Trigger)
    if (currentOption == Aimbot_Cheat.ID)
    {
        Aimbot();
        // Llamar a subwidgets de Aimbot (ej. AIM_Visible_CheckWidget.Render() etc.)
        // ...
    }
    else if (currentOption == Triggerbot_Cheat.ID)
    {
        Trigger();
        // Llamar a subwidgets de Trigger
        // ...
    }

    // Ejemplo: render de tus 3 test widgets
    // comboWidget.Render();
    // trackBarWidget.Render();
    // colorPickerWidget.Render();

    ImGui::EndChild();
}
 