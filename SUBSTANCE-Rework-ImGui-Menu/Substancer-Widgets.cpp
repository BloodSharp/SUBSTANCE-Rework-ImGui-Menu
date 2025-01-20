#include "pch.h"

ImFont* SUBSTANCE::pIconFont = 0;
ImFont* SUBSTANCE::pFont15 = 0;
ImFont* SUBSTANCE::pFont12Light = 0;
ImFont* SUBSTANCE::pFont10Light = 0;

HeaderBar SUBSTANCE::HeaderBar;

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

void DrawStaticBorder(ImDrawList* drawList, ImVec2 widgetMin, ImVec2 widgetMax, float borderPercent, ImU32 borderColU32, float lineThickness)
{
	static float CornerSize = 15.0f;

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
	float botMinY = widgetMax.y - 1.0f;
	float topMinY = widgetMin.y;

	// Top
	drawList->AddLine(ImVec2(topMinX, topMinY), ImVec2(topMidX, topMinY), borderColU32, lineThickness);
	drawList->AddLine(ImVec2(topMidX2, topMinY), ImVec2(topMaxX, topMinY), borderColU32, lineThickness);

	// Bottom
	drawList->AddLine(ImVec2(botMinX, botMinY), ImVec2(botMidX, botMinY), borderColU32, lineThickness);
	drawList->AddLine(ImVec2(botMidX2, botMinY), ImVec2(botMaxX, botMinY), borderColU32, lineThickness);

	// Left
	drawList->AddLine(ImVec2(widgetMin.x, leftMinY), ImVec2(widgetMin.x, leftMidY), borderColU32, lineThickness);
	drawList->AddLine(ImVec2(widgetMin.x, leftMidY2), ImVec2(widgetMin.x, leftMaxY), borderColU32, lineThickness);

	// Right
	drawList->AddLine(ImVec2(rightX, rightMinY), ImVec2(rightX, rightMidY), borderColU32, lineThickness);
	drawList->AddLine(ImVec2(rightX, rightMidY2), ImVec2(rightX, rightMaxY), borderColU32, lineThickness);
}

void SUBSTANCE::LeftSection(void(*pItemRenderFunction)())
{
	const float leftSectionWidth = 220.0f;
	const float topMargin = 10.0f;
	const float bottomMargin = 10.0f;

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + topMargin);
	ImGui::BeginChild("LeftSection", ImVec2(leftSectionWidth, 0));

	pItemRenderFunction();

	ImGui::EndChild();
}

void SUBSTANCE::RightSection(void(*pItemRenderFunction)())
{
	//ImVec2 windowSize = ImGui::GetIO().DisplaySize;
	//ImVec2 windowSize = ImGui::GetWindowSize();
	float leftSectionWidth = 220.0f;

	ImGui::SameLine();
	ImGui::BeginChild("RightSection", ImVec2(ImGui::GetWindowSize().x - leftSectionWidth, 0));

	float marginX = 15.0f;
	float marginY = 10.0f;

	ImVec2 childStartPos = ImGui::GetCursorPos();
	ImGui::SetCursorPos(ImVec2(0, childStartPos.y + marginY));

	float adjustedWidth = (ImGui::GetWindowSize().x - leftSectionWidth) - (2 * marginX);

	// Actualizar tamaño del header
	SUBSTANCE::HeaderBar.Size = ImVec2(604, 40);
	//SUBSTANCE::HeaderBar.Size = ImVec2(adjustedWidth, SUBSTANCE::HeaderBar.Size.y);

	// Render header
	double dt = ImGui::GetIO().DeltaTime;
	SUBSTANCE::HeaderBar.Render(dt);

	ImGui::BeginGroup();
	ImGui::Dummy(ImVec2(0, 1));
	ImGui::BeginChild("RightSectionItems", ImVec2(ImGui::GetWindowSize().x - /*marginX*/ 4.0f, 0), ImGuiChildFlags_None);
	pItemRenderFunction();
	ImGui::EndChild();
	ImGui::EndGroup();

	ImGui::EndChild();
}

void UniqueSection(void(*pItemRenderFunction)())
{
	pItemRenderFunction();
}

bool SUBSTANCE::ToggleButton(const char* szTextAndAlsoID, const char* szIcon, ImVec2 size, bool isActive)
{
	bool bHovered = false;
	bool bHeld = false;
	bool bClicked = false;

	// Colores de los estilos
	ImVec4 inactiveBgColor = ImVec4(0.153f, 0.153f, 0.200f, 1.000f); // Gris oscuro
	ImVec4 activeBgColor = ImVec4(0.439f, 0.698f, 0.675f, 1.000f);   // Celeste
	ImVec4 inactiveTextColor = ImVec4(1, 1, 1, 1.0f);        // Blanco
	ImVec4 activeTextColor = ImVec4(1, 1, 1, 1.0f);          // Negro
	ImVec4 decorationColor = ImVec4(0.153f, 0.153f, 0.200f, 1.000f); // Color oscuro para decoración
	ImVec4 decorationColor2 = ImVec4(0.153f, 0.153f, 0.200f, 0.800f);

	const ImVec2 titleSize = ImGui::CalcTextSize(szTextAndAlsoID, 0, true);
	const ImVec2 widgetMin = ImGui::GetCursorScreenPos();
	const ImVec2 widgetMax = widgetMin + size;
	const ImGuiID id = ImGui::GetID(szTextAndAlsoID);
	const ImGuiStyle& style = ImGui::GetStyle();

	// Capturar clics invisibles
	const ImRect bb(widgetMin, widgetMax);
	ImGui::ItemSize(size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;
	bClicked = ImGui::ButtonBehavior(bb, id, &bHovered, &bHeld);
	/*
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f); // Sin bordes redondeados
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
	if (ImGui::InvisibleButton(id, size))
	{
		*isActive = true;
		clicked = true;
	}
	ImGui::PopStyleVar(2);
	*/

	// Coordenadas del botón
	ImVec2 buttonMin = ImGui::GetItemRectMin();
	ImVec2 buttonMax = ImGui::GetItemRectMax();

	if (isActive)
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
	drawList->AddRectFilled(buttonMin, buttonMax, ImGui::ColorConvertFloat4ToU32(isActive ? activeBgColor : inactiveBgColor));

	// Decoración si el botón está activo
	if (isActive)
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
	//drawList->AddImage(imageTextureID, imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));
	//drawList->AddRectFilled(imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y), ImGui::ColorConvertFloat4ToU32(isActive ? activeTextColor : inactiveTextColor));
	ImGui::PushFont(SUBSTANCE::pIconFont);
	drawList->AddText(imagePos, ImGui::ColorConvertFloat4ToU32(isActive ? activeTextColor : inactiveTextColor), szIcon);
	ImGui::PopFont();

	// Texto centrado verticalmente, a la derecha del ícono
	ImVec2 textSize = ImGui::CalcTextSize(szTextAndAlsoID, 0, true);
	ImVec2 textPos = ImVec2(imagePos.x + imageSize.x + 10, buttonCenter.y - (textSize.y / 2));
	ImVec2 textPosMax = textPos + textSize;
	ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32(isActive ? activeTextColor : inactiveTextColor));
	ImGui::RenderTextClipped(textPos, textPosMax, szTextAndAlsoID, 0, &textSize);
	ImGui::PopStyleColor();

	return bClicked;
}

