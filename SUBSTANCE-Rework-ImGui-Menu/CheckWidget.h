#pragma once
// ------------------------------------------------------------------------------
// Clase CheckWidget en C++
//
class CheckWidget
{
public:
    std::string ID = "check.default";
    std::string Title = "Check Widget";
    std::string Description = "Este widget incluye funcionalidad de selección.";
    ImVec2      Size = ImVec2(300.0f, 150.0f);

    ImVec4 BackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    ImVec4 TitleColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 DescriptionColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    ImVec4 BorderColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    float MarginX = 15.0f;
    float MarginY = 10.0f;
    float CornerSize = 15.0f;
    float LineThickness = 1.0f;
    float BorderPercent = 0.5f;

    float BorderOffset = 0.0f;

private:
    bool _checked = false;

public:
    // Propiedad Checked con evento OnCheckedChanged
    bool GetChecked() const { return _checked; }
    void SetChecked(bool value)
    {
        if (_checked != value)
        {
            _checked = value;
            OnCheckedChanged(); // Dispara el evento
        }
    }

    // Evento, en C++ podríamos usar std::function
    std::function<void()> CheckedChanged;

protected:
    void OnCheckedChanged()
    {
        if (CheckedChanged) CheckedChanged();
    }

public:
    float FadeSpeed = 0.1f;
    float currentBorderAlpha = 0.0f;

    // Propiedades del botón ícono
    std::string BottomRightIconName = "check_icon";
    ImVec4      BottomRightIconBgColor = ImVec4(0.3f, 1.0f, 0.3f, 1.0f);
    bool        IconButtonVisible = true;
    float       IconButtonRounding = 0.0f;
    float       IconButtonSize = 15.0f;

    bool        Enabled = true;

    // Render
    void Render()
    {
        ImGui::BeginChild(("Section" + ID).c_str(), Size, false, ImGuiWindowFlags_NoScrollbar);

        // Asegurar BorderPercent en [0..1] 
        BorderPercent = std::max<float>(0.0f, std::min<float>(1.0f, BorderPercent));

        // Ajustar colores si está deshabilitado
        ImVec4 bgColor = BackgroundColor;
        ImVec4 titleColor = TitleColor;
        ImVec4 descColor = DescriptionColor;
        ImVec4 borderColor = BorderColor;
        ImVec4 iconBgColor = BottomRightIconBgColor;

        if (!Enabled)
        {
            bgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
            ImVec4 grayLight = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
            titleColor = grayLight;
            descColor = grayLight;
            borderColor = grayLight;
            iconBgColor = grayLight;
        }

        ImVec2 widgetMin = ImGui::GetCursorScreenPos();
        ImVec2 widgetMax = ImVec2(widgetMin.x + Size.x, widgetMin.y + Size.y);
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // Dibujar fondo
        drawList->AddRectFilled(widgetMin, widgetMax, ImGui::ColorConvertFloat4ToU32(bgColor));

        ImVec2 localMin = ImVec2(widgetMin.x - ImGui::GetWindowPos().x, widgetMin.y - ImGui::GetWindowPos().y);
        ImVec2 localMax = ImVec2(widgetMax.x - ImGui::GetWindowPos().x, widgetMax.y - ImGui::GetWindowPos().y);

        float wrapPos = localMax.x - MarginX;
        ImGui::PushTextWrapPos(wrapPos);

        // Título
        ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY));
        //ImFont* widgetTitleFont = Core::Instances::fontManager.GetFont("widget_title");
        //ImGui::PushFont(widgetTitleFont);
        ImGui::TextColored(titleColor, "%s", Title.c_str());
        //ImGui::PopFont();

        ImVec2 titleSize = ImGui::CalcTextSize(Title.c_str());
        float lineHeight = ImGui::GetTextLineHeight();
        float descriptionTopOffset = lineHeight + (MarginY * 0.5f);

        // Descripción
        ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY + descriptionTopOffset));
       /* ImFont* widgetDesFont = Core::Instances::fontManager.GetFont("widget_des");
        ImGui::PushFont(widgetDesFont);*/
        ImGui::TextColored(descColor, "%s", Description.c_str());
        //ImGui::PopFont();

        ImGui::PopTextWrapPos();

        // Borde (fade)
        float targetAlpha = _checked ? 1.0f : 0.0f;
        currentBorderAlpha += (targetAlpha - currentBorderAlpha) * FadeSpeed;
        ImVec4 adjustedBorderColor = ImVec4(borderColor.x, borderColor.y, borderColor.z, borderColor.w * currentBorderAlpha);
        ImU32 borderColU32 = ImGui::ColorConvertFloat4ToU32(adjustedBorderColor);

        if (currentBorderAlpha > 0.01f)
        {
            DrawStaticBorder(drawList, widgetMin, widgetMax, BorderPercent, borderColU32, LineThickness);
        }

        // Área del botón ícono
        ImVec2 iconBoxMin = ImVec2(widgetMax.x - MarginX - IconButtonSize, widgetMax.y - MarginY - IconButtonSize);
        ImVec2 iconBoxMax = ImVec2(iconBoxMin.x + IconButtonSize, iconBoxMin.y + IconButtonSize);

        // Dibujar ícono si es visible
        if (IconButtonVisible && CheckedChanged) // asumiendo que la acción recae en el click
        {
            ImU32 iconBgU32 = ImGui::ColorConvertFloat4ToU32(iconBgColor);
            drawList->AddRectFilled(iconBoxMin, iconBoxMax, iconBgU32, IconButtonRounding);

            /*ImTextureID icon = Core::Instances::imageManager.GetImage(BottomRightIconName);
            if (icon != nullptr)
            {
                drawList->AddImage(icon, iconBoxMin, iconBoxMax);
            }*/
        }

        // InvisibleButton para todo el widget
        ImGui::SetCursorScreenPos(widgetMin);
        if (ImGui::InvisibleButton(ID.c_str(), Size))
        {
            ImVec2 mousePos = ImGui::GetIO().MousePos;
            bool insideIcon = (mousePos.x >= iconBoxMin.x && mousePos.x < iconBoxMax.x &&
                mousePos.y >= iconBoxMin.y && mousePos.y < iconBoxMax.y);

            if (Enabled)
            {
                // Al hacer clic, togglear el check
                SetChecked(!GetChecked());
            }
        }

        ImGui::EndChild();
    }

