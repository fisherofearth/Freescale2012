using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.IO;
using FIFOSimulation;
using System.Drawing.Imaging;
namespace ManageImage
{
    class PublicParameters
    {
        //定义(类宏)------------------------------------
        public int left = 1;
        public int right = 0;
        public int IsAvailable = 1;
        public int NotAvailable = 0;
        public int selectLeftRight = 2;
        //定义(类宏)------------------------------------

        //内部参数设定--------------------------------------------
        public int zZoom = 1, xZoom = 1;
        public int skiplineControl_C = 30;
        public int ThresholdAvailableK = 6;
        public int ThresholdLostEdge = 1;
        public int initSearchRange_left1 = 157;
        public int initSearchRange_left0 = 70;
        public int initSearchRange_right1 = 90;
        public int initSearchRange_right0 = 3;
        public int defortSearchRadius = 10;//搜索半径
        public int[,] curvature = new int[2,30];
        //内部参数设定--------------------------------------------

        //显示用全局变量------------------------------------------
        public string displayData1;
        public string displayData2;
        public string displayData3;
        public string displayData4;
        public string displayData5;
        public string displayData6;
        public string displayData7;
        public int[, ,] displaySearchRange = new int[2, 2, 30];
        public int[,] displayJudgeK = new int[2, 30];
        public int[,] displayCurvature = new int[2,30];
        public string[,] displayR = new string[2,30];

        //显示用全局变量------------------------------------------

        //外部参数------------------------------------------------
        public int skipLines;
        public int ThresholdEdge;
        public int skiplineControl;
        //外部参数------------------------------------------------

        //其他----------------------------------------------------
        public Bitmap MyBitmap;
        public FileInfo[] files;
        public struct point_BTX
        {
            public int x;
            public int z;
            public int available;
        };
        public point_BTX[,] edgePoint = new point_BTX[2, 30];
        public point_BTX[,] activeEdgePoint = new point_BTX[2, 30];
        public point_BTX[,] characterPoint = new point_BTX[2, 5];
        

        public struct character
        {
            public int x;
            public int z;
            public int l1;
            public int l2;
            public int k1;
            public int k2;
            public int r;
        };
        public character pathCharacter = new character();
        public character[] pathCharacterLR = new character[2];

        public Size showSize = new Size(640, 480);
        public string Configuration;
        public int[,] searchRange = new int[2, 2];
        public int[] C_activeEdge = new int[2];
        //public int[] F_continueSearch = new int[2];
        public int[,] pixelBuffer = new int[3, 160];
        public int F_crossroad;
        public int F_curve;
        //其他----------------------------------------------------
    }
    
//===========================================================================================

    class ManageImage_Fish
    {

        public PublicParameters PP = new PublicParameters();//虚拟公共参数
        public FIFOSimulation_BTX FS = new FIFOSimulation_BTX();//虚拟FIFO

       

        //宏定义实现
        private int left = 1;
        private int right = 0;
        private int IsAvailable = 1;
        private int NotAvailable = 0;
        private int selectLeftRight = 2;

