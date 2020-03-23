#pragma once
#include "../vfunc.hpp"

struct tPlayerInfo
{
	char name[32];
	int  id;
	char guid[33];
	struct
	{
		unsigned int id;
		char name[32];
	} buddy;
	bool bot;
	bool hltv;
	unsigned int files[4];
	unsigned char downloaded;
};

class CEngineClient {
public:
	size_t GetLocalPlayer()
	{
		using fn = size_t(__thiscall*)(void*);
		return CallVFunction<fn>(this, 12)(this);
	}

	void GetViewAngles(Angle& Angles)
	{
		using fn = void(__thiscall*)(void*, Angle & Angles);
		return CallVFunction<fn>(this, 19)(this, Angles);
	}

	void SetViewAngles(Angle& Angles)
	{
		using fn = void(__thiscall*)(void*, Angle & Angles);
		return CallVFunction<fn>(this, 20)(this, Angles);
	}

	void GetScreenSize(int& width, int& height)
	{
		using fn = void(__thiscall*)(void*, int&, int&);
		return CallVFunction<fn>(this, 5)(this, width, height);
	}

	bool IsInGame()
	{
		using fn = bool(__thiscall*)(void*);
		return CallVFunction<fn>(this, 26)(this);
	}

	const D3DMATRIX& GetViewMatrix()
	{
		using fn = const D3DMATRIX& (__thiscall*)(void*);
		return CallVFunction<fn>(this, 36)(this);
	}
};