#include "hooks.hpp"
#include "../utilities/fonts.hpp"
#include "../utilities/utilities.hpp"
#include "../features/gui/menu.hpp"
#include "../features/visuals/visuals.hpp"
#include "../features/aimbot/aimbot.hpp"
#include "../features/miscellaneous/misc.hpp"
#include "../features/engineprediction/engineprediction.hpp"
#include "../options.hpp"

vfunc_hook d3d_hook;
vfunc_hook client_hook;
vfunc_hook mdlrender_hook;
vfunc_hook prediction_hook;
vfunc_hook panel_hook;

WNDPROC oWndProc = nullptr;
tCreateMove oCreateMove;
CTimer g_Timer;

__forceinline void Hooks::Init() {
	if (d3d_hook.setup(Interfaces::D3DDevice))
		d3d_hook.hook_index(42, hkEndScene);	

	if (mdlrender_hook.setup(Interfaces::ModelRender))
		mdlrender_hook.hook_index(20, hkDrawModelExecute);

	if (prediction_hook.setup(Interfaces::Prediction))
		prediction_hook.hook_index(19, hkRunCommand);

	if (panel_hook.setup(Interfaces::Panel))
		panel_hook.hook_index(41, hkPaintTraverse);

	if (client_hook.setup(Interfaces::CHLClient)) {
		oCreateMove = client_hook.get_original<tCreateMove>(21);
		DWORD dw_input_pointer = Utilities::FindSignature((DWORD)oCreateMove, 0x100, (byte*)"\x8B\x0D", "xx");
		if (dw_input_pointer != NULL)
		{
			dw_input_pointer += 0x2;
			Interfaces::Input = **(CInput * **)dw_input_pointer;
		}
		client_hook.hook_index(21, hkCreateMove);
		client_hook.hook_index(35, hkFrameStageNotify);
	}
}

LRESULT __stdcall hkWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplDX9_WndProcHandler(hwnd, msg, wParam, lParam);
	return CallWindowProcW(oWndProc, hwnd, msg, wParam, lParam);
}

void __fastcall Hooks::hkPaintTraverse(IPanel* p_panel, void*, unsigned int panel, bool force_repaint, bool allow_force)
{
	static auto oPaintTraverse = panel_hook.get_original<tPaintTraverse>(41);
	oPaintTraverse(p_panel, panel, force_repaint, allow_force);

	static unsigned int OverlayPopupPanel;
	if (!OverlayPopupPanel)
	{
		const char* szName = Interfaces::Panel->GetName(panel);
		printf("Panel name: %s\n", szName);
		if (szName[0] == 'O' && szName[7] == 'P')
			OverlayPopupPanel = panel;	
	}

	if (OverlayPopupPanel == panel)
		Interfaces::Panel->SetMouseInputEnabled(panel, Globals::MenuOpened);
}

void __fastcall Hooks::hkFrameStageNotify(void* p_client, void*, ClientFrameStage_t stage)
{
	static auto oFrameStageNotify = client_hook.get_original<tFrameStageNotify>(35);
	const auto local_player = Interfaces::EntityList->GetEntity(Interfaces::EngineClient->GetLocalPlayer());

	if (Interfaces::EngineClient->IsInGame() && local_player->IsAlive() && Interfaces::Input->m_fCameraInThirdPerson)
		Interfaces::Prediction->SetLocalViewAngles(Globals::LastRealAngles);
	
	oFrameStageNotify(p_client, stage);
}

void __fastcall Hooks::hkDrawModelExecute(IVModelRender* p_modelrender, void* edx, DrawModelState_t& state, ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
{
	static auto oDrawModelExecute = mdlrender_hook.get_original<tDrawModelExecute>(20);

	IMaterial* material_norm = nullptr;
	if (!material_norm) {
		material_norm = Interfaces::MaterialSystem->FindMaterial("debug/debugambientcube", "Model textures");
		material_norm->IncrementReferenceCount();
	}

	auto forceMaterial = [](bool ignorez, float* clr, IMaterial* mat) {
		auto material = mat;
		Interfaces::RenderView->SetColorModulation(clr);
		Interfaces::RenderView->SetBlend(clr[3]);
		material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, ignorez);
		Interfaces::ModelRender->ForcedMaterialOverride(material);
	};

	CEntity* player = Interfaces::EntityList->GetEntity(pInfo.entity_index);

	auto chamsMat = material_norm;
	float vis_clr[4] = { 1.f, 1.f, 0.f, 1.f};
	float hidden_clr[4] = { 1.f, 0.f, 0.f, 1.f };

	if (Vars.chams_enabled && player && material_norm && player->IsAlive() && player->IsPlayer()
		&& !(Vars.esp_ignore_local && pInfo.entity_index == Interfaces::EngineClient->GetLocalPlayer()))
	{
		if (!Vars.esp_visible_only) {
			forceMaterial(true, hidden_clr, material_norm);
			oDrawModelExecute(p_modelrender, state, pInfo, pCustomBoneToWorld);
		}
		forceMaterial(false, vis_clr, chamsMat);
	}

	oDrawModelExecute(p_modelrender, state, pInfo, pCustomBoneToWorld);
	Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
}

