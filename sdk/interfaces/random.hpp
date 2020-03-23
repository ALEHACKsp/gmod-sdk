#pragma once

class CUniformRandomStream
{
public:
	// Sets the seed of the random number generator
	virtual void	SetSeed(int iSeed);

	// Generates random numbers

	virtual float	RandomFloat(float flMinVal = 0.0f, float flMaxVal = 1.0f);
	virtual int		RandomInt(int iMinVal, int iMaxVal);
	virtual float	RandomFloatExp(float flMinVal = 0.0f, float flMaxVal = 1.0f, float flExponent = 1.0f);
public:
	int GenerateRandomNumber();
	int m_idum;
	int m_iy;
	int m_iv[32];

	unsigned char PAD0[4];
};