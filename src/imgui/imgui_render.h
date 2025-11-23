#pragma once
#include <CVector.h>
#include <imgui.h>
#include <string>

class CRender
{
public:
	CRender()
	{
		pESPFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(R"(C:\Windows\Fonts\arialbd.ttf)", 20.f);
	};

	ImFont* pESPFont;

	static float DrawString(const ImFont* pFont, const std::string& Text, CVector vecPos, float fSize, const ImColor &Color, bool bCenter);

	static void DrawLine(const CVector &vecFrom, const CVector &vecTo, const ImColor &Color, float fThickness);

	static void D3DLine(const CVector& vecFrom, const CVector &vecTo, const ImColor &Color, float fThickness);

	static void DrawCircle(const CVector &vecPos, float fRadius, const ImColor &Color, float fThickness);

	static void DrawCircleFilled(const CVector &vecPos, float fRadius, const ImColor &Color);

	static void DrawRect(const CVector &vecFrom, const CVector &vecTo, const ImColor &Color, float fThickness);

	static void DrawRectFilled(const CVector &vecFrom, const CVector &vecTo, const ImColor &Color);

	static void DrawBoundingBox(const CVector &vecPos, const CVector &vecMin, const CVector &vecMax, float fRotation, const ImColor &Color, float fThickness, bool bFilled = false, const ImColor &Filled = ImColor(0, 0, 0, 0));
};

extern CRender* pRender;
