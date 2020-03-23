#include "sdk.hpp"
#include "createinterface.hpp"
#include "../utilities/utilities.hpp"

namespace Interfaces {
	LUA::Shared* Lua = nullptr;
	CEngineClient* EngineClient = nullptr;
	CEntityList* EntityList = nullptr;
	IDirect3DDevice9* D3DDevice = nullptr;
	CInput* Input = nullptr;
	IVModelInfo* ModelInfo = nullptr;
	IEngineTrace* EngineTrace = nullptr;
	CUniformRandomStream* Random = nullptr;
	CGlobalVarsBase* GlobalVars = nullptr;
	IVModelRender* ModelRender = nullptr;
	IMaterialSystem* MaterialSystem = nullptr;
	IVRenderView* RenderView = nullptr;
	IPrediction* Prediction = nullptr;
	IGameMovement* GameMovement = nullptr;
	IMoveHelper* MoveHelper = nullptr;
	IPanel* Panel = nullptr;
	void* CHLClient = nullptr;

	__forceinline void Init() {
		typedef void(*RandomSeed_t)(int);

		RandomSeed_t RandomSeed = reinterpret_cast<RandomSeed_t>(GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomSeed"));

		Lua = CreateInterface<LUA::Shared>("lua_shared.dll", "LUASHARED");
		EntityList = CreateInterface<CEntityList>("client.dll", "VClientEntityList");
		EngineClient = CreateInterface<CEngineClient>("engine.dll", "VEngineClient");
		Prediction = CreateInterface<IPrediction>("client.dll", "VClientPrediction001");
		ModelInfo = CreateInterface<IVModelInfo>("engine.dll", "VModelInfoClient006");
		EngineTrace = CreateInterface<IEngineTrace>("engine.dll", "EngineTraceClient003");
		ModelRender = CreateInterface<IVModelRender>("engine.dll", "VEngineModel016");
		MaterialSystem = CreateInterface<IMaterialSystem>("materialsystem.dll", "VMaterialSystem080");
		RenderView = CreateInterface<IVRenderView>("engine.dll", "VEngineRenderView014");
		GameMovement = CreateInterface<IGameMovement>("client.dll", "GameMovement001");
		Panel = CreateInterface<IPanel>("vgui2.dll", "VGUI_Panel009");
		CHLClient = CreateInterface<void>("client.dll", "VClient017");

		MoveHelper = **(IMoveHelper * **)(Utilities::PatternScan("client.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 0x2);
		Random = **(CUniformRandomStream * **)((uintptr_t)RandomSeed + 0x5);
		GlobalVars = **(CGlobalVarsBase * **)((*(uintptr_t * *)CHLClient)[0] + 0x55);
		D3DDevice = **(IDirect3DDevice9 * **)(Utilities::PatternScan("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
	}

	__forceinline void Print() {
		std::cout << "Interfaces::Lua: 0x" << std::hex << Interfaces::Lua << std::endl;
		std::cout << "Interfaces::EntityList: 0x" << std::hex << Interfaces::EntityList << std::endl;
		std::cout << "Interfaces::EngineClient: 0x" << std::hex << Interfaces::EngineClient << std::endl;
		std::cout << "Interfaces::CHLClient: 0x" << std::hex << Interfaces::CHLClient << std::endl;
		std::cout << "Interfaces::Prediction: 0x" << std::hex << Interfaces::Prediction << std::endl;
		std::cout << "Interfaces::ModelInfo: 0x" << std::hex << Interfaces::ModelInfo << std::endl;

		std::cout << "Interfaces::D3DDevice: 0x" << std::hex << Interfaces::D3DDevice << std::endl;
	}
}