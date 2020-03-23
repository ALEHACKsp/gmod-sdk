#pragma once
#include "../sdk/sdk.hpp"
#include <DirectXMath.h>

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)

namespace Math {
	extern float GRD_TO_BOG(float GRD);
	extern float VectorNormalize(Vector& v);
	extern float GetFov(Angle viewAngle, Angle aimAngle);
	extern bool WorldToScreen(const Vector& in, Vector& out);
	extern void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out);
	extern Angle CalcAngle(const Vector& src, const Vector& dst);
	extern void NormalizeAngles(Angle& angles);
	extern void ClampAngles(Angle& angles);
	extern void AngleVectors(const Angle& angles, Vector& forward, Vector& right, Vector& up);
}