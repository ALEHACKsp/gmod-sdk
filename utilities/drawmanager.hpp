#pragma once
#include "../includes.hpp"

namespace Draw
{
	extern __forceinline void Circle(const ImVec2& centre, float radius, const ImVec4& color, int num_segments, float thickness);
	extern __forceinline void Rect(const ImVec2& from, const ImVec2& to, const ImVec4& color, float rounding, uint32_t roundingCornersFlags, float thickness);
	extern __forceinline void FilledRect(const ImVec2& from, const ImVec2& to, const ImVec4& color, float rounding, uint32_t roundingCornersFlags);
	extern __forceinline void Line(const ImVec2& a, const ImVec2& b, const ImVec4& color, float thickness);
	extern __forceinline void Text(const ImVec2& from, const char* text, const ImVec4& color);
}