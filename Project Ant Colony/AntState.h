#pragma once

#include<iostream>

enum class Activity
{
	IDLE,
	FORAGING,
	GATHERING,
	ATTACKING,
	PATROLLING,
	EGGLAYING,
	DEAD
};

//map Activity to string for debugging
std::string printActivity(Activity target_activity);

/*
{
	{Activity::IDLE, "IDEL"},
	{Activity::FORAGING, "FORAGING"},
	{Activity::GATHERING, "GATHERING"},
	{Activity::ATTACKING, "ATTACKING"},
	{Activity::PATROLLING, "PATROLLING"},
	{Activity::EGGLAYING, "EGGLAYING"},
	{Activity::DEAD, "DEAD"}
};
*/


struct State
{
	bool MOVING{ 0 };
	bool STAIONERY{ 0 };
	bool EATING{ 0 };
	bool CARRYING{ 0 };
	bool DEAD{ 0 };
	bool DECAYING{ 0 };
};
