#include "structs.hpp"
#include "../utilities/netvars.hpp"

int CEntity::GetFlags()
{
	return *reinterpret_cast<int*>((DWORD)this + 0x350);
}

size_t CEntity::GetIndex()
{
	void* networkable = this + 8;
	using fn = size_t(__thiscall*)(void*);
	return CallVFunction<fn>(networkable, 9)(networkable);
}

CWeapon* CEntity::GetActiveWeapon() {
	return (CWeapon*)(Interfaces::EntityList->GetEntityFromHandle(this->m_hActiveWeapon()));
}

const char* CEntity::GetClassName()
{
	void* networkable = this + 8;
	using fn = const char** (__thiscall*)(PVOID);
	return *(CallVFunction<fn>(networkable, 2)(networkable) + 2);
}

inline bool CEntity::IsNPC()
{
	return std::memcmp(this->GetClassName(), "npc", 3);
}

bool CEntity::IsPlayer() {
	typedef bool(__thiscall * fn)(void*);
	return CallVFunction<fn>(this, 130)(this);
}

bool CEntity::IsDormant() {
	typedef bool(__thiscall * fn)(void*);
	return CallVFunction<fn>(this, 48)(this);
}

bool CEntity::IsAlive()
{
	if (!this) return false;
	typedef bool(__thiscall * fn)(void*);
	return CallVFunction<fn>(this, 129)(this);
}

int CEntity::GetHealth()
{
	typedef int(__thiscall * fn)(void*);
	return CallVFunction<fn>(this, 108)(this);
}

int CEntity::GetMaxHealth()
{
	typedef int(__thiscall * fn)(void*);
	return CallVFunction<fn>(this, 109)(this);
}

Vector CEntity::GetViewOffset()
{
	return *reinterpret_cast<Vector*>((DWORD)this + 0xE8);
}

Vector CEntity::GetEyePos()
{
	return this->GetAbsOrigin() + this->GetViewOffset();
}

Vector& CEntity::GetAbsOrigin()
{
	typedef Vector& (__thiscall * OriginalFn)(void*);
	return CallVFunction<OriginalFn>(this, 9)(this);
}

