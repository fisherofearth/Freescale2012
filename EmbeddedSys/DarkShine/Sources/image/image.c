#include "image.h"
#include "publicParameters.h"

uchar videoData[100][7][2];
uchar Pr_SelectFIFO = 0;
uchar F_selectFifo = 0;


struct RANGE searchRange[2];
struct POINT edgePoint[2][30];

int pixelBuffer[3][160];

struct LINE edgeLimitLines[2];

// 图像处理最终特征 ------>>>
struct CHARACTER chrctr_BTX[2];

int edgeStartZ[2];
int C_activePoint[2];

int F_crossRoad = 0;
int F_startLine = 0;

// <<<------

// ------------>>>
uint iSkipPixel, iSkipLine;
#define 	SkipOnePixel() 	al422b_SendReadClock()
#define 	SkipOneLine() 	SkipPixels(160)
#define 	ReadPixel()		al422b_ReadData()
#define 	SkipPixels(x) 	for(iSkipPixel=0;iSkipPixel<(x);iSkipPixel++){SkipOnePixel();}
#define 	SkipLines(x) 	for(iSkipLine=0;iSkipLine<(x);iSkipLine++){SkipPixels(160);}
// <<<------------


uchar bufferImage[128][8]; //[x][y]
/*
 * 	函数名称：	DisplayImage
 *  函数功能：	显示图像
 *  输入参数：	length  	（像素）	图像宽度            1~128
 *  			height 		（像素）	图像高度		1~64
 *    			locationX 	（像素）	图像位置X	0~127
 *       		locationY 	（x8像素）图像位置Y	0~7
 *  ！！注意：OLED的坐标原点在左下角！
 */
void DisplayImage(int length, int height, int locationX, int locationY) {
	int x = 0, y = 0;
	for (y = 0; y < (((height-1) / 8) + 1); y++) {
		oled_SetXY(locationX, (locationY + y));
		for (x = 0; x < length; x++) {
			oled_WriteData(bufferImage[x][y]);
			bufferImage[x][y] = 0;//dispose bufferImage
		}
	}
}

uchar d[8] ={
	1,2,4,8,
	16,32,64,128
		
};
void DisplayVideo(void) {
	int i, j;
	int tmp,tmp2;

	Pr_SelectFIFO = ~Pr_SelectFIFO;
	//Pr_SelectFIFO=1;
	al422b_SelectFIFO(Pr_SelectFIFO);
	al422b_Reset();
	
	SkipLines(skipLines);
	
	for (j = 0; j < 30; j++) {
		tmp = ((29 - j) / skiplineControl);
		SkipLines(tmp);
				
		tmp = j/8;
		tmp2 = j%8;
		
		
		
		for(i=79;i>=0;i--) {
			if(ReadPixel()>128){
				bufferImage[i][tmp] |= d[tmp2];
			}

			SkipOnePixel();
		}
		//SkipLines(1);
	}
	
	DisplayImage(80, 32, 0, 0);
}

//计算搜索区域
#define defaultRange 10
void SetSearchRange(int LorR, int base, int range) {
	searchRange[LorR].start = base - range;
	searchRange[LorR].end = base + range;
	if (LorR == left) {
		if (searchRange[left].end > leftSearchRange)
			searchRange[left].end = leftSearchRange;

	} else {
		if (searchRange[right].start < rightSearchRange)
			searchRange[right].start = rightSearchRange;
	}
}

//计算拐点
struct POINT GetInflectionPoint(int LorR) {
	int z;
	struct POINT tmpReturn;
	//struct POINT tmpPoint;	
	int tmp = 0;
	int max = 0;
	tmpReturn = edgePoint[LorR][edgeStartZ[LorR]];

	
	
	edgeLimitLines[LorR] = LPP(chrctr_BTX[LorR].head, chrctr_BTX[LorR].tail);

	for (z = edgeStartZ[LorR]; z < C_activePoint[LorR]; z++) {
		
		tmp = DPL(edgePoint[LorR][z],edgeLimitLines[LorR]);
		tmp = ABS(tmp);
		if(tmp > max)
		{
			max = tmp;
			tmpReturn = edgePoint[LorR][z];
		}
	}
	return tmpReturn;
}

