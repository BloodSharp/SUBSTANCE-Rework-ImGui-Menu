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
	bool SliderFloat(const char* szTextAndAlsoID, const char* szDescription, float* pCurrentFloatOption, float fMinimumValue, float fMaximumValue);
	bool SliderFloat2(const char* szTextAndAlsoID, const char* szDescription, float* pCurrentFloatOption, float fMinimumValue, float fMaximumValue);
	bool ColorPicker(const char* szTextAndAlsoID, const char* szDescription, ImVec4* pColor);
	bool QuandFloat(const char* label, const char* description, float* v0, float* v1, float v_min0 = 0.0f, float v_max0 = 0.0f, float v_min1 = 0.0f, float v_max1 = 0.0f, const char* format0 = "%.2f", const char* format1 = "%.2f", ImGuiSliderFlags flags = 0);
	bool QuandInt(const char* label, const char* description, int* v0, int* v1, int v_min0 = 0, int v_max0 = 0, int v_min1 = 0, int v_max1 = 0, const char* format0 = "%d", const char* format1 = "%d", ImGuiSliderFlags flags = 0);
	bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags = 0, const float* ref_col = 0);
	bool ColorEdit4(const char* label, const char* szDescription, float col[4], ImGuiColorEditFlags flags = 0); // display a color square/button, hover for details, return true when pressed.
}