        public void ManageImage(int SelectFileNum)
        {
            InitPublicParameters();//初始化公共参数

            GetBitmapFromFile(SelectFileNum);//打开指定目录的bitmap
            FS.FIFODataFromBitmap(PP.MyBitmap);//将bitmap的数据缓存到虚拟FIFO中
            FS.ResetRead();//FIFO读重置

            SearchEdge();//搜索边沿
            GetAvtiveEdgePoint();
            AnalyseCharacter();//分析特征

            DisplayEdge();//显示边沿
            DiaplaySearchRange();//显示搜索范围
            LoadDisplayData();//载入显示数据

            
        }
        private void GetAvtiveEdgePoint()
        {
            int z;
            int LorR;

            PP.C_activeEdge[left] = 0;
            PP.C_activeEdge[right] = 0;

            for (z = 0; z < 30; z++)
            {                
                for (LorR = 0; LorR < 2; LorR++)
                {
                    if (PP.edgePoint[LorR, z].available == 1)
                    {
                        PP.activeEdgePoint[LorR, PP.C_activeEdge[LorR]].x = PP.edgePoint[LorR, z].x;
                        PP.activeEdgePoint[LorR, PP.C_activeEdge[LorR]].z = PP.edgePoint[LorR, z].z;
                        PP.C_activeEdge[LorR]++;
                    }
                }
            }
        }
        private void AnalyseCharacter()
        {
            int i;
            int LorR;
            int tmp;
            //获取第0点（最近点）
            for (LorR = 0; LorR < 2; LorR++)
            {
                PP.characterPoint[LorR, 0].x = PP.activeEdgePoint[LorR, 0].x;
                PP.characterPoint[LorR, 0].z = PP.activeEdgePoint[LorR, 0].z;
            }
            //获取第4点（最远点）
            for (LorR = 0; LorR < 2; LorR++)
            {
                tmp = PP.C_activeEdge[LorR] - 1;
                if (tmp > 0)
                {
                    PP.characterPoint[LorR, 4].x = PP.activeEdgePoint[LorR, tmp].x;
                    PP.characterPoint[LorR, 4].z = PP.activeEdgePoint[LorR, tmp].z;
                }
            }

            //获取第1点
            for (LorR = 0; LorR < 2; LorR++)
            {
                PP.characterPoint[LorR, 1] = PP.characterPoint[LorR, 4];//初值
                for (i = 0; i < 30; i++)
                {
                    if (PP.curvature[LorR, i] > 3)
                    {
                        PP.characterPoint[LorR, 1].x = PP.edgePoint[LorR, i].x;
                        PP.characterPoint[LorR, 1].z = PP.edgePoint[LorR, i].z;
                        break;
                    }
                }
            }
            //获取第2点
            for (LorR = 0; LorR < 2; LorR++)
            {
                for (i = 0; i < PP.C_activeEdge[LorR]; i++)
                {
                    if (PP.characterPoint[LorR, 1].z == PP.activeEdgePoint[LorR, i].z)
                    {
                        tmp = (i + PP.C_activeEdge[LorR]) / 2;
                        PP.characterPoint[LorR, 2].x = PP.activeEdgePoint[LorR, tmp].x;
                        PP.characterPoint[LorR, 2].z = PP.activeEdgePoint[LorR, tmp].z;
                    }
                }
            }


            //计算单边特征
            for (LorR = 0; LorR < 2; LorR++)
            {

                PP.pathCharacterLR[LorR].x = PP.characterPoint[LorR, 0].x;
                PP.pathCharacterLR[LorR].z = PP.characterPoint[LorR, 0].z;

                tmp = (PP.characterPoint[LorR, 1].z - PP.characterPoint[LorR, 0].z);
                if (tmp != 0)
                    PP.pathCharacterLR[LorR].k1 = ((PP.characterPoint[LorR, 1].x - PP.characterPoint[LorR, 0].x) * 10) / tmp;
                else
                    PP.pathCharacterLR[LorR].k1 = 0;

                tmp = (PP.characterPoint[LorR, 4].z - PP.characterPoint[LorR, 1].z);
                if (tmp != 0)
                    PP.pathCharacterLR[LorR].k2 = ((PP.characterPoint[LorR, 4].x - PP.characterPoint[LorR, 1].x) * 10) / tmp;
                else
                    PP.pathCharacterLR[LorR].k2 = 0;

                PP.pathCharacterLR[LorR].l1 = (Math.Abs((PP.characterPoint[LorR, 1].x - PP.characterPoint[LorR, 0].x) )) + ((PP.characterPoint[LorR, 1].z - PP.characterPoint[LorR, 0].z) * 5);
                PP.pathCharacterLR[LorR].l2 = (Math.Abs((PP.characterPoint[LorR, 4].x - PP.characterPoint[LorR, 1].x) )) + ((PP.characterPoint[LorR, 4].z - PP.characterPoint[LorR, 1].z) * 5);
                PP.pathCharacterLR[LorR].r = CalculateRadius3Points(
                    PP.characterPoint[LorR, 4].x * 10,
                    PP.characterPoint[LorR, 4].z,
                    PP.characterPoint[LorR, 2].x * 10,
                    PP.characterPoint[LorR, 2].z,
                    PP.characterPoint[LorR, 1].x * 10,
                    PP.characterPoint[LorR, 1].z
                    );
            }


            //计算综合特征
            PP.pathCharacter.x = Average2Nums(PP.pathCharacterLR[left].x, PP.pathCharacterLR[right].x);
            PP.pathCharacter.z = Average2Nums(PP.pathCharacterLR[left].z, PP.pathCharacterLR[right].z);
            PP.pathCharacter.k1 = Average2Nums(PP.pathCharacterLR[left].k1, PP.pathCharacterLR[right].k1);
            PP.pathCharacter.k2 = Average2Nums(PP.pathCharacterLR[left].k2, PP.pathCharacterLR[right].k2);
            PP.pathCharacter.l1 = Average2Nums(PP.pathCharacterLR[left].l1, PP.pathCharacterLR[right].l1);
            PP.pathCharacter.l2 = Average2Nums(PP.pathCharacterLR[left].l2, PP.pathCharacterLR[right].l2);
            PP.pathCharacter.r = Average2Nums(PP.pathCharacterLR[left].r, PP.pathCharacterLR[right].r);
        }
        private int Average2Nums(int num1, int num2)
        {
            return (num1 + num2) / 2;
        }


