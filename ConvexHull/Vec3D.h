#pragma once

class Vec3D
{
public:
	Vec3D() : X(0), Y(0), Z(0) {}

	Vec3D(double x, double y, double z) : X(x), Y(y), Z(z) {}
	
	Vec3D(Vec3D& pt1, Vec3D& pt2)
	{
		X = pt2.X - pt1.X;
		Y = pt2.Y - pt1.Y;
		Z = pt2.Y - pt1.Y;
	}

	~Vec3D() {}

	inline static double DotP(Vec3D &vec1, Vec3D &vec2)
	{
		return vec1.X*vec2.X + vec1.Y*vec2.Y + vec1.Z*vec2.Z;
	}

	inline static Vec3D CrossP(Vec3D &vec1, Vec3D &vec2)
	{
		return Vec3D(vec1.Y*vec2.Z - vec1.Z*vec2.Y, vec1.Z*vec2.X - vec1.X*vec2.Y, vec1.X*vec2.Y - vec1.Y*vec2.X);
	}

	inline static Vec3D NormCrossP(Vec3D &vec1, Vec3D &vec2)
	{
		Vec3D normCross = Vec3D((vec1.Y*vec2.Z - vec1.Z*vec2.Y), (vec1.Z*vec2.X - vec1.X*vec2.Y), (vec1.X*vec2.Y - vec1.Y*vec2.X));
		double normCrossLen = normCross.Length();
		normCross.X /= normCrossLen;
		normCross.Y /= normCrossLen;
		normCross.Z /= normCrossLen;

		return normCross;
	}

	inline double Length()
	{
		return sqrt(X*X + Y*Y + Z*Z);
	}

	inline double DotP(Vec3D &vec2)
	{
		return X*vec2.X + Y*vec2.Y + Z*vec2.Z;
	}
	inline Vec3D CrossP(Vec3D &vec2)
	{
		return Vec3D(Y*vec2.Z - Z*vec2.Y, Z*vec2.X - X*vec2.Y, X*vec2.Y - Y*vec2.X);
	}
	inline Vec3D NormCrossP(Vec3D &vec2)
	{
		return Vec3D::NormCrossP(*this, vec2);
	}


	double X;
	double Y;
	double Z;

};

