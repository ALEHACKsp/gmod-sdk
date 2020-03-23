#include "math.hpp"
#include "utilities.hpp"
#include "../sdk/sdk.hpp"

namespace Math
{
	float GRD_TO_BOG(float GRD) {
		return (M_PI / 180) * GRD;
	}

	float VectorNormalize(Vector& v)
	{
		float l = v.Length();

		if (l != 0.0f)
		{
			v /= l;
		}
		else
		{
			v.x = v.y = 0.0f; v.z = 1.0f;
		}

		return l;
	}

	float GetFov(Angle viewAngle, Angle aimAngle)
	{
		Angle delta = aimAngle - viewAngle;
		Math::NormalizeAngles(delta);
		Math::ClampAngles(delta);
		return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
	}

	static bool ScreenTransform(const Vector& in, Vector& out)
	{
		static auto& w2sMatrix = Interfaces::EngineClient->GetViewMatrix();

		out.x = w2sMatrix.m[0][0] * in.x + w2sMatrix.m[0][1] * in.y + w2sMatrix.m[0][2] * in.z + w2sMatrix.m[0][3];
		out.y = w2sMatrix.m[1][0] * in.x + w2sMatrix.m[1][1] * in.y + w2sMatrix.m[1][2] * in.z + w2sMatrix.m[1][3];
		out.z = 0.0f;

		float w = w2sMatrix.m[3][0] * in.x + w2sMatrix.m[3][1] * in.y + w2sMatrix.m[3][2] * in.z + w2sMatrix.m[3][3];

		if (w < 0.001f) {
			out.x *= 100000;
			out.y *= 100000;
			return false;
		}

		out.x /= w;
		out.y /= w;

		return true;
	}

	bool WorldToScreen(const Vector& in, Vector& out)
	{
		if (ScreenTransform(in, out)) {
			out.x = (Globals::ScreenWidth / 2.0f) + (out.x * Globals::ScreenWidth) / 2.0f;
			out.y = (Globals::ScreenHeight / 2.0f) - (out.y * Globals::ScreenHeight) / 2.0f;

			return true;
		}
		return false;
	}

	void VectorTransform(const Vector& in1, const matrix3x4_t& in2, Vector& out)
	{
		out[0] = in1.Dot(in2[0]) + in2[0][3];
		out[1] = in1.Dot(in2[1]) + in2[1][3];
		out[2] = in1.Dot(in2[2]) + in2[2][3];
	}

	Angle CalcAngle(const Vector& src, const Vector& dst)
	{
		Angle vAngle;
		Vector delta((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));
		double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

		vAngle.x = float(atanf(float(delta.z / hyp)) * 57.295779513082f);
		vAngle.y = float(atanf(float(delta.y / delta.x)) * 57.295779513082f);
		vAngle.z = 0.0f;

		if (delta.x >= 0.0)
			vAngle.y += 180.0f;

		return vAngle;
	}

	void NormalizeAngles(Angle& angles)
	{
		for (auto i = 0; i < 3; i++)
		{
			while (angles[i] < -180.0f)
			{
				angles[i] += 360.0f;
			}
			while (angles[i] > 180.0f)
			{
				angles[i] -= 360.0f;
			}
		}
	}

	void ClampAngles(Angle& angles)
	{
		if (angles.x > 89.0f) angles.x = 89.0f;
		else if (angles.x < -89.0f) angles.x = -89.0f;

		if (angles.y > 180.0f) angles.y = 180.0f;
		else if (angles.y < -180.0f) angles.y = -180.0f;

		angles.z = 0;
	}

	void AngleVectors(const Angle& angles, Vector& forward, Vector& right, Vector& up)
	{
		float sr, sp, sy, cr, cp, cy;

		DirectX::XMScalarSinCos(&sp, &cp, DEG2RAD(angles[0]));
		DirectX::XMScalarSinCos(&sy, &cy, DEG2RAD(angles[1]));
		DirectX::XMScalarSinCos(&sr, &cr, DEG2RAD(angles[2]));

		forward.x = (cp * cy);
		forward.y = (cp * sy);
		forward.z = (-sp);
		right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right.y = (-1 * sr * sp * sy + -1 * cr * cy);
		right.z = (-1 * sr * cp);
		up.x = (cr * sp * cy + -sr * -sy);
		up.y = (cr * sp * sy + -sr * cy);
		up.z = (cr * cp);
	}
}