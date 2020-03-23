#pragma once
#include "../../includes.hpp"

namespace Gui {
	extern __forceinline void Run();
	extern __forceinline void HandleAlpla(double val);
	extern __forceinline bool ImguiCustomButton(const char* label, bool SelectedSubTab, const ImVec2& size_arg);

	extern __forceinline void SetupStyles(ImGuiStyle& style);
	extern __forceinline void SetupColors(ImGuiStyle& style);

	extern int iTabSelected;
	extern ImFont* pTabsFont;
	extern float flMainFrameAlpha;
}

class CTimer {
public:
	void Run() {
		QueryPerformanceCounter(&OldTime);
		QueryPerformanceFrequency(&Freq);
	}
	double GetDelta() {
		LARGE_INTEGER CurrentTime;
		QueryPerformanceCounter(&CurrentTime);
		double delta = (double)(CurrentTime.QuadPart - OldTime.QuadPart) / (double)Freq.QuadPart;
		OldTime = CurrentTime;
		return delta;
	}
private:
	LARGE_INTEGER OldTime, Freq;
};
