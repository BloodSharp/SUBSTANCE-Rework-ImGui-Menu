#pragma once

// ---------------------------------------------------------------------------------
// Clase FunctionWidget en C++
//
class FunctionWidget
{
public:
    std::string ID = "func.default";
    std::string Title = "Mi Función";
    std::string Description = "Esta función se encarga de realizar operaciones complejas.";
    ImVec2      Size = ImVec2(300.0f, 150.0f);

    ImVec4 BackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    ImVec4 TitleColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 DescriptionColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
    ImVec4 BorderColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    ImVec4 OnColor = ImVec4(0.3f, 1.0f, 0.3f, 1.0f);
    ImVec4 OffColor = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
    bool   ShowStatusLabel = true;

    float  MarginX = 15.0f;
    float  MarginY = 10.0f;
    float  CornerSize = 15.0f;
    float  LineThickness = 1.0f;
    float  BorderPercent = 0.5f;

    bool   Animating = false;
    float  BorderOffset = 0.0f;

    // NUEVA PROPIEDAD para controlar la velocidad de animación
    float  AnimationSpeed = 0.9f;

    // Checked
private:
    bool _checked = false;
public:
    bool GetChecked() const { return _checked; }
    void SetChecked(bool value)
    {
        if (_checked != value)
        {
            _checked = value;
            // Disparar evento
            OnCheckedChanged();
        }
    }

    // Evento en C++: podrías usar un std::function o algo similar
    std::function<void()> CheckedChanged = nullptr;
protected:
    void OnCheckedChanged()
    {
        if (CheckedChanged) CheckedChanged();
    }

public:
    float FadeSpeed = 0.1f;
    float currentBorderAlpha = 0.0f;

    // Icon Button
    std::string BottomRightIconName = "atom_icon";
    ImVec4      BottomRightIconBgColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    bool        IconButtonVisible = true;
    float       IconButtonRounding = 5.0f;
    float       IconButtonSize = 20.0f;

    bool        Enabled = true;

    // Evento para el botón ícono
    std::function<void()> ButtonClicked = nullptr;

