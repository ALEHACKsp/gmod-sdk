#include "aimbot.hpp"
#include "../../options.hpp"

namespace Aimbot {
	void Antiam(CUserCmd* cmd) {
		if (!Vars.antiaim_enabled)
			return;

		if (cmd->buttons & IN_ATTACK)
			return;

		cmd->viewangles.x = 89;
		cmd->viewangles.y += 180;
	}

	void Run(CUserCmd* cmd) {
		if (!Vars.aim_enabled)
			return;

		const auto local_player = Interfaces::EntityList->GetEntity(Interfaces::EngineClient->GetLocalPlayer());

		float bestFov = 180.f;
		Angle bestAngle = Angle(0, 0, 0);
		CEntity* bestTarget = nullptr;
		bool a = false;

		for (size_t i = 0; i <= Interfaces::EntityList->GetHighestEntityIndex(); i++) {
			CEntity* entity = Interfaces::EntityList->GetEntity(i);
			if (!entity) continue;
			if (i == Interfaces::EngineClient->GetLocalPlayer()) continue;
			if (!entity->IsPlayer()) continue;
			if (!entity->IsAlive()) continue;

			matrix3x4_t matrix[128];
			entity->SetupBones(matrix, 128, 0x100, 0);
			Vector hitPos = Vector(0, 0, 0);

			for (int i = 0; i < 9; i++) {
				Vector pos = GetHitboxPosByMatrix(entity, 0, matrix);
				if (!Utilities::IsVisible(local_player, entity, pos))
					continue;
				hitPos = pos;
			}

			if (hitPos == Vector(0, 0, 0))
				return;

			Angle aimAng = Math::CalcAngle(local_player->GetEyePos(), hitPos);

			float fov = Math::GetFov(cmd->viewangles, aimAng);

			if (fov > Vars.aim_fov) continue;

			if (fov < bestFov) {
				bestFov = fov;
				bestTarget = entity;
				bestAngle = aimAng;
			}
		}
		if (bestTarget) {
			auto hWeapon = local_player->GetActiveWeapon();
			bool aimActive = !hWeapon->IsHoldingTool() && !hWeapon->IsExplosive() && hWeapon->CanFire();
			if (!aimActive) return;
			if (Vars.aim_autofire) cmd->buttons |= IN_ATTACK;
			if (cmd->buttons & IN_ATTACK) {
				cmd->viewangles = bestAngle;
				if (!Vars.aim_silent)
					Interfaces::EngineClient->SetViewAngles(cmd->viewangles);
			}
		}
	}

	Vector GetHitboxPosByMatrix(CEntity* entity, int Hitbox, matrix3x4_t* Matrix)
	{
		studiohdr_t* hdr = Interfaces::ModelInfo->GetStudioModel(entity->GetModel());
		mstudiohitboxset_t* set = hdr->pHitboxSet(0);
		mstudiobbox_t* hitbox = set->pHitbox(Hitbox);

		if (hitbox)
		{
			Vector vMin, vMax, vCenter, sCenter;
			Math::VectorTransform(hitbox->bbmin, Matrix[hitbox->bone], vMin);
			Math::VectorTransform(hitbox->bbmax, Matrix[hitbox->bone], vMax);
			vCenter = (vMin + vMax) * 0.5;

			return vCenter;
		}

		return Vector(0, 0, 0);
	}
}