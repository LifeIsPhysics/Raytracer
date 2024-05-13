#pragma once

#include "vec3.h"

class ray
{
public:
	ray();
	ray(const vec3& A, const vec3& B) : _origin(A), _direction(B) {}
	
	const vec3& origin() const { return _origin; }
	const vec3& direction() const { return _direction; }

	vec3 at(double t) const {
		return _origin + t * _direction;
	}

private:
	vec3 _origin;
	vec3 _direction;
};


