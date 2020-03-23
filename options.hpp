#pragma once

class CVariables {
public:
	bool aim_enabled = false;
	bool aim_autofire = false;
	bool aim_silent = false;
	float aim_fov = 0.f;

	bool antiaim_enabled = false;

	bool esp_enabled = false;
	bool chams_enabled = false;
	bool esp_ignore_local = false;
	bool esp_visible_only = false;
	bool esp_box = false;
	bool esp_healthbar = false;
	bool esp_name = false;

	bool misc_bhop = false;
};
extern CVariables Vars;