bool SUBSTANCE::ComboBox(const char* szTextAndAlsoID, const char* szDescription, int* pCurrentIntegerOption, const char* szOptions)
{
	bool bChangedValue = false;

	static const ImVec2 Size = ImVec2(190.0f, 100.0f);
	static const ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	static const ImVec4 whiteColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	static const ImVec4 greenColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	static const ImVec4 redColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	static const ImVec4 almostWhiteColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	static const ImVec4 grayLight = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	static const float IconButtonRounding = 0.0f;
	static const float IconButtonSize = 15.0f;
	static const float LineThickness = 1.0f;
	static const float BorderPercent = 0.5f;
	static const float MarginX = 15.0f;
	static const float MarginY = 10.0f;
	static const float ComboBoxMargin = 10.0f;

	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float lineHeight = ImGui::GetTextLineHeight();
	const float descriptionTopOffset = lineHeight + (MarginY * 0.5f);

	static const ImVec4 titleColor = whiteColor;
	static const ImVec4 descColor = almostWhiteColor;
	// Estilos del ComboBox
	static const ImVec4 ComboBoxTextColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	static const ImVec4 ComboBoxBackgroundColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	static const ImVec4 ComboBoxHoveredColor = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
	static const ImVec4 ComboBoxActiveColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	static const ImVec4 ComboBoxItemHoveredColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
	static const ImVec4 ComboBoxItemSelectedColor = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

	const ImVec2 titleSize = ImGui::CalcTextSize(szTextAndAlsoID, 0, true);
	const ImVec2 descriptionSize = ImGui::CalcTextSize(szDescription, 0, true);
	const ImVec2 widgetMin = ImGui::GetCursorScreenPos();
	const ImVec2 widgetMax = widgetMin + Size;
	const ImGuiID id = ImGui::GetID(szTextAndAlsoID);
	const ImGuiStyle& style = ImGui::GetStyle();

	const ImRect bb(widgetMin, widgetMax);
	ImGui::ItemSize(Size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Dibujar fondo
	drawList->AddRectFilled(widgetMin, widgetMax, ImGui::ColorConvertFloat4ToU32(bgColor));

	ImVec2 localMin = ImVec2(widgetMin.x - ImGui::GetWindowPos().x, widgetMin.y - ImGui::GetWindowPos().y);
	ImVec2 localMax = ImVec2(widgetMax.x - ImGui::GetWindowPos().x, widgetMax.y - ImGui::GetWindowPos().y);

	float wrapPos = localMax.x - MarginX;
	ImGui::PushTextWrapPos(wrapPos);

	// Título
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY));
	ImGui::PushFont(SUBSTANCE::pFont15);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f), bb.Max - style.FramePadding, szTextAndAlsoID, 0, &titleSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	// Descripción
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY + descriptionTopOffset));
	ImGui::PushFont(SUBSTANCE::pFont10Light);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f + descriptionTopOffset), bb.Max - style.FramePadding, szDescription, 0, &descriptionSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	ImGui::PopTextWrapPos();

	// Posición del ComboBox
	float comboBoxY = widgetMax.y - ComboBoxMargin - textLineHeight;
	ImGui::SetCursorScreenPos(ImVec2(widgetMin.x + MarginX, comboBoxY));
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

	// Combo
	if (ImGui::Combo(std::string(std::string("##") + szTextAndAlsoID).c_str(), pCurrentIntegerOption, szOptions))
	{
		bChangedValue = true;
	}

	ImGui::PopStyleColor(10);
	ImGui::PopItemWidth();

	ImGui::SetCursorScreenPos(ImVec2(widgetMin.x, widgetMax.y + 8));

	return bChangedValue;
}

bool SUBSTANCE::Function(const char* szTextAndAlsoID, const char* szDescription, bool* pBooleanValue, bool* pBooleanEnableItems)
{
	bool bHovered = false;
	bool bHeld = false;
	bool bClicked = false;

	static const ImVec2 Size = ImVec2(200.0f, 100.0f);
	static const ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	static const ImVec4 whiteColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	static const ImVec4 greenColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	static const ImVec4 redColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	static const ImVec4 almostWhiteColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	static const ImVec4 grayLight = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	static const float IconButtonRounding = 0.0f;
	static const float IconButtonSize = 15.0f;
	static const float LineThickness = 1.0f;
	static const float BorderPercent = 0.5f;
	static const float MarginX = 15.0f;
	static const float MarginY = 10.0f;

	float lineHeight = ImGui::GetTextLineHeight();
	float descriptionTopOffset = lineHeight + (MarginY * 0.5f);

	ImVec4 titleColor = *pBooleanValue ? whiteColor : grayLight;
	ImVec4 descColor = *pBooleanValue ? almostWhiteColor : grayLight;
	ImVec4 iconBgColor = *pBooleanValue ? greenColor : redColor;

	const ImVec2 titleSize = ImGui::CalcTextSize(szTextAndAlsoID, 0, true);
	const ImVec2 descriptionSize = ImGui::CalcTextSize(szDescription, 0, true);
	const ImVec2 widgetMin = ImGui::GetCursorScreenPos();
	const ImVec2 widgetMax = widgetMin + Size;
	const ImVec2 iconBoxMin = ImVec2(widgetMax.x - MarginX - IconButtonSize, widgetMax.y - MarginY - IconButtonSize);
	const ImVec2 iconBoxMax = ImVec2(iconBoxMin.x + IconButtonSize, iconBoxMin.y + IconButtonSize);
	const ImGuiID id = ImGui::GetID(szTextAndAlsoID);
	const ImGuiStyle& style = ImGui::GetStyle();

	const ImRect bb(widgetMin, widgetMax);
	ImGui::ItemSize(Size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;
	bClicked = ImGui::ButtonBehavior(bb, id, &bHovered, &bHeld);
	if (bClicked)
	{
		ImVec2 mousePos = ImGui::GetIO().MousePos;

		bool insideIcon = (mousePos.x >= iconBoxMin.x && mousePos.x < iconBoxMax.x &&
			mousePos.y >= iconBoxMin.y && mousePos.y < iconBoxMax.y);
		if (insideIcon)
			*pBooleanEnableItems = !*pBooleanEnableItems;
		else
			*pBooleanValue = !*pBooleanValue;
	}

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Dibujar fondo
	drawList->AddRectFilled(widgetMin, widgetMax, ImGui::ColorConvertFloat4ToU32(bgColor));

	ImVec2 localMin = ImVec2(widgetMin.x - ImGui::GetWindowPos().x, widgetMin.y - ImGui::GetWindowPos().y);
	ImVec2 localMax = ImVec2(widgetMax.x - ImGui::GetWindowPos().x, widgetMax.y - ImGui::GetWindowPos().y);

	float wrapPos = localMax.x - MarginX;
	ImGui::PushTextWrapPos(wrapPos);

	// Título
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY));
	ImGui::PushFont(SUBSTANCE::pFont15);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f), bb.Max - style.FramePadding, szTextAndAlsoID, 0, &titleSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	// Descripción
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY + descriptionTopOffset));
	ImGui::PushFont(SUBSTANCE::pFont10Light);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f + descriptionTopOffset), bb.Max - style.FramePadding, szDescription, 0, &descriptionSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	ImGui::PopTextWrapPos();

	// Borde
	if (*pBooleanValue)
	{
		DrawStaticBorder(drawList, widgetMin, widgetMax, BorderPercent, ImColor(whiteColor), LineThickness);
	}

	// Lugar del Texto
	const char* onOffText = *pBooleanValue ? "On" : "Off";
	ImVec2 onOffSize = ImGui::CalcTextSize(onOffText);
	//float bottomY = localMax.y - MarginY - onOffSize.y - localMin.y;
	float bottomY = localMax.y - (lineHeight + (MarginY /** 0.5f*/)) - onOffSize.y - localMin.y;
	float bottomX = localMin.x + MarginX;
	ImGui::SetCursorPos(ImVec2(bottomX, bottomY));
	//ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMax.y - MarginY - onOffSize.y));
	//ImGui::PushFont(SUBSTANCE::pFont10Light);
	ImGui::PushStyleColor(ImGuiCol_Text, iconBgColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f + bottomY), bb.Max - style.FramePadding, onOffText, 0, &onOffSize);
	ImGui::PopStyleColor();
	//ImGui::PopFont();

	// Dibujar ícono si es visible
	//if (IconButtonVisible && CheckedChanged) // asumiendo que la acción recae en el click
	{
		//ImU32 iconBgU32 = ImGui::ColorConvertFloat4ToU32(iconBgColor);
		drawList->AddRectFilled(iconBoxMin, iconBoxMax, ImColor(iconBgColor), IconButtonRounding);
		//drawList->AddText(iconBoxMin, ImColor(iconBgColor), *pBooleanEnableItems ? "On" : "Off");

		/*ImTextureID icon = Core::Instances::imageManager.GetImage(BottomRightIconName);
		if (icon != nullptr)
		{
			drawList->AddImage(icon, iconBoxMin, iconBoxMax);
		}*/
	}

	ImGui::SetCursorScreenPos(ImVec2(widgetMin.x, widgetMax.y + 8));

	return bClicked;
}

