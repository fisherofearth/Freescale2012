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

int brakingPower;//�ƶ����ȣ��ƶ�ʱ�����ת�Ķ�����
int brakingTime;//����е��ƶ�ʱ�䣨��̣�,��1/60��Ϊ��λ

int slidePower;//ת����ʱ�趨�Ķ���

int thresholdSlide;

int middleX;//X������ֵ


int slowdownPower;//���ٺ�ĳ�ʼ����
// <<<-------