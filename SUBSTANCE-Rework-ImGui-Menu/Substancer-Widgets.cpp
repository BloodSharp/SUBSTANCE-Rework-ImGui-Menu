#include "pch.h"

ImFont* SUBSTANCE::pFont15 = 0;
ImFont* SUBSTANCE::pFont10Light = 0;

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

bool SUBSTANCE::ComboBox(const char* szTextAndAlsoID, const char* szDescription, int* pCurrentIntegerOption, const char* szOptions)
{
	bool bChangedValue = false;

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

	// T�tulo
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY));
	ImGui::PushFont(SUBSTANCE::pFont15);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f), bb.Max - style.FramePadding, szTextAndAlsoID, 0, &titleSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	// Descripci�n
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY + descriptionTopOffset));
	ImGui::PushFont(SUBSTANCE::pFont10Light);
	ImGui::TextColored(descColor, szDescription);
	ImGui::PopFont();

	ImGui::PopTextWrapPos();

	// Posici�n del ComboBox
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

	// T�tulo
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY));
	ImGui::PushFont(SUBSTANCE::pFont15);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f), bb.Max - style.FramePadding, szTextAndAlsoID, 0, &titleSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	// Descripci�n
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY + descriptionTopOffset));
	ImGui::PushFont(SUBSTANCE::pFont10Light);
	ImGui::TextColored(descColor, szDescription);
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
	float bottomY = localMax.y - MarginY - onOffSize.y;
	float bottomX = localMin.x + MarginX;
	//drawList->AddText(ImVec2(bottomX, bottomY), ImColor(iconBgColor), onOffText);
	ImGui::SetCursorPos(ImVec2(bottomX, bottomY));
	ImGui::TextColored(ImColor(iconBgColor), onOffText);

	// Dibujar �cono si es visible
	//if (IconButtonVisible && CheckedChanged) // asumiendo que la acci�n recae en el click
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

	ImVec4 titleColor = whiteColor;
	ImVec4 descColor = almostWhiteColor;
	ImVec4 iconBgColor = *pBooleanValue ? greenColor : redColor;

	const ImVec2 titleSize = ImGui::CalcTextSize(szTextAndAlsoID, 0, true);
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

	// T�tulo
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY));
	ImGui::PushFont(SUBSTANCE::pFont15);
	ImGui::PushStyleColor(ImGuiCol_Text, titleColor);
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x + 10.0f, bb.Min.y + style.FramePadding.y + 7.0f), bb.Max - style.FramePadding, szTextAndAlsoID, 0, &titleSize);
	ImGui::PopStyleColor();
	ImGui::PopFont();

	// Descripci�n
	ImGui::SetCursorPos(ImVec2(localMin.x + MarginX, localMin.y + MarginY + descriptionTopOffset));
	ImGui::PushFont(SUBSTANCE::pFont10Light);
	ImGui::TextColored(descColor, szDescription);
	ImGui::PopFont();

	ImGui::PopTextWrapPos();

	// Borde
	if (*pBooleanValue)
	{
		DrawStaticBorder(drawList, widgetMin, widgetMax, BorderPercent, ImColor(whiteColor), LineThickness);
	}

	// �rea del bot�n �cono
	ImVec2 iconBoxMin = ImVec2(widgetMax.x - MarginX - IconButtonSize, widgetMax.y - MarginY - IconButtonSize);
	ImVec2 iconBoxMax = ImVec2(iconBoxMin.x + IconButtonSize, iconBoxMin.y + IconButtonSize);

	// Dibujar �cono si es visible
	//if (IconButtonVisible && CheckedChanged) // asumiendo que la acci�n recae en el click
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