bool SUBSTANCE::CheckBox(const char* szTextAndAlsoID, const char* szDescription, bool* pBooleanValue)
{
	bool bHovered = false;
	bool bHeld = false;
	bool bClicked = false;

	static const ImVec2 Size = ImVec2(190.0f, 100.0f);
	static const ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	static const ImVec4 whiteColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	static const ImVec4 greenColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	static const ImVec4 redColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	static const ImVec4 almostWhiteColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	static const ImVec4 grayLight = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	static const float IconButtonRounding = 0.0f;
	static const float IconButtonSize = 15.0f;
	static const float LineThickness = 1.0f;
	static const float BorderPercent = 0.5f;
	static const float MarginX = 15.0f;
	static const float MarginY = 10.0f;

	float lineHeight = ImGui::GetTextLineHeight();
	float descriptionTopOffset = lineHeight + (MarginY * 0.5f);

	ImVec4 titleColor = whiteColor;
	ImVec4 descColor = almostWhiteColor;
	ImVec4 iconBgColor = *pBooleanValue ? greenColor : redColor;

	const ImVec2 titleSize = ImGui::CalcTextSize(szTextAndAlsoID, 0, true);
	const ImVec2 descriptionSize = ImGui::CalcTextSize(szDescription, 0, true);
	const ImVec2 widgetMin = ImGui::GetCursorScreenPos();
	const ImVec2 widgetMax = widgetMin + Size;
	const ImGuiID id = ImGui::GetID(szTextAndAlsoID);
	const ImGuiStyle& style = ImGui::GetStyle();

	const ImRect bb(widgetMin, widgetMax);
	ImGui::ItemSize(Size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;
	bClicked = ImGui::ButtonBehavior(bb, id, &bHovered, &bHeld);
	if (bClicked)
		*pBooleanValue = !*pBooleanValue;

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Dibujar fondo
	drawList->AddRectFilled(widgetMin, widgetMax, ImGui::ColorConvertFloat4ToU32(bgColor));

	ImVec2 localMin = ImVec2(widgetMin.x - ImGui::GetWindowPos().x, widgetMin.y - ImGui::GetWindowPos().y);
	ImVec2 localMax = ImVec2(widgetMax.x - ImGui::GetWindowPos().x, widgetMax.y - ImGui::GetWindowPos().y);

	float wrapPos = localMax.x - MarginX;
	ImGui::PushTextWrapPos(wrapPos);

	// Título
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY));
	ImGui::PushFont(SUBSTANCE::pFont15);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f), bb.Max - style.FramePadding, szTextAndAlsoID, 0, &titleSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	// Descripción
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY + descriptionTopOffset));
	ImGui::PushFont(SUBSTANCE::pFont10Light);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f + descriptionTopOffset), bb.Max - style.FramePadding, szDescription, 0, &descriptionSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();
	/*
	ImGui::PushFont(SUBSTANCE::pFont10Light);
	ImGui::TextColored(descColor, szDescription);
	ImGui::PopFont();
	*/

	ImGui::PopTextWrapPos();

	// Borde
	if (*pBooleanValue)
	{
		DrawStaticBorder(drawList, widgetMin, widgetMax, BorderPercent, ImColor(whiteColor), LineThickness);
	}

	// Área del botón ícono
	ImVec2 iconBoxMin = ImVec2(widgetMax.x - MarginX - IconButtonSize, widgetMax.y - MarginY - IconButtonSize);
	ImVec2 iconBoxMax = ImVec2(iconBoxMin.x + IconButtonSize, iconBoxMin.y + IconButtonSize);

	// Dibujar ícono si es visible
	//if (IconButtonVisible && CheckedChanged) // asumiendo que la acción recae en el click
	{
		//ImU32 iconBgU32 = ImGui::ColorConvertFloat4ToU32(iconBgColor);
		//drawList->AddRectFilled(iconBoxMin, iconBoxMax, iconBgU32, IconButtonRounding);
		drawList->AddText(iconBoxMin, ImColor(iconBgColor), *pBooleanValue ? "On" : "Off");

		/*ImTextureID icon = Core::Instances::imageManager.GetImage(BottomRightIconName);
		if (icon != nullptr)
		{
			drawList->AddImage(icon, iconBoxMin, iconBoxMax);
		}*/
	}

	ImGui::SetCursorScreenPos(ImVec2(widgetMin.x, widgetMax.y + 8));

	return bClicked;
}

bool SUBSTANCE::SliderInt(const char* szTextAndAlsoID, const char* szDescription, int* pCurrentIntegerOption, int iMinimumValue, int iMaximumValue)
{
	bool bChangedValue = false;

	static const ImVec2 Size = ImVec2(190.0f, 100.0f);
	static const ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	static const ImVec4 whiteColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	static const ImVec4 greenColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	static const ImVec4 redColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	static const ImVec4 almostWhiteColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	static const ImVec4 grayLight = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	static const float IconButtonRounding = 0.0f;
	static const float IconButtonSize = 15.0f;
	static const float LineThickness = 1.0f;
	static const float BorderPercent = 0.5f;
	static const float MarginX = 15.0f;
	static const float MarginY = 10.0f;
	static const float ComboBoxMargin = 10.0f;

	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float lineHeight = ImGui::GetTextLineHeight();
	const float descriptionTopOffset = lineHeight + (MarginY * 0.5f);

	static const ImVec4 titleColor = whiteColor;
	static const ImVec4 descColor = almostWhiteColor;
	// Estilos del ComboBox
	static const ImVec4 ComboBoxTextColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	static const ImVec4 ComboBoxBackgroundColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	static const ImVec4 ComboBoxHoveredColor = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
	static const ImVec4 ComboBoxActiveColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	static const ImVec4 ComboBoxItemHoveredColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
	static const ImVec4 ComboBoxItemSelectedColor = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

	const ImVec2 titleSize = ImGui::CalcTextSize(szTextAndAlsoID, 0, true);
	const ImVec2 descriptionSize = ImGui::CalcTextSize(szDescription, 0, true);
	const ImVec2 widgetMin = ImGui::GetCursorScreenPos();
	const ImVec2 widgetMax = widgetMin + Size;
	const ImGuiID id = ImGui::GetID(szTextAndAlsoID);
	const ImGuiStyle& style = ImGui::GetStyle();

	const ImRect bb(widgetMin, widgetMax);
	ImGui::ItemSize(Size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Dibujar fondo
	drawList->AddRectFilled(widgetMin, widgetMax, ImGui::ColorConvertFloat4ToU32(bgColor));

	ImVec2 localMin = ImVec2(widgetMin.x - ImGui::GetWindowPos().x, widgetMin.y - ImGui::GetWindowPos().y);
	ImVec2 localMax = ImVec2(widgetMax.x - ImGui::GetWindowPos().x, widgetMax.y - ImGui::GetWindowPos().y);

	float wrapPos = localMax.x - MarginX;
	ImGui::PushTextWrapPos(wrapPos);

	// Título
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY));
	ImGui::PushFont(SUBSTANCE::pFont15);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f), bb.Max - style.FramePadding, szTextAndAlsoID, 0, &titleSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	// Descripción
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY + descriptionTopOffset));
	ImGui::PushFont(SUBSTANCE::pFont10Light);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f + descriptionTopOffset), bb.Max - style.FramePadding, szDescription, 0, &descriptionSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	ImGui::PopTextWrapPos();

	// Posición del ComboBox
	float comboBoxY = widgetMax.y - ComboBoxMargin - textLineHeight;
	ImGui::SetCursorScreenPos(ImVec2(widgetMin.x + MarginX, comboBoxY));
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

	// Combo
	if (ImGui::SliderInt(std::string(std::string("##") + szTextAndAlsoID).c_str(), pCurrentIntegerOption, iMinimumValue, iMaximumValue))
	{
		bChangedValue = true;
	}

	ImGui::PopStyleColor(10);
	ImGui::PopItemWidth();

	ImGui::SetCursorScreenPos(ImVec2(widgetMin.x, widgetMax.y + 8));

	return bChangedValue;
}

bool SUBSTANCE::SliderInt2(const char* szTextAndAlsoID, const char* szDescription, int* pCurrentIntegerOption, int iMinimumValue, int iMaximumValue)
{
	bool bChangedValue = false;

	static const ImVec2 Size = ImVec2(190.0f, 100.0f);
	static const ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	static const ImVec4 whiteColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	static const ImVec4 greenColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	static const ImVec4 redColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	static const ImVec4 almostWhiteColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	static const ImVec4 grayLight = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	static const float IconButtonRounding = 0.0f;
	static const float IconButtonSize = 15.0f;
	static const float LineThickness = 1.0f;
	static const float BorderPercent = 0.5f;
	static const float MarginX = 15.0f;
	static const float MarginY = 10.0f;
	static const float ComboBoxMargin = 10.0f;

	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float lineHeight = ImGui::GetTextLineHeight();
	const float descriptionTopOffset = lineHeight + (MarginY * 0.5f);

	static const ImVec4 titleColor = whiteColor;
	static const ImVec4 descColor = almostWhiteColor;
	// Estilos del ComboBox
	static const ImVec4 ComboBoxTextColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	static const ImVec4 ComboBoxBackgroundColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	static const ImVec4 ComboBoxHoveredColor = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
	static const ImVec4 ComboBoxActiveColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	static const ImVec4 ComboBoxItemHoveredColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
	static const ImVec4 ComboBoxItemSelectedColor = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

	const ImVec2 titleSize = ImGui::CalcTextSize(szTextAndAlsoID, 0, true);
	const ImVec2 descriptionSize = ImGui::CalcTextSize(szDescription, 0, true);
	const ImVec2 widgetMin = ImGui::GetCursorScreenPos();
	const ImVec2 widgetMax = widgetMin + Size;
	const ImGuiID id = ImGui::GetID(szTextAndAlsoID);
	const ImGuiStyle& style = ImGui::GetStyle();

	const ImRect bb(widgetMin, widgetMax);
	ImGui::ItemSize(Size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Dibujar fondo
	drawList->AddRectFilled(widgetMin, widgetMax, ImGui::ColorConvertFloat4ToU32(bgColor));

	ImVec2 localMin = ImVec2(widgetMin.x - ImGui::GetWindowPos().x, widgetMin.y - ImGui::GetWindowPos().y);
	ImVec2 localMax = ImVec2(widgetMax.x - ImGui::GetWindowPos().x, widgetMax.y - ImGui::GetWindowPos().y);

	float wrapPos = localMax.x - MarginX;
	ImGui::PushTextWrapPos(wrapPos);

	// Título
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY));
	ImGui::PushFont(SUBSTANCE::pFont15);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f), bb.Max - style.FramePadding, szTextAndAlsoID, 0, &titleSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	// Descripción
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY + descriptionTopOffset));
	ImGui::PushFont(SUBSTANCE::pFont10Light);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f + descriptionTopOffset), bb.Max - style.FramePadding, szDescription, 0, &descriptionSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	ImGui::PopTextWrapPos();

	// Posición del ComboBox
	float comboBoxY = widgetMax.y - ComboBoxMargin - textLineHeight;
	ImGui::SetCursorScreenPos(ImVec2(widgetMin.x + MarginX, comboBoxY));
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

	// Combo
	if (ImGui::SliderInt2(std::string(std::string("##") + szTextAndAlsoID).c_str(), pCurrentIntegerOption, iMinimumValue, iMaximumValue))
	{
		bChangedValue = true;
	}

	ImGui::PopStyleColor(10);
	ImGui::PopItemWidth();

	ImGui::SetCursorScreenPos(ImVec2(widgetMin.x, widgetMax.y + 8));

	return bChangedValue;
}

