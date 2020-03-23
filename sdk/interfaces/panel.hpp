#pragma once
#include "../vfunc.hpp"

class IPanel {
public:
	const char* GetName(int iIndex)
	{
		typedef const char* (__thiscall * fn)(PVOID, int);
		return CallVFunction<fn>(this, 36)(this, iIndex);
	}
	void SetMouseInputEnabled(unsigned int vguiPanel, bool state)
	{
		typedef void(__thiscall * fn)(PVOID, unsigned int, bool);
		return CallVFunction<fn>(this, 32)(this, vguiPanel, state);
	}
};