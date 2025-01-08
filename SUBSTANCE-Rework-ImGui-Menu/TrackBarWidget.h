#pragma once
class TrackBarWidget
{
public:
    std::string  ID = "trackbar.default";
    std::string  Title = "TrackBar Widget";
    std::string  Description = "Este widget tiene un trackbar interactivo.";
    ImVec2       Size = ImVec2(300.0f, 150.0f);

    ImVec4 BackgroundColor = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
    ImVec4 TitleColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 DescriptionColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);

    // Propiedades TrackBar
    float Minimum = 0.0f;
    float Maximum = 100.0f;
    float Value = 50.0f;

    ImVec4 TrackBarBackgroundColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    ImVec4 TrackBarFillColor = ImVec4(0.3f, 0.7f, 0.3f, 1.0f);
    ImVec4 TrackBarHoveredColor = ImVec4(0.4f, 0.8f, 0.4f, 1.0f);
    ImVec4 TrackBarThumbColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Evento en C++: std::function<void()>
    std::function<void()> ValueChanged;

protected:
    void OnValueChanged()
    {
        if (ValueChanged) ValueChanged();
    }

public:
    void Render()
    {
        // Asegurar un alto mínimo para acomodar 3 líneas de texto, etc.
        float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
        Size.y = std::max<float>(Size.y, textLineHeight * 3.0f);

        ImGui::BeginChild(("Section" + ID).c_str(), Size, false, ImGuiWindowFlags_NoScrollbar);

        ImVec4 bgColor = BackgroundColor;
        ImVec4 titleColor = TitleColor;
        ImVec4 descColor = DescriptionColor;

        ImVec2 widgetMin = ImGui::GetCursorScreenPos();
        ImVec2 widgetMax = ImVec2(widgetMin.x + Size.x, widgetMin.y + Size.y);
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // Fondo
        drawList->AddRectFilled(widgetMin, widgetMax, ImGui::ColorConvertFloat4ToU32(bgColor));

        ImVec2 localMin = ImVec2(widgetMin.x - ImGui::GetWindowPos().x, widgetMin.y - ImGui::GetWindowPos().y);
        ImVec2 localMax = ImVec2(widgetMax.x - ImGui::GetWindowPos().x, widgetMax.y - ImGui::GetWindowPos().y);

        float wrapPos = localMax.x - 15.0f;
        ImGui::PushTextWrapPos(wrapPos);

        // Título
        ImGui::SetCursorPos(ImVec2(localMin.x + 15.0f, localMin.y + 10.0f));
        {
          /*  ImFont* fontTitle = Core::Instances::fontManager.GetFont("widget_title");
            ImGui::PushFont(fontTitle);*/
            ImGui::TextColored(titleColor, "%s", Title.c_str());
            //ImGui::PopFont();
        }

        // Descripción
        ImVec2 titleSize = ImGui::CalcTextSize(Title.c_str());
        float lineHeight = ImGui::GetTextLineHeight();
        float descTopOff = lineHeight + 5.0f;

        ImGui::SetCursorPos(ImVec2(localMin.x + 15.0f, localMin.y + 10.0f + descTopOff));
        {
           /* ImFont* fontDes = Core::Instances::fontManager.GetFont("widget_des");
            ImGui::PushFont(fontDes);*/
            ImGui::TextColored(descColor, "%s", Description.c_str());
            //ImGui::PopFont();
        }

        ImGui::PopTextWrapPos();

        // TrackBar 
        float trackBarY = localMax.y - 25.0f;
        ImGui::SetCursorPos(ImVec2(localMin.x + 15.0f, trackBarY));
        ImGui::PushItemWidth(Size.x - 30.0f);

        // Aplica colores al slider
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertFloat4ToU32(TrackBarBackgroundColor));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImGui::ColorConvertFloat4ToU32(TrackBarThumbColor));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImGui::ColorConvertFloat4ToU32(TrackBarHoveredColor));
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImGui::ColorConvertFloat4ToU32(TrackBarFillColor));

        float oldValue = Value;
        // En Dear ImGui, ImGui::SliderFloat(label, &value, min, max):
        if (ImGui::SliderFloat(("##TrackBar" + ID).c_str(), &Value, Minimum, Maximum))
        {
            if (std::fabs(Value - oldValue) > std::numeric_limits<float>::epsilon())
            {
                OnValueChanged();
            }
        }

        ImGui::PopStyleColor(4);
        ImGui::PopItemWidth();

        ImGui::EndChild();
    }
};

//
// Ejemplo de uso (pseudocódigo):
//
// TrackBarWidget trackbar;
// trackbar.Title = "Volumen";
// trackbar.Description = "Controla el nivel de audio";
// trackbar.ValueChanged = [](){
//     std::cout << "Valor cambiado!\n";
// };
//
// // En tu bucle principal de render:
// trackbar.Render();
//

