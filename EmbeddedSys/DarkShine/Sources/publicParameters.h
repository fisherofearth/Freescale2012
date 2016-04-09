#ifndef __PUBLICPARAMETERS_H__
#define __PUBLICPARAMETERS_H__


extern int P_PointT;
extern int P_PointH;
extern int P_K_hi;
extern int P_PointI;
extern int P_K_ht;
extern int P_R;
extern int P_MPC;//min power in courve

extern int D_baseSpeed;
extern int D_maxPower;//max value :1249
extern int D_minPower;
extern int P_PID;

extern int thresholdEdge;

extern int speedEnterCurve;
extern int brakingPower;//制动力度（制动时电机反转的动力）
extern int brakingTime;//弯道中的制动时间（最短）
extern int slidePower;//转向不足时设定的动力

extern int thresholdSlide;
extern int middleX;//X坐标中值
extern int slowdownPower;//减速后的初始动力
#endif