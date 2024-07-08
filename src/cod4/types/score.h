#pragma once
#include "material.h"
struct score_t
{
	int client;
	int score;
	int ping;
	int deaths;
	int team;
	int kills;
	int rank;
	int assists;
	Material* hStatusIcon;
	Material* hRankIcon;
};