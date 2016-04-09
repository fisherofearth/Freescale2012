#include "publicParameters.h"


// save in MicroSD ------>>>

int P_PointT;
int P_PointH;
int P_K_hi;
int P_PointI;
int P_K_ht;
int P_R;
int P_MPC;//min power in courve

int D_baseSpeed;
int D_maxPower ;//max value :1249
int D_minPower;
int P_PID;

int thresholdEdge ;

int speedEnterCurve;

int brakingPower;//制动力度（制动时电机反转的动力）
int brakingTime;//弯道中的制动时间（最短）,以1/60秒为单位

int slidePower;//转向不足时设定的动力

int thresholdSlide;

int middleX;//X坐标中值


int slowdownPower;//减速后的初始动力
// <<<-------