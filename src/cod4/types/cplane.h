#pragma once
struct cplane_t
{
	float normal[3];
	float dist;
	char type;  // for fast side tests: 0,1,2 = axial, 3 = nonaxial
	char signbits;  // signx + (signy<<1) + (signz<<2), used as lookup during collision
	char pad[2];
};