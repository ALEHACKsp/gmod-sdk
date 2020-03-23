#include "engineprediction.hpp"

void CPredictionSystem::Start(CUserCmd* cmd, CEntity* player)
{
	m_player = player;

	if (!m_pPredictionRandomSeed)
		m_pPredictionRandomSeed = *(int**)(Utilities::PatternScan("client.dll", "C7 05 ?? ?? ?? ?? ?? ?? ?? ?? 5D C3 8B 40 30") + 0x2);
	
	*m_pPredictionRandomSeed = MD5_PseudoRandom(cmd->command_nr) & 0x7FFFFFFF;

	m_flOldCurtime = Interfaces::GlobalVars->curtime;
	m_flOldFrametime = Interfaces::GlobalVars->frametime;


	Interfaces::GlobalVars->curtime = m_player->GetTickBase() * Interfaces::GlobalVars->interval_per_tick;
	Interfaces::GlobalVars->frametime = Interfaces::GlobalVars->interval_per_tick;

	Interfaces::GameMovement->StartTrackPredictionErrors(m_player);

	memset(&m_MoveData, 0, sizeof(m_MoveData));
	Interfaces::Prediction->SetupMove(m_player, cmd, Interfaces::MoveHelper, &m_MoveData);
	Interfaces::GameMovement->ProcessMovement(m_player, &m_MoveData);
	Interfaces::Prediction->FinishMove(m_player, cmd, &m_MoveData);

}
void CPredictionSystem::End()
{
	Interfaces::GameMovement->FinishTrackPredictionErrors(m_player);

	*m_pPredictionRandomSeed = -1;
	m_current_cmd = nullptr;

	Interfaces::GlobalVars->curtime = m_flOldCurtime;
	Interfaces::GlobalVars->frametime = m_flOldFrametime;
}

CPredictionSystem EnginePrediction;