bool SUBSTANCE::SliderFloat(const char* szTextAndAlsoID, const char* szDescription, float* pCurrentFloatOption, float fMinimumValue, float fMaximumValue)
{
	bool bChangedValue = false;

	static const ImVec2 Size = ImVec2(190.0f, 100.0f);
	static const ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	static const ImVec4 whiteColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	static const ImVec4 greenColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	static const ImVec4 redColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	static const ImVec4 almostWhiteColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	static const ImVec4 grayLight = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	static const float IconButtonRounding = 0.0f;
	static const float IconButtonSize = 15.0f;
	static const float LineThickness = 1.0f;
	static const float BorderPercent = 0.5f;
	static const float MarginX = 15.0f;
	static const float MarginY = 10.0f;
	static const float ComboBoxMargin = 10.0f;

	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float lineHeight = ImGui::GetTextLineHeight();
	const float descriptionTopOffset = lineHeight + (MarginY * 0.5f);

	static const ImVec4 titleColor = whiteColor;
	static const ImVec4 descColor = almostWhiteColor;
	// Estilos del ComboBox
	static const ImVec4 ComboBoxTextColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	static const ImVec4 ComboBoxBackgroundColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	static const ImVec4 ComboBoxHoveredColor = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
	static const ImVec4 ComboBoxActiveColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	static const ImVec4 ComboBoxItemHoveredColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
	static const ImVec4 ComboBoxItemSelectedColor = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

	const ImVec2 titleSize = ImGui::CalcTextSize(szTextAndAlsoID, 0, true);
	const ImVec2 descriptionSize = ImGui::CalcTextSize(szDescription, 0, true);
	const ImVec2 widgetMin = ImGui::GetCursorScreenPos();
	const ImVec2 widgetMax = widgetMin + Size;
	const ImGuiID id = ImGui::GetID(szTextAndAlsoID);
	const ImGuiStyle& style = ImGui::GetStyle();

	const ImRect bb(widgetMin, widgetMax);
	ImGui::ItemSize(Size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Dibujar fondo
	drawList->AddRectFilled(widgetMin, widgetMax, ImGui::ColorConvertFloat4ToU32(bgColor));

	ImVec2 localMin = ImVec2(widgetMin.x - ImGui::GetWindowPos().x, widgetMin.y - ImGui::GetWindowPos().y);
	ImVec2 localMax = ImVec2(widgetMax.x - ImGui::GetWindowPos().x, widgetMax.y - ImGui::GetWindowPos().y);

	float wrapPos = localMax.x - MarginX;
	ImGui::PushTextWrapPos(wrapPos);

	// Título
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY));
	ImGui::PushFont(SUBSTANCE::pFont15);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f), bb.Max - style.FramePadding, szTextAndAlsoID, 0, &titleSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	// Descripción
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY + descriptionTopOffset));
	ImGui::PushFont(SUBSTANCE::pFont10Light);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f + descriptionTopOffset), bb.Max - style.FramePadding, szDescription, 0, &descriptionSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	ImGui::PopTextWrapPos();

	// Posición del ComboBox
	float comboBoxY = widgetMax.y - ComboBoxMargin - textLineHeight;
	ImGui::SetCursorScreenPos(ImVec2(widgetMin.x + MarginX, comboBoxY));
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

	// Combo
	if (ImGui::SliderFloat(std::string(std::string("##") + szTextAndAlsoID).c_str(), pCurrentFloatOption, fMinimumValue, fMaximumValue))
	{
		bChangedValue = true;
	}

	ImGui::PopStyleColor(10);
	ImGui::PopItemWidth();

	ImGui::SetCursorScreenPos(ImVec2(widgetMin.x, widgetMax.y + 8));

	return bChangedValue;
}

bool SUBSTANCE::SliderFloat2(const char* szTextAndAlsoID, const char* szDescription, float* pCurrentFloatOption, float fMinimumValue, float fMaximumValue)
{
	bool bChangedValue = false;

	static const ImVec2 Size = ImVec2(190.0f, 100.0f);
	static const ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	static const ImVec4 whiteColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	static const ImVec4 greenColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	static const ImVec4 redColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	static const ImVec4 almostWhiteColor = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);
	static const ImVec4 grayLight = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	static const float IconButtonRounding = 0.0f;
	static const float IconButtonSize = 15.0f;
	static const float LineThickness = 1.0f;
	static const float BorderPercent = 0.5f;
	static const float MarginX = 15.0f;
	static const float MarginY = 10.0f;
	static const float ComboBoxMargin = 10.0f;

	const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
	const float lineHeight = ImGui::GetTextLineHeight();
	const float descriptionTopOffset = lineHeight + (MarginY * 0.5f);

	static const ImVec4 titleColor = whiteColor;
	static const ImVec4 descColor = almostWhiteColor;
	// Estilos del ComboBox
	static const ImVec4 ComboBoxTextColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	static const ImVec4 ComboBoxBackgroundColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	static const ImVec4 ComboBoxHoveredColor = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
	static const ImVec4 ComboBoxActiveColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	static const ImVec4 ComboBoxItemHoveredColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
	static const ImVec4 ComboBoxItemSelectedColor = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

	const ImVec2 titleSize = ImGui::CalcTextSize(szTextAndAlsoID, 0, true);
	const ImVec2 descriptionSize = ImGui::CalcTextSize(szDescription, 0, true);
	const ImVec2 widgetMin = ImGui::GetCursorScreenPos();
	const ImVec2 widgetMax = widgetMin + Size;
	const ImGuiID id = ImGui::GetID(szTextAndAlsoID);
	const ImGuiStyle& style = ImGui::GetStyle();

	const ImRect bb(widgetMin, widgetMax);
	ImGui::ItemSize(Size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Dibujar fondo
	drawList->AddRectFilled(widgetMin, widgetMax, ImGui::ColorConvertFloat4ToU32(bgColor));

	ImVec2 localMin = ImVec2(widgetMin.x - ImGui::GetWindowPos().x, widgetMin.y - ImGui::GetWindowPos().y);
	ImVec2 localMax = ImVec2(widgetMax.x - ImGui::GetWindowPos().x, widgetMax.y - ImGui::GetWindowPos().y);

	float wrapPos = localMax.x - MarginX;
	ImGui::PushTextWrapPos(wrapPos);

	// Título
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY));
	ImGui::PushFont(SUBSTANCE::pFont15);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f), bb.Max - style.FramePadding, szTextAndAlsoID, 0, &titleSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	// Descripción
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY + descriptionTopOffset));
	ImGui::PushFont(SUBSTANCE::pFont10Light);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f + descriptionTopOffset), bb.Max - style.FramePadding, szDescription, 0, &descriptionSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	ImGui::PopTextWrapPos();

	// Posición del ComboBox
	float comboBoxY = widgetMax.y - ComboBoxMargin - textLineHeight;
	ImGui::SetCursorScreenPos(ImVec2(widgetMin.x + MarginX, comboBoxY));
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

	// Combo
	if (ImGui::SliderFloat2(std::string(std::string("##") + szTextAndAlsoID).c_str(), pCurrentFloatOption, fMinimumValue, fMaximumValue))
	{
		bChangedValue = true;
	}

	ImGui::PopStyleColor(10);
	ImGui::PopItemWidth();

	ImGui::SetCursorScreenPos(ImVec2(widgetMin.x, widgetMax.y + 8));

	return bChangedValue;
}

struct quand_state
{
	float slow0, slow1, tickness;
	int move_y, move_x;
	float move_yy, move_xx;
};