        private void SearchEdge()
        {
            //定义局部变量
            int x = 0, z = 0;
            int tmp;
            int LorR = 0;
            int[] C_continueLostEdge = new int[selectLeftRight];
            int[] C_continuousEdge = new int[selectLeftRight];
            int[] C_curveJudge = new int[selectLeftRight];
            int[] C_crossroadJudge = new int[selectLeftRight];
            int[] C_activePoint = new int[selectLeftRight];
            int[] C_giveupThisEdge = new int[selectLeftRight];

            int F_giveupThisImage = 0;
            int[] F_giveupThisEdge = new int[selectLeftRight];
            int[] F_giveupReborn = new int[selectLeftRight];
            //初始化局部变量
            for (LorR = 0; LorR < 2; LorR++)//左右各处理一次
            {
                C_continueLostEdge[LorR] = 0;
                C_continuousEdge[LorR] = 0;
                C_activePoint[LorR] = 0;
                C_curveJudge[LorR] = 0;
                C_crossroadJudge[LorR] = 0;
                C_giveupThisEdge[LorR] = 0;
                F_giveupThisEdge[LorR] = 0;
                F_giveupReborn[LorR] = 0;
                InitSearchRange(LorR);
            }

            //丢弃拍到车头的图像
            FS.SkipLines(PP.skipLines);

            for (z = 0; z < 30; z++)
            {
                for (x = 0; x < 160; x++) PP.pixelBuffer[(z % 3), x] = FS.ReadPinxel();//从模拟FIFO读数据
                FS.SkipLines((PP.skiplineControl_C - z) / PP.skiplineControl);//每读取一行就要跳过几行，且近大远小

                //边沿检测
                for (LorR = 0; LorR < 2; LorR++)
                {
                    if (F_giveupThisEdge[LorR] == 1) continue;
                    if (SearchEdgeNextLine(LorR, z) == IsAvailable)
                    {
                        C_continueLostEdge[LorR] = 0;
                        C_continuousEdge[LorR]++;
                        C_activePoint[LorR]++;
                    }
                    else
                    {
                        C_continueLostEdge[LorR]++;
                        C_continuousEdge[LorR] = 0;
                    }
                }

                //校验
                for (LorR = 0; LorR < 2; LorR++)
                {
                    if (F_giveupThisEdge[LorR] == 1)
                    {
                        C_giveupThisEdge[LorR]++;
                        continue;
                    }

                    if (C_continuousEdge[LorR] >= 3)//当连续检测到三个边沿点时，对第三个边沿点进行有效性验算
                    {
                        if (JudgeAvailability(LorR, z) == NotAvailable)
                        {
                            PP.edgePoint[LorR, z].x = 0;
                            PP.edgePoint[LorR, z].z = 0;
                            PP.edgePoint[LorR, z].available = 0;
                            InitSearchRange(LorR);

                            C_continueLostEdge[LorR]++;
                            C_continuousEdge[LorR] = 0;
                            C_activePoint[LorR]--;

                            F_giveupThisEdge[LorR] = 1;

                        }
                    }
                    if (C_continueLostEdge[LorR] >= 1)
                    {
                        if (z > 6)
                        {
                            F_giveupThisEdge[LorR] = 1;//放弃边沿
                        }
                    }

                    if (C_activePoint[LorR] > 6)
                    {
                        tmp = CalculateCurvature(LorR, z);
                        if (tmp >=5) F_giveupReborn[LorR] = 1;
                        PP.curvature[LorR, z] = tmp;
                        PP.displayCurvature[LorR, z] = tmp;
                    }
                }

                //边沿复活
                for (LorR = 0; LorR < 2; LorR++)
                {
                    if (F_giveupReborn[LorR] == 0)
                    {
                        if (z < 25)
                        {
                            if (C_giveupThisEdge[LorR] > 3)
                            {
                                if (SearchEdgeWhenLostEdge(LorR, z) == IsAvailable)
                                {
                                    C_continueLostEdge[LorR] = 0;
                                    C_continuousEdge[LorR] = 2;
                                    C_activePoint[LorR] += 2;
                                    C_giveupThisEdge[LorR] = 0;
                                    F_giveupThisEdge[LorR] = 0;
                                }
                            }
                        }
                    }
                }
              
                /* if (F_giveupThisEdge[left] == 1)
                 {
                     if (F_giveupThisEdge[right] == 1) break;
                 }*/
            }
            DeleteIndividualPoint(left);
            DeleteIndividualPoint(right);
        }
        private void DeleteIndividualPoint(int LorR)
        {
            int i;
            if (PP.edgePoint[LorR, 1].available == 0) PP.edgePoint[LorR, 0].available = 0;
            if (PP.edgePoint[LorR, 28].available == 0) PP.edgePoint[LorR, 29].available = 0;
            for(i=1;i<29;i++) 
            {
                if (PP.edgePoint[LorR, (i - 1)].available == 0)
                {
                    if (PP.edgePoint[LorR, (i + 1)].available == 0)
                        PP.edgePoint[LorR, i].available = 0;
                }
            }
        }
        //丢失边沿后搜索边沿
        private int SearchEdgeWhenLostEdge(int LorR, int z)
        {
            int i, x;
            int tmp1 = 0;
            int p;
            int tmpI;
            int[] tmpEdge1 = { 0, 0 };
            int[] tmpEdge2 = { 160, 160 };
            int counter = 0;

            int T = 4;
            if (LorR == left)
            {
                p = (-1);
                SetSearchRangeP(left, 80 + 50, 80 - 30+(30-z));
            }
            else
            {
                p = 1;
                SetSearchRangeP(right, 80 + 30 - (30 - z), 80 - 50);
            }

            tmpI = PP.searchRange[LorR, 1] - PP.searchRange[LorR, 0];//依据搜索范围计算循环次数
            for (i = 0; i < tmpI; i++)
            {
                if (LorR == left) x = PP.searchRange[left, 0] + i;
                else x = PP.searchRange[right, 1] - i;

                tmp1 = FS.CalculateGradient(PP.pixelBuffer[(z % 3), (x + 1)], PP.pixelBuffer[(z % 3), (x - 1)]);//横向领域梯度
                tmp1 *= p;
                if (tmp1 > PP.ThresholdEdge)//判断边缘
                {
                    tmpEdge1[counter] = x;
                    counter++;
                    if (counter == 1) break;
                }
            }

            counter = 0;
            for (i = 0; i < tmpI; i++)
            {
                if (LorR == left) x = PP.searchRange[left, 0] + i;
                else x = PP.searchRange[right, 1] - i;

                tmp1 = FS.CalculateGradient(PP.pixelBuffer[((z - 1) % 3), (x + 1)], PP.pixelBuffer[((z - 1) % 3), (x - 1)]);//横向领域梯度
                tmp1 *= p;
                if (tmp1 > PP.ThresholdEdge)//判断边缘
                {
                    tmpEdge2[counter] = x;
                    counter++;
                    if (counter == 1) break;
                }
            }
            if ((Math.Abs(tmpEdge1[0] - tmpEdge2[1])) < T)
            {
                CalculateX(LorR, z, tmpEdge1[0], tmpEdge2[1]);
                return IsAvailable;
            }
            if ((Math.Abs(tmpEdge1[0] - tmpEdge2[0])) < T)
            {
                CalculateX(LorR, z, tmpEdge1[0], tmpEdge2[0]);
                return IsAvailable;
            }
            if ((Math.Abs(tmpEdge1[1] - tmpEdge2[1])) < T)
            {
                CalculateX(LorR, z, tmpEdge1[1], tmpEdge2[1]);
                return IsAvailable;
            }
            if ((Math.Abs(tmpEdge1[1] - tmpEdge2[0])) < T)
            {
                CalculateX(LorR, z, tmpEdge1[1], tmpEdge2[0]);
                return IsAvailable;
            }
            return NotAvailable;
        }
        //这个函数仅仅为SearchEdgeWhenLostEdge（）服务
        private void CalculateX(int LorR,int z, int x1 ,int x2)
        {
            PP.edgePoint[LorR, z].x = x1;
            PP.edgePoint[LorR, z].z = z;
            PP.edgePoint[LorR, z].available = IsAvailable;
            PP.edgePoint[LorR, z - 1].x = x2;
            PP.edgePoint[LorR, z - 1].z = z - 1;
            PP.edgePoint[LorR, z - 1].available = IsAvailable;
            SetSearchRangeBR(LorR, x1, PP.defortSearchRadius);
        }



