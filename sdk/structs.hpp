#pragma once
#include "vfunc.hpp"

#include "../utilities/utilities.hpp"
#include "../utilities/math.hpp"
#include "../utilities/netvars.hpp"

#define NETVAR(type, name, table, netvar) \
	type& name##() const { \
		static const auto _##name = Netvars::GetOffset(hash::fnv1a_32(table), hash::fnv1a_32(netvar)); \
		return *(type*)(uintptr_t(this) + _##name); \
	}

class CWeapon;

class CEntity {
public:
	int GetFlags();
	size_t GetIndex();

	NETVAR(uintptr_t, m_hActiveWeapon, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	NETVAR(uintptr_t, m_vecOrigin, "DT_BaseEntity", "m_vecOrigin");

	CWeapon* GetActiveWeapon();
	const char* GetClassName();
	bool IsNPC();
	bool IsPlayer();
	bool IsDormant();
	bool IsAlive();
	int GetHealth();
	int GetMaxHealth();
	Vector GetViewOffset();
	Vector GetEyePos();
	Vector& GetAbsOrigin();
	bool SetupBones(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
	void* GetModel();
	int& GetTickBase();
};

class CWeapon {
public:
	bool UsesLua();
	bool PushEntity();
	bool HasPrimaryAmmo();
	float LUASpread();
	const char* GetWeaponBase();
	float TTTSpread();
	float LUASpread2();
	const Vector& orgGetBulletSpread();
	Vector GetBulletSpread();
	const char* GetName();
	bool IsExplosive();
	bool IsHoldingTool();
	bool IsNospreadWeapon();
	float m_flNextPrimaryAttack();
	bool CanFire();
};