bool QuandScalar(const char* label, const char* description, ImGuiDataType data_type, void* p_data0, void* p_data1, const void* p_min0, const void* p_max0, const void* p_min1, const void* p_max1, const char* format0, const char* format1, ImGuiSliderFlags flags)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems) return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	const ImVec2 description_size = ImGui::CalcTextSize(description, NULL, true);
	const ImVec2 Size = ImVec2(190.f, 100.f);
	const ImVec2 widgetMin = ImGui::GetCursorScreenPos();
	const ImVec2 widgetMax = widgetMin + Size;

	//const float w = ImGui::GetContentRegionMax().x - style.WindowPadding.x;
	const float w = 190.f;

	ImVec2 pos = window->DC.CursorPos;
	const ImRect frame_bb(pos + ImVec2(10, 55), pos + ImVec2(w - 10, Size.y - 5));
	const ImRect rect(pos, pos + Size);
	const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;

	const ImVec4 accent = ImColor(114, 149, 255);
	const ImVec4 background = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	const ImVec4 text_active = ImColor(255, 255, 255);
	const ImVec4 text = ImColor(81, 85, 84);
	const float rounding = 4;

	const float MarginX = 15.0f;
	const float MarginY = 10.0f;
	const float lineHeight = ImGui::GetTextLineHeight();
	const float descriptionTopOffset = lineHeight + (MarginY * 0.5f);
	const float valueTopOffset = 2 * (lineHeight + (MarginY * 0.5f));

	ImGui::ItemSize(rect, 0.f);
	if (!ImGui::ItemAdd(rect, id)) return false;

	if (format0 == NULL) format0 = ImGui::DataTypeGetInfo(data_type)->PrintFmt;
	if (format1 == NULL) format1 = ImGui::DataTypeGetInfo(data_type)->PrintFmt;

	bool hovered, held, pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held);

	bool temp_input_is_active = temp_input_allowed && ImGui::TempInputIsActive(id);

	ImRect grab_bb0, grab_bb1;

	static std::map<ImGuiID, quand_state> anim;
	auto it_anim = anim.find(id);

	if (it_anim == anim.end())
	{
		anim.insert({ id, quand_state() });
		it_anim = anim.find(id);
	}

	const bool value_changed0 = ImGui::SliderBehavior(ImRect(frame_bb.Min - ImVec2(7, 0), frame_bb.Max + ImVec2(5, 0)), id, data_type, p_data0, p_min0, p_max0, format0, flags, &grab_bb0);
	const bool value_changed1 = ImGui::SliderBehavior(ImRect(frame_bb.Min - ImVec2(0, 7), frame_bb.Max + ImVec2(0, 4)), id, data_type, p_data1, p_max1, p_min1, format1, flags | ImGuiSliderFlags_Vertical, &grab_bb1);

	if (value_changed0) ImGui::MarkItemEdited(id);

	char value_buf0[64];
	char value_buf1[64];
	const char* value_buf_end0 = value_buf0 + ImGui::DataTypeFormatString(value_buf0, IM_ARRAYSIZE(value_buf0), data_type, p_data0, format0);
	const char* value_buf_end1 = value_buf1 + ImGui::DataTypeFormatString(value_buf1, IM_ARRAYSIZE(value_buf1), data_type, p_data1, format1);

	ImGui::GetWindowDrawList()->AddRectFilled(rect.Min, rect.Max, ImGui::GetColorU32(background), rounding);

	it_anim->second.slow0 = ImLerp(it_anim->second.slow0, grab_bb0.Min.x - (frame_bb.Min.x) + 7, g.IO.DeltaTime * 25.f);
	it_anim->second.slow1 = ImLerp(it_anim->second.slow1, grab_bb1.Min.y - (frame_bb.Min.y) + 7, g.IO.DeltaTime * 25.f);

	ImGui::GetWindowDrawList()->AddLine(ImVec2(it_anim->second.slow0 + frame_bb.Min.x, grab_bb0.Min.y), ImVec2(it_anim->second.slow0 + frame_bb.Min.x, grab_bb0.Max.y), ImGui::GetColorU32(accent), 0.5f);
	//ImGui::GetWindowDrawList()->AddShadowRect(ImVec2(it_anim->second.slow0 + frame_bb.Min.x, grab_bb0.Min.y), ImVec2(it_anim->second.slow0 + frame_bb.Min.x, grab_bb0.Max.y), GetColorU32(c::accent, 0.5f), 25.f, ImVec2(0, 0), 100.f);

	ImGui::GetWindowDrawList()->AddLine(ImVec2(grab_bb1.Min.x, it_anim->second.slow1 + frame_bb.Min.y), ImVec2(grab_bb1.Max.x, it_anim->second.slow1 + frame_bb.Min.y), ImGui::GetColorU32(accent), 0.5f);
	//ImGui::GetWindowDrawList()->AddShadowRect(ImVec2(grab_bb1.Min.x, it_anim->second.slow1 + frame_bb.Min.y), ImVec2(grab_bb1.Max.x, it_anim->second.slow1 + frame_bb.Min.y), GetColorU32(c::accent, 0.5f), 25.f, ImVec2(0, 0), 100.f);

	//ImGui::PushFont(SUBSTANCE::pFont15);
	//ImGui::GetWindowDrawList()->AddText(/*font::lexend_bold*/0, 17, ImVec2(rect.Min.x + 10, rect.Min.y + 10), ImGui::GetColorU32(text_active), label);
	//ImGui::PopFont();

	std::string blah0(value_buf0), blah1(value_buf1);

	std::string label_x = "[X, " + blah0 + " Y, " + blah1 + "]";

	//ImGui::PushFont(SUBSTANCE::pFont10Light);
	//ImGui::GetWindowDrawList()->AddText(/*font::lexend_bold*/0, 17, ImVec2(rect.Min.x + 10, rect.Min.y + 30), ImGui::GetColorU32(text), description);
	//ImGui::GetWindowDrawList()->AddText(/*font::lexend_bold*/0, 17, rect.Min + ImVec2(0, 7)/*, rect.Max - ImVec2(15, 10)*/, ImGui::GetColorU32(text_active), label_x.c_str());
	//ImGui::PopFont();

	// Título
	ImGui::SetCursorPos(ImVec2(rect.Min.x + MarginX, rect.Min.y + MarginY));
	ImGui::PushFont(SUBSTANCE::pFont15);
	ImGui::PushStyleColor(ImGuiCol_Text, text_active);
	ImGui::RenderTextClipped(ImVec2(rect.Min.x + style.FramePadding.x + 10.0f, rect.Min.y + style.FramePadding.y + 7.0f), rect.Max - style.FramePadding, label, 0, &label_size);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	// Descripción
	ImGui::SetCursorPos(ImVec2(rect.Min.x + MarginX, rect.Min.y + MarginY + descriptionTopOffset));
	ImGui::PushFont(SUBSTANCE::pFont10Light);
	ImGui::PushStyleColor(ImGuiCol_Text, text_active);
	ImGui::RenderTextClipped(ImVec2(rect.Min.x + style.FramePadding.x + 10.0f, rect.Min.y + style.FramePadding.y + 7.0f + descriptionTopOffset), rect.Max - style.FramePadding, description, 0, &description_size);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	// Valor
	ImGui::SetCursorPos(ImVec2(rect.Min.x + MarginX, rect.Min.y + MarginY + valueTopOffset));
	ImGui::PushFont(SUBSTANCE::pFont10Light);
	ImGui::PushStyleColor(ImGuiCol_Text, text_active);
	ImGui::RenderText(ImVec2(rect.Min.x + style.FramePadding.x + 10.0f, rect.Min.y + style.FramePadding.y + 7.0f + valueTopOffset), label_x.c_str(), 0);
	//ImGui::RenderTextClipped(ImVec2(rect.Min.x + style.FramePadding.x + 10.0f, rect.Min.y + style.FramePadding.y + 7.0f), rect.Max - style.FramePadding, label_x.c_str(), 0);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	ImGui::SetCursorScreenPos(ImVec2(widgetMin.x, widgetMax.y + 8));
}

bool SUBSTANCE::QuandFloat(const char* label, const char* description, float* v0, float* v1, float v_min0, float v_max0, float v_min1, float v_max1, const char* format0, const char* format1, ImGuiSliderFlags flags)
{
	bool bReturnValue = false;
	ImGui::PushItemWidth(180.f);
	bReturnValue = QuandScalar(label, description, ImGuiDataType_Float, v0, v1, &v_min0, &v_max0, &v_min1, &v_max1, format0, format1, flags);
	ImGui::PopItemWidth();
	return bReturnValue;
}

bool SUBSTANCE::QuandInt(const char* label, const char* description, int* v0, int* v1, int v_min0, int v_max0, int v_min1, int v_max1, const char* format0, const char* format1, ImGuiSliderFlags flags)
{
	return QuandScalar(label, description, ImGuiDataType_S32, v0, v1, &v_min0, &v_max0, &v_min1, &v_max1, format0, format1, flags);
}

