#pragma once

#include "pch.h"

namespace SUBSTANCE
{
	extern HeaderBar HeaderBar;
	extern ImFont* pIconFont;
	extern ImFont* pFont15;
	extern ImFont* pFont12Light;
	extern ImFont* pFont10Light;

	// Secciones
	void LeftSection(void(*pItemRenderFunction)());
	void RightSection(void(*pItemRenderFunction)());
	void UniqueSection(void(*pItemRenderFunction)());

	bool ToggleButton(const char* szTextAndAlsoID, const char* szIcon, ImVec2 size, bool isActive);
	bool Function(const char* szTextAndAlsoID, const char* szDescription, bool* pBooleanValue, bool* pBooleanEnableItems);
	bool CheckBox(const char* szTextAndAlsoID, const char* szDescription, bool* pBooleanValue);
	bool ComboBox(const char* szTextAndAlsoID, const char* szDescription, int* pCurrentIntegerOption, const char* szOptions);
	bool SliderInt(const char* szTextAndAlsoID, const char* szDescription, int* pCurrentIntegerOption, int iMinimumValue, int iMaximumValue);
	bool SliderInt2(const char* szTextAndAlsoID, const char* szDescription, int* pCurrentIntegerOption, int iMinimumValue, int iMaximumValue);
	bool SliderFloat2(const char* szTextAndAlsoID, const char* szDescription, int* pCurrentIntegerOption, int iMinimumValue, int iMaximumValue);
	bool ColorPicker(const char* szTextAndAlsoID, const char* szDescription, ImVec4* pColor);
}