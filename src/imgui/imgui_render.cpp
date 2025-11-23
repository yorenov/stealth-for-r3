#include "imgui/imgui_render.h"

#include "Cheat/Utils.h"

CRender* pRender;

float CRender::DrawString(const ImFont* pFont, const std::string& Text, CVector vecPos, const float fSize, const ImColor &Color, const bool bCenter)
{
	float y = 0.0f;
	const ImVec2 TextSize = pFont->CalcTextSizeA(fSize, FLT_MAX, 0.0f, Text.c_str());

	if (bCenter)
		vecPos.x -= TextSize.x / 2.0f;

	ImGui::GetBackgroundDrawList()->AddText(pFont, fSize, ImVec2((vecPos.x) + 1, vecPos.y + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, Color.Value.w)), Text.c_str());
	ImGui::GetBackgroundDrawList()->AddText(pFont, fSize, ImVec2((vecPos.x) - 1, vecPos.y - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, Color.Value.w)), Text.c_str());
	ImGui::GetBackgroundDrawList()->AddText(pFont, fSize, ImVec2((vecPos.x) + 1, vecPos.y - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, Color.Value.w)), Text.c_str());
	ImGui::GetBackgroundDrawList()->AddText(pFont, fSize, ImVec2((vecPos.x) - 1, vecPos.y + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, Color.Value.w)), Text.c_str());
	ImGui::GetBackgroundDrawList()->AddText(pFont, fSize, ImVec2(vecPos.x, vecPos.y), Color, Text.c_str());
	
	y = vecPos.y + TextSize.y;

	return y;
}

void CRender::DrawLine(const CVector &vecFrom, const CVector &vecTo, const ImColor &Color, const float fThickness)
{
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vecFrom.x, vecFrom.y), ImVec2(vecTo.x, vecTo.y), Color, fThickness);
}

void CRender::D3DLine(const CVector& vecFrom, const CVector &vecTo, const ImColor &Color, const float fThickness)
{
	CVector vecFromScreen, vecToScreen;
	Utils::CalcScreenCoors(&vecFrom, &vecFromScreen);
	Utils::CalcScreenCoors(&vecTo, &vecToScreen);

	if (vecFromScreen.z < 1.f || vecToScreen.z < 1.f)
		return;

	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vecFromScreen.x, vecFromScreen.y), ImVec2(vecToScreen.x, vecToScreen.y), Color, fThickness);
}

void CRender::DrawCircle(const CVector &vecPos, const float fRadius, const ImColor &Color, const float fThickness)
{
	ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(vecPos.x, vecPos.y), fRadius * 1.5f, Color, 128, fThickness);
}

void CRender::DrawCircleFilled(const CVector &vecPos, const float fRadius, const ImColor &Color)
{
	ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(vecPos.x, vecPos.y), fRadius * 1.5f, Color, 128);
}

void CRender::DrawRect(const CVector &vecFrom, const CVector &vecTo, const ImColor &Color, const float fThickness)
{
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(vecFrom.x, vecFrom.y), ImVec2(vecTo.x, vecTo.y), Color, 1.f, 0, fThickness);
}

void CRender::DrawRectFilled(const CVector &vecFrom, const CVector &vecTo, const ImColor &Color)
{
	ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(vecFrom.x, vecFrom.y), ImVec2(vecTo.x, vecTo.y), Color);
}