// ColorEdit supports RGB and HSV inputs. In case of RGB input resulting color may have undefined hue and/or saturation.
// Since widget displays both RGB and HSV values we must preserve hue and saturation to prevent these values resetting.
static void ColorEditRestoreHS(const float* col, float* H, float* S, float* V)
{
	ImGuiContext& g = *GImGui;
	IM_ASSERT(g.ColorEditCurrentID != 0);
	if (g.ColorEditSavedID != g.ColorEditCurrentID || g.ColorEditSavedColor != ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
		return;

	// When S == 0, H is undefined.
	// When H == 1 it wraps around to 0.
	if (*S == 0.0f || (*H == 0.0f && g.ColorEditSavedHue == 1))
		*H = g.ColorEditSavedHue;

	// When V == 0, S is undefined.
	if (*V == 0.0f)
		*S = g.ColorEditSavedSat;
}

// Note: ColorPicker4() only accesses 3 floats if ImGuiColorEditFlags_NoAlpha flag is set.
// (In C++ the 'float col[4]' notation for a function argument is equivalent to 'float* col', we only specify a size to facilitate understanding of the code.)
// FIXME: we adjust the big color square height based on item width, which may cause a flickering feedback loop (if automatic height makes a vertical scrollbar appears, affecting automatic width..)
// FIXME: this is trying to be aware of style.Alpha but not fully correct. Also, the color wheel will have overlapping glitches with (style.Alpha < 1.0)
struct picker_state
{
	float hue_bar;
	float alpha_bar;
	float circle;
	ImVec2 circle_move;
};

bool SUBSTANCE::ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags, const float* ref_col)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImDrawList* draw_list = window->DrawList;
	ImGuiStyle& style = g.Style;
	ImGuiIO& io = g.IO;

	const float width = ImGui::CalcItemWidth();
	g.NextItemData.ClearFlags();

	ImGui::PushID(label);
	ImGui::BeginGroup();

	if (!(flags & ImGuiColorEditFlags_NoSidePreview))
		flags |= ImGuiColorEditFlags_NoSmallPreview;

	if (!(flags & ImGuiColorEditFlags_NoOptions))
		ImGui::ColorPickerOptionsPopup(col, flags);

	// Read stored options
	if (!(flags & ImGuiColorEditFlags_PickerMask_))
		flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_PickerMask_;
	if (!(flags & ImGuiColorEditFlags_InputMask_))
		flags |= ((g.ColorEditOptions & ImGuiColorEditFlags_InputMask_) ? g.ColorEditOptions : ImGuiColorEditFlags_DefaultOptions_) & ImGuiColorEditFlags_InputMask_;
	IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_PickerMask_)); // Check that only 1 is selected
	IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));  // Check that only 1 is selected
	if (!(flags & ImGuiColorEditFlags_NoOptions))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags_AlphaBar);

	// Setup
	const float MarginX = 15.f;
	int components = (flags & ImGuiColorEditFlags_NoAlpha) ? 3 : 4;
	bool alpha_bar = (flags & ImGuiColorEditFlags_AlphaBar) && !(flags & ImGuiColorEditFlags_NoAlpha);
	ImVec2 picker_pos = ImVec2(window->DC.CursorPos.x + MarginX, window->DC.CursorPos.y);
	ImVec2 bar_pos = window->DC.CursorPos + ImVec2(0, 85);
	float square_sz = ImGui::GetFrameHeight();
	//float bars_width = 209.f; // Arbitrary smallish width of Hue/Alpha picking bars
	float bars_width = 160.f; // Arbitrary smallish width of Hue/Alpha picking bars
	float sv_picker_size = ImMax(bars_width * 1, width - (alpha_bar ? 2 : 1) * (bars_width + style.ItemInnerSpacing.x)) + 0; // Saturation/Value picking box
	//float sv_picker_size = bars_width; // Saturation/Value picking box
	float sv_bar_size = 23; // Saturation/Value picking box
	//float bar0_pos_x = GetWindowPos().x;// + style.WindowPadding.x;
	float bar0_pos_x = ImGui::GetWindowPos().x + MarginX;
	float bar1_pos_x = bar0_pos_x;
	float bars_triangles_half_sz = IM_FLOOR(bars_width * 0.20f);

	float backup_initial_col[4];
	memcpy(backup_initial_col, col, components * sizeof(float));

	float wheel_thickness = sv_picker_size * 0.08f;
	float wheel_r_outer = sv_picker_size * 0.50f;
	float wheel_r_inner = wheel_r_outer - wheel_thickness;
	ImVec2 wheel_center(picker_pos.x + (sv_picker_size + bars_width) * 0.5f, picker_pos.y + sv_picker_size * 0.5f);

	// Note: the triangle is displayed rotated with triangle_pa pointing to Hue, but most coordinates stays unrotated for logic.
	float triangle_r = wheel_r_inner - (int)(sv_picker_size * 0.027f);
	ImVec2 triangle_pa = ImVec2(triangle_r, 0.0f); // Hue point.
	ImVec2 triangle_pb = ImVec2(triangle_r * -0.5f, triangle_r * -0.866025f); // Black point.
	ImVec2 triangle_pc = ImVec2(triangle_r * -0.5f, triangle_r * +0.866025f); // White point.

	float H = col[0], S = col[1], V = col[2];
	float R = col[0], G = col[1], B = col[2];
	if (flags & ImGuiColorEditFlags_InputRGB)
	{
		// Hue is lost when converting from greyscale rgb (saturation=0). Restore it.
		ImGui::ColorConvertRGBtoHSV(R, G, B, H, S, V);
		ColorEditRestoreHS(col, &H, &S, &V);
	}
	else if (flags & ImGuiColorEditFlags_InputHSV)
	{
		ImGui::ColorConvertHSVtoRGB(H, S, V, R, G, B);
	}

	bool value_changed = false, value_changed_h = false, value_changed_sv = false;

	ImGui::PushItemFlag(ImGuiItemFlags_NoNav, true);

	// SV rectangle logic
	ImGui::InvisibleButton("sv", ImVec2(sv_picker_size, sv_picker_size - 80));
	if (ImGui::IsItemActive())
	{
		S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size - 1));
		V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 80));

		// Greatly reduces hue jitter and reset to 0 when hue == 255 and color is rapidly modified using SV square.
		if (g.ColorEditSavedColor == ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0)))
			H = g.ColorEditSavedHue;
		value_changed = value_changed_sv = true;
	}

	// Hue bar logic
	ImGui::SetCursorScreenPos(ImVec2(bar0_pos_x, bar_pos.y));
	ImGui::InvisibleButton("hue", ImVec2(bars_width, sv_bar_size));
	if (ImGui::IsItemActive())
	{
		H = 1.f - ImSaturate((io.MousePos.x - bar_pos.x) / (bars_width - 1));
		value_changed = value_changed_h = true;
	}

	// Alpha bar logic
	if (alpha_bar)
	{
		ImGui::SetCursorScreenPos(ImVec2(bar1_pos_x, bar_pos.y + 16));
		ImGui::InvisibleButton("alpha", ImVec2(bars_width, sv_bar_size));
		if (ImGui::IsItemActive())
		{
			col[3] = ImSaturate((io.MousePos.x - bar_pos.x) / (bars_width - 1));
			value_changed = true;
		}
	}
	ImGui::PopItemFlag(); // ImGuiItemFlags_NoNav

	// Convert back color to RGB
	if (value_changed_h || value_changed_sv)
	{
		if (flags & ImGuiColorEditFlags_InputRGB)
		{
			ImGui::ColorConvertHSVtoRGB(H, S, V, col[0], col[1], col[2]);
			g.ColorEditSavedHue = H;
			g.ColorEditSavedSat = S;
			g.ColorEditSavedColor = ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0));
		}
		else if (flags & ImGuiColorEditFlags_InputHSV)
		{
			col[0] = H;
			col[1] = S;
			col[2] = V;
		}
	}

	// R,G,B and H,S,V slider color editor
	bool value_changed_fix_hue_wrap = false;

	if (value_changed_fix_hue_wrap && (flags & ImGuiColorEditFlags_InputRGB))
	{
		float new_H, new_S, new_V;
		ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
		if (new_H <= 0 && H > 0)
		{
			if (new_V <= 0 && V != new_V)
				ImGui::ColorConvertHSVtoRGB(H, S, new_V <= 0 ? V * 0.5f : new_V, col[0], col[1], col[2]);
			else if (new_S <= 0)
				ImGui::ColorConvertHSVtoRGB(H, new_S <= 0 ? S * 0.5f : new_S, new_V, col[0], col[1], col[2]);
		}
	}

	if (value_changed)
	{
		if (flags & ImGuiColorEditFlags_InputRGB)
		{
			R = col[0];
			G = col[1];
			B = col[2];
			ImGui::ColorConvertRGBtoHSV(R, G, B, H, S, V);
			ColorEditRestoreHS(col, &H, &S, &V);   // Fix local Hue as display below will use it immediately.
		}
		else if (flags & ImGuiColorEditFlags_InputHSV)
		{
			H = col[0];
			S = col[1];
			V = col[2];
			ImGui::ColorConvertHSVtoRGB(H, S, V, R, G, B);
		}
	}

	const int style_alpha8 = IM_F32_TO_INT8_SAT(style.Alpha);
	const ImU32 col_black = IM_COL32(0, 0, 0, style_alpha8);
	const ImU32 col_white = IM_COL32(255, 255, 255, style_alpha8);
	const ImU32 col_midgrey = IM_COL32(128, 128, 128, style_alpha8);
	const ImU32 col_hues[7] = { IM_COL32(255,0,0,style_alpha8), IM_COL32(255,0,255,style_alpha8), IM_COL32(0,0,255,style_alpha8),IM_COL32(0,255,255,style_alpha8), IM_COL32(0,255,0,style_alpha8), IM_COL32(255,255,0,style_alpha8), IM_COL32(255,0,0,style_alpha8) };
	//const ImU32 col_hues[7] = { IM_COL32(255,0,0,style_alpha8), IM_COL32(255,255,0,style_alpha8), IM_COL32(0,255,0,style_alpha8), IM_COL32(0,255,255,style_alpha8), IM_COL32(0,0,255,style_alpha8), IM_COL32(255,0,255,style_alpha8), IM_COL32(255,0,0,style_alpha8) };

	ImVec4 hue_color_f(1, 1, 1, style.Alpha); ImGui::ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
	ImU32 hue_color32 = ImGui::ColorConvertFloat4ToU32(hue_color_f);
	ImU32 user_col32_striped_of_alpha = ImGui::ColorConvertFloat4ToU32(ImVec4(R, G, B, style.Alpha)); // Important: this is still including the main rendering/style alpha!!

	ImVec2 sv_cursor_pos;

	ImGui::GetWindowDrawList()->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size - 80), col_white, hue_color32, hue_color32, col_white);
	ImGui::GetWindowDrawList()->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size - 80), 0, 0, col_black, col_black);
	ImGui::RenderFrameBorder(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), 0.0f);
	sv_cursor_pos.x = ImClamp(IM_ROUND(picker_pos.x + ImSaturate(S) * sv_picker_size), picker_pos.x, picker_pos.x + sv_picker_size - 2); // Sneakily prevent the circle to stick out too much
	sv_cursor_pos.y = ImClamp(IM_ROUND(picker_pos.y + ImSaturate(1 - V) * (sv_picker_size - 80)), picker_pos.y + 2, picker_pos.y + sv_picker_size - 80);

	static std::map<ImGuiID, picker_state> anim;
	auto it_anim = anim.find(ImGui::GetID(label));

	if (it_anim == anim.end())
	{
		anim.insert({ ImGui::GetID(label), picker_state() });
		it_anim = anim.find(ImGui::GetID(label));
	}

	// Render Hue Bar
	for (int i = 0; i < 6; ++i)
		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(bar0_pos_x + i * (bars_width / 6), bar_pos.y + 6), ImVec2(bar0_pos_x + (i + 1) * (bars_width / 6), bar_pos.y + sv_bar_size - 7), col_hues[i], col_hues[i + 1], col_hues[i + 1], col_hues[i]);
	ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(bar0_pos_x, bar_pos.y + 6), ImVec2(bar0_pos_x + bars_width, bar_pos.y + sv_bar_size - 7), ImGui::GetColorU32(ImGuiCol_PopupBg), 0, 0, 0);

	float bar0_line_x = IM_ROUND(bar_pos.x + (1.f - H) * bars_width);

	bar0_line_x = ImClamp(bar0_line_x, picker_pos.x + 3.f, picker_pos.x + 204.f);

	it_anim->second.hue_bar = ImLerp(it_anim->second.hue_bar, bar0_line_x - bar_pos.x, g.IO.DeltaTime * 20.f);

	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(it_anim->second.hue_bar + bar_pos.x - 3, bar_pos.y + 3), ImVec2(it_anim->second.hue_bar + bar_pos.x + 5, bar_pos.y + 19), ImColor(255, 255, 255, 255), 30.f);

	it_anim->second.circle_move = ImLerp(it_anim->second.circle_move, sv_cursor_pos - bar_pos, g.IO.DeltaTime * 20.f);
	it_anim->second.circle = ImLerp(it_anim->second.circle, value_changed_sv ? 4.f : 7.f, g.IO.DeltaTime * 20.f);

	ImGui::GetWindowDrawList()->AddCircleFilled(it_anim->second.circle_move + bar_pos, it_anim->second.circle, user_col32_striped_of_alpha, 32);
	ImGui::GetWindowDrawList()->AddCircle(it_anim->second.circle_move + bar_pos + ImVec2(0, 1), it_anim->second.circle, IM_COL32(50, 50, 50, 150 * g.Style.Alpha), 32);
	ImGui::GetWindowDrawList()->AddCircle(it_anim->second.circle_move + bar_pos, it_anim->second.circle, IM_COL32(230, 230, 230, 255 * g.Style.Alpha), 32);

	if (alpha_bar)
	{
		float alpha = ImSaturate(col[3]);
		ImRect bar1_bb(bar1_pos_x, bar_pos.y + 20, bar1_pos_x + bars_width, bar_pos.y + 20 + sv_bar_size);

		ImGui::RenderColorRectWithAlphaCheckerboard(draw_list, bar1_bb.Min + ImVec2(0, 8), bar1_bb.Max - ImVec2(0, 5), 0, bar1_bb.GetWidth() / 50.0f, ImVec2(0.0f, 0.0f));

		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(bar1_bb.Min + ImVec2(0, 8), bar1_bb.Max - ImVec2(0, 5), user_col32_striped_of_alpha & ~IM_COL32_A_MASK, user_col32_striped_of_alpha, user_col32_striped_of_alpha, user_col32_striped_of_alpha & ~IM_COL32_A_MASK);
		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(bar1_bb.Min + ImVec2(0, 8), bar1_bb.Max - ImVec2(0, 5), ImGui::GetColorU32(ImGuiCol_PopupBg), 0, 0, 0);
		float bar1_line_x = IM_ROUND(bar_pos.x + alpha * bars_width);

		bar1_line_x = ImClamp(bar1_line_x, bar_pos.x, picker_pos.x + 200.f);
		it_anim->second.alpha_bar = ImLerp(it_anim->second.alpha_bar, bar1_line_x - bar_pos.x/* + 5.f*/, g.IO.DeltaTime * 25.f);
		ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(it_anim->second.alpha_bar + bar_pos.x, bar1_bb.Min.y + 13.0f), 6, IM_COL32(255, 255, 255, 255), 100.f);
	}

	ImGui::EndGroup();

	if (value_changed && memcmp(backup_initial_col, col, components * sizeof(float)) == 0)
		value_changed = false;
	if (value_changed)
		ImGui::MarkItemEdited(g.LastItemData.ID);

	ImGui::PopID();

	ImGui::Dummy(ImVec2(0.f, 8.f));

	return value_changed;
}

