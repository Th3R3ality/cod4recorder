#pragma once
enum InvalidCmdHintType
{
	INVALID_CMD_NONE = 0x0,
	INVALID_CMD_NO_AMMO_BULLETS = 0x1,
	INVALID_CMD_NO_AMMO_FRAG_GRENADE = 0x2,
	INVALID_CMD_NO_AMMO_SPECIAL_GRENADE = 0x3,
	INVALID_CMD_NO_AMMO_FLASH_GRENADE = 0x4,
	INVALID_CMD_STAND_BLOCKED = 0x5,
	INVALID_CMD_CROUCH_BLOCKED = 0x6,
	INVALID_CMD_TARGET_TOO_CLOSE = 0x7,
	INVALID_CMD_LOCKON_REQUIRED = 0x8,
	INVALID_CMD_NOT_ENOUGH_CLEARANCE = 0x9,
};