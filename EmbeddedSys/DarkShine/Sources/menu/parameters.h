#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__
#include "publicParameters.h"


char *itemsName[7]=
{
	"START!!!",
	"Set Group 1",
	"Set Group 2",
	"Video",
	"Debug mode 1",
	"Debug mode 2",
	"Debug mode 3",
};





#define lengthOfGroup1 7
struct PARAMETERS parametersGroup1[lengthOfGroup1]=
{
	{"P_PointT",&P_PointT,	0,100},
	{"P_PointI",&P_PointI,	0,100},
	{"P_PointH",&P_PointH,	0,100},
	{"P_K_ht",	&P_K_ht,	0,100},
	{"P_K_hi",	&P_K_hi,	0,100},
	{"P_R",		&P_R,		0,100},
	{"P_MPC",	&P_MPC,		0,100},
};

#define lengthOfGroup2 	12
struct PARAMETERS parametersGroup2[lengthOfGroup2]=
{
	{"D_baseSpeed",	&D_baseSpeed,	0,800},
	{"speedCurve",	&speedEnterCurve,0,800},
	{"D_maxPower",	&D_maxPower,	0,1249},
	{"D_minPower",	&D_minPower,	0,1249},
	{"P_PID",		&P_PID,			0,100},
	{"thrshEdge",	&thresholdEdge,	0,255},
	{"brakPower",	&brakingPower,	0,1249},
	{"brakTime",	&brakingTime,	0,60},
	{"slidePower",	&slidePower,	0,1249},
	{"thrshSlide",	&thresholdSlide,	0,1249},
	{"middleX",	&middleX,	0,320},
	{"downPower",	&slowdownPower,	0,1249}
		
	
};

#endif