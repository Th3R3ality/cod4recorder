#pragma once
enum ImpactType
{
	IMPACT_TYPE_NONE = 0x0,
	IMPACT_TYPE_BULLET_SMALL = 0x1,
	IMPACT_TYPE_BULLET_LARGE = 0x2,
	IMPACT_TYPE_BULLET_AP = 0x3,
	IMPACT_TYPE_SHOTGUN = 0x4,
	IMPACT_TYPE_GRENADE_BOUNCE = 0x5,
	IMPACT_TYPE_GRENADE_EXPLODE = 0x6,
	IMPACT_TYPE_ROCKET_EXPLODE = 0x7,
	IMPACT_TYPE_PROJECTILE_DUD = 0x8,
	IMPACT_TYPE_COUNT = 0x9,
};