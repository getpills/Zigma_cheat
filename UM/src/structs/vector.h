#pragma once
#include "../include.h"

#include "../src/offsets/client_dll.hpp"
#include "../src/offsets/offsets.hpp"



const float PI = acos(-1.0);

extern int screenWidth = GetSystemMetrics(SM_CXSCREEN);
extern int screenHeight = GetSystemMetrics(SM_CYSCREEN);

struct Vector2
{
	float x, y;
};

struct Vector4
{
	float w, x, y, z;
};


struct view_matrix_t
{
	float* operator[ ](int index)
	{
		return matrix[index];
	}

	float matrix[4][4];

};


struct Vector3
{


	// constructor
	Vector3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) { }

	// operator overloads
	 const Vector3& operator-(const Vector3& other) const noexcept
	{
		return Vector3{ x - other.x, y - other.y, z - other.z };
	}

	const Vector3& operator+(const Vector3& other) const noexcept
	{
		return Vector3{ x + other.x, y + other.y, z + other.z };
	}

	const Vector3& operator/(const float factor) const noexcept
	{
		return Vector3{ x / factor, y / factor, z / factor };
	}

	 const Vector3& operator*(const float factor) const noexcept
	{
		return Vector3{ x * factor, y * factor, z * factor };
	}


	 Vector3 WTS(view_matrix_t matrix)
	 {
		 float _x = matrix[0][0] * x + matrix[0][1] * y + matrix[0][2] * z + matrix[0][3];
		 float _y = matrix[1][0] * x + matrix[1][1] * y + matrix[1][2] * z + matrix[1][3];

		 float w = matrix[3][0] * x + matrix[3][1] * y + matrix[3][2] * z + matrix[3][3];


		 if (w < 0.01f)
			 return false;


		 float inv_w = 1.f / w;
		 _x *= inv_w;
		 _y *= inv_w;

		 float x = screenWidth / 2;
		 float y = screenHeight / 2;

		 x += 0.5f * _x * screenWidth + 0.5f;
		 y -= 0.5f * _y * screenHeight + 0.5f;

		 return{ x, y, w };


	 }



	 const Vector3 ToAngle() const noexcept
	 {
		 return Vector3{
		   std::atan2(-z, std::hypot(x, y)) * (180.0f / PI),
		   std::atan2(y, x) * (180.0f / PI),
		   0.0f };
	 }

	 const bool IsZero() const noexcept
	 {
		 return x == 0.f && y == 0.f && z == 0.f;
	 }

	// struct data
	float x, y, z;
	
};


