#pragma once
struct visionSetVars_t
{
	bool glowEnable;
	float glowBloomCutoff;
	float glowBloomDesaturation;
	float glowBloomIntensity0;
	float glowBloomIntensity1;
	float glowRadius0;
	float glowRadius1;
	float glowSkyBleedIntensity0;
	float glowSkyBleedIntensity1;
	bool filmEnable;
	float filmBrightness;
	float filmContrast;
	float filmDesaturation;
	bool filmInvert;
	float filmLightTint[3];
	float filmDarkTint[3];
};