    // Render
    void Render()
    {
        // Iniciar un child region
        ImGui::BeginChild(("Section" + ID).c_str(), Size, false, ImGuiWindowFlags_NoScrollbar);

        // Asegurar que BorderPercent esté entre 0..1
        BorderPercent = std::max<float>(0.0f, std::min<float>(1.0f, BorderPercent));

        // Ajustar colores si está deshabilitado
        ImVec4 bgColor = BackgroundColor;
        ImVec4 titleColor = TitleColor;
        ImVec4 descColor = DescriptionColor;
        ImVec4 borderColor = BorderColor;
        ImVec4 onColor = OnColor;
        ImVec4 offColor = OffColor;
        ImVec4 iconBgColor = BottomRightIconBgColor;

        if (!Enabled)
        {
            bgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
            ImVec4 gray = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
            titleColor = gray;
            descColor = gray;
            borderColor = gray;
            onColor = gray;
            offColor = gray;
            iconBgColor = gray;
        }

        ImVec2 widgetMin = ImGui::GetCursorScreenPos();
        ImVec2 widgetMax = ImVec2(widgetMin.x + Size.x, widgetMin.y + Size.y);
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // Fondo
        drawList->AddRectFilled(widgetMin, widgetMax, ImGui::ColorConvertFloat4ToU32(bgColor));

        // Calcular posiciones "local"
        ImVec2 localMin = ImVec2(widgetMin.x - ImGui::GetWindowPos().x, widgetMin.y - ImGui::GetWindowPos().y);
        ImVec2 localMax = ImVec2(widgetMax.x - ImGui::GetWindowPos().x, widgetMax.y - ImGui::GetWindowPos().y);

        float wrapPos = localMax.x - MarginX;
        ImGui::PushTextWrapPos(wrapPos);

        // Título
        ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY));
        ImVec4 actualTitleColor = _checked ? titleColor : descColor;

        // Fuentes personalizadas (ejemplo)
       /* ImFont* widgetTitleFont = Core::Instances::fontManager.GetFont("widget_title");*/
        //ImGui::PushFont(widgetTitleFont);
        ImGui::TextColored(actualTitleColor, "%s", Title.c_str());
        //ImGui::PopFont();

        ImVec2 titleSize = ImGui::CalcTextSize(Title.c_str());
        float lineHeight = ImGui::GetTextLineHeight();
        float descriptionTopOffset = lineHeight + (MarginY * 0.5f);

        // Descripción
        ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY + descriptionTopOffset));
        //ImFont* widgetDesFont = Core::Instances::fontManager.GetFont("widget_des");
        //ImGui::PushFont(widgetDesFont);
        ImGui::TextColored(descColor, "%s", Description.c_str());
        //ImGui::PopFont();

        if (ShowStatusLabel)
        {
            // ON / OFF
            std::string onOffText = _checked ? "ON" : "OFF";
            ImVec2 onOffSize = ImGui::CalcTextSize(onOffText.c_str());

            float bottomY = localMax.y - MarginY - onOffSize.y;
            float bottomX = localMin.x + MarginX;
            ImGui::SetCursorPos(ImVec2(bottomX, bottomY));

            ImVec4 onOffVec = _checked ? onColor : offColor;
            ImGui::TextColored(onOffVec, "%s", onOffText.c_str());

            ImGui::PopTextWrapPos();
        }

        // Borde
        float targetAlpha = _checked ? 1.0f : 0.0f;
        currentBorderAlpha += (targetAlpha - currentBorderAlpha) * FadeSpeed;
        ImVec4 adjustedBorderColor = ImVec4(borderColor.x, borderColor.y, borderColor.z, borderColor.w * currentBorderAlpha);
        ImU32 borderColU32 = ImGui::ColorConvertFloat4ToU32(adjustedBorderColor);

        if (currentBorderAlpha > 0.01f)
        {
            if (Animating)
            {
                float dt = ImGui::GetIO().DeltaTime;
                BorderOffset += dt * AnimationSpeed;
                BorderOffset = fmodf(BorderOffset, 1.0f); // ciclo

                DrawAnimatedBorder(drawList, widgetMin, widgetMax, BorderPercent, borderColU32, LineThickness);
            }
            else
            {
                DrawStaticBorder(drawList, widgetMin, widgetMax, BorderPercent, borderColU32, LineThickness);
            }
        }

        // Área del botón ícono
        ImVec2 iconBoxMin = ImVec2(widgetMax.x - MarginX - IconButtonSize, widgetMax.y - MarginY - IconButtonSize);
        ImVec2 iconBoxMax = ImVec2(iconBoxMin.x + IconButtonSize, iconBoxMin.y + IconButtonSize);

        if (IconButtonVisible)
        {
            ImU32 iconBgU32 = ImGui::ColorConvertFloat4ToU32(iconBgColor);
            // Relleno del fondo
            drawList->AddRectFilled(iconBoxMin, iconBoxMax, iconBgU32, IconButtonRounding);

            // Intentar dibujar la imagen
            /*ImTextureID icon = Core::Instances::imageManager.GetImage(BottomRightIconName);
            if (icon != nullptr)
            {
                drawList->AddImage(icon, iconBoxMin, iconBoxMax);
            }*/
        }

        // InvisibleButton para gestionar clicks
        ImGui::SetCursorScreenPos(widgetMin);
        if (ImGui::InvisibleButton(ID.c_str(), Size))
        {
            ImVec2 mousePos = ImGui::GetIO().MousePos;

            bool insideIcon = false;
            if (IconButtonVisible && ButtonClicked != nullptr)
            {
                insideIcon = (mousePos.x >= iconBoxMin.x && mousePos.x < iconBoxMax.x &&
                    mousePos.y >= iconBoxMin.y && mousePos.y < iconBoxMax.y);
            }

            if (insideIcon && Enabled)
            {
                // Clic en el botón ícono
                if (ButtonClicked) ButtonClicked();
            }
            else if (Enabled)
            {
                // Cambiar estado de Checked
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

        // Top line
        drawList->AddLine(ImVec2(topMinX, widgetMin.y), ImVec2(topMidX, widgetMin.y), borderColU32, lineThickness);
        drawList->AddLine(ImVec2(topMidX2, widgetMin.y), ImVec2(topMaxX, widgetMin.y), borderColU32, lineThickness);

        // Bottom line
        drawList->AddLine(ImVec2(botMinX, widgetMax.y), ImVec2(botMidX, widgetMax.y), borderColU32, lineThickness);
        drawList->AddLine(ImVec2(botMidX2, widgetMax.y), ImVec2(botMaxX, widgetMax.y), borderColU32, lineThickness);

        // Left line
        drawList->AddLine(ImVec2(widgetMin.x, leftMinY), ImVec2(widgetMin.x, leftMidY), borderColU32, lineThickness);
        drawList->AddLine(ImVec2(widgetMin.x, leftMidY2), ImVec2(widgetMin.x, leftMaxY), borderColU32, lineThickness);

        // Right line
        drawList->AddLine(ImVec2(rightX, rightMinY), ImVec2(rightX, rightMidY), borderColU32, lineThickness);
        drawList->AddLine(ImVec2(rightX, rightMidY2), ImVec2(rightX, rightMaxY), borderColU32, lineThickness);
    }

    void DrawAnimatedBorder(ImDrawList* drawList, ImVec2 widgetMin, ImVec2 widgetMax, float borderPercent, ImU32 borderColU32, float lineThickness)
    {
        float length = borderPercent;
        float start = BorderOffset;
        float end = start + length;

        int steps = 30; // subdivisiones

        if (end > 1.0f)
        {
            float firstSegment = 1.0f - start;
            float secondSegment = end - 1.0f;

            ImVec2 startPos = GetPointOnPerimeter(widgetMin, widgetMax, start);
            for (int i = 1; i <= steps; i++)
            {
                float t = start + (firstSegment * (float)i / (float)steps);
                ImVec2 pos = GetPointOnPerimeter(widgetMin, widgetMax, t);
                drawList->AddLine(startPos, pos, borderColU32, lineThickness);
                startPos = pos;
            }

            startPos = GetPointOnPerimeter(widgetMin, widgetMax, 0.0f);
            for (int i = 1; i <= steps; i++)
            {
                float t = (secondSegment * (float)i / (float)steps);
                ImVec2 pos = GetPointOnPerimeter(widgetMin, widgetMax, t);
                drawList->AddLine(startPos, pos, borderColU32, lineThickness);
                startPos = pos;
            }
        }
        else
        {
            ImVec2 startPos = GetPointOnPerimeter(widgetMin, widgetMax, start);
            float segmentLength = end - start;
            for (int i = 1; i <= steps; i++)
            {
                float t = start + (segmentLength * (float)i / (float)steps);
                ImVec2 pos = GetPointOnPerimeter(widgetMin, widgetMax, t);
                drawList->AddLine(startPos, pos, borderColU32, lineThickness);
                startPos = pos;
            }
        }
    }

    std::tuple<float, float, float, float> CalcHorizontalLines(float minX, float maxX, float cSize, float p)
    {
        float totalLength = maxX - minX;
        float initialGap = totalLength - 2.0f * cSize;
        float gapActual = initialGap * (1.0f - p);
        float expand = (initialGap - gapActual) * 0.5f;

        float leftLineEnd = minX + cSize + expand;
        float rightLineStart = maxX - cSize - expand;
        return std::make_tuple(minX, leftLineEnd, rightLineStart, maxX);
    }

    std::tuple<float, float, float, float> CalcVerticalLines(float minY, float maxY, float cSize, float p)
    {
        float totalLength = maxY - minY;
        float initialGap = totalLength - 2.0f * cSize;
        float gapActual = initialGap * (1.0f - p);
        float expand = (initialGap - gapActual) * 0.5f;

        float topLineEnd = minY + cSize + expand;
        float bottomLineStart = maxY - cSize - expand;
        return std::make_tuple(minY, topLineEnd, bottomLineStart, maxY);
    }

    ImVec2 GetPointOnPerimeter(ImVec2 min, ImVec2 max, float t)
    {
        float width = max.x - min.x;
        float height = max.y - min.y;
        float perimeter = 2.0f * (width + height);
        float distance = t * perimeter;

        float topLength = width;
        float rightLength = height;
        float bottomLength = width;
        float leftLength = height;

        if (distance <= topLength)
        {
            return ImVec2(min.x + distance, min.y);
        }
        distance -= topLength;

        if (distance <= rightLength)
        {
            return ImVec2(max.x, min.y + distance);
        }
        distance -= rightLength;

        if (distance <= bottomLength)
        {
            return ImVec2(max.x - distance, max.y);
        }
        distance -= bottomLength;

        // Izquierda
        return ImVec2(min.x, max.y - distance);
    }
};

// ------------------------------------------------------------------------------
// Ejemplo de uso (pseudocódigo en tu main):
// 
//  FunctionWidget widget;
//  widget.Title = "Operaciones Avanzadas";
//  widget.Description = "Realiza cálculos intensivos...";
//  widget.CheckedChanged = []() { std::cout << "CheckedChanged!\n"; };
//  widget.ButtonClicked = []() { std::cout << "IconButton Pressed!\n"; };
//
//  // En tu bucle de render:
//  widget.Render();
//
// ---------------------

