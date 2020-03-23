#include "utilities.hpp"
#include "../sdk/sdk.hpp"

namespace Globals {
	bool MenuOpened = false;
	int ScreenWidth = 0;
	int ScreenHeight = 0;
	Vector LastRealAngles;
}

namespace Utilities
{
	__forceinline bool IsLoaded() {
		return GetModuleHandleA("client.dll") && GetModuleHandleA("shaderapidx9.dll") 
			&& GetModuleHandleA("engine.dll");
	}
	__forceinline void AttachConsole() {
		AllocConsole();
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);
	}

	bool bKeyPress[256];
	bool IsKeyPressed(int iKey)
	{
		if (GetAsyncKeyState(iKey))
		{
			if (!bKeyPress[iKey])
			{
				bKeyPress[iKey] = true;
				return true;
			}
		}
		else
			bKeyPress[iKey] = false;
		return false;
	}

	bool IsVisible(CEntity* LocalPlayer, CEntity* player, Vector TraceEndPos)
	{
		if (!LocalPlayer)
			return false;

		trace_t Trace;
		CTraceFilter Filter;
		Filter.pSkip = LocalPlayer;
		Ray_t Ray;

		Ray.Init(LocalPlayer->GetEyePos(), TraceEndPos);
		Interfaces::EngineTrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

		if (Trace.m_pEnt == player || Trace.fraction >= 0.98f) // thats basically visible.
			return true;

		return false;
	}

	uint64_t PatternScan(const char* szModule, const char* szSignature)
	{
		MODULEINFO modInfo;
		GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
		DWORD startAddress = (DWORD)modInfo.lpBaseOfDll;
		DWORD endAddress = startAddress + modInfo.SizeOfImage;
		const char* pat = szSignature;
		DWORD firstMatch = 0;
		for (DWORD pCur = startAddress; pCur < endAddress; pCur++) {
			if (!*pat) return firstMatch;
			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
				if (!firstMatch) firstMatch = pCur;
				if (!pat[2]) return firstMatch;
				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
				else pat += 2;    //one ?
			}
			else {
				pat = szSignature;
				firstMatch = 0;
			}
		}
		return NULL;
	}

	DWORD FindSignature(DWORD start_offset, DWORD size, BYTE* pattern, char mask[])
	{
		DWORD pos = 0;
		int searchLen = strlen(mask) - 1;

		for (DWORD retAddress = start_offset; retAddress < start_offset + size; retAddress++)
		{
			if (*(BYTE*)retAddress == pattern[pos] || mask[pos] == '?') {
				if (mask[pos + 1] == '\0')
					return (retAddress - searchLen);
				pos++;
			}
			else
				pos = 0;
		}
		return NULL;
	}
}