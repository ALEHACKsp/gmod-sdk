#pragma once
#include "../vfunc.hpp"
class CEntity;

class CEntityList
{
public:
	CEntity* GetEntity(int idx)
	{
		using fn = CEntity*(__thiscall*)(void*, int);
		return CallVFunction<fn>(this, 3)(this, idx);
	}

	CEntity* GetEntityFromHandle(int idx)
	{
		using fn = CEntity* (__thiscall*)(void*, int);
		return CallVFunction<fn>(this, 4)(this, idx);
	}

	size_t GetHighestEntityIndex()
	{
		using fn = size_t(__thiscall*)(void*);
		return CallVFunction<fn>(this, 6)(this);
	}
};
