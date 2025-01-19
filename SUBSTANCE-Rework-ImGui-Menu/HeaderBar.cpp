#include "pch.h"

HeaderBar::HeaderBar()
{
    // Semilla un tanto arbitraria
    rng.seed(std::random_device{}());
    dist = std::uniform_int_distribution<int>(0, (int)randomChars.size() - 1);
}

void HeaderBar::ResetAnimationTimer()
{
    animationTimer = 0.0;
    currentCharIndex = 0;
}

std::string HeaderBar::GetAnimatedText(double deltaTime)
{
    // Si no hemos revelado todo el texto
    if (currentCharIndex < (int)LeftLabelText.size())
    {
        animationTimer += deltaTime;
        if (animationTimer >= TextRevealDelay)
        {
            animationTimer -= TextRevealDelay;
            currentCharIndex++;
        }
        // Generar texto parcialmente revelado
        std::string revealedText = LeftLabelText.substr(0, currentCharIndex);
        std::string randomPart = GenerateRandomPart((int)LeftLabelText.size() - currentCharIndex);
        return revealedText + randomPart;
    }
    else
    {
        // Texto completo
        return LeftLabelText;
    }
}

std::string HeaderBar::GenerateRandomPart(int length)
{
    std::string result;
    result.reserve(length);
    for (int i = 0; i < length; i++)
    {
        int idx = dist(rng);
        result.push_back(randomChars[idx]);
    }
    return result;
}

void HeaderBar::Render(double deltaTime)
{
    // Crear un child region con las dimensiones de Size
    ImGui::BeginChild(
        ("Section" + ID).c_str(),
        Size,
        false, // no border
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
    );

    // Push de fuente (ejemplo)
    ImGui::PushFont(SUBSTANCE::pFont12Light);

    // Cursor de inicio
    ImVec2 widgetMin = ImGui::GetCursorScreenPos();
    ImVec2 widgetMax = ImVec2(widgetMin.x + Size.x, widgetMin.y + Size.y);

    // Obtener drawList
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Dibujar fondo
    drawList->AddRectFilled(
        widgetMin, widgetMax,
        ImGui::ColorConvertFloat4ToU32(BackgroundColor)
    );

    // Calcular posiciones
    float centerY = widgetMin.y + (Size.y * 0.5f) - (ImGui::GetTextLineHeight() * 0.5f);

    // Texto animado
    std::string animatedText = GetAnimatedText(deltaTime);

    // Label izquierdo
    ImVec2 leftLabelPos = ImVec2(widgetMin.x + LeftLabelIndent, centerY);
    ImGui::SetCursorScreenPos(leftLabelPos);
    ImGui::PushStyleColor(ImGuiCol_Text, LeftLabelColor);
    ImGui::TextUnformatted(animatedText.c_str());
    ImGui::PopStyleColor();

    // Label derecho
    float rightLabelWidth = ImGui::CalcTextSize(RightLabelText.c_str()).x;
    float rightLabelX = widgetMax.x - RightLabelIndent - rightLabelWidth;
    ImVec2 rightLabelPos = ImVec2(rightLabelX, centerY);
    ImGui::SetCursorScreenPos(rightLabelPos);
    ImGui::PushStyleColor(ImGuiCol_Text, RightLabelColor);
    ImGui::TextUnformatted(RightLabelText.c_str());
    ImGui::PopStyleColor();

    ImGui::PopFont();

    // Fin del child
    ImGui::EndChild();
}