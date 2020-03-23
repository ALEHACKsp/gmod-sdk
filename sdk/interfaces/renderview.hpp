#pragma once

class IVRenderView
{
public:
	virtual void			unused_0() = 0;
	virtual void			unused_1() = 0;
	virtual void			unused_2() = 0;
	virtual void			unused_3() = 0;
	virtual void			SetBlend(float blend) = 0;
	virtual float			GetBlend(void) = 0;
	virtual void			SetColorModulation(float const* blend) = 0;
	virtual void			GetColorModulation(float* blend) = 0;
};