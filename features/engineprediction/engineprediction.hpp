#pragma once
#include "../../sdk/sdk.hpp"

class CUserCmd;

class CPredictionSystem {
public:
	void Start(CUserCmd* cmd, CEntity* player);
	void End();

private:
	float m_flOldCurtime;
	float m_flOldFrametime;

	CEntity* m_player;
	CMoveData m_MoveData = { };
	CUserCmd* m_current_cmd;
	int* m_pPredictionRandomSeed;
}; extern CPredictionSystem EnginePrediction;
