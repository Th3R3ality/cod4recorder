#pragma once
#include "../enums/offhandsecondaryclass.h"
#include "../enums/viewlocktypes.h"
#include "sprintstate.h"
#include "mantlestate.h"
#include "../enums/actionslottype.h"
#include "actionslotparam.h"
#include "objective.h"
#include "hud.h"
#include "../../vec.h"

// playerState_t is the information needed by both the client and server
// to predict player motion and actions
// nothing outside of pmove should modify these, or some degree of prediction error
// will occur
struct playerState_t
{
	int commandTime;
	int pm_type;
	int bobCycle;
	int pm_flags;
	int weapFlags;
	int otherFlags;
	int pm_time;
	fvec3 origin;
	fvec3 velocity;
	fvec2 oldVelocity;
	int weaponTime;
	int weaponDelay;
	int grenadeTimeLeft;
	int throwBackGrenadeOwner;
	int throwBackGrenadeTimeLeft;
	int weaponRestrictKickTime;
	int foliageSoundTime;
	int gravity;
	float leanf;
	int speed;
	fvec3 delta_angles;
	int groundEntityNum;
	float vLadderVec[3];
	int jumpTime;
	float jumpOriginZ;
	int legsTimer;
	int legsAnim;
	int torsoTimer;
	int torsoAnim;
	int legsAnimDuration;
	int torsoAnimDuration;
	int damageTimer;
	int damageDuration;
	int flinchYawAnim;
	int movementDir;
	int eFlags;
	int eventSequence;
	int events[4];
	unsigned int eventParms[4];
	int oldEventSequence;
	int clientNum;
	int offHandIndex;
	OffhandSecondaryClass offhandSecondary;
	unsigned int weapon;
	int weaponstate;
	unsigned int weaponShotCount;
	float fWeaponPosFrac;
	int adsDelayTime;
	int spreadOverride;
	int spreadOverrideState;
	int viewmodelIndex;
	fvec3 viewangles;
	int viewHeightTarget;
	float viewHeightCurrent;
	int viewHeightLerpTime;
	int viewHeightLerpTarget;
	int viewHeightLerpDown;
	float viewAngleClampBase[2];
	float viewAngleClampRange[2];
	int damageEvent;
	int damageYaw;
	int damagePitch;
	int damageCount;
	int stats[5];
	int ammo[128];
	int ammoclip[128];
	unsigned int weapons[4];
	unsigned int weaponold[4];
	unsigned int weaponrechamber[4];
	float proneDirection;
	float proneDirectionPitch;
	float proneTorsoPitch;
	ViewLockTypes viewlocked;
	int viewlocked_entNum;
	int cursorHint;
	int cursorHintString;
	int cursorHintEntIndex;
	int iCompassPlayerInfo;
	int radarEnabled;
	int locationSelectionInfo;
	SprintState sprintState;
	float fTorsoPitch;
	float fWaistPitch;
	float holdBreathScale;
	int holdBreathTimer;
	float moveSpeedScaleMultiplier;
	MantleState mantleState;
	float meleeChargeYaw;
	int meleeChargeDist;
	int meleeChargeTime;
	int perks;
	ActionSlotType actionSlotType[4];
	ActionSlotParam actionSlotParam[4];
	int entityEventSequence;
	int weapAnim;
	float aimSpreadScale;
	int shellshockIndex;
	int shellshockTime;
	int shellshockDuration;
	float dofNearStart;
	float dofNearEnd;
	float dofFarStart;
	float dofFarEnd;
	float dofNearBlur;
	float dofFarBlur;
	float dofViewmodelStart;
	float dofViewmodelEnd;
	int hudElemLastAssignedSoundID;
	Objective_t objective[16];
	char weaponmodels[128];
	int deltaTime;
	int killCamEntity;
	hud_t hud;
};