void __fastcall Hooks::hkRunCommand(IPrediction* p_prediction, void*, CEntity* entity, CUserCmd* cmd, IMoveHelper* move_helper)
{
	static auto oRunCommand = prediction_hook.get_original<void(__thiscall*)(void*, CEntity*, CUserCmd*, void*)>(19);
	oRunCommand(p_prediction, entity, cmd, move_helper);

	Interfaces::MoveHelper = move_helper;
}

void __fastcall Hooks::hkCreateMove(void* p_client, void*, int sequence_number, float input_sample_frametime, bool active)
{
	auto FixMovement = [](CUserCmd* cmd, CUserCmd old_cmd)
	{
		float f1, f2, yaw_delta = cmd->viewangles.y - old_cmd.viewangles.y;
		if (old_cmd.viewangles.y < 0.f)
			f1 = 360.0f + old_cmd.viewangles.y;
		else
			f1 = old_cmd.viewangles.y;

		if (cmd->viewangles.y < 0.0f)
			f2 = 360.0f + cmd->viewangles.y;
		else
			f2 = cmd->viewangles.y;

		if (f2 < f1)
			yaw_delta = abs(f2 - f1);
		else
			yaw_delta = 360.0f - abs(f1 - f2);

		yaw_delta = 360.0f - yaw_delta;

		cmd->forwardmove = cos(DEG2RAD(yaw_delta)) * old_cmd.forwardmove + cos(DEG2RAD(yaw_delta + 90.f)) * old_cmd.sidemove;
		cmd->sidemove = sin(DEG2RAD(yaw_delta)) * old_cmd.forwardmove + sin(DEG2RAD(yaw_delta + 90.f)) * old_cmd.sidemove;

		cmd->buttons &= ~IN_MOVERIGHT;
		cmd->buttons &= ~IN_MOVELEFT;
		cmd->buttons &= ~IN_FORWARD;
		cmd->buttons &= ~IN_BACK;
	};

	oCreateMove(p_client, sequence_number, input_sample_frametime, active);

	if (!Interfaces::Input)
		return;

	CUserCmd* cmd = Interfaces::Input->cmds + (sequence_number % 90);
	if (!cmd)
		return;

	CVerifiedUserCmd* verified_cmd = Interfaces::Input->verified_cmds + (sequence_number % 90);
	if (!verified_cmd)
		return;

	const auto local_player = Interfaces::EntityList->GetEntity(Interfaces::EngineClient->GetLocalPlayer());

	if (!local_player->IsAlive())
		return;

	const auto old_cmd = *cmd;

	unsigned long move;
	_asm mov move, ebp;
	bool& bSendPacket = *(***(bool****)(move)-1);

	Miscellaneous::BunnyHop(cmd);

	EnginePrediction.Start(cmd, local_player);

	Aimbot::Run(cmd);
	Aimbot::Antiam(cmd);

	EnginePrediction.End();

	Math::NormalizeAngles(cmd->viewangles);
	Math::ClampAngles(cmd->viewangles);

	FixMovement(cmd, old_cmd);

	Globals::LastRealAngles = cmd->viewangles;

	verified_cmd->m_cmd = *cmd;
	verified_cmd->m_crc = cmd->GetChecksum();
}

std::once_flag endscene;
long __stdcall Hooks::hkEndScene(IDirect3DDevice9* device)
{
	static auto oEndScene = d3d_hook.get_original<long(__stdcall*)(IDirect3DDevice9*)>(42);

	std::call_once
	(
		endscene, [&]
		{
			auto gmod = FindWindowW(L"Valve001", nullptr);

			oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(gmod, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hkWndProc)));

			ImGui_ImplDX9_Init(gmod, device);
			ImGuiStyle& style = ImGui::GetStyle();

			Gui::SetupColors(ImGui::GetStyle());
			Gui::SetupStyles(ImGui::GetStyle());

			auto& io = ImGui::GetIO();
			ImFontConfig config;
			config.FontDataOwnedByAtlas = false;

			Gui::pTabsFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(gotham_pro_compressed_data,
				gotham_pro_compressed_size, 15, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

			io.FontDefault = io.Fonts->AddFontFromMemoryCompressedTTF(Ubuntu_compressed_data, Ubuntu_compressed_size, 13.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());

			io.IniFilename = nullptr;
		}
	);

	Interfaces::EngineClient->GetScreenSize(Globals::ScreenWidth, Globals::ScreenHeight);

	if (Utilities::IsKeyPressed(VK_INSERT))
		Globals::MenuOpened = !Globals::MenuOpened;

	ImGui_ImplDX9_NewFrame();

	Visuals::RunPlayerESP();

	Gui::HandleAlpla(g_Timer.GetDelta());
	Gui::Run();

	ImGui::Render();

	g_Timer.Run();
	return oEndScene(device);
}