//搜索边沿并量化路径特征
#define thresholdStartLine 35
void SearchEdge() {
	int x = 0;
	int z = 0;
	int tmp;
	int tmp1, tmp2;
	int LorR = 0;
	int buffer[160];
	int F_continueSearch[2] = { 1, 1 };
	int F_maybeCrossRoad[2] = { 0, 0 };
	int F_reborn = 0;

	int buffer_startLine[10][3];

	Pr_SelectFIFO = ~Pr_SelectFIFO;
	al422b_SelectFIFO(Pr_SelectFIFO);
	al422b_Reset();

	// initial parameters --->>>
	F_startLine = 0;

	edgeStartZ[left] = 0;
	edgeStartZ[right] = 0;

	C_activePoint[left] = 0;
	C_activePoint[right] = 0;

	searchRange[right].start = rightSearchRange;
	searchRange[right].end = midSearchRange;
	searchRange[left].start = midSearchRange;
	searchRange[left].end = leftSearchRange;

	SkipLines(skipLines);

	for (z = 0; z < 30; z++) {

		if(z<10)
		{
			tmp = ((29 - z) / skiplineControl) - 1;
			SkipLines(tmp);
			
			//起跑线搜索
			tmp = 38 - z;
			tmp1 = (160-tmp)/2;
			tmp2 = (160+tmp)/2;
			for(x=0;x<tmp1;x++)SkipOnePixel();
				x++;
				buffer_startLine[z][0] = ReadPixel();//right
			for(;x<80;x++)SkipOnePixel();
				x++;
				buffer_startLine[z][2] = ReadPixel();//middle
			for(;x<tmp2;x++)SkipOnePixel();
				x++;
				buffer_startLine[z][1] = ReadPixel();//left
			for(;x<160;x++)SkipOnePixel();
			
			
			
		}
		else
		{
			tmp = ((29 - z) / skiplineControl);
			SkipLines(tmp);
			
			if(z == 10)
			{
				tmp1 = 10;
				for(x=0;x<9;x++)
				{
					if((ABS(buffer_startLine[x][0] - buffer_startLine[x+1][0])) > thresholdStartLine)
					{
						tmp1 = x;
						break;
					}
					
				}
				tmp2 = 10;
				for(x=0;x<9;x++)
				{
					if((ABS(buffer_startLine[x][1] - buffer_startLine[x+1][1])) > thresholdStartLine)
					{
						tmp2 = x;
						break;
					}
				}
				tmp = 10;
				for(x=0;x<9;x++)
				{
					if((ABS(buffer_startLine[x][2] - buffer_startLine[x+1][2])) > thresholdStartLine)
					{
						tmp = x;
						break;
					}
				
				}
				if(tmp1 < 10)
				{
					if(tmp2 < 10)
					{
						if((ABS(tmp1-tmp2)) < 2)
						{
							if(tmp == 10)
							{
								F_startLine = 1;
							}
						}
					}
				}
			}
		}

		if(searchRange[right].end > searchRange[left].start)
		{
			searchRange[left].start = (searchRange[right].end + searchRange[left].start) / 2;
			searchRange[right].end = searchRange[left].start;
		}

		tmp = searchRange[right].start;
		SkipPixels(tmp);
		for(x=searchRange[right].start;x<searchRange[right].end;x++)buffer[x] = ReadPixel();
		tmp = searchRange[left].start - searchRange[right].end;
		SkipPixels(tmp);
		for(x=searchRange[left].start;x<searchRange[left].end;x++)buffer[x] = ReadPixel();
		tmp = 160 - searchRange[left].end;
		SkipPixels(tmp);

		//right
		LorR = right;
		if(F_continueSearch[LorR] == 1)
		{
			F_continueSearch[LorR] = 0;
			for(x=(searchRange[LorR].end-1-2);x>=searchRange[LorR].start;x--)
			{
				if(Gradient(buffer[x+2] ,buffer[x]) > thresholdEdge)
				{
					if(F_reborn == 1)
						edgeStartZ[LorR] = 10;
					else
					{
						if(z>1)//判断折角
						{
							if((ABS((x - edgePoint[LorR][z-1].x) - (edgePoint[LorR][z-1].x - edgePoint[LorR][z-2].x)))> 5)
							{
								break;
							}
						}
						else
						{
							if(z == 1)//排除十字路口第2点扫描到横线
							{
								if((edgePoint[LorR][0].x - x) > 5)break;
							}
							
						}
					}
					edgePoint[LorR][z].x = x;
					edgePoint[LorR][z].z = z;
					SetSearchRange(LorR, x, defaultRange);
					F_continueSearch[LorR] = 1;
					C_activePoint[LorR] ++;

					break;
				}
			}
		}

		//left
		LorR = left;
		if(F_continueSearch[LorR] == 1)
		{
			F_continueSearch[LorR] = 0;
			for(x=searchRange[LorR].start+2;x<searchRange[LorR].end;x++)
			{
				if(Gradient(buffer[x-2] ,buffer[x]) > thresholdEdge)
				{
					if(F_reborn == 1)
					edgeStartZ[LorR] = 10;
					else
					{
						if(z>1)//判断折角
						{
							if((ABS((x - edgePoint[LorR][z-1].x) - (edgePoint[LorR][z-1].x - edgePoint[LorR][z-2].x)))> 5)
							{
								break;
							}
						}
						else{
							if(z == 1)//排除十字路口第2点扫描到横线
							{
								if((x - edgePoint[LorR][0].x) > 5)break;
							}
						}
					}
					edgePoint[LorR][z].x = x;
					edgePoint[LorR][z].z = z;
					SetSearchRange(LorR, x, defaultRange);
					F_continueSearch[LorR] = 1;
					C_activePoint[LorR] ++;
					break;
				}
			}
		}

		switch(z)
		{
			case 2://头两行都没有抓到边沿的情况 判定
			if((C_activePoint[left] + C_activePoint[right]) < 3)
			F_reborn = 1;
			break;

			case 9://“复活吧,我的勇士”
			if(F_reborn == 1)
			{
				F_continueSearch[left] = 1;
				F_continueSearch[right] = 1;

				C_activePoint[left] = 0;
				C_activePoint[right] = 0;

				searchRange[right].start = 30;
				searchRange[right].end = 79;
				searchRange[left].start = 79;
				searchRange[left].end = 130;

			}
			break;

			case 10:
			if((C_activePoint[left]+ C_activePoint[right]) < 2)goto myEnd;
			break;
		}

	}

	myEnd:

	if ((C_activePoint[left] + C_activePoint[right]) < 8) {
		F_crossRoad = 1;
	}
	if ((C_activePoint[left] + C_activePoint[right]) > 20) {
		F_crossRoad = 0;
	}
	for (LorR = 0; LorR < 2; LorR++) {
		if(C_activePoint[LorR] == 0)C_activePoint[LorR] = 1;
		chrctr_BTX[LorR].tail = edgePoint[LorR][(C_activePoint[LorR]-1) + edgeStartZ[LorR]];
		chrctr_BTX[LorR].head = edgePoint[LorR][edgeStartZ[LorR]];
		chrctr_BTX[LorR].inflection = GetInflectionPoint(LorR);

		
		if(C_activePoint[LorR]>20)
			chrctr_BTX[LorR].mid = edgePoint[LorR][19 + edgeStartZ[LorR]];
		else
			chrctr_BTX[LorR].mid = edgePoint[LorR][(C_activePoint[LorR]-1) + edgeStartZ[LorR]];

		
		chrctr_BTX[LorR].k_hi =KPP(chrctr_BTX[LorR].inflection,chrctr_BTX[LorR].head);
		//chrctr_BTX[LorR].k_it =KPP(chrctr_BTX[LorR].tail,chrctr_BTX[LorR].inflection);
		chrctr_BTX[LorR].k_it =KPP(chrctr_BTX[LorR].tail,chrctr_BTX[LorR].mid);
		
		chrctr_BTX[LorR].k_ht =KPP(chrctr_BTX[LorR].tail,chrctr_BTX[LorR].head);

		chrctr_BTX[LorR].R_hit = chrctr_BTX[LorR].k_it- chrctr_BTX[LorR].k_hi;
	}

}

void DisplayEdge() {
	int z, i;
	
	
	for (z = 0; z < 4; z++) {
		for (i = 0; i < 80; i++) {
			bufferImage[i][z] = 0;
		}
	}
	
	for (z = edgeStartZ[left]; z < C_activePoint[left]; z++) {
		bufferImage[79-(edgePoint[left][z].x/2)][(z)/8] |= d[(edgePoint[left][z].z)%8];
	}
	
	for (z = edgeStartZ[right]; z < C_activePoint[right]; z++) {
		bufferImage[79-(edgePoint[right][z].x/2)][(z)/8] |= d[(edgePoint[right][z].z)%8];
	}
	DisplayImage(80, 30, 0, 0);
}








