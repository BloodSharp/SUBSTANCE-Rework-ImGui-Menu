#pragma once

#include "pch.h"

//class AimbotView : public IView
//{
//public:
//    AimbotView();
//    const char* getID() override;
//    const char* getText() override;
//    ImTextureID virtual getIcon();
//    void Render() override;
//    HeaderBar myHeader;
//    FunctionWidget Aimbot_Cheat;
//    std::vector<FunctionWidget*> Widgets;
//    std::string currentOption;
//};
//

class AimbotView : public IView
{
public:
    AimbotView();

    // Métodos de la interfaz IView
    virtual const char* getID() override;
    virtual const char* getText() override;
    virtual ImTextureID getIcon() override;
    virtual void Render() override;

private:
    // Miembros
    bool m_bChecked = false; // Ejemplo de bool interno si lo necesitas

    // Header principal
    HeaderBar myHeader;

    // Funciones/cheats en la columna izquierda
    FunctionWidget Aimbot_Cheat;
    FunctionWidget Triggerbot_Cheat;
    std::vector<FunctionWidget*> Widgets;

    // Opción actual (Aimbot/Triggerbot)
    std::string currentOption;

    // === Combos, trackbars, color pickers de la parte derecha ===
    // Ejemplo de 3 widgets para test:
    ComboBoxWidget comboWidget;
    TrackBarWidget trackBarWidget;
    ColorPickerWidget colorPickerWidget;

    // Widgets del Aimbot
    bool ConfigOptions = false;
    CheckWidget AIM_Visible_CheckWidget;
    CheckWidget AIM_AimAtAll_CheckWidget;
    CheckWidget AIM_StickTarget_CheckWidget;
    ComboBoxWidget AIM_Location_ComboWidget;
    TrackBarWidget AIM_Fov_trackBarWidget;
    ComboBoxWidget AIM_Type_ComboWidget;
    CheckWidget AIM_Driver_First_CheckWidget;
    CheckWidget AIM_AutoAim_First_CheckWidget;
    CheckWidget AIM_Vehicle_First_CheckWidget;
    CheckWidget AIM_Draw_Fov_CheckWidget;
    CheckWidget AIM_Draw_TargetLine_CheckWidget;
    ColorPickerWidget AIM_Fov_Color_ColorPickerWidget;
    ColorPickerWidget AIM_TargetColor_ColorPickerWidget;

    // Widgets del Trigger
    bool ConfigTriggerOptions = false;
    TrackBarWidget Triggerbot_Interval_trackBarWidget;
    TrackBarWidget Triggerbot_Delay_trackBarWidget;

private:
    // Métodos privados equivalentes a "Aimbot()" y "Trigger()"
    void Aimbot();
    void Trigger();

    // Callbacks que replican Aim_Checks() y Trigger_Checks() del C#.
    void Aim_Checks(void* sender);
    void Trigger_Checks(void* sender);

    // (Opcional) método para UpdateColors() si lo necesitas
    void UpdateColors(FunctionWidget* widget);
};
