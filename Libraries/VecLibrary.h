#pragma once

#include <cmath>
//Mathmatical Constants
const double PI = 3.14159265359;
const double e = 2.718281828459;


//Defining a 2 Dimensional Vector
typedef struct {
	float x;
	float y;
} Vec2;

//Defining a Vector4, w value set to 1 as default as it shouldnt be changed often
typedef struct{
	float x;
	float y;
	float z; 
	float w = 1;

} Vec4;


inline Vec4 VecAdd(Vec4 a, Vec4 b){
	Vec4 sum;
	sum.x = a.x + b.x;
	sum.y = a.y + b.y;
	sum.z = a.z + b.z;

	return sum;
}



inline Vec4 VecSub(Vec4 a, Vec4 b){
	Vec4 dif;

	dif.x = a.x - b.x;
	dif.y = a.y - b.y;
	dif.z = a.z - b.z;

	return dif;
}


//Function that lineraly interpolates between 2 points over a given time
inline float lerp(float init_value, float dest_value, float time){
	return init_value + (dest_value - init_value) * time;
}

//Function that finds the magnitude of a given Vector4
inline float magnitude(Vec4 a){
	return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

inline float distance(Vec4 a, Vec4 b){
	return sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y))
	     + ((a.z - b.z) * (a.z - b.z)));
}

//normalizes a given vector
inline Vec4 normalize(Vec4 a){
	float mag = magnitude(a);
	Vec4 result;

	result.x = a.x / mag;
	result.y = a.y / mag;
	result.z = a.z / mag;

	return result;

}
//Returns the dot product of two Vector4's
inline float dot(Vec4 a , Vec4 b){
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline Vec4 cross(Vec4 a , Vec4 b ){
	Vec4 cProd;
	cProd.x = (a.y * b.z) - (a.z * b.y);
	cProd.y = (a.z * b.x) - (a.x * b.z);
	cProd.z = (a.x * b.y) - (a.y * b.x);
	return cProd;
	
}

inline float angle(Vec4 a, Vec4 b){
	return acos( dot(a, b) / (magnitude(a) * magnitude(b)) ) * (180/PI);
}


