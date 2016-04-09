#include "publicParameters.h"


#define sector_publicParameters 0

#define lengthSDParameters	19
struct SD_PARAMETERS SDParametersList[lengthSDParameters] =
{
		{&P_PointT},
		{&P_PointH},
		{&P_K_hi},
		{&P_PointI},
		{&P_K_ht},
		{&P_R},
		{&P_MPC},
		{&D_maxPower},
		{&D_minPower},
		{&P_PID},
		{&thresholdEdge},
		{&D_baseSpeed},
		{&speedEnterCurve},
		{&brakingPower},
		{&brakingTime},
		{&slidePower},
		{&thresholdSlide},
		{&middleX},
		{&slowdownPower}
};