        //计算边沿线的曲率，注意：一定要保证有效点数量大于等于3
        private int CalculateCurvature(int LorR, int z)
        {
            int i;

            int summationDifferenceK = 0;
            int[] bufferX = new int[30];
            int[] bufferZ = new int[30];
            int C_active = 0;

        
            for (i = 0; i <= z; i++)
            {
                if (PP.edgePoint[LorR, i].available == IsAvailable)
                {
                    bufferX[C_active] = PP.edgePoint[LorR, i].x;
                    bufferZ[C_active] = PP.edgePoint[LorR, i].z;
                    C_active++;
                }
            }

            if (C_active > 0)
            {
                i = C_active - 1;
                summationDifferenceK = CalculateRadius3Points(5 * bufferX[i], bufferZ[i], 5 * bufferX[i / 2], bufferZ[i / 2], 5 * bufferX[0], bufferZ[0]);


                PP.displayR[LorR, z] = z.ToString() + ":{";
                PP.displayR[LorR, z] += "[" + bufferX[i].ToString() + ",";
                PP.displayR[LorR, z] += bufferZ[i].ToString() + "]";
                PP.displayR[LorR, z] += "[" + bufferX[i / 2].ToString() + ",";
                PP.displayR[LorR, z] += bufferZ[i / 2].ToString() + "] ";
                PP.displayR[LorR, z] += "[" + bufferX[0].ToString() + ",";
                PP.displayR[LorR, z] += bufferZ[0].ToString() + "]" + "}-->";
                PP.displayR[LorR, z] += summationDifferenceK.ToString();

            }
            return summationDifferenceK;
        }

