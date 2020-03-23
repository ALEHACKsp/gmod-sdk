#include "visuals.hpp"
#include "../../utilities/math.hpp"
#include "../../utilities/drawmanager.hpp"
#include "../../options.hpp"

void Visuals::RunPlayerESP() {
	auto get_box = [](Vector feet, Vector head) -> RECT {
		RECT ret;

		auto h_ = fabs(head.y - feet.y);
		auto w_ = h_ / 2.0f;

		ret.left = (int)(feet.x - w_ * 0.5f);
		ret.right = (int)(ret.left + w_);
		ret.bottom = (feet.y > head.y ? (int)(feet.y) : (int)(head.y));
		ret.top = (feet.y > head.y ? (int)(head.y) : (int)(feet.y));

		return ret;
	};

	if (!Interfaces::EngineClient->IsInGame())
		return;

	if (!Vars.esp_enabled)
		return;

	CEntity* LocalPlayer = Interfaces::EntityList->GetEntity(Interfaces::EngineClient->GetLocalPlayer());

	for (size_t i = 0; i <= Interfaces::EntityList->GetHighestEntityIndex(); i++) {
		CEntity* entity = Interfaces::EntityList->GetEntity(i);
		if (!entity) continue;
		if (entity == LocalPlayer && Vars.esp_ignore_local) continue;
		if (!entity->IsPlayer()) continue;
		if (!entity->IsAlive()) continue;

		Vector feet, eye;
		if (!Math::WorldToScreen(entity->GetAbsOrigin(), feet) || !Math::WorldToScreen(entity->GetEyePos(), eye))
			continue;

		auto box = get_box(feet, eye);

		auto box_clr = Utilities::IsVisible(LocalPlayer, entity, entity->GetEyePos()) ? ImColor(1.f, 1.f, 0.f) : ImColor(1.f, 0.f, 0.f);

		if (Vars.esp_box) {
			Draw::Rect(ImVec2(box.left - 1, box.top - 1), ImVec2(box.right + 1, box.bottom + 1), ImColor(0.f, 0.f, 0.f), 5, 0, 1.f);
			Draw::Rect(ImVec2(box.left, box.top), ImVec2(box.right, box.bottom), box_clr, 5, 0, 1.f);
			Draw::Rect(ImVec2(box.left + 1, box.top + 1), ImVec2(box.right - 1, box.bottom - 1), ImColor(0.f, 0.f, 0.f), 5, 0, 1.f);
		}

		if (Vars.esp_healthbar) {
			float CurHealth = entity->GetHealth();
			float maxHp = entity->GetMaxHealth();

			Draw::FilledRect(ImVec2(box.left - 6, box.top), ImVec2(box.left - 3, box.bottom), ImColor(0.f, 0.f, 0.f), 5, 0);

			auto x_start = box.left - 5;
			auto y_start = box.top + 1;
			auto y_end = box.bottom - 1;

			auto y_size = (y_end - y_start) / maxHp * CurHealth;

			Draw::Line(ImVec2(x_start, y_end - y_size), ImVec2(x_start, y_end), ImColor(0.f, 1.f, 0.f), 1);
		}

		if (Vars.esp_name) {
			auto getName = [](CEntity* entity) -> std::string
			{
				auto glua = Interfaces::Lua->CreateInterface(LUA::type::client);

				if (!glua)
					return {};

				glua->PushSpecial(LUA::special::glob);
				glua->GetField(-1, "Entity");
				glua->PushNumber(entity->GetIndex());
				glua->Call(1, 1);

				glua->GetField(-1, "Name");
				glua->Push(-2);
				glua->Call(1, 1);

				std::string name = glua->GetString();
				glua->Pop(3);

				return name;
			};

			auto name = getName(entity);
			auto name_size = ImGui::CalcTextSize(name.c_str());
			auto x_start = box.left + (box.right - box.left) / 2.f;
			Draw::Text(ImVec2(x_start - (name_size.x / 2.f), box.top - name_size.y - 1), name.c_str(), ImColor(1.f, 1.f, 1.f));
		}
	}
}