private:
    void DrawStaticBorder(ImDrawList* drawList, ImVec2 widgetMin, ImVec2 widgetMax, float borderPercent, ImU32 borderColU32, float lineThickness)
    {
        auto horizontalLines = CalcHorizontalLines(widgetMin.x, widgetMax.x, CornerSize, borderPercent);
        auto verticalLines = CalcVerticalLines(widgetMin.y, widgetMax.y, CornerSize, borderPercent);

        float topMinX = std::get<0>(horizontalLines);
        float topMidX = std::get<1>(horizontalLines);
        float topMidX2 = std::get<2>(horizontalLines);
        float topMaxX = std::get<3>(horizontalLines);

        float botMinX = std::get<0>(horizontalLines);
        float botMidX = std::get<1>(horizontalLines);
        float botMidX2 = std::get<2>(horizontalLines);
        float botMaxX = std::get<3>(horizontalLines);

        float leftMinY = std::get<0>(verticalLines);
        float leftMidY = std::get<1>(verticalLines);
        float leftMidY2 = std::get<2>(verticalLines);
        float leftMaxY = std::get<3>(verticalLines);

        float rightMinY = std::get<0>(verticalLines);
        float rightMidY = std::get<1>(verticalLines);
        float rightMidY2 = std::get<2>(verticalLines);
        float rightMaxY = std::get<3>(verticalLines);
        
        float rightX = widgetMax.x - 1.0f;

        // Top
        drawList->AddLine(ImVec2(topMinX, widgetMin.y), ImVec2(topMidX, widgetMin.y), borderColU32, lineThickness);
        drawList->AddLine(ImVec2(topMidX2, widgetMin.y), ImVec2(topMaxX, widgetMin.y), borderColU32, lineThickness);

        // Bottom
        drawList->AddLine(ImVec2(botMinX, widgetMax.y), ImVec2(botMidX, widgetMax.y), borderColU32, lineThickness);
        drawList->AddLine(ImVec2(botMidX2, widgetMax.y), ImVec2(botMaxX, widgetMax.y), borderColU32, lineThickness);

        // Left
        drawList->AddLine(ImVec2(widgetMin.x, leftMinY), ImVec2(widgetMin.x, leftMidY), borderColU32, lineThickness);
        drawList->AddLine(ImVec2(widgetMin.x, leftMidY2), ImVec2(widgetMin.x, leftMaxY), borderColU32, lineThickness);

        // Right
        drawList->AddLine(ImVec2(rightX, rightMinY), ImVec2(rightX, rightMidY), borderColU32, lineThickness);
        drawList->AddLine(ImVec2(rightX, rightMidY2), ImVec2(rightX, rightMaxY), borderColU32, lineThickness);
    }

    std::tuple<float, float, float, float> CalcHorizontalLines(float minX, float maxX, float cSize, float p)
    {
        float totalLength = maxX - minX;
        float initialGap = totalLength - 2.0f * cSize;
        float gapActual = initialGap * (1.0f - p);
        float expand = (initialGap - gapActual) * 0.5f;

        float leftLineEnd = minX + cSize + expand;
        float rightLineStart = maxX - cSize - expand;
        return { minX, leftLineEnd, rightLineStart, maxX };
    }

    std::tuple<float, float, float, float> CalcVerticalLines(float minY, float maxY, float cSize, float p)
    {
        float totalLength = maxY - minY;
        float initialGap = totalLength - 2.0f * cSize;
        float gapActual = initialGap * (1.0f - p);
        float expand = (initialGap - gapActual) * 0.5f;

        float topLineEnd = minY + cSize + expand;
        float bottomLineStart = maxY - cSize - expand;
        return { minY, topLineEnd, bottomLineStart, maxY };
    }
};

// ------------------------------------------------------------------------------
// Ejemplo de uso:
//
//  CheckWidget checkW;
//  checkW.Title = "Mi Check";
//  checkW.Description = "Muestra un check e icono";
//  checkW.CheckedChanged = [](){
//      std::cout << "Check toggled!\n";
//  };
//
//  // En tu bucle de render:
//  checkW.Render();
//
// ------------------------------------------------------------------------------

