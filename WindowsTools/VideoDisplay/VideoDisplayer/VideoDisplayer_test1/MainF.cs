using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Imaging;

namespace VideoDisplayer_test1
{
    public partial class MainF : Form
    {
        
        Bitmap testBitmap;
        public MainF()
        {
            InitializeComponent();
           
        }
        public int widthVideo = 160;
        public int heightVideo = 120;
        public int zoomRate = 2;
        Size mysize ;
        private void Form1_Load(object sender, EventArgs e)
        {
            pictureBox2.Load(@".\white.bmp");
            mysize = new Size(widthVideo, heightVideo);
            testBitmap = new Bitmap(pictureBox2.Image, mysize);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            SwitchSerialPort1();
        }
        private void SwitchSerialPort1()
        {
            try
            {
                if (serialPort1.IsOpen == true)
                {
                    serialPort1.Close();
                    button2.Text = "OPEN";
                    label2.BackColor = Color.Red;
                    label2.Text = "串口已关闭";
                    button1.Enabled = false;
                    button3.Enabled = false;
                }
                else
                {
                    serialPort1.PortName = "COM" + Convert.ToInt32(textBox_com.Text);
                    serialPort1.BaudRate = Convert.ToInt32(comboBox_baudrate.Text);
                    serialPort1.Open();
                    button2.Text = "CLOSE";
                    label2.Text = "串口已开启";
                    label2.BackColor = Color.Green;
                    button1.Enabled = true;
                    button3.Enabled = true;

                }
            }
            catch
            {
                MessageBox.Show("串口打开发生了悲剧！", "Error");
            }
        }
        int x = 0, y = 0;
        Color pixelColor = new Color();

        byte[] bufferUart = new byte[40000];
        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            serialPort1.Read(bufferUart, 0, serialPort1.ReceivedBytesThreshold);
            this.BeginInvoke(new mydele(serialport_ManageData));
        }
        private delegate void mydele();
        private void serialport_ManageData()
        {
            serialport_dataread();
            serialPort1.DiscardInBuffer();
            if (checkBox3.Checked == true) serialPort1.Write(UartBuffT, 0, 1);
        }
        private void serialport_dataread()
        {
            int i;
            button1.Enabled = false;
            x = 0;
            y = 0;
            progressBar1.Maximum = serialPort1.ReceivedBytesThreshold;//进度条控制
            for (i = 0; i < serialPort1.ReceivedBytesThreshold; i++)
            {
                if (bufferUart[i] == 0xff)
                {
                    x = 0;
                    y++;
                    if (y >= testBitmap.Height) y = 0;
                }
                else
                {
                    if (bufferUart[i] < 0xfd)
                    {
                        pixelColor = Color.FromArgb((0xff - bufferUart[i]), Color.Black);

                        testBitmap.SetPixel(x, y, pixelColor);
                        x++;
                        if (x >= testBitmap.Width) x = 0;
                    }
                }
                progressBar1.Value = i;//进度条控制
            }

            if (checkBox1.Checked == true) testBitmap.RotateFlip(RotateFlipType.Rotate180FlipY);
            if (checkBox2.Checked == true) testBitmap.RotateFlip(RotateFlipType.Rotate180FlipX);


            pictureBox2.Image = testBitmap;

            pictureBox2.SizeMode = PictureBoxSizeMode.StretchImage;
           
            pictureBox2.Size = new Size(widthVideo * zoomRate, heightVideo * zoomRate);

            button1.Enabled = true;
        }


        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            testBitmap.Dispose();
        }
        byte[] UartBuffT = {255};
        private void button1_Click(object sender, EventArgs e)
        {
            //button1.Enabled = false;
            serialPort1.Write(UartBuffT, 0, 1);
        }
        private void timer_delay_Tick(object sender, EventArgs e)
        {
            C_delay++;
            if (C_delay > 2)
            {
                
                timer_delay.Dispose();
                timer_delay.Enabled = false;
            }
        }
        //清除串口缓存
        private void button3_Click(object sender, EventArgs e)
        {
            serialPort1.DiscardInBuffer();
        }

        //保存图像
        public string filename;
        private void button4_Click(object sender, EventArgs e)
        {
            SaveBitmap();
        }
        void SaveBitmap()
        {
            filename = textBox1.Text + @"\" + DateTime.Now.Year + DateTime.Now.Month + DateTime.Now.Day 
                       + DateTime.Now.Hour + DateTime.Now.Minute + DateTime.Now.Second + ".bmp";
            testBitmap.Save(filename, testBitmap.RawFormat);
            
        }
        //图像分析
        private void button5_Click(object sender, EventArgs e)
        {
            analyse analyseForm = new analyse();
            analyseForm.Show();
            //analyseForm.textBox1.Text = filename;
            
        }

        private void button6_Click(object sender, EventArgs e)
        {
             folderBrowserDialog1.ShowDialog();
             textBox1.Text = folderBrowserDialog1.SelectedPath;
            
        }

        private void button7_Click(object sender, EventArgs e)
        {
            int i;
            progressBar1.Maximum = serialPort1.ReceivedBytesThreshold;
            richTextBox1.Text = "";
            string tmp_w = "";
            for (i = 0; i < serialPort1.ReceivedBytesThreshold; i++)
            {

                progressBar1.Value = i;
                if (bufferUart[i] < 100) tmp_w += " ";
                if (bufferUart[i] < 10) tmp_w += " ";
                tmp_w += bufferUart[i] + " ";
            }
            richTextBox1.Text = tmp_w;
        }
        int C_delay = 0;
        

    }
}
