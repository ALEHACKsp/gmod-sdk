#include "misc.hpp"

namespace Miscellaneous {
	__forceinline void BunnyHop(CUserCmd* cmd) {
		const auto local_player = Interfaces::EntityList->GetEntity(Interfaces::EngineClient->GetLocalPlayer());

		if (Vars.misc_bhop && cmd->buttons & IN_JUMP && !(local_player->GetFlags() & FL_ONGROUND))
		{
			cmd->buttons &= ~IN_JUMP;
		}
	}

	void Thirdperson(ClientFrameStage_t stage)
	{
		const auto local_player = Interfaces::EntityList->GetEntity(Interfaces::EngineClient->GetLocalPlayer());

		static bool toggle = false;
		if (!local_player) return;

		if (Utilities::IsKeyPressed(VK_MENU))
			toggle = !toggle;

		if (local_player->IsAlive() && toggle)
		{
			if (!Interfaces::Input->m_fCameraInThirdPerson)
				Interfaces::Input->m_fCameraInThirdPerson = true;
		}
		else
			Interfaces::Input->m_fCameraInThirdPerson = false;

		if (stage == FRAME_RENDER_START && Interfaces::EngineClient->IsInGame() && local_player)
		{
			if (local_player->IsAlive() && Interfaces::Input->m_fCameraInThirdPerson) {
				Interfaces::Prediction->SetLocalViewAngles(Angle(89, 180, 0));
			}
		}
	}
}