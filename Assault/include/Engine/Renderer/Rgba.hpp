#pragma once

#ifndef RGBA_HPP
#define RGBA_HPP

class Rgba
{
public:
	//constructors
	Rgba();
	Rgba(float red, float green, float blue, float alpha);

	//setters
	void SetRGBA(float in_r, float in_g, float in_b, float in_a);
	void SetRGB(float in_r, float in_g, float in_b);
	void setAlpha(float in_a);

	//getters
	void GetRGBA(float& out_r, float& out_g, float& out_b, float& out_a) const;

	//variables
	float Red;
	float Green;
	float Blue;
	float Alpha;
};
#endif // !RGBA_HPP
