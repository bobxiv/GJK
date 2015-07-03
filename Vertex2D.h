#pragma once

class Vertex2D
{
public:

	float x,y;
	
	Vertex2D(float _x, float _y): x(_x), y(_y)
	{}

	bool operator==(const Vertex2D &b) const
	{
		return (x==b.x && y==b.y);
	}

	Vertex2D operator+(const Vertex2D &b) const
	{
		return Vertex2D(x+b.x, y+b.y);
	}

	Vertex2D operator-(const Vertex2D &b) const
	{
		return Vertex2D(x-b.x, y-b.y);
	}

	Vertex2D operator*(const float &s) const
	{
		return Vertex2D(x*s, y*s);
	}

	Vertex2D operator/(const float &s) const
	{
		return Vertex2D(x/s, y/s);
	}

	Vertex2D operator*(const Vertex2D &b) const
	{
		return Vertex2D(x*b.x, y*b.y);
	}

	static float Dot(const Vertex2D &a, const Vertex2D &b)
	{
		return (a.x*b.x+a.y*b.y);
	}

	static float Cross(const Vertex2D &a, const Vertex2D &b)
	{
		return (a.x*b.y-b.x*a.y);
	}

	static float Norm(const Vertex2D &a)
	{
		return sqrt(a.x*a.x+a.y*a.y);
	}
};