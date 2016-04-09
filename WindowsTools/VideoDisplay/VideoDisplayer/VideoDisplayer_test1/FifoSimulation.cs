using System;
using System.Collections.Generic;
using System.Linq;
using System.Drawing;
using System.Windows.Forms;



namespace FIFOSimulation
{
    public  class FIFOSimulation_BTX
    {
        int[] data = new int[393216];
        int readPointer;
        int writePointer;
       
        public int CalculateGradient(int start, int end)
        {
            return (start - end);
        }

        //将bitmap中的像素的灰度值导入FIFO缓冲器（模拟摄像头数据存入FIFO）
        public void FIFODataFromBitmap(Bitmap photo)
        {
            int x, z;
            for (z = 0; z < 120; z++)
            {
                for (x = 0; x < 160; x++)
                {
                    data[(z*160+x)]= 255- photo.GetPixel(x, z).A;
                }
            }
            readPointer = 0;
        }

        //从fifo中读取一个像素的灰度
        public int ReadPinxel()
        {
            int tmp;
            tmp = data[readPointer];
            readPointer++;
            return tmp;
        }

        //跳过像素
        public void SkipPixels(int n)
        {
            readPointer += n;
        }

        //跳过行
        public void SkipLines(int n)
        {
            readPointer += n * 160;
        }
        public void ResetRead()
        {
            readPointer = 0;
        }
    }
}
