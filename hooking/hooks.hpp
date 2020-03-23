#pragma once
#include "../sdk/sdk.hpp"
#include "vmt.hpp"

typedef void(__thiscall* tCreateMove)(void*, int, float, bool);
typedef void(__thiscall* tDrawModelExecute)(IVModelRender*, DrawModelState_t& state, ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
typedef void(__thiscall* tFrameStageNotify)(void*, ClientFrameStage_t);
typedef void(__thiscall* tPaintTraverse)(void*, unsigned int, bool, bool);

namespace Hooks {
	extern __forceinline void Init();

	void __fastcall hkPaintTraverse(IPanel* p_panel, void*, unsigned int panel, bool force_repaint, bool allow_force);
	void __fastcall hkFrameStageNotify(void* p_client, void*, ClientFrameStage_t stage);
	void __fastcall hkDrawModelExecute(IVModelRender* p_modelrender, void*, DrawModelState_t& state, ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
	void __fastcall hkRunCommand(IPrediction* p_prediction, void*, CEntity* entity, CUserCmd* cmd, IMoveHelper* move_helper);
	void __fastcall hkCreateMove(void* p_client, void*, int sequence_number, float input_sample_frametime, bool active);
	long __stdcall hkEndScene(IDirect3DDevice9* device);
}

extern IMGUI_API LRESULT   ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);