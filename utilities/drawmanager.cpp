#include "drawmanager.hpp"

namespace Draw
{
	__forceinline void Circle(const ImVec2& centre, float radius, const ImVec4& color, int num_segments, float thickness) {
		ImGui::GetOverlayDrawList()->AddCircle(centre, radius, ImGui::GetColorU32(color), num_segments, thickness);
	}

	__forceinline void Rect(const ImVec2& from, const ImVec2& to, const ImVec4& color, float rounding, uint32_t roundingCornersFlags, float thickness)
	{
		ImGui::GetOverlayDrawList()->AddRect(from, to, ImGui::GetColorU32(color), rounding, roundingCornersFlags, thickness);
	}

	__forceinline void FilledRect(const ImVec2& from, const ImVec2& to, const ImVec4& color, float rounding, uint32_t roundingCornersFlags)
	{
		ImGui::GetOverlayDrawList()->AddRectFilled(from, to, ImGui::GetColorU32(color), rounding, roundingCornersFlags);
	}

	__forceinline void Line(const ImVec2& a, const ImVec2& b, const ImVec4& color, float thickness)
	{
		ImGui::GetOverlayDrawList()->AddLine(a, b, ImGui::GetColorU32(color), thickness);
	}

	__forceinline void Text(const ImVec2& from, const char* text, const ImVec4& color)
	{
		ImGui::GetOverlayDrawList()->AddText(from, ImGui::GetColorU32(color), text);
	}
}