void CRender::DrawBoundingBox(const CVector &vecPos, const CVector &vecMin, const CVector &vecMax, const float fRotation, const ImColor &Color, const float fThickness, const bool bFilled, const ImColor &Filled)
{
	CVector vecCornerScreen[8], vecCorner[8] =
	{
		CVector(vecMin.x, vecMin.y, vecMin.z),
		CVector(vecMin.x, vecMax.y, vecMin.z),
		CVector(vecMax.x, vecMax.y, vecMin.z),
		CVector(vecMax.x, vecMin.y, vecMin.z),
		CVector(vecMin.x, vecMin.y, vecMax.z),
		CVector(vecMin.x, vecMax.y, vecMax.z),
		CVector(vecMax.x, vecMax.y, vecMax.z),
		CVector(vecMax.x, vecMin.y, vecMax.z)
	};

	for (int i = 0; i < 8; i++)
	{
		const float fSin = sin(fRotation);
		const float fCos = cos(fRotation);
		const float x = vecCorner[i].x;
		const float y = vecCorner[i].y;
		vecCorner[i].x = vecPos.x + (x * fCos) - (y * fSin);
		vecCorner[i].y = vecPos.y + (x * fSin) + (y * fCos);
		vecCorner[i].z += vecPos.z;
		Utils::CalcScreenCoors(&vecCorner[i], &vecCornerScreen[i]);

		if (vecCornerScreen[i].z < 1.0f)
			return;
	}

	if (bFilled)
	{
		const auto colorESP = ImColor(Filled.Value.x, Filled.Value.y, Filled.Value.z, 0.2f);
		ImGui::GetBackgroundDrawList()->AddQuadFilled(ImVec2(vecCornerScreen[0].x, vecCornerScreen[0].y), ImVec2(vecCornerScreen[1].x, vecCornerScreen[1].y), ImVec2(vecCornerScreen[2].x, vecCornerScreen[2].y), ImVec2(vecCornerScreen[3].x, vecCornerScreen[3].y), colorESP);
		ImGui::GetBackgroundDrawList()->AddQuadFilled(ImVec2(vecCornerScreen[4].x, vecCornerScreen[4].y), ImVec2(vecCornerScreen[5].x, vecCornerScreen[5].y), ImVec2(vecCornerScreen[6].x, vecCornerScreen[6].y), ImVec2(vecCornerScreen[7].x, vecCornerScreen[7].y), colorESP);
		ImGui::GetBackgroundDrawList()->AddQuadFilled(ImVec2(vecCornerScreen[0].x, vecCornerScreen[0].y), ImVec2(vecCornerScreen[1].x, vecCornerScreen[1].y), ImVec2(vecCornerScreen[5].x, vecCornerScreen[5].y), ImVec2(vecCornerScreen[4].x, vecCornerScreen[4].y), colorESP);
		ImGui::GetBackgroundDrawList()->AddQuadFilled(ImVec2(vecCornerScreen[1].x, vecCornerScreen[1].y), ImVec2(vecCornerScreen[2].x, vecCornerScreen[2].y), ImVec2(vecCornerScreen[6].x, vecCornerScreen[6].y), ImVec2(vecCornerScreen[5].x, vecCornerScreen[5].y), colorESP);
		ImGui::GetBackgroundDrawList()->AddQuadFilled(ImVec2(vecCornerScreen[2].x, vecCornerScreen[2].y), ImVec2(vecCornerScreen[3].x, vecCornerScreen[3].y), ImVec2(vecCornerScreen[7].x, vecCornerScreen[7].y), ImVec2(vecCornerScreen[6].x, vecCornerScreen[6].y), colorESP);
		ImGui::GetBackgroundDrawList()->AddQuadFilled(ImVec2(vecCornerScreen[3].x, vecCornerScreen[3].y), ImVec2(vecCornerScreen[0].x, vecCornerScreen[0].y), ImVec2(vecCornerScreen[4].x, vecCornerScreen[4].y), ImVec2(vecCornerScreen[7].x, vecCornerScreen[7].y), colorESP);
	}

	for (int i = 1; i <= 4; i++)
	{
		DrawLine(vecCornerScreen[i - 1], vecCornerScreen[i % 4], Color, fThickness);
		DrawLine(vecCornerScreen[i - 1], vecCornerScreen[i + 3], Color, fThickness);
		DrawLine(vecCornerScreen[i + 3], vecCornerScreen[i % 4 + 4], Color, fThickness);
	}
}