/// <summary>
/// Widget personalizado ToggleButton con imagen, texto y estilo activo/inactivo.
/// </summary>
/// <param name="id">Identificador �nico del bot�n.</param>
/// <param name="imageTextureID">La textura de la imagen del �cono.</param>
/// <param name="label">El texto del bot�n.</param>
/// <param name="size">Tama�o del bot�n.</param>
/// <param name="isActive">Referencia al estado del bot�n (ON/OFF).</param>
bool ToggleButton(const char* szTextAndAlsoID, const char* szIcon, ImVec2 size, bool* isActive)
{
	bool clicked = false;

	// Colores de los estilos
	ImVec4 inactiveBgColor = ImVec4(0.153f, 0.153f, 0.200f, 1.000f); // Gris oscuro
	ImVec4 activeBgColor = ImVec4(0.439f, 0.698f, 0.675f, 1.000f);   // Celeste
	ImVec4 inactiveTextColor = ImVec4(1, 1, 1, 1.0f);        // Blanco
	ImVec4 activeTextColor = ImVec4(1, 1, 1, 1.0f);          // Negro
	ImVec4 decorationColor = ImVec4(0.153f, 0.153f, 0.200f, 1.000f); // Color oscuro para decoraci�n
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

	// Coordenadas del bot�n
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

	// Fondo del bot�n
	drawList->AddRectFilled(buttonMin, buttonMax, ImGui::ColorConvertFloat4ToU32(*isActive ? activeBgColor : inactiveBgColor));

	// Decoraci�n si el bot�n est� activo
	if (*isActive)
	{
		// 1. Rect�ngulo como barra decorativa
		float rectHeight = (buttonMax.y - buttonMin.y) * 0.6f; // Altura del rect�ngulo
		float rectWidth = 5.0f; // Ancho peque�o para parecer una barra
		ImVec2 rectBottomRight = ImVec2(buttonMax.x, buttonMax.y);
		ImVec2 rectTopLeft = ImVec2(buttonMax.x - rectWidth, buttonMax.y - rectHeight);

		// Dibujar la barra
		drawList->AddRectFilled(rectTopLeft, rectBottomRight, ImGui::ColorConvertFloat4ToU32(decorationColor));

		// 2. Tri�ngulo ligeramente m�s ancho que la barra
		ImVec2 trianglePoint1 = ImVec2(rectTopLeft.x + 1, rectTopLeft.y);                   // Esquina superior izquierda del rect�ngulo
		ImVec2 trianglePoint2 = ImVec2(rectTopLeft.x + 1 + rectWidth, rectTopLeft.y); // Extremo derecho del tri�ngulo
		ImVec2 trianglePoint3 = ImVec2(rectTopLeft.x + 1, rectTopLeft.y - 5);             // Punto inclinado hacia arriba

		drawList->AddTriangleFilled(trianglePoint1, trianglePoint2, trianglePoint3, ImGui::ColorConvertFloat4ToU32(decorationColor));

		// --- Decoraci�n Inferior Derecha ---

		float bottomRectWidth = 28.0f; // Ancho de la barra horizontal
		float bottomRectHeight = 5.0f; // Altura de la barra
		ImVec2 bottomRectTopLeft = ImVec2(buttonMax.x - bottomRectWidth, buttonMax.y - bottomRectHeight);
		ImVec2 bottomRectBottomRight = ImVec2(buttonMax.x, buttonMax.y);

		// Dibujar la barra inferior
		drawList->AddRectFilled(bottomRectTopLeft, bottomRectBottomRight, ImGui::ColorConvertFloat4ToU32(decorationColor));

		// Tri�ngulo inferior derecho
		ImVec2 bottomTrianglePoint1 = ImVec2(bottomRectTopLeft.x, bottomRectTopLeft.y + 1);         // Esquina superior izquierda de la barra
		ImVec2 bottomTrianglePoint2 = ImVec2(bottomRectTopLeft.x - 5, bottomRectTopLeft.y + 1);  // Unos p�xeles a la izquierda, misma altura
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
		ImVec2 bottomTrianglePoint2b = ImVec2((bottomRectTopLeft.x - 5) - separation, bottomRectTopLeft.y + 1);  // Unos p�xeles a la izquierda, misma altura
		ImVec2 bottomTrianglePoint3b = ImVec2(bottomRectTopLeft.x - separation, bottomRectBottomRight.y + 1);     // Esquina inferior izquierda de la barra

		drawList->AddTriangleFilled(bottomTrianglePoint1b, bottomTrianglePoint2b, bottomTrianglePoint3b, ImGui::ColorConvertFloat4ToU32(decorationColor));

		// Tri�ngulo en el lado derecho con la hipotenusa apuntando hacia arriba
		ImVec2 bottomTriangleRightPoint1b = ImVec2(bottomRectBottomRightb.x, bottomRectBottomRightb.y + 1);       // Punto inferior derecho del rect�ngulo
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
		bottomTrianglePoint2b = ImVec2((bottomRectTopLeft.x - 5) - separation, bottomRectTopLeft.y + 1);  // Unos p�xeles a la izquierda, misma altura
		bottomTrianglePoint3b = ImVec2(bottomRectTopLeft.x - separation, bottomRectBottomRight.y + 1);     // Esquina inferior izquierda de la barra

		drawList->AddTriangleFilled(bottomTrianglePoint1b, bottomTrianglePoint2b, bottomTrianglePoint3b, ImGui::ColorConvertFloat4ToU32(decorationColor));

		// Tri�ngulo en el lado derecho con la hipotenusa apuntando hacia arriba
		bottomTriangleRightPoint1b = ImVec2(bottomRectBottomRightb.x, bottomRectBottomRightb.y + 1);       // Punto inferior derecho del rect�ngulo
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
		bottomTrianglePoint2b = ImVec2((bottomRectTopLeft.x - 5) - separation, bottomRectTopLeft.y + 1);  // Unos p�xeles a la izquierda, misma altura
		bottomTrianglePoint3b = ImVec2(bottomRectTopLeft.x - separation, bottomRectBottomRight.y + 1);     // Esquina inferior izquierda de la barra

		drawList->AddTriangleFilled(bottomTrianglePoint1b, bottomTrianglePoint2b, bottomTrianglePoint3b, ImGui::ColorConvertFloat4ToU32(decorationColor));

		// Tri�ngulo en el lado derecho con la hipotenusa apuntando hacia arriba
		bottomTriangleRightPoint1b = ImVec2(bottomRectBottomRightb.x, bottomRectBottomRightb.y + 1);       // Punto inferior derecho del rect�ngulo
		bottomTriangleRightPoint2b = ImVec2(bottomRectBottomRightb.x + 5, bottomRectBottomRightb.y + 1);   // Extiende 5px hacia la derecha, misma altura inferior
		bottomTriangleRightPoint3b = ImVec2(bottomRectBottomRightb.x, bottomRectTopLeftb.y + 1);           // Punto superior, creando la punta hacia arriba

		drawList->AddTriangleFilled(bottomTriangleRightPoint1b, bottomTriangleRightPoint2b, bottomTriangleRightPoint3b, ImGui::ColorConvertFloat4ToU32(decorationColor));



		float bottomRectWidthc = 89.0f; // Ancho de la barra horizontal
		float bottomRectHeightc = 5.0f; // Altura de la barra
		ImVec2 bottomRectTopLeftc = ImVec2((buttonMax.x - bottomRectWidthc) - 7, (buttonMax.y - bottomRectHeightc) - 7);
		ImVec2 bottomRectBottomRightc = ImVec2(buttonMax.x - 7, buttonMax.y - 7);

		// Dibujar la barra inferior
		drawList->AddRectFilled(bottomRectTopLeftc, bottomRectBottomRightc, ImGui::ColorConvertFloat4ToU32(decorationColor2));

		// Tri�ngulo inferior derecho
		ImVec2 bottomTrianglePoint1c = ImVec2(bottomRectTopLeftc.x - 5, bottomRectTopLeftc.y);         // Esquina superior izquierda de la barra
		ImVec2 bottomTrianglePoint2c = ImVec2(bottomRectTopLeftc.x, bottomRectTopLeftc.y);  // Unos p�xeles a la izquierda, misma altura
		ImVec2 bottomTrianglePoint3c = ImVec2(bottomRectTopLeftc.x, bottomRectBottomRightc.y);     // Esquina inferior izquierda de la barra

		drawList->AddTriangleFilled(bottomTrianglePoint1c, bottomTrianglePoint2c, bottomTrianglePoint3c, ImGui::ColorConvertFloat4ToU32(decorationColor2));

	}

	// Imagen del bot�n (�cono)
	float imageHeight = size.y * 0.4f;
	ImVec2 imageSize = ImVec2(imageHeight, imageHeight);
	ImVec2 imagePos = ImVec2(buttonMin.x + 10, buttonCenter.y - (imageSize.y / 2));
	drawList->AddImage(imageTextureID, imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y));

	// Texto centrado verticalmente, a la derecha del �cono
	ImVec2 textSize = ImGui::CalcTextSize(label);
	ImVec2 textPos = ImVec2(imagePos.x + imageSize.x + 10, buttonCenter.y - (textSize.y / 2));
	drawList->AddText(textPos, ImGui::ColorConvertFloat4ToU32(*isActive ? activeTextColor : inactiveTextColor), label);

	return clicked;
}