        //获取Bitmap
        private void GetBitmapFromFile(int SelectFileNum)
        {
            PP.MyBitmap = new Bitmap(PP.files[SelectFileNum].FullName);
            PP.MyBitmap.RotateFlip(RotateFlipType.Rotate180FlipX);
            PP.MyBitmap.RotateFlip(RotateFlipType.Rotate180FlipY);
        }

        //显示边沿
        private void DisplayEdge()
        {
            int x = 0, z = 0;
            int i;
            int shiftZ = 0;
            Color EdgeColor;

            shiftZ = 0;
            EdgeColor = Color.FromArgb(255, Color.Red);
            for (i = 0; i < 30; i++)
            {
                if (PP.edgePoint[left, i].available == IsAvailable)
                {
                    x = PP.edgePoint[left, i].x;
                    z = PP.edgePoint[left, i].z + shiftZ;
                    PP.MyBitmap.SetPixel(x, z + PP.skipLines, EdgeColor);

                }
                shiftZ += (PP.skiplineControl_C - i) / PP.skiplineControl;
            }

            shiftZ = 0;
            EdgeColor = Color.FromArgb(255, Color.Blue);
            for (i = 0; i < 30; i++)
            {
                if (PP.edgePoint[right, i].available == IsAvailable)
                {
                    x = PP.edgePoint[right, i].x;
                    z = PP.edgePoint[right, i].z + shiftZ;

                    PP.MyBitmap.SetPixel(x, z + PP.skipLines, EdgeColor);

                }
                shiftZ += (PP.skiplineControl_C - i) / PP.skiplineControl;
            }
        }

        //显示搜索范围
        private void DiaplaySearchRange()
        {
            int z = 0;
            int shiftZ = 0;
            Color rangeColor_left1 = Color.FromArgb(255, Color.Gold);
            Color rangeColor_left0 = Color.FromArgb(255, Color.Yellow);
            Color rangeColor_right1 = Color.FromArgb(255, Color.Orange);
            Color rangeColor_right0 = Color.FromArgb(255, Color.LightYellow);


            for (z = 0; z < 30; z++)
            {
                PP.MyBitmap.SetPixel(PP.displaySearchRange[left, 0, z], z + PP.skipLines + shiftZ, rangeColor_left0);
                PP.MyBitmap.SetPixel(PP.displaySearchRange[left, 1, z], z + PP.skipLines + shiftZ, rangeColor_left1);
                PP.MyBitmap.SetPixel(PP.displaySearchRange[right, 0, z], z + PP.skipLines + shiftZ, rangeColor_right0);
                PP.MyBitmap.SetPixel(PP.displaySearchRange[right, 1, z], z + PP.skipLines + shiftZ, rangeColor_right1);

                shiftZ += (PP.skiplineControl_C - z) / PP.skiplineControl;
            }

        }

