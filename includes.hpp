#include <windows.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <assert.h>
#include <Psapi.h>
#include <array>
#include <algorithm>
#include <cstdint>
#include <d3d9.h>
#include <map>
#include <mutex>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "user32.lib")

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx9.h"