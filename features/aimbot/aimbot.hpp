#pragma once
#include "../../sdk/sdk.hpp"

namespace Aimbot {
	extern void Antiam(CUserCmd* cmd);
	extern void Run(CUserCmd* cmd);
	extern Vector GetHitboxPosByMatrix(CEntity* entity, int Hitbox, matrix3x4_t* Matrix);
}
