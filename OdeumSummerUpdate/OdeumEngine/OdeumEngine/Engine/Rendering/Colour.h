#ifndef COLOUR_H
#define COLOUR_H

#include "../../pch.h"

class Colour
{
public:

	Colour(float r_, float g_, float b_, float a_)
	{
		r = r_;
		g = g_;
		b = b_;
		a = a_;
	}

	__declspec(property( get = GetR, put = PutR )) float r;
	__declspec(property( get = GetG, put = PutG )) float g;
	__declspec(property( get = GetB, put = PutB )) float b;
	__declspec(property( get = GetA, put = PutA )) float a;

	float GetR() { return DirectX::XMVectorGetX(m_colourValue); }
	void PutR(float val_) { DirectX::XMVectorSetX(m_colourValue, val_); }
	float GetG() { return DirectX::XMVectorGetY(m_colourValue); }
	void PutG(float val_) { DirectX::XMVectorSetY(m_colourValue, val_); }
	float GetB() { return DirectX::XMVectorGetZ(m_colourValue); }
	void PutB(float val_) { DirectX::XMVectorSetZ(m_colourValue, val_); }
	float GetA() { return DirectX::XMVectorGetW(m_colourValue); }
	void PutA(float val_) { DirectX::XMVectorSetW(m_colourValue, val_); }

private:
	DirectX::XMVECTORF32 m_colourValue;
};

#endif