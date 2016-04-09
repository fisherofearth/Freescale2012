#include "image.h"

/*
 * 求平均数
 * Average of value-value
 */
#define	Average(v1,v2) 	((v1 + v2) / 2)

/*
 * 求梯度
 * Gradient of value-value
 */
#define Gradient(v1,v2)	(v1 - v2)

/*
 * 求绝对值
 */
#define	ABS(x) 			((x)<0?-(x):(x))

/*
 * 求点到直线的距离
 * Distance of point-LINE
 * 输入k、b应为真值的10倍
 * 返回结果为真值的10倍
 */
int DPL(struct POINT P, struct LINE L)
{
	return ((L.k * P.x) - (P.z * 100) + (L.b*100));
}
 

/*
 * 求两点的中间点
 * midPOINT of POINT-POINT
 */
struct POINT MPP(struct POINT P1, struct POINT P2) {
	struct POINT tmp;
	tmp.x = Average(P1.x , P2.x);
	tmp.z = Average(P1.z , P2.z);
	return tmp;
}

/*
 * 根据两点求直线
 * LINE of POINT-POINT
 */
struct LINE LPP(struct POINT P1, struct POINT P2)
{
	struct LINE tmp;
	//if(P1.x == P2.x)P1.x++;
	tmp.k = ((ABS(P2.z-P1.z))*100)/(P2.x-P1.x);
	tmp.b = (P1.z) - ((P1.x*(ABS( P2.z -P1.z)))/(P2.x-P1.x));
	return tmp;
}


/*
 * 根据两点求倾斜程度
 * P1 - P2
 */
int KPP(struct POINT P1, struct POINT P2)
{
	//if(P1.z == P2.z)P1.z ++;
	return (((P1.x-P2.x)*10) / (ABS(P1.z - P2.z)));
}