bool CEntity::SetupBones(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
{
	void* pRenderable = (void*)(this + 0x4);
	typedef bool(__thiscall * OriginalFn)(PVOID, matrix3x4_t*, int, int, float);
	return (CallVFunction<OriginalFn>(pRenderable, 16))(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
}

void* CEntity::GetModel()
{
	void* pRenderable = (void*)(this + 0x4);
	typedef void* (__thiscall * OriginalFn)(PVOID);
	return CallVFunction<OriginalFn>(pRenderable, 9)(pRenderable);
}

int& CEntity::GetTickBase()
{
	return *(int*)((uintptr_t)this + 0x2580);
}

bool CWeapon::UsesLua() {
	return CallVFunction<bool(__thiscall*)(PVOID)>(this, 170)(this);
}

bool CWeapon::PushEntity() {
	return CallVFunction<bool(__thiscall*)(PVOID)>(this, 172)(this);
}

bool CWeapon::HasPrimaryAmmo() {
	return CallVFunction<bool(__thiscall*)(PVOID)>(this, 27)(this);
}

float CWeapon::LUASpread()
{
	if (!this->UsesLua()) // UsesLua
		return -1.0f;

	auto Lua = Interfaces::Lua->CreateInterface(LUA::type::client);
	this->PushEntity();
	Lua->GetField(-1, "Primary");
	if (!Lua->IsType(-1, 5))
	{
		Lua->Pop(2);
		return -1.0f;
	}
	Lua->GetField(-1, "Spread");
	if (!Lua->IsType(-1, 3))
	{
		Lua->Pop(3);
		return -1.0f;
	}
	double out = Lua->GetNumber(-1);
	Lua->Pop(3);
	return out;
}

const char* CWeapon::GetWeaponBase()
{
	auto Lua = Interfaces::Lua->CreateInterface(LUA::type::client);
	this->PushEntity();
	Lua->GetField(-1, "Base");
	if (!Lua->IsType(-1, 4))
	{
		Lua->Pop(2);
		return "";
	}
	const char* out = Lua->GetString(-1);
	Lua->Pop(2);
	return out;
}

float CWeapon::TTTSpread()
{
	if (!this->UsesLua())
		return -1.0f;

	auto Lua = Interfaces::Lua->CreateInterface(LUA::type::client);
	this->PushEntity();
	Lua->GetField(-1, "Primary");
	if (!Lua->IsType(-1, 5))
	{
		Lua->Pop(2);
		return -1.0f;
	}
	Lua->GetField(-1, "Cone");
	if (!Lua->IsType(-1, 3))
	{
		Lua->Pop(3);
		return -1.0f;
	}
	double out = Lua->GetNumber(-1);
	Lua->Pop(3);
	return out;
}

float CWeapon::LUASpread2()
{
	if (!this->UsesLua())
		return -1.0f;

	auto Lua = Interfaces::Lua->CreateInterface(LUA::type::client);
	this->PushEntity();
	Lua->GetField(-1, "Spread");
	if (!Lua->IsType(-1, 3))
	{
		Lua->Pop(2);
		return -1.0f;
	}
	double out = Lua->GetNumber(-1);
	Lua->Pop(2);
	return out;
}

const Vector& CWeapon::orgGetBulletSpread() {
	return CallVFunction<Vector & (__thiscall*)(PVOID)>(this, 66)(this);
}

Vector CWeapon::GetBulletSpread()
{
	if (this == nullptr)
		return Vector(0, 0, 0);

	if (CallVFunction<bool(__thiscall*)(PVOID)>(this, 170)(this)) // UsesLua
	{
		float LUASpreadCone = this->LUASpread();
		if (!strcmp(this->GetWeaponBase(), "weapon_tttbase"))
			LUASpreadCone = this->TTTSpread();

		if (LUASpreadCone != -1.0f)
			return Vector(LUASpreadCone, LUASpreadCone, LUASpreadCone);

		LUASpreadCone = this->LUASpread2();
		if (LUASpreadCone != -1.0f)
			return Vector(LUASpreadCone, LUASpreadCone, LUASpreadCone);
	}

	return this->orgGetBulletSpread();
}

const char* CWeapon::GetName() {
	return CallVFunction<const char* (__thiscall*)(PVOID)>(this, 366)(this);
}

bool CWeapon::IsExplosive()
{
	if (!this)
		return false;

	char const* name = this->GetName();
	if (!name)
		return false;
	/*
	weapon_physgun weapon_physcannon gmod_tool
	*/

	if (!strcmp(name, "weapon_rpg") || !strcmp(name, "weapon_frag") || !strcmp(name, "weapon_slam") || !strcmp(name, "weapon_bugbait"))
		return true;

	return false;
}

bool CWeapon::IsHoldingTool()
{
	if (!this)
		return false;

	char const* name = this->GetName();
	if (!name)
		return false;
	/*
		weapon_physgun weapon_physcannon gmod_tool
	*/

	if (!strcmp(name, "weapon_physgun") || !strcmp(name, "weapon_physcannon") || !strcmp(name, "gmod_tool")
		|| !strcmp(name, "gmod_tool") || !strcmp(name, "weapon_crowbar") || !strcmp(name, "weapon_stunstick"))
		return true;

	return false;
}

bool CWeapon::IsNospreadWeapon()
{
	char const* name = this->GetName();
	if (!name)
		return false;

	if (!strcmp(name, "weapon_357") || !strcmp(name, "weapon_crossbow"))
		return true;

	return false;
}

float CWeapon::m_flNextPrimaryAttack()
{
	if (this == nullptr)
		return 0.0f;
	return *reinterpret_cast<float*>((DWORD)this + 0x17CC);
}

bool CWeapon::CanFire()
{
	if (!this)
		return false;

	if (/*this->m_flNextPrimaryAttack() <= Interfaces::GlobalVars->curtime &&*/ this->HasPrimaryAmmo())
		return true;

	return false;
}