         //显示特征线
       /* private void DiaplayCalculatorLine()
        {
            int[] pointX =  new int[5];       
            int[] pointZ =  new int[5];
            Color Color_CalculatorLine = Color.FromArgb(255, Color.Green);
            point[0] = PP.pathCharacter.x
            PP.MyBitmap.SetPixel(
        }*/

        //初始化全局公共参数（图像处理）
        private void InitPublicParameters()
        {
            int i, j;
            int LorR;
            for (i = 0; i < PP.edgePoint.GetLength(0); i++)
            {
                for (j = 0; j < PP.edgePoint.GetLength(1); j++)
                {
                    PP.edgePoint[i, j].x = 0;
                    PP.edgePoint[i, j].z = 0;
                    PP.edgePoint[i, j].available = 0;
                }
            }
            for (i = 0; i < 30; i++)
            {
                for (LorR = 0; LorR < 2; LorR++)
                {
                    PP.activeEdgePoint[LorR, i].x = 0;
                    PP.activeEdgePoint[LorR, i].z = 0;
                }
            }

            for (i = 0; i < 5; i++)
            {
                PP.characterPoint[left, i].x = 0;
                PP.characterPoint[left, i].z = 0;
                PP.characterPoint[left, i].available = 0;
                PP.characterPoint[right, i].x = 0;
                PP.characterPoint[right, i].z = 0;
                PP.characterPoint[right, i].available = 0;
            }

            for (i = 0; i < 30; i++)
            {
                PP.displaySearchRange[right, 0, i] = 0;
                PP.displaySearchRange[right, 1, i] = 0;
                PP.displaySearchRange[left, 0, i] = 0;
                PP.displaySearchRange[left, 1, i] = 0;
            }
            for (i = 0; i < 30; i++)
            {
                PP.displayJudgeK[left, i] = 0;
                PP.displayJudgeK[right, i] = 0;
            }

            for (i = 0; i < 30; i++)
            {
                PP.curvature[left, i] = 0;
                PP.curvature[right, i] = 0;
                PP.displayCurvature[left, i] = 0;
                PP.displayCurvature[right, i] = 0;
            }
            for (i = 0; i < 30; i++)
            {
                PP.displayR[left ,i] = "";
                PP.displayR[right,i] = "";
            }
            for (i = 0; i < 5; i++)
            {
                PP.characterPoint[left, i].x = 0;
                PP.characterPoint[left, i].z = 0;
                PP.characterPoint[right, i].x = 0;
                PP.characterPoint[right, i].z = 0;
            }
            //读取外部参数
           
            PP.F_crossroad = 0;//十字路口标志位复位
            PP.F_curve = 0;//弯道标志位复位

            //清空有效边沿计数
            PP.C_activeEdge[left] = 0;
            PP.C_activeEdge[right] = 0;

            //PP.F_continueSearch[left] = 1;
            //PP.F_continueSearch[right] = 1;
        }

        //三点 算半径(圆弧程度)
        private int CalculateRadius3Points(int x1, int z1, int x2, int z2, int x3, int z3)
        {
            int Dx1, Dz1;
            int Dx2, Dz2;

            Dx1 = x1 - x2;
            Dx1 *= PP.xZoom;
            Dz1 = Math.Abs(z1 - z2);
            Dz1 *= PP.zZoom;

            Dx2 = x2 - x3;
            Dx2 *= PP.xZoom;
            Dz2 = Math.Abs(z2 - z3);
            Dz2 *= PP.zZoom;

            if (Dz1 == 0) return 0;
            if (Dz2 == 0) return 0;
            return ((Dx1 * Dz2) - (Dx2 * Dz1)) / (Dz1 * Dz2);
        }

        //依据前三个有效点判定当前点是否有效
        private int JudgeAvailability(int LorR, int z)
        {
            int tmp;
            tmp = CalculateRadius3Points(
                    PP.edgePoint[LorR, z].x,
                    PP.edgePoint[LorR, z].z,
                    PP.edgePoint[LorR, z - 1].x,
                    PP.edgePoint[LorR, z-1].z,
                    PP.edgePoint[LorR, z - 2].x,
                    PP.edgePoint[LorR, z-2].z
                );

            tmp = Math.Abs(tmp);

            PP.displayJudgeK[LorR, z] = tmp;

            if (tmp > PP.ThresholdAvailableK)
            {
                return NotAvailable;
            }
            else return IsAvailable;
        }

