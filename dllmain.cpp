#include "includes.hpp"
#include "sdk/sdk.hpp"
#include "utilities/utilities.hpp"
#include "utilities/netvars.hpp"
#include "hooking/hooks.hpp"

void Init() {

	while (!Utilities::IsLoaded())
		std::this_thread::sleep_for(std::chrono::milliseconds(25));

	Utilities::AttachConsole();

	Interfaces::Init();
	Interfaces::Print();
	Netvars::Init();
	Hooks::Init();

	//while (true)
		//std::this_thread::sleep_for(std::chrono::seconds(25));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call != 1)
		return 1;

	if (hModule)
		DisableThreadLibraryCalls(hModule);

	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Init, 0, 0, nullptr);
	return 1;
}

