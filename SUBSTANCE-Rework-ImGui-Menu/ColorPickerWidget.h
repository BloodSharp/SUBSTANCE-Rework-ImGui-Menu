#pragma once
// -----------------------------------------------------------------------------
// Clase ColorPickerWidget en C++
//
class ColorPickerWidget
{
public:
    std::string ID = "colorpicker.default";
    std::string Title = "Color Picker";
    std::string Description = "Selecciona un color.";
    ImVec2      Size = ImVec2(300.0f, 150.0f);

    ImVec4 BackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    ImVec4 TitleColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 DescriptionColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);

    // Propiedades del selector de color
    ImVec4 SelectedColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    bool   EnableAlpha = true;  // Mostrar barra de alpha

    // Evento ColorChanged (en C++: std::function<void()> o similar)
    std::function<void()> ColorChanged;

protected:
    void OnColorChanged()
    {
        if (ColorChanged) ColorChanged();
    }

public:
    void Render()
    {
        // Crear un child region
        ImGui::BeginChild(("Section" + ID).c_str(), Size, false, ImGuiWindowFlags_NoScrollbar);

        // Clonar las variables de color
        ImVec4 bgColor = BackgroundColor;
        ImVec4 titleColor = TitleColor;
        ImVec4 descColor = DescriptionColor;

        ImVec2 widgetMin = ImGui::GetCursorScreenPos();
        ImVec2 widgetMax = ImVec2(widgetMin.x + Size.x, widgetMin.y + Size.y);
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // Fondo
        drawList->AddRectFilled(widgetMin, widgetMax, ImGui::ColorConvertFloat4ToU32(bgColor));

        // Convertir a "local" pos
        ImVec2 localMin = ImVec2(widgetMin.x - ImGui::GetWindowPos().x, widgetMin.y - ImGui::GetWindowPos().y);
        ImVec2 localMax = ImVec2(widgetMax.x - ImGui::GetWindowPos().x, widgetMax.y - ImGui::GetWindowPos().y);

        float wrapPos = localMax.x - 15.0f;
        ImGui::PushTextWrapPos(wrapPos);

        // Título
        ImGui::SetCursorPos(ImVec2(localMin.x + 15.0f, localMin.y + 10.0f));
        /*ImFont* fontTitle = Core::Instances::fontManager.GetFont("widget_title");
        ImGui::PushFont(fontTitle);*/
        ImGui::TextColored(titleColor, "%s", Title.c_str());
        //ImGui::PopFont();

        // Descripción
        ImVec2 titleSize = ImGui::CalcTextSize(Title.c_str());
        float  lineHeight = ImGui::GetTextLineHeight();
        float  descTopOff = lineHeight + 5.0f;

        ImGui::SetCursorPos(ImVec2(localMin.x + 15.0f, localMin.y + 10.0f + descTopOff));
        /*ImFont* fontDes = Core::Instances::fontManager.GetFont("widget_des");
        ImGui::PushFont(fontDes);*/
        ImGui::TextColored(descColor, "%s", Description.c_str());
        //ImGui::PopFont();

        ImGui::PopTextWrapPos();

        // Renderizar el selector de color en la esquina inferior derecha
        float margin = 15.0f;
        ImVec2 colorPickerPos = ImVec2(
            localMax.x - margin * 2.0f,
            localMax.y - margin * 2.0f
        );
        ImGui::SetCursorPos(colorPickerPos);
        ImGui::PushItemWidth(100.0f); // ancho fijo de 100

        // Guardar color anterior
        ImVec4 oldColor = SelectedColor;

        if (EnableAlpha)
        {
            // RGBA
            if (ImGui::ColorEdit4(("##ColorPicker" + ID).c_str(), (float*)&SelectedColor, ImGuiColorEditFlags_NoInputs))
            {
                // Comparar oldColor vs newColor
                if (!(oldColor.x == SelectedColor.x &&
                    oldColor.y == SelectedColor.y &&
                    oldColor.z == SelectedColor.z &&
                    oldColor.w == SelectedColor.w))
                {
                    OnColorChanged();
                }
            }
        }
        else
        {
            // Solo RGB (comentar/descomentar según tu necesidad)
            // if (ImGui::ColorEdit3(("##ColorPicker" + ID).c_str(), (float*)&SelectedColor, ImGuiColorEditFlags_NoInputs))
            // {
            //     if (!(oldColor.x == SelectedColor.x &&
            //           oldColor.y == SelectedColor.y &&
            //           oldColor.z == SelectedColor.z))
            //     {
            //         OnColorChanged();
            //     }
            // }
        }

        ImGui::PopItemWidth();

        ImGui::EndChild();
    }
};

// -----------------------------------------------------------------------------
// Ejemplo de uso:
//
//  ColorPickerWidget colorPicker;
//  colorPicker.Title = "Mi Selector de Color";
//  colorPicker.Description = "Elige tu color preferido";
//  colorPicker.ColorChanged = [&](){
//      // Se invoca cuando el color cambia
//  };
//
//  // En tu bucle principal de render:
//  colorPicker.Render();
//
// -----------------------------------------------------------------------------

