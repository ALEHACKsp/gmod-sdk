#pragma once
#include "../includes.hpp"
#include "hash.hpp"

#define INRANGE(x,a,b)    (x >= a && x <= b)
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

class CEntity;
class Vector;

namespace Globals {
	extern bool MenuOpened;
	extern int ScreenWidth;
	extern int ScreenHeight;
	extern Vector LastRealAngles;
}

class CEntity;
class Vector;

namespace Utilities {
	extern __forceinline bool IsLoaded();
	extern __forceinline void AttachConsole();
	extern __forceinline bool IsKeyPressed(int iKey);
	extern bool IsVisible(CEntity* LocalPlayer, CEntity* player, Vector TraceEndPos);
	extern uint64_t PatternScan(const char* szModule, const char* szSignature);
	extern DWORD FindSignature(DWORD start_offset, DWORD size, BYTE* pattern, char mask[]);
}