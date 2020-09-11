#ifndef COLOUR_H
#define COLOUR_H

#include "DXIncludes.h"

class Colour
{
public:

	Colour(float r_, float g_, float b_, float a_ = 1.0f)
	{
		m_colourValue.v = DirectX::XMVectorSet(r_, g_, b_, a_);
	}

	void operator=(Colour& other_)
	{
		m_colourValue = other_.m_colourValue;
	}

	inline float GetR() { return DirectX::XMVectorGetX(m_colourValue); }
	inline void SetR(float val_) { m_colourValue.f[0] = val_; }
	inline float GetG() { return DirectX::XMVectorGetY(m_colourValue); }
	inline void SetG(float val_) { m_colourValue.f[1] = val_; }
	inline float GetB() { return DirectX::XMVectorGetZ(m_colourValue); }
	inline void SetB(float val_) { m_colourValue.f[2] = val_; }
	inline float GetA() { return DirectX::XMVectorGetW(m_colourValue); }
	inline void SetA(float val_) { m_colourValue.f[3] = val_; }

	float* GetPtr() { return reinterpret_cast<float*>(this); }

private:
	DirectX::XMVECTORF32 m_colourValue;
};

#endif