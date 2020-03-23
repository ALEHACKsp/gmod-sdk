#pragma once
#include "vfunc.hpp"
#include "../includes.hpp"
#include "math/vector.hpp"
#include "misc/checksum.hpp"
#include "interfaces/entitylist.hpp"
#include "interfaces/engineclient.hpp"
#include "interfaces/lua.hpp"
#include "interfaces/input.hpp"
#include "interfaces/modelinfo.hpp"
#include "interfaces/enginetrace.hpp"
#include "interfaces/globalvars.hpp"
#include "interfaces/materialsystem.hpp"
#include "interfaces/modelrender.hpp"
#include "interfaces/renderview.hpp"
#include "interfaces/prediction.hpp"
#include "interfaces/gamemovement.hpp"
#include "interfaces/panel.hpp"
#include "interfaces/random.hpp"
#include "structs.hpp"

namespace Interfaces {
	extern LUA::Shared* Lua;
	extern CEngineClient* EngineClient;
	extern CEntityList* EntityList;
	extern IDirect3DDevice9* D3DDevice;
	extern CInput* Input;
	extern IVModelInfo* ModelInfo;
	extern IEngineTrace* EngineTrace;
	extern CUniformRandomStream* Random;
	extern CGlobalVarsBase* GlobalVars;
	extern IVModelRender* ModelRender;
	extern IMaterialSystem* MaterialSystem;
	extern IVRenderView* RenderView;
	extern IPrediction* Prediction;
	extern IGameMovement* GameMovement;
	extern IMoveHelper* MoveHelper;
	extern IPanel* Panel;
	extern void* CHLClient;


	extern __forceinline void Init();
	extern __forceinline void Print();
}

enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};
