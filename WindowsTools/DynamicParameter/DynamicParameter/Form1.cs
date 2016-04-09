using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace DynamicParameter
{
    public partial class Form1 : Form
    {
        Graphics MyGraphic;
        int[] dynamicData = new int[1000];
        int pointer = 0;

        public Form1()
        {
            InitializeComponent();
            pointer = 0;

            MyGraphic = pictureBox1.CreateGraphics();
            richTextBox1.Text = "a" + "\t" + "spd" + "\t" + "svro";
           
        }


        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                if (serialPort1.IsOpen == true)
                {
                    serialPort1.Close();
                    button2.Text = "OPEN";
                    label2.BackColor = Color.Red;
                    label2.Text = "串口已关闭";
                }
                else
                {
                    serialPort1.PortName = "COM" + Convert.ToInt32(textBox_com.Text);
                    serialPort1.BaudRate = Convert.ToInt32(comboBox_baudrate.Text);
                    serialPort1.Open();
                    button2.Text = "CLOSE";
                    label2.Text = "串口已开启";
                    label2.BackColor = Color.Green;
                }
            }
            catch
            {
                MessageBox.Show("串口打开发生了悲剧！", "Error");
            }
        }

        
        Pen mainLine = new Pen(Brushes.Red, 1);
        Pen speedLine = new Pen(Brushes.Yellow, 1);
        Pen sevroLine = new Pen(Brushes.Blue, 1);


        Pen midLine = new Pen(Brushes.Green, 1);
        Pen backgroundLine = new Pen(Brushes.Black, 1);


        void drawOther()
        {
            int x1,x2;

            x1 = pointer;
            x2 = pointer - 1;

            MyGraphic.DrawLine(midLine, x1 + 1, 0, x2, 255);
            MyGraphic.DrawLine(backgroundLine, x1, 0, x2, 255);
        }

        void drawMain()
        {
            MyGraphic.DrawLine(mainLine, pointer, nowD[0], pointer - 1, lastD[0]);
        }

        void drawSpeed()
        {
            MyGraphic.DrawLine(speedLine, pointer, nowD[1], pointer - 1, lastD[1]);
        }
        void drawSevro()
        {
            MyGraphic.DrawLine(sevroLine, pointer, nowD[2], pointer - 1, lastD[2]);
        }


        byte[] bufferRealData = new byte[5];
        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            int i;
            serialPort1.Read(bufferRealData, 0, 5);
            if (bufferRealData[0] != 0xFF)
            {
                for (i = 0; i < 4; i++)
                {
                    if (serialPort1.ReadByte() == 0xFE) break;
                }
            }
            else
            {
                this.BeginInvoke(new mydele(draw));
            }
              
        }
        private delegate void mydele();

        int[] lastD = new int[5];
        int[] nowD = new int[5];


        private void draw()
        {

            /*nowD[0] = 255 - realData[0];
            nowD[1] = 255 - realData[1];
            nowD[2] = 255 - realData[2];
            */
            nowD[0] = 255 - bufferRealData[1];//a
            nowD[1] = bufferRealData[2];//speed
            nowD[2] = 255 - bufferRealData[3];//sevro

            richTextBox1.Text += "\n";
            richTextBox1.Text += nowD[0] + "\t";
            richTextBox1.Text += nowD[1] + "\t";
            richTextBox1.Text += nowD[2] + "\t";

            pointer++;
            if (pointer >= 1000) pointer = 1;

            drawOther();
            drawMain();
            drawSpeed();
            drawSevro();

            lastD[0] = nowD[0];
            lastD[1] = nowD[1];
            lastD[2] = nowD[2];
           
        }

        private void button1_Click(object sender, EventArgs e)
        {
            richTextBox1.Text = "a" + "\t" + "spd" + "\t" + "svro";
        }

        private void button3_Click(object sender, EventArgs e)
        {
            richTextBox1.SelectAll();
            richTextBox1.Copy();
        }

    }
}
