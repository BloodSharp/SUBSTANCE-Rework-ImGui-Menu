#include "pch.h"

bool Interface::Menu::Enabled = true;
ImVec2 Gui_Size = ImVec2(1070, 550);
ImFont* pTitleFont = 0;
ImFont* pTitleFont2 = 0;
ImFont* pNavigationFont = 0;

/// <summary>
/// Widget personalizado ToggleButton con imagen, texto y estilo activo/inactivo.
/// </summary>
/// <param name="id">Identificador único del botón.</param>
/// <param name="imageTextureID">La textura de la imagen del ícono.</param>
/// <param name="label">El texto del botón.</param>
/// <param name="size">Tamaño del botón.</param>
/// <param name="isActive">Referencia al estado del botón (ON/OFF).</param>
bool ToggleButton(const char* id, ImTextureID imageTextureID, const char* label, ImVec2 size, bool *isActive)
{
    bool clicked = false;

    // Colores de los estilos
    ImVec4 inactiveBgColor = ImVec4(0.153f, 0.153f, 0.200f, 1.000f); // Gris oscuro
    ImVec4 activeBgColor = ImVec4(0.439f, 0.698f, 0.675f, 1.000f);   // Celeste
    ImVec4 inactiveTextColor = ImVec4(1, 1, 1, 1.0f);        // Blanco
    ImVec4 activeTextColor = ImVec4(1, 1, 1, 1.0f);          // Negro
    ImVec4 decorationColor = ImVec4(0.153f, 0.153f, 0.200f, 1.000f); // Color oscuro para decoración
    ImVec4 decorationColor2 = ImVec4(0.153f, 0.153f, 0.200f, 0.800f);

    // Capturar clics invisibles
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f); // Sin bordes redondeados
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    if (ImGui::InvisibleButton(id, size))
    {
        *isActive = true;
        clicked = true;
    }
    ImGui::PopStyleVar(2);

    // Coordenadas del botón
    ImVec2 buttonMin = ImGui::GetItemRectMin();
    ImVec2 buttonMax = ImGui::GetItemRectMax();

    if (*isActive)
    {
        buttonMax.x += 5.0f;
    }
    else
    {
        buttonMax.x -= 5.0f;
    }

    ImVec2 buttonCenter = ImVec2((buttonMin.x + buttonMax.x) / 2, (buttonMin.y + buttonMax.y) / 2);

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Fondo del botón
    drawList->AddRectFilled(buttonMin, buttonMax, ImGui::ColorConvertFloat4ToU32(*isActive ? activeBgColor : inactiveBgColor));

    // Decoración si el botón está activo
    if (*isActive)
    {
        // 1. Rectángulo como barra decorativa
        float rectHeight = (buttonMax.y - buttonMin.y) * 0.6f; // Altura del rectángulo
        float rectWidth = 5.0f; // Ancho pequeño para parecer una barra
        ImVec2 rectBottomRight = ImVec2(buttonMax.x, buttonMax.y);
        ImVec2 rectTopLeft = ImVec2(buttonMax.x - rectWidth, buttonMax.y - rectHeight);

        // Dibujar la barra
        drawList->AddRectFilled(rectTopLeft, rectBottomRight, ImGui::ColorConvertFloat4ToU32(decorationColor));

        // 2. Triángulo ligeramente más ancho que la barra
        ImVec2 trianglePoint1 = ImVec2(rectTopLeft.x + 1, rectTopLeft.y);                   // Esquina superior izquierda del rectángulo
        ImVec2 trianglePoint2 = ImVec2(rectTopLeft.x + 1 + rectWidth, rectTopLeft.y); // Extremo derecho del triángulo
        ImVec2 trianglePoint3 = ImVec2(rectTopLeft.x + 1, rectTopLeft.y - 5);             // Punto inclinado hacia arriba

        drawList->AddTriangleFilled(trianglePoint1, trianglePoint2, trianglePoint3, ImGui::ColorConvertFloat4ToU32(decorationColor));

        // --- Decoración Inferior Derecha ---

        float bottomRectWidth = 28.0f; // Ancho de la barra horizontal
        float bottomRectHeight = 5.0f; // Altura de la barra
        ImVec2 bottomRectTopLeft = ImVec2(buttonMax.x - bottomRectWidth, buttonMax.y - bottomRectHeight);
        ImVec2 bottomRectBottomRight = ImVec2(buttonMax.x, buttonMax.y);

        // Dibujar la barra inferior
        drawList->AddRectFilled(bottomRectTopLeft, bottomRectBottomRight, ImGui::ColorConvertFloat4ToU32(decorationColor));

        // Triángulo inferior derecho
        ImVec2 bottomTrianglePoint1 = ImVec2(bottomRectTopLeft.x, bottomRectTopLeft.y + 1);         // Esquina superior izquierda de la barra
        ImVec2 bottomTrianglePoint2 = ImVec2(bottomRectTopLeft.x - 5, bottomRectTopLeft.y + 1);  // Unos píxeles a la izquierda, misma altura
        ImVec2 bottomTrianglePoint3 = ImVec2(bottomRectTopLeft.x, bottomRectBottomRight.y + 1);     // Esquina inferior izquierda de la barra

        drawList->AddTriangleFilled(bottomTrianglePoint1, bottomTrianglePoint2, bottomTrianglePoint3, ImGui::ColorConvertFloat4ToU32(decorationColor));

        bottomRectWidth = 8.0f;
        bottomRectTopLeft = ImVec2(buttonMax.x - bottomRectWidth, buttonMax.y - bottomRectHeight);

        int separation = 40;

        ImVec2 bottomRectTopLeftb = ImVec2((buttonMax.x - bottomRectWidth) - separation, buttonMax.y - bottomRectHeight);
        ImVec2 bottomRectBottomRightb = ImVec2(buttonMax.x - separation, buttonMax.y);

        // Dibujar la barra inferior
        drawList->AddRectFilled(bottomRectTopLeftb, bottomRectBottomRightb, ImGui::ColorConvertFloat4ToU32(decorationColor));

        ImVec2 bottomTrianglePoint1b = ImVec2(bottomRectTopLeft.x - separation, bottomRectTopLeft.y + 1);         // Esquina superior izquierda de la barra
        ImVec2 bottomTrianglePoint2b = ImVec2((bottomRectTopLeft.x - 5) - separation, bottomRectTopLeft.y + 1);  // Unos píxeles a la izquierda, misma altura
        ImVec2 bottomTrianglePoint3b = ImVec2(bottomRectTopLeft.x - separation, bottomRectBottomRight.y + 1);     // Esquina inferior izquierda de la barra

        drawList->AddTriangleFilled(bottomTrianglePoint1b, bottomTrianglePoint2b, bottomTrianglePoint3b, ImGui::ColorConvertFloat4ToU32(decorationColor));

        // Triángulo en el lado derecho con la hipotenusa apuntando hacia arriba
        ImVec2 bottomTriangleRightPoint1b = ImVec2(bottomRectBottomRightb.x, bottomRectBottomRightb.y + 1);       // Punto inferior derecho del rectángulo
        ImVec2 bottomTriangleRightPoint2b = ImVec2(bottomRectBottomRightb.x + 5, bottomRectBottomRightb.y + 1);   // Extiende 5px hacia la derecha, misma altura inferior
        ImVec2 bottomTriangleRightPoint3b = ImVec2(bottomRectBottomRightb.x, bottomRectTopLeftb.y + 1);           // Punto superior, creando la punta hacia arriba

        drawList->AddTriangleFilled(bottomTriangleRightPoint1b, bottomTriangleRightPoint2b, bottomTriangleRightPoint3b, ImGui::ColorConvertFloat4ToU32(decorationColor));


        bottomRectWidth = 8.0f;
        bottomRectTopLeft = ImVec2(buttonMax.x - bottomRectWidth, buttonMax.y - bottomRectHeight);

        separation = 60;

        bottomRectTopLeftb = ImVec2((buttonMax.x - bottomRectWidth) - separation, buttonMax.y - bottomRectHeight);
        bottomRectBottomRightb = ImVec2(buttonMax.x - separation, buttonMax.y);

        // Dibujar la barra inferior
        drawList->AddRectFilled(bottomRectTopLeftb, bottomRectBottomRightb, ImGui::ColorConvertFloat4ToU32(decorationColor));

        bottomTrianglePoint1b = ImVec2(bottomRectTopLeft.x - separation, bottomRectTopLeft.y + 1);         // Esquina superior izquierda de la barra
        bottomTrianglePoint2b = ImVec2((bottomRectTopLeft.x - 5) - separation, bottomRectTopLeft.y + 1);  // Unos píxeles a la izquierda, misma altura
        bottomTrianglePoint3b = ImVec2(bottomRectTopLeft.x - separation, bottomRectBottomRight.y + 1);     // Esquina inferior izquierda de la barra

        drawList->AddTriangleFilled(bottomTrianglePoint1b, bottomTrianglePoint2b, bottomTrianglePoint3b, ImGui::ColorConvertFloat4ToU32(decorationColor));

        // Triángulo en el lado derecho con la hipotenusa apuntando hacia arriba
        bottomTriangleRightPoint1b = ImVec2(bottomRectBottomRightb.x, bottomRectBottomRightb.y + 1);       // Punto inferior derecho del rectángulo
        bottomTriangleRightPoint2b = ImVec2(bottomRectBottomRightb.x + 5, bottomRectBottomRightb.y + 1);   // Extiende 5px hacia la derecha, misma altura inferior
        bottomTriangleRightPoint3b = ImVec2(bottomRectBottomRightb.x, bottomRectTopLeftb.y + 1);           // Punto superior, creando la punta hacia arriba

        drawList->AddTriangleFilled(bottomTriangleRightPoint1b, bottomTriangleRightPoint2b, bottomTriangleRightPoint3b, ImGui::ColorConvertFloat4ToU32(decorationColor));

        bottomRectWidth = 8.0f;
        bottomRectTopLeft = ImVec2(buttonMax.x - bottomRectWidth, buttonMax.y - bottomRectHeight);

        separation = 80;

        bottomRectTopLeftb = ImVec2((buttonMax.x - bottomRectWidth) - separation, buttonMax.y - bottomRectHeight);
        bottomRectBottomRightb = ImVec2(buttonMax.x - separation, buttonMax.y);

        // Dibujar la barra inferior
        drawList->AddRectFilled(bottomRectTopLeftb, bottomRectBottomRightb, ImGui::ColorConvertFloat4ToU32(decorationColor));

        bottomTrianglePoint1b = ImVec2(bottomRectTopLeft.x - separation, bottomRectTopLeft.y + 1);         // Esquina superior izquierda de la barra
        bottomTrianglePoint2b = ImVec2((bottomRectTopLeft.x - 5) - separation, bottomRectTopLeft.y + 1);  // Unos píxeles a la izquierda, misma altura
        bottomTrianglePoint3b = ImVec2(bottomRectTopLeft.x - separation, bottomRectBottomRight.y + 1);     // Esquina inferior izquierda de la barra

        drawList->AddTriangleFilled(bottomTrianglePoint1b, bottomTrianglePoint2b, bottomTrianglePoint3b, ImGui::ColorConvertFloat4ToU32(decorationColor));

        // Triángulo en el lado derecho con la hipotenusa apuntando hacia arriba
        bottomTriangleRightPoint1b = ImVec2(bottomRectBottomRightb.x, bottomRectBottomRightb.y + 1);       // Punto inferior derecho del rectángulo
        bottomTriangleRightPoint2b = ImVec2(bottomRectBottomRightb.x + 5, bottomRectBottomRightb.y + 1);   // Extiende 5px hacia la derecha, misma altura inferior
        bottomTriangleRightPoint3b = ImVec2(bottomRectBottomRightb.x, bottomRectTopLeftb.y + 1);           // Punto superior, creando la punta hacia arriba

        drawList->AddTriangleFilled(bottomTriangleRightPoint1b, bottomTriangleRightPoint2b, bottomTriangleRightPoint3b, ImGui::ColorConvertFloat4ToU32(decorationColor));



        float bottomRectWidthc = 89.0f; // Ancho de la barra horizontal
        float bottomRectHeightc = 5.0f; // Altura de la barra
        ImVec2 bottomRectTopLeftc = ImVec2((buttonMax.x - bottomRectWidthc) - 7, (buttonMax.y - bottomRectHeightc) - 7);
        ImVec2 bottomRectBottomRightc = ImVec2(buttonMax.x - 7, buttonMax.y - 7);

        // Dibujar la barra inferior
        drawList->AddRectFilled(bottomRectTopLeftc, bottomRectBottomRightc, ImGui::ColorConvertFloat4ToU32(decorationColor2));

        // Triángulo inferior derecho
        ImVec2 bottomTrianglePoint1c = ImVec2(bottomRectTopLeftc.x - 5, bottomRectTopLeftc.y);         // Esquina superior izquierda de la barra
        ImVec2 bottomTrianglePoint2c = ImVec2(bottomRectTopLeftc.x, bottomRectTopLeftc.y);  // Unos píxeles a la izquierda, misma altura
        ImVec2 bottomTrianglePoint3c = ImVec2(bottomRectTopLeftc.x, bottomRectBottomRightc.y);     // Esquina inferior izquierda de la barra

        drawList->AddTriangleFilled(bottomTrianglePoint1c, bottomTrianglePoint2c, bottomTrianglePoint3c, ImGui::ColorConvertFloat4ToU32(decorationColor2));

    }

    // Imagen del botón (ícono)
    float imageHeight = size.y * 0.4f;
    ImVec2 imageSize = ImVec2(imageHeight, imageHeight);
    ImVec2 imagePos = ImVec2(buttonMin.x + 10, buttonCenter.y - (imageSize.y / 2));
    drawList->AddImage(imageTextureID, imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));

    // Texto centrado verticalmente, a la derecha del ícono
    ImVec2 textSize = ImGui::CalcTextSize(label);
    ImVec2 textPos = ImVec2(imagePos.x + imageSize.x + 10, buttonCenter.y - (textSize.y / 2));
    drawList->AddText(textPos, ImGui::ColorConvertFloat4ToU32(*isActive ? activeTextColor : inactiveTextColor), label);

    return clicked;
}