        //根据已有基点检测下一行的边沿点
        private int SearchEdgeNextLine(int LorR, int z)
        {
            int i, x;
            int tmp1 = 0;
            int p;
            int tmpI;

            if (LorR == left) p = (-1);
            else p = 1;

            tmpI = PP.searchRange[LorR, 1] - PP.searchRange[LorR, 0];//依据搜索范围计算循环次数
            for (i = 0; i < tmpI; i++)
            {
                if (LorR == left) x = PP.searchRange[left, 0] + i;
                else x = PP.searchRange[right, 1] - i;

                tmp1 = FS.CalculateGradient(PP.pixelBuffer[(z % 3), (x + 1)], PP.pixelBuffer[(z % 3), (x - 1)]);//横向领域梯度
                tmp1 *= p;
                if (tmp1 > PP.ThresholdEdge)//判断边缘
                {
                    PP.edgePoint[LorR, z].x = x;
                    PP.edgePoint[LorR, z].z = z;
                    PP.edgePoint[LorR, z].available = IsAvailable;
                    PP.C_activeEdge[LorR]++;

                    SetSearchRangeBR(LorR, x, PP.defortSearchRadius);
                    PP.displaySearchRange[LorR, 0, z] = PP.searchRange[LorR, 0];
                    PP.displaySearchRange[LorR, 1, z] = PP.searchRange[LorR, 1];
                    return IsAvailable;
                }
            }
            return NotAvailable;
        }

        //从中间向两侧搜做边沿
        private int SearchEdgeFromMid(int z)
        {
            int C_available = 0;

            PP.searchRange[left, 0] = 80 - 20;
            PP.searchRange[left, 1] = 80 + 40;
            C_available += SearchEdgeNextLine(left, z);

            PP.searchRange[right, 0] = 80 - 40;
            PP.searchRange[right, 1] = 80 + 20;
            C_available += SearchEdgeNextLine(right, z);

            //if (C_available == 2) return IsAvailable;
            return NotAvailable;
        }

        //初始化搜索范围
        private void InitSearchRange(int LorR)
        {
            if (LorR == left)
            {
                SetSearchRangeP(LorR, PP.initSearchRange_left1, PP.initSearchRange_left0);
            }
            else
            {
                SetSearchRangeP(LorR, PP.initSearchRange_right1, PP.initSearchRange_right0);
            }
        }

        //设定搜索范围(绝对坐标)
        private void SetSearchRangeP(int LorR,int X1,int X0)
        {
            if (LorR == left)
            {
                PP.searchRange[LorR, 0] = X0;
                PP.searchRange[LorR, 1] = X1;
            }
            else
            {
                PP.searchRange[LorR, 0] = X0;
                PP.searchRange[LorR, 1] = X1;
            }
        }

        //设定搜索范围(基点+半径)
        private void SetSearchRangeBR(int LorR, int basePoint, int range)
        {
            PP.searchRange[LorR, 0] = basePoint - range;

            PP.searchRange[LorR, 1] = basePoint + range;

            if (PP.searchRange[right, 0] < 3) PP.searchRange[right, 0] = 0;
            if (PP.searchRange[left, 1] > 157) PP.searchRange[left, 1] = 157;
        }

