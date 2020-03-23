#include "menu.hpp"
#include "../../utilities/utilities.hpp"
#include "../../options.hpp"

#define _xor_(s) std::string(s)

int Gui::iTabSelected = 0;
ImFont* Gui::pTabsFont = nullptr;
float Gui::flMainFrameAlpha = 0.f;

__forceinline void Gui::Run()
{
	if (flMainFrameAlpha == 0)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, flMainFrameAlpha);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

	auto controlscolor = ImVec4(1.f, 0.f, 0.f, 1.f);
	ImGui::GetStyle().Colors[ImGuiCol_CheckMark] = controlscolor;
	ImGui::GetStyle().Colors[ImGuiCol_ColumnActive] = controlscolor;
	ImGui::GetStyle().Colors[ImGuiCol_ColumnHovered] = controlscolor;
	ImGui::GetStyle().Colors[ImGuiCol_Separator] = controlscolor;
	ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] = controlscolor;
	ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] = controlscolor;
	ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrab] = controlscolor;
	ImGui::GetStyle().Colors[ImGuiCol_ScrollbarGrabHovered] = controlscolor;
	ImGui::GetStyle().Colors[ImGuiCol_ResizeGrip] = ImVec4(controlscolor.x, controlscolor.y, controlscolor.z, 0.0f);
	ImGui::GetStyle().Colors[ImGuiCol_ResizeGripHovered] = controlscolor;
	ImGui::GetStyle().Colors[ImGuiCol_ResizeGripActive] = controlscolor;
	ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] = ImVec4(controlscolor.x, controlscolor.y, controlscolor.z, 0.5f);
	ImGui::GetStyle().Colors[ImGuiCol_Header] = ImVec4(controlscolor.x, controlscolor.y, controlscolor.z, 0.5f);//ImGuiCol_HeaderHovered
	ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] = ImVec4(controlscolor.x, controlscolor.y, controlscolor.z, 0.5f);//ImGuiCol_HeaderHovered


	if (ImGui::Begin(_xor_("sdk").c_str(), &Globals::MenuOpened, ImVec2(520, 265), flMainFrameAlpha,
		ImGuiWindowFlags_NoScrollWithMouse
		| ImGuiWindowFlags_NoTitleBar
		| ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoResize))
	{
		ImGui::BeginGroup(); {
			ImGui::PushFont(pTabsFont);

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
			if (Gui::ImguiCustomButton(_xor_("Aimbot").c_str(), iTabSelected == 0, ImVec2(173, 50)))
				iTabSelected = 0; ImGui::SameLine();
			if (Gui::ImguiCustomButton(_xor_("Visuals").c_str(), iTabSelected == 1, ImVec2(173, 50)))
				iTabSelected = 1;  ImGui::SameLine();
			if (Gui::ImguiCustomButton(_xor_("Misc").c_str(), iTabSelected == 2, ImVec2(173, 50)))
				iTabSelected = 2;
			ImGui::PopStyleVar();

			ImGui::PopFont();

		} ImGui::EndGroup();

		ImGui::Spacing();

		ImGui::Dummy(ImVec2(2.f, 0));
		ImGui::SameLine();

		if (ImGui::BeginChild(_xor_("##func").c_str(), ImVec2(0, 0))) {
			switch (iTabSelected)
			{
			case 0:
				ImGui::Checkbox("Enable Aimbot", &Vars.aim_enabled);
				ImGui::Checkbox("Auto Fire", &Vars.aim_autofire);
				ImGui::Checkbox("Silent", &Vars.aim_silent);
				ImGui::SliderFloat("FOV", &Vars.aim_fov, 0.f, 180.f);

				ImGui::Spacing();

				ImGui::Checkbox("Enable Antiam", &Vars.antiaim_enabled);
				break;
			case 1:
				ImGui::Checkbox("Enable ESP", &Vars.esp_enabled);
				ImGui::Checkbox("Enable Chams", &Vars.chams_enabled);

				ImGui::Checkbox("Ignore LocalPlayer", &Vars.esp_ignore_local);

				ImGui::Checkbox("Boxes", &Vars.esp_box);
				ImGui::Checkbox("Healthbars", &Vars.esp_healthbar);
				ImGui::Checkbox("Names", &Vars.esp_name);
				break;
			case 2:
				ImGui::Checkbox("BunnyHop", &Vars.misc_bhop);
				break;
			}

		} ImGui::EndChild();
	} ImGui::End();
	ImGui::PopStyleVar(5);
}

__forceinline void Gui::HandleAlpla(double val) {
	static const float speed = 2.5f; // hardcoded value
	if (Globals::MenuOpened) {
		if (flMainFrameAlpha < 1.f)
			flMainFrameAlpha += val * speed;
	}
	else {
		if (flMainFrameAlpha > 0.f)
			flMainFrameAlpha -= val * speed;
	}
	// clamp
	if (flMainFrameAlpha > 1.f) flMainFrameAlpha = 1.f;
	if (flMainFrameAlpha < 0.f) flMainFrameAlpha = 0.f;
}

__forceinline bool Gui::ImguiCustomButton(const char* label, bool SelectedSubTab, const ImVec2& size_arg)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ImGui::ItemSize(bb, style.FramePadding.y);
	const ImGuiID id = window->GetID(label);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

	// Render

	if (SelectedSubTab)
	{
		ImGui::RenderFrame(ImVec2(bb.Min.x, bb.Max.y), ImVec2(bb.Max.x, bb.Max.y + 2), ImGui::GetColorU32(ImGui::GetStyle().Colors[ImGuiCol_CheckMark]));
	}

	ImGui::PushStyleColor(ImGuiCol_Text, SelectedSubTab ? ImGui::GetStyle().Colors[ImGuiCol_CheckMark] : ImVec4(1.f, 1.00f, 1.00f, 1.00f));
	ImGui::RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x, bb.Min.y + style.FramePadding.y), ImVec2(bb.Max.x - style.FramePadding.x, bb.Max.y - style.FramePadding.y), label, NULL, &label_size, style.ButtonTextAlign, &bb);
	ImGui::PopStyleColor();

	return pressed;
}

__forceinline void Gui::SetupColors(ImGuiStyle& style)
{
	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.f, 0.f, 0.f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.11f, 0.11f, 0.11f, 0.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.09f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 0.88f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f); // TEST
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.59f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.045f, 0.045f, 0.045f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.045f, 0.045f, 0.045f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.045f, 0.045f, 0.045f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.24f, 0.40f, 0.95f, 1.00f);
	style.Colors[ImGuiCol_Separator] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.40f, 0.95f, 0.59f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.045f, 0.045f, 0.045f, 1.00f);
}

__forceinline void Gui::SetupStyles(ImGuiStyle& style)
{
	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2(6, 6);
	style.WindowBorderSize = 0.f;
	style.WindowRounding = 0.0f;
	style.WindowRounding = 0.f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(2, 2);
	style.FrameRounding = 0.f;
	style.ItemSpacing = ImVec2(6, 4);
	style.FrameBorderSize = 0.0f;
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 19.0f;
	style.ColumnsMinSpacing = 1.0f;
	style.ScrollbarSize = 8.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.CurveTessellationTol = 1.25f;
}