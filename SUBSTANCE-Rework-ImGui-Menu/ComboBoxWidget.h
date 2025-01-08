#pragma once
// ---------------------------------------------------------------------------------
// Clase ComboBoxWidget en C++
//
class ComboBoxWidget
{
public:
    std::string ID = "combo.default";
    std::string Title = "Mi Widget";
    std::string Description = "Este widget tiene un combobox interactivo.";
    ImVec2      Size = ImVec2(300.0f, 150.0f);

    ImVec4 BackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    ImVec4 TitleColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 DescriptionColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);

    float MarginX = 15.0f;
    float MarginY = 10.0f;
    float ComboBoxMargin = 10.0f;

    // Lista de items del ComboBox
    std::vector<std::string> ComboBoxItems
    {
        "Opción 1",
        "Opción 2",
        "Opción 3"
    };
    int SelectedIndex = 0;

    // Estilos del ComboBox
    ImVec4 ComboBoxTextColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 ComboBoxBackgroundColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    ImVec4 ComboBoxHoveredColor = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
    ImVec4 ComboBoxActiveColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    ImVec4 ComboBoxItemHoveredColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
    ImVec4 ComboBoxItemSelectedColor = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

    // Evento: SelectedIndexChanged
    std::function<void()> SelectedIndexChanged;

protected:
    void OnSelectedIndexChanged()
    {
        if (SelectedIndexChanged) SelectedIndexChanged();
    }

public:
    void Render()
    {
        // Asegurarnos de que la altura sea suficiente para el margen + 2 líneas + combobox
        // en C++ no siempre sabrás la textlineheight sin ImGui. Se asume a posteriori.
        float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
        Size.y = std::max<float>(
            Size.y,
            MarginY + 2.0f * textLineHeight + ComboBoxMargin
        );

        ImGui::BeginChild(
            ("Section" + ID).c_str(),
            Size,
            false,
            ImGuiWindowFlags_NoScrollbar
        );

        ImVec4 bgColor = BackgroundColor;
        ImVec4 titleColor = TitleColor;
        ImVec4 descColor = DescriptionColor;

        ImVec2 widgetMin = ImGui::GetCursorScreenPos();
        ImVec2 widgetMax = ImVec2(widgetMin.x + Size.x, widgetMin.y + Size.y);
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // Dibujar fondo
        drawList->AddRectFilled(
            widgetMin,
            widgetMax,
            ImGui::ColorConvertFloat4ToU32(bgColor)
        );

        // Calcular localMin / localMax
        ImVec2 localMin = ImVec2(widgetMin.x - ImGui::GetWindowPos().x, widgetMin.y - ImGui::GetWindowPos().y);
        ImVec2 localMax = ImVec2(widgetMax.x - ImGui::GetWindowPos().x, widgetMax.y - ImGui::GetWindowPos().y);

        float wrapPos = localMax.x - MarginX;
        ImGui::PushTextWrapPos(wrapPos);

        // Título
        ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY));
      /*  ImFont* fontTitle = Core::Instances::fontManager.GetFont("widget_title");
        ImGui::PushFont(fontTitle);*/
        ImGui::TextColored(titleColor, "%s", Title.c_str());
        //ImGui::PopFont();

        ImVec2 titleSize = ImGui::CalcTextSize(Title.c_str());
        float lineHeight = ImGui::GetTextLineHeight();
        float descriptionTopOffset = lineHeight + (MarginY * 0.5f);

        // Descripción
        ImGui::SetCursorPos(
            ImVec2(localMin.x + MarginX,
                localMin.y + MarginY + descriptionTopOffset)
        );
       /* ImFont* fontDes = Core::Instances::fontManager.GetFont("widget_des");
        ImGui::PushFont(fontDes);*/
        ImGui::TextColored(descColor, "%s", Description.c_str());
        //ImGui::PopFont();

        ImGui::PopTextWrapPos();

        // Posición del ComboBox
        float comboBoxY = localMax.y - ComboBoxMargin - textLineHeight;
        ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, comboBoxY));
        ImGui::PushItemWidth(Size.x - (2.0f * MarginX));

        // Estilos del ComboBox
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertFloat4ToU32(ComboBoxBackgroundColor));
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32(ComboBoxTextColor));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImGui::ColorConvertFloat4ToU32(ComboBoxHoveredColor));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImGui::ColorConvertFloat4ToU32(ComboBoxActiveColor));
        ImGui::PushStyleColor(ImGuiCol_Button, ImGui::ColorConvertFloat4ToU32(ComboBoxBackgroundColor));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::ColorConvertFloat4ToU32(ComboBoxHoveredColor));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImGui::ColorConvertFloat4ToU32(ComboBoxActiveColor));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGui::ColorConvertFloat4ToU32(ComboBoxItemHoveredColor));
        ImGui::PushStyleColor(ImGuiCol_Header, ImGui::ColorConvertFloat4ToU32(ComboBoxItemSelectedColor));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImGui::ColorConvertFloat4ToU32(ComboBoxItemSelectedColor));

        // Convertir std::vector<std::string> a array de const char*
        std::vector<const char*> itemsCStr;
        itemsCStr.reserve(ComboBoxItems.size());
        for (auto& s : ComboBoxItems)
        {
            itemsCStr.push_back(s.c_str());
        }

        // Combo
        // Esperamos que SelectedIndex no se salga de bounds.
        // Reemplaza "ImGui::Combo" (label, &SelectedIndex, itemsCStr.data(), itemsCStr.size())
        // con la versión que uses en c++.
        if (ImGui::Combo(
            ("##ComboBox" + ID).c_str(),
            &SelectedIndex,
            itemsCStr.data(),
            static_cast<int>(itemsCStr.size())
        ))
        {
            // Llamar al evento
            OnSelectedIndexChanged();
        }

        ImGui::PopStyleColor(10);
        ImGui::PopItemWidth();

        ImGui::EndChild();
    }
};

//
// Ejemplo de uso (pseudocódigo):
//
//  ComboBoxWidget comboW;
//  comboW.Title = "Mi Widget Combo";
//  comboW.Description = "Tiene un combo interactivo.";
//  comboW.ComboBoxItems = {"Item A", "Item B", "Item C"};
//  comboW.SelectedIndexChanged = [](){
//      std::cout << "Cambió la selección!\n";
//  };
//
//  // En tu bucle principal de render:
//  comboW.Render();
//

