using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;
using System.Windows.Forms;



namespace MicroSD
{
    /// <summary>
    /// zgke@sina.com
    /// 2008-09-26
    /// 磁盘扇区
    /// </summary>
    public class Driver
    {
        private const uint GENERIC_READ = 0x80000000;
        private const uint GENERIC_WRITE = 0x40000000;
        
        private const uint FILE_SHARE_READ = 0x00000001;
        private const uint FILE_SHARE_WRITE = 0x00000002;

        private const uint OPEN_EXISTING = 3;         

        [DllImport("kernel32.dll", SetLastError = true)]
        private static extern SafeFileHandle CreateFileA(string lpFileName, uint dwDesiredAccess, uint dwShareMode, IntPtr lpSecurityAttributes, uint dwCreationDisposition, uint dwFlagsAndAttributes, IntPtr hTemplateFile);
   
       
        private System.IO.FileStream _DirverStream;
        private long _SectorLength = 0;       
        private SafeFileHandle _DirverHandle;

        /// <summary>
        /// 扇区数
        /// </summary>
        public long SectorLength { get { return _SectorLength; } } 

        /// <summary>
        /// 获取磁盘扇区信息
        /// </summary>
        /// <param name="DirverName">G:</param>
        public Driver(string DirverName)
        {
            if (DirverName == null && DirverName.Trim().Length == 0) return;
            _DirverHandle = CreateFileA("\\\\.\\" + DirverName.Trim(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, IntPtr.Zero, OPEN_EXISTING, 0, IntPtr.Zero);
            _DirverStream = new System.IO.FileStream(_DirverHandle, System.IO.FileAccess.ReadWrite);

            GetSectorCount();
        }


        /// <summary>
        /// 扇区显示转换
        /// </summary>
        /// <param name="SectorBytes">扇区 长度512</param>
        /// <returns>EB 52 90 ......55 AA</returns>
        public string GetString(byte[] SectorBytes)
        {
            if (SectorBytes.Length != 512) return "";
            StringBuilder ReturnText = new StringBuilder();

            int RowCount = 0;
            for (int i = 0; i != 512; i++)
            {
                ReturnText.Append(SectorBytes[i].ToString("X02") + " ");

                if (RowCount == 15)
                {
                    ReturnText.Append("/r/n");
                    RowCount = -1;
                }

                RowCount++;
            }

            return ReturnText.ToString();

        }               
        /// <summary>
        /// 获取扇区数
        /// </summary>
        private void GetSectorCount()
        {
            if (_DirverStream == null) return;
            _DirverStream.Position = 0;

            byte[] ReturnByte = new byte[512];
            _DirverStream.Read(ReturnByte, 0, 512); //获取第1扇区
          
            if (ReturnByte[0] == 0xEB && ReturnByte[1] == 0x58)           //DOS的好象都是32位
            {
                _SectorLength = (long)BitConverter.ToInt32(new byte[] { ReturnByte[32], ReturnByte[33], ReturnByte[34], ReturnByte[35] }, 0);
            }
            if (ReturnByte[0] == 0xEB && ReturnByte[1] == 0x52)          //NTFS好象是64位
            {
                _SectorLength = BitConverter.ToInt64(new byte[] { ReturnByte[40], ReturnByte[41], ReturnByte[42], ReturnByte[43], ReturnByte[44], ReturnByte[45], ReturnByte[46], ReturnByte[47] }, 0);
            }

        }      

      
        /// <summary>
        /// 读一个扇区
        /// </summary>
        /// <param name="SectorIndex">扇区号</param>
        /// <returns>如果扇区数字大于分区信息的扇区数 返回NULL</returns>
        public byte[] ReadSector(long SectorIndex)
        {           
            if (SectorIndex > _SectorLength) return null;
            _DirverStream.Position = SectorIndex * 512;           
            byte[] ReturnByte = new byte[512];
            _DirverStream.Read(ReturnByte, 0, 512); //获取扇区
            return ReturnByte;
        }
        /// <summary>
        /// 写入数据
        /// </summary>
        /// <param name="SectorBytes">扇区 长度512</param>
        /// <param name="SectorIndex">扇区位置</param>
        public void WritSector(byte[] SectorBytes, long SectorIndex)
        {
            if (SectorBytes.Length != 512) return;
            if (SectorIndex > _SectorLength) return;
            _DirverStream.Position = SectorIndex * 512;
            _DirverStream.Write(SectorBytes, 0, 512); //写入扇区  
        }
        
        
        /// <summary>
        /// 关闭
        /// </summary>
        public void Close()
        {
            _DirverStream.Close();
        }    

    }

}

/*
Zgke.DriverLoader T = new Zgke.DriverLoader("E:");

            byte[] Temp = T.ReadSector(0);

            MessageBox.Show(T.GetString(Temp));
*/