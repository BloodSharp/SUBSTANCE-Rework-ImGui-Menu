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