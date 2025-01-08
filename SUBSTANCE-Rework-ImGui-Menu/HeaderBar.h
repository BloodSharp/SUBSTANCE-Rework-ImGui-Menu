#pragma once
// ---------------------------------------------------------------------------------
// Encapsulamos nuestra clase HeaderBar en un namespace si lo deseas
// (Ajústalo según tu estructura).
//
class HeaderBar
{
public:
    // Identificador
    std::string ID = "header.default";

    // Propiedades principales
    ImVec2 Size = ImVec2(10.0f, 50.0f);
    ImVec4 BackgroundColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

    // Propiedades del label izquierdo
    std::string LeftLabelText = "Izquierda";
    ImVec4 LeftLabelColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    float LeftLabelIndent = 15.0f;

    // Propiedades del label derecho
    std::string RightLabelText = "Derecha";
    ImVec4 RightLabelColor = ImVec4(0.580f, 0.580f, 0.596f, 1.000f);
    float RightLabelIndent = 15.0f;

    // Efecto de texto progresivo
    float TextRevealDelay = 0.2f; // Retraso en segundos por carácter
    double animationTimer = 0.0;
    int currentCharIndex = 0;

    // Generador de caracteres aleatorios (C++11).
    // En C++ real, puedes usar std::mt19937 con una semilla, etc.
    std::vector<char> randomChars = {
        '!','@','#','$','%','^','&','*','(',
        ')','-','_','=','+','[',']','{','}',
        '|',';',':','\'','\"',',','.',
        '<','>','?','/','\\','~','`'
    };

    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

    // Constructor: inicializar random
    HeaderBar()
    {
        // Semilla un tanto arbitraria
        rng.seed(std::random_device{}());
        dist = std::uniform_int_distribution<int>(0, (int)randomChars.size() - 1);
    }

    // Restablecer la animación
    void ResetAnimationTimer()
    {
        animationTimer = 0.0;
        currentCharIndex = 0;
    }

    // Método que actualiza y devuelve el texto animado
    std::string GetAnimatedText(double deltaTime)
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

    // Generar caracteres aleatorios para la parte oculta
    std::string GenerateRandomPart(int length)
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

    // Método para renderizar la barra
    void Render(double deltaTime)
    {
        // Crear un child region con las dimensiones de Size
        ImGui::BeginChild(
            ("Section" + ID).c_str(),
            Size,
            false, // no border
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse
        );

        //// Push de fuente (ejemplo)
        //ImFont* widgetFont = Core::Instances::fontManager.GetFont("widget_header");
        //ImGui::PushFont(widgetFont);

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

      /*   Pop font
        ImGui::PopFont();*/

        // Fin del child
        ImGui::EndChild();
    }
};

//
// Ejemplo de uso en tu main code (pseudocódigo):
//
//  HeaderBar myHeader;
//  myHeader.Size = ImVec2(300, 50);
//  myHeader.LeftLabelText = "Left Animated";
//  myHeader.RightLabelText = "Right Static";
//
//  // Cada frame, llamas a:
//  double dt = ...; // deltaTime
//  myHeader.Render(dt);
//