        private void LoadDisplayData()
        {
            int[] shiftZ = new int[30];
            int tmp = 0;
            int i;
            int LorR;

            PP.displayData1 = "";
            PP.displayData2 = "";
            PP.displayData3 = "";
            PP.displayData4 = "";
            PP.displayData5 = "";
            PP.displayData6 = "";
            PP.displayData7 = "";

            for (i = 2; i < 30; i++)
            {
                shiftZ[i] = tmp + PP.skipLines;
                tmp += (PP.skiplineControl_C - i) / PP.skiplineControl;
            }
            for (i = 29; i >= 0; i--)
            {
                PP.displayData1 += "i=" + i.ToString() + "|";

                PP.displayData1 += "Xl=" + PP.edgePoint[PP.left, i].x.ToString() + " ";
                PP.displayData1 += "Zl=" + PP.edgePoint[PP.left, i].z.ToString() + "+" + shiftZ[i].ToString() + "=" + (PP.edgePoint[left, i].z + shiftZ[i]).ToString() + " ";
                PP.displayData1 += "A=" + PP.edgePoint[PP.left, i].available.ToString();

                PP.displayData1 += "|";

                PP.displayData1 += "Xr=" + PP.edgePoint[PP.right, i].x.ToString() + " ";
                PP.displayData1 += "Zr=" + PP.edgePoint[PP.right, i].z.ToString() + "+" + shiftZ[i].ToString() + "=" + (PP.edgePoint[left, i].z + shiftZ[i]).ToString() + " ";
                PP.displayData1 += "A=" + PP.edgePoint[PP.right, i].available.ToString();

                PP.displayData1 += "|";

                PP.displayData1 += "SR=[" + PP.displaySearchRange[PP.left, 1, i] + " "
                                            + PP.displaySearchRange[PP.left, 0, i] + " "
                                            + PP.displaySearchRange[PP.right, 1, i] + " "
                                            + PP.displaySearchRange[PP.right, 0, i] + "]";
                PP.displayData1 += "\n";
            }

            for (i = 29; i >= 0; i--)
            {
                PP.displayData2 += i.ToString() + " ";
                PP.displayData2 += "L=" + PP.displayJudgeK[left, i] + " ";
                PP.displayData2 += "R=" + PP.displayJudgeK[right, i] + " ";
                PP.displayData2 += "\n";
            }

            for (i = 29; i >= 0; i--)
            {
                PP.displayData3 += i.ToString()+" ";
                PP.displayData3 += PP.displayCurvature[left, i] + " " + PP.displayCurvature[right, i];
                PP.displayData3 += "\n";
            }
            for (i = 29; i >= 0; i--)
            {
                PP.displayData4 += PP.displayR[left,i];
                PP.displayData4 += "(left)";
                PP.displayData4 += "  ";
                PP.displayData4 += PP.displayR[right, i];
                PP.displayData4 += "(right)";
                PP.displayData4 += "\n";
            }
            for (i = 4; i >= 0; i--)
            {
                PP.displayData5 += i.ToString() + ":";
                PP.displayData5 += "(" + PP.characterPoint[left, i].x.ToString()+ ","+ PP.characterPoint[left, i].z.ToString()+") ";
                PP.displayData5 += "(" + PP.characterPoint[right, i].x.ToString() + "," + PP.characterPoint[right, i].z.ToString() + ") ";
                PP.displayData5 += "\n";
            }
            for (i = 0; i < 1; i++)
            {
                for (LorR = 0; LorR < 2; LorR++)
                {
                    PP.displayData6 += LorR.ToString() + "\n";
                    PP.displayData6 += "x=" + PP.pathCharacterLR[LorR].x.ToString()+ "\n";
                    PP.displayData6 += "z=" + PP.pathCharacterLR[LorR].z.ToString() + "\n";
                    PP.displayData6 += "l1=" + PP.pathCharacterLR[LorR].l1.ToString() + "\n";
                    PP.displayData6 += "l2=" + PP.pathCharacterLR[LorR].l2.ToString() + "\n";
                    PP.displayData6 += "k1=" + PP.pathCharacterLR[LorR].k1.ToString() + "\n";
                    PP.displayData6 += "k2=" + PP.pathCharacterLR[LorR].k2.ToString() + "\n";
                    PP.displayData6 += "r=" + PP.pathCharacterLR[LorR].r.ToString()  + "\n" + "\n";
                }

                PP.displayData6 +="x = "+ PP.pathCharacter.x.ToString()+ "\n";
                PP.displayData6 += "z = " + PP.pathCharacter.z.ToString() + "\n";
                PP.displayData6 += "l1= " + PP.pathCharacter.l1.ToString()+ "\n";
                PP.displayData6 += "l2= " + PP.pathCharacter.l2.ToString() +"\n";
                PP.displayData6 += "k1= " + PP.pathCharacter.k1.ToString() + "\n";
                PP.displayData6 += "k2= " + PP.pathCharacter.k2.ToString() + "\n";
                PP.displayData6 += "r = " + PP.pathCharacter.r.ToString() + "\n";
            }
            PP.displayData7 += "leftActivePoint :"+ PP.C_activeEdge[left].ToString();
            PP.displayData7 += "\n";
            PP.displayData7 += "rightActivePoint:" + PP.C_activeEdge[right].ToString();
            PP.displayData7 += "\n";
            for (i = 29; i >= 0; i--)
            {
                PP.displayData7 += "(" + PP.activeEdgePoint[left, i].x.ToString() + "," + PP.activeEdgePoint[left, i].z.ToString() + ") ";
                PP.displayData7 += "(" + PP.activeEdgePoint[right, i].x.ToString() + "," + PP.activeEdgePoint[right, i].z.ToString() + ") ";
                PP.displayData7 += "\n";
            }
        }
    }


}