bool SUBSTANCE::ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	const ImVec4 text_active = ImColor(255, 255, 255);

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const float square_sz = ImGui::GetFrameHeight();
	const float w_full = ImGui::CalcItemWidth();
	const float w_button = (flags & ImGuiColorEditFlags_NoSmallPreview) ? 0.0f : (square_sz + style.ItemInnerSpacing.x);
	const float w_inputs = w_full - w_button;
	const char* label_display_end = ImGui::FindRenderedTextEnd(label);
	const ImVec4 background = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	g.NextItemData.ClearFlags();


	ImGui::BeginGroup();
	ImGui::PushID(label);
	const bool set_current_color_edit_id = (g.ColorEditCurrentID == 0);
	if (set_current_color_edit_id)
		g.ColorEditCurrentID = window->IDStack.back();

	// If we're not showing any slider there's no point in doing any HSV conversions
	const ImGuiColorEditFlags flags_untouched = flags;
	if (flags & ImGuiColorEditFlags_NoInputs)
		flags = (flags & (~ImGuiColorEditFlags_DisplayMask_)) | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions;

	// Context menu: display and modify options (before defaults are applied)
	if (!(flags & ImGuiColorEditFlags_NoOptions))
		ImGui::ColorEditOptionsPopup(col, flags);

	// Read stored options
	if (!(flags & ImGuiColorEditFlags_DisplayMask_))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DisplayMask_);
	if (!(flags & ImGuiColorEditFlags_DataTypeMask_))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags_DataTypeMask_);
	if (!(flags & ImGuiColorEditFlags_PickerMask_))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags_PickerMask_);
	if (!(flags & ImGuiColorEditFlags_InputMask_))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags_InputMask_);
	flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_));
	IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_DisplayMask_)); // Check that only 1 is selected
	IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiColorEditFlags_InputMask_));   // Check that only 1 is selected

	const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
	const bool hdr = (flags & ImGuiColorEditFlags_HDR) != 0;
	const int components = alpha ? 4 : 3;

	// Convert to the formats we need
	float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
	if ((flags & ImGuiColorEditFlags_InputHSV) && (flags & ImGuiColorEditFlags_DisplayRGB))
		ImGui::ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
	else if ((flags & ImGuiColorEditFlags_InputRGB) && (flags & ImGuiColorEditFlags_DisplayHSV))
	{
		// Hue is lost when converting from grayscale rgb (saturation=0). Restore it.
		ImGui::ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
		ColorEditRestoreHS(col, &f[0], &f[1], &f[2]);
	}
	int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

	bool value_changed = false;
	bool value_changed_as_float = false;

	const ImVec2 pos = window->DC.CursorPos;
	const float inputs_offset_x = (style.ColorButtonPosition == ImGuiDir_Left) ? w_button : 0.0f;
	window->DC.CursorPos.x = pos.x + inputs_offset_x;

	if ((flags & (ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV)) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
	{
		// RGB/HSV 0..255 Sliders
		const float w_item_one = ImMax(1.0f, IM_FLOOR((w_inputs - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
		const float w_item_last = ImMax(1.0f, IM_FLOOR(w_inputs - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

		const bool hide_prefix = (w_item_one <= ImGui::CalcTextSize((flags & ImGuiColorEditFlags_Float) ? "M:0.000" : "M:000").x);
		static const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
		static const char* fmt_table_int[3][4] =
		{
			{   "%3d",   "%3d",   "%3d",   "%3d" }, // Short display
			{ "R:%3d", "G:%3d", "B:%3d", "A:%3d" }, // Long display for RGBA
			{ "H:%3d", "S:%3d", "V:%3d", "A:%3d" }  // Long display for HSVA
		};
		static const char* fmt_table_float[3][4] =
		{
			{   "%0.3f",   "%0.3f",   "%0.3f",   "%0.3f" }, // Short display
			{ "R:%0.3f", "G:%0.3f", "B:%0.3f", "A:%0.3f" }, // Long display for RGBA
			{ "H:%0.3f", "S:%0.3f", "V:%0.3f", "A:%0.3f" }  // Long display for HSVA
		};
		const int fmt_idx = hide_prefix ? 0 : (flags & ImGuiColorEditFlags_DisplayHSV) ? 2 : 1;

		for (int n = 0; n < components; n++)
		{
			if (n > 0)
				ImGui::SameLine(0, style.ItemInnerSpacing.x);
			ImGui::SetNextItemWidth((n + 1 < components) ? w_item_one : w_item_last);

			// FIXME: When ImGuiColorEditFlags_HDR flag is passed HS values snap in weird ways when SV values go below 0.
			if (flags & ImGuiColorEditFlags_Float)
			{
				value_changed |= ImGui::DragFloat(ids[n], &f[n], 1.0f / 255.0f, 0.0f, hdr ? 0.0f : 1.0f, fmt_table_float[fmt_idx][n]);
				value_changed_as_float |= value_changed;
			}
			else
			{
				value_changed |= ImGui::DragInt(ids[n], &i[n], 1.0f, 0, hdr ? 0 : 255, fmt_table_int[fmt_idx][n]);
			}
			if (!(flags & ImGuiColorEditFlags_NoOptions))
				ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
		}
	}
	else if ((flags & ImGuiColorEditFlags_DisplayHex) != 0 && (flags & ImGuiColorEditFlags_NoInputs) == 0)
	{
		// RGB Hexadecimal Input
		char buf[64];
		if (alpha)
			ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255), ImClamp(i[3], 0, 255));
		else
			ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));
		ImGui::SetNextItemWidth(w_inputs);
		if (ImGui::InputText("##Text", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
		{
			value_changed = true;
			char* p = buf;
			while (*p == '#' || ImCharIsBlankA(*p))
				p++;
			i[0] = i[1] = i[2] = 0;
			i[3] = 0xFF; // alpha default to 255 is not parsed by scanf (e.g. inputting #FFFFFF omitting alpha)
			int r;
			if (alpha)
				r = sscanf(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]); // Treat at unsigned (%X is unsigned)
			else
				r = sscanf(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
			IM_UNUSED(r); // Fixes C6031: Return value ignored: 'sscanf'.
		}
		if (!(flags & ImGuiColorEditFlags_NoOptions))
			ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
	}

	ImGuiWindow* picker_active_window = NULL;
	if (!(flags & ImGuiColorEditFlags_NoSmallPreview))
	{
		const float button_offset_x = ((flags & ImGuiColorEditFlags_NoInputs) || (style.ColorButtonPosition == ImGuiDir_Left)) ? 0.0f : w_inputs + style.ItemInnerSpacing.x;
		window->DC.CursorPos = ImVec2(pos.x + button_offset_x, pos.y);

		const ImVec4 col_v4(col[0], col[1], col[2], alpha ? col[3] : 1.0f);
		/*
		if (ColorButton("##ColorButton", col_v4, flags))
		{
			if (!(flags & ImGuiColorEditFlags_NoPicker))
			{
				// Store current color and open a picker
				g.ColorPickerRef = col_v4;
				OpenPopup("picker");
				SetNextWindowPos(g.LastItemData.Rect.GetBL() + ImVec2(0.0f, style.ItemSpacing.y));
			}
		}
		*/
		if (!(flags & ImGuiColorEditFlags_NoOptions))
			ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);

		//PushStyleColor(ImGuiCol_PopupBg, GetColorU32(c::child::background));
		//PushStyleColor(ImGuiCol_Border, GetColorU32(c::picker::outline));

		//PushStyleVar(ImGuiStyleVar_PopupRounding, c::picker::rounding);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));

		//if (BeginPopup("picker"))
		{
			if (g.CurrentWindow->BeginCount == 1)
			{
				picker_active_window = g.CurrentWindow;
				ImGui::GetWindowDrawList()->AddRectFilled(pos, pos + ImVec2(190.f, 175.f), ImColor(background));

				ImGui::Dummy(ImVec2(0, 5.f));
				ImGui::Dummy(ImVec2(8.f, 0.f));
				ImGui::SameLine();
				ImGui::BeginGroup();

				float wrapPos = 190.f - 15.f;
				//ImGui::PushTextWrapPos(wrapPos);
				ImGui::PushFont(SUBSTANCE::pFont15);
				ImGui::Text(label);
				ImGui::PopFont();
				ImGui::PushFont(SUBSTANCE::pFont10Light);
				//ImGui::TextWrapped("Description sample Description sample Description sample");
				ImGui::Text("Description sample");
				ImGui::PopFont();
				ImGui::EndGroup();

				ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags_DataTypeMask_ | ImGuiColorEditFlags_PickerMask_ | ImGuiColorEditFlags_InputMask_ | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
				ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | ImGuiColorEditFlags_DisplayMask_ | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;
				//SetNextItemWidth(square_sz * 12.0f); // Use 256 + bar sizes?
				//SetNextItemWidth(100.f); // Use 256 + bar sizes?
				value_changed |= SUBSTANCE::ColorPicker4("##picker", col, picker_flags, &g.ColorPickerRef.x);
			}
			//EndPopup();
		}
		//PopStyleColor(2);
		//PopStyleVar(2);
		ImGui::PopStyleVar();
	}

	/*
	if (label != label_display_end && !(flags & ImGuiColorEditFlags_NoLabel))
	{
		char buf[64];

		SameLine(0.0f, style.ItemInnerSpacing.x);
		window->DC.CursorPos.x = pos.x - w_button + ((flags & ImGuiColorEditFlags_NoInputs) ? w_button : w_full);

		ImGui::GetWindowDrawList()->AddText(window->DC.CursorPos + ImVec2(0, 0), GetColorU32(text_active), label);

		ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));
	}
	*/

	// Convert back
	if (value_changed && picker_active_window == NULL)
	{
		if (!value_changed_as_float)
			for (int n = 0; n < 4; n++)
				f[n] = i[n] / 255.0f;
		if ((flags & ImGuiColorEditFlags_DisplayHSV) && (flags & ImGuiColorEditFlags_InputRGB))
		{
			g.ColorEditSavedHue = f[0];
			g.ColorEditSavedSat = f[1];
			ImGui::ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
			g.ColorEditSavedID = g.ColorEditCurrentID;
			g.ColorEditSavedColor = ImGui::ColorConvertFloat4ToU32(ImVec4(f[0], f[1], f[2], 0));
		}
		if ((flags & ImGuiColorEditFlags_DisplayRGB) && (flags & ImGuiColorEditFlags_InputHSV))
			ImGui::ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

		col[0] = f[0];
		col[1] = f[1];
		col[2] = f[2];
		if (alpha)
			col[3] = f[3];
	}

	if (set_current_color_edit_id)
		g.ColorEditCurrentID = 0;
	ImGui::PopID();
	ImGui::EndGroup();

	// Drag and Drop Target
	// NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
	if ((g.LastItemData.StatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(flags & ImGuiColorEditFlags_NoDragDrop) && ImGui::BeginDragDropTarget())
	{
		bool accepted_drag_drop = false;
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
		{
			memcpy((float*)col, payload->Data, sizeof(float) * 3); // Preserve alpha if any //-V512 //-V1086
			value_changed = accepted_drag_drop = true;
		}
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
		{
			memcpy((float*)col, payload->Data, sizeof(float) * components);
			value_changed = accepted_drag_drop = true;
		}

		// Drag-drop payloads are always RGB
		if (accepted_drag_drop && (flags & ImGuiColorEditFlags_InputHSV))
			ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], col[0], col[1], col[2]);
		ImGui::EndDragDropTarget();
	}

	// When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
	if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
		g.LastItemData.ID = g.ActiveId;

	if (value_changed && g.LastItemData.ID != 0) // In case of ID collision, the second EndGroup() won't catch g.ActiveId
		ImGui::MarkItemEdited(g.LastItemData.ID);

	return value_changed;
}