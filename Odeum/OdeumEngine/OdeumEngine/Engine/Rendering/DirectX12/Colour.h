#ifndef COLOUR_H
#define COLOUR_H

#include "D3DIncludes.h"

class Colour
{
public:

	Colour(float r_, float g_, float b_, float a_ = 1.0f)
	{
		m_colourValue.v = DirectX::XMVectorSet(r_, g_, b_, a_);
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

	operator DirectX::XMVECTOR() const { return m_colourValue; }

	inline uint32_t ConvertToBit()
	{
		return (uint32_t)((uint32_t)(GetR() * 32.0f) | ((uint32_t)(GetG() * 32.0f) << 8) | ((uint32_t)(GetB() * 32.0f) << 16) | ((uint32_t)(GetA() * 32.0f) << 24));
	}

private:
	DirectX::XMVECTORF32 m_colourValue;
};

#endif