void Interface::Menu::Initialize()
{
}

void SetActiveView(const char* ID)
{
    for (IView* view : Views)
    {
        if (view->getID() != ID)
            view->m_bChecked = false;
    }
}

void NavigationSection(float width)
{
    ImVec4 navigationBgColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    float marginX = 20.0f; // Margen horizontal
    float marginTop = 10.0f; // Margen superior


    ImGui::PushStyleColor(ImGuiCol_ChildBg, navigationBgColor);
    if (ImGui::BeginChild("NavigationSection", ImVec2(width, 0)))
    {
        ImGui::PushFont(pNavigationFont);
        // Configurar margen superior e izquierdo
        ImGui::SetCursorPos(ImVec2(marginX, marginTop));

        // Tamaño de los botones
        float buttonWidth = width - 2 * marginX;
        ImVec2 buttonSize = ImVec2(buttonWidth, 45);

        // foreach (var view in Views)
        for (IView* view : Views)
        {
            bool isChecked = view->m_bChecked;
            ImGui::SetCursorPosX(marginX);
            if (ToggleButton(view->getID(), view->getIcon(), view->getText(), buttonSize, &isChecked))
            {
                view->m_bChecked = isChecked;
                SetActiveView(view->getID());
            }
        }

        ImGui::PopFont();
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
}

void MainSection()
{
    float titleBarHeight = 50.0f;
    ImVec2 windowSize = ImVec2(Gui_Size.x, Gui_Size.y);

    // --- Sección Principal ---
    ImGui::SetCursorPos(ImVec2(0, titleBarHeight));
    if (ImGui::BeginChild("MainSection", ImVec2(windowSize.x, windowSize.y - titleBarHeight), ImGuiWindowFlags_NoScrollbar))
    {

        float navigationWidth = 200.0f;
        float viewsWidth = (windowSize.x - navigationWidth);

        // --- Sección Izquierda: Navegación ---
        NavigationSection(navigationWidth);

        ImGui::SameLine();

        // --- Sección Derecha: Vista Activa ---
        if (ImGui::BeginChild("ViewSection", ImVec2(viewsWidth - 20, 0)))
        {
            /*
            var firstCheckedView = views.FirstOrDefault(v = > v.Checked);
            if (firstCheckedView != null)
            {
                firstCheckedView.Render();
            }
            else
            {
                ImGui::Text("Vista no encontrada.");
            }
            */
            
            for (IView* view : Views)
            { 
                if (view->m_bChecked) {
					view->Render();
					break;
                }
            }
        }
        ImGui::EndChild();
    }
    ImGui::EndChild();
}

void TitleBar()
{
    ImVec4 titleBarColor = ImVec4(0.043f, 0.047f, 0.059f, 1.0f); // #0B0C0F
    float titleBarHeight = 50.0f;
    ImVec2 windowSize = Gui_Size;

    // --- Barra del Título ---
    ImGui::SetCursorPos(ImVec2(0, 0)); // Coloca la posición inicial
    ImGui::PushStyleColor(ImGuiCol_ChildBg, titleBarColor); // Color de la barra
    if (ImGui::BeginChild("TitleBar", ImVec2(windowSize.x, titleBarHeight), ImGuiWindowFlags_NoScrollbar))
    {
        // Centramos el texto verticalmente
        float textHeight = ImGui::CalcTextSize("SUBSTANCE 2.0").y;
        float verticalOffset = (titleBarHeight - textHeight) * 0.5f;

        // Texto Izquierdo: SUBSTANCE 2.0 con sangría
        ImGui::SetCursorPos(ImVec2(15, verticalOffset)); // Sangría de 15 píxeles
        //ImGui::PushFont(ImGui::GetFont()); 
        ImGui::PushFont(pTitleFont);
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "SUBSTANCE 2.0"); // Blanco
        ImGui::PopFont();
        // Texto Derecho: Past Owl [DEV]
        ImGui::PushFont(pTitleFont2);
        ImGui::SameLine(windowSize.x - 150); // Posición derecha
        ImGui::SetCursorPosY(verticalOffset);
        ImGui::TextColored(ImVec4(1, 1, 1, 1), "Past Owl");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "[DEV]"); // Rojo salmón
        ImGui::PopFont();

    }
    ImGui::EndChild();
    ImGui::PopStyleColor();

    /*
    if (Core.Instances.OverlayMode == OverlayMode.Normal)
    {
        // --- Verificar si el cursor está en el área de la barra de título ---
        ImVec2 mousePos = ImGui::GetMousePos(); // Posición absoluta del mouse
        ImVec2 titleBarMin = ImVec2(0, 0); // Esquina superior izquierda de la barra
        ImVec2 titleBarMax = ImVec2(windowSize.x, titleBarHeight); // Esquina inferior derecha de la barra

        // Calcular si el mouse está dentro del área de la barra
        bool isMouseOverTitleBar =
            mousePos.x >= titleBarMin.x && mousePos.x <= titleBarMax.x &&
            mousePos.y >= titleBarMin.y && mousePos.y <= titleBarMax.y;

        // Actualizar EnableDrag solo si el cursor está sobre la barra de título
        bool IsFocusOnMainImguiWindow = (Form.ActiveForm == Core.Instances.OverlayWindow);
        Core.Instances.OverlayWindow.EnableDrag = isMouseOverTitleBar && IsFocusOnMainImguiWindow;
    }
    */
}

void Interface::Menu::Render()
{
    if (Interface::Menu::Enabled)
    {
        // Colores definidos
        ImVec4 backgroundColor = ImVec4(0.071f, 0.071f, 0.090f, 1.000f); // #454545

        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(Gui_Size, ImGuiCond_Once);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, backgroundColor); // Color del fondo
        ImGui::Begin("Background", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

        // Llamamos a las secciones individuales
        TitleBar();
        MainSection();

        ImGui::End(); // Fin del Background
        ImGui::PopStyleColor();
    }
}
