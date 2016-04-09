using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;



namespace TimerPoints
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            richTextBox1.Text = "";
            richTextBox1.Text += "P[0]" + "\t";
            richTextBox1.Text += "P[1]" + "\t";
            richTextBox1.Text += "P[2]" + "\t";
            richTextBox1.Text += "P[3]" + "\t";
            richTextBox1.Text += "P[4]" + "\t";
            richTextBox1.Text += "P[5]" + "\t";
            richTextBox1.Text += "P[6]" + "\t";
            richTextBox1.Text += "P[7]" + "\t";
            richTextBox1.Text += "P[8]" + "\t";
            richTextBox1.Text += "P[9]" + "\n";

            richTextBox2.Text = "";
            richTextBox2.Text += "P[1]" + "\t";
            richTextBox2.Text += "P[2]" + "\t";
            richTextBox2.Text += "P[3]" + "\t";
            richTextBox2.Text += "P[4]" + "\t";
            richTextBox2.Text += "P[5]" + "\t";
            richTextBox2.Text += "P[6]" + "\t";
            richTextBox2.Text += "P[7]" + "\t";
            richTextBox2.Text += "P[8]" + "\t";
            richTextBox2.Text += "P[9]" + "\n";

            richTextBox3.Text = "";
            richTextBox3.Text += "[0-1]" + "\t";
            richTextBox3.Text += "[1-2]" + "\t";
            richTextBox3.Text += "[2-3]" + "\t";
            richTextBox3.Text += "[3-4]" + "\t";
            richTextBox3.Text += "[4-5]" + "\t";
            richTextBox3.Text += "[5-6]" + "\t";
            richTextBox3.Text += "[6-7]" + "\t";
            richTextBox3.Text += "[7-8]" + "\t";
            richTextBox3.Text += "[8-9]" + "\n";
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
                    button1.Enabled = false;
                    button3.Enabled = false;
                    button4.Enabled = false;
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
                    button4.Enabled = true;
                }
            }
            catch
            {
                MessageBox.Show("串口打开发生了悲剧！", "Error");
            }
        }


       
        private delegate void mydele();
        


        byte[] CMD = { 0, 1, 2 ,3,4};
        string[] listCMD = { "S", "E" };
        string tmpString;
        string state_uart = "end";


        /*
         * 将长度为6的一个字符串转换为int数据
         */
        int StringToInt(string data)
        {
            int re = 0;

            re += (Convert.ToInt16(data.Substring(0, 1))) * 10000;
            re += (Convert.ToInt16(data.Substring(1, 1))) * 1000;
            re += (Convert.ToInt16(data.Substring(2, 1)))  * 100;
            re += (Convert.ToInt16(data.Substring(3, 1)))  * 10;
            re += (Convert.ToInt16(data.Substring(4, 1))) * 1;

            return re;
        }

 

        int length;
        int[] displayData = new int[20];
        void ManageData()
        {
            int i;
            string tmpStringData;


            for (i = 0; i < length; i++)
            {
                tmpStringData = tmpString.Substring((6 * i), 6);
                displayData[i] = StringToInt(tmpStringData);
            }



            for (i = 0; i < length; i++)
            {
                richTextBox1.Text += displayData[i].ToString() + "\t";
                richTextBox2.Text += displayData[i].ToString() + "\t";
            }
            richTextBox3.Text += displayData[0].ToString() + "\t";
            for (i = 1; i < length; i++)
            {
                richTextBox3.Text += (displayData[i] - displayData[i-1]).ToString() + "\t";
            }

            richTextBox1.Text += "\n";
            richTextBox2.Text += "\n";
            

            richTextBox3.Text += "\n";

            this.richTextBox1.Select(richTextBox1.TextLength, 0);
            richTextBox1.ScrollToCaret();

            this.richTextBox2.Select(richTextBox2.TextLength, 0);
            richTextBox2.ScrollToCaret();

            this.richTextBox3.Select(richTextBox3.TextLength, 0);
            richTextBox3.ScrollToCaret();

        }
        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            switch (state_uart)
            {
                case "start":
                    tmpString = serialPort1.ReadExisting();
                    length = StringToInt(tmpString);
                    //this.Invoke(new mydele(DisplayLength));
                    state_uart = "confirm";
                    serialPort1.ReceivedBytesThreshold = 6 * length + 2;
                    serialPort1.Write(CMD, 4, 1);
                    break;
                case "confirm":
                     tmpString = serialPort1.ReadExisting();
                     this.BeginInvoke(new mydele(ManageData));
                     
                    state_uart = "end";
                    serialPort1.ReceivedBytesThreshold = 1;
                    serialPort1.Write(CMD, 1, 1);

                    
                    break;
                case "end":
                    tmpString = serialPort1.ReadExisting();
                    if (tmpString.IndexOf("S") == (tmpString.Length-1))
                    //this.Invoke(new mydele(DisplayUartData));
                    state_uart = "start";
                    serialPort1.ReceivedBytesThreshold = 6;
                    serialPort1.Write(CMD, 3, 1);
                    break;
            }
        }


        //向下位机发送的请求命令
        /*
        0xff    restart
        0xfe    shut down lazer
        0xfd    ask data transport
        */
        byte[] CMDtoMCU = { 253, 254, 255 };

        /*
        * report
        */
        private void button4_Click(object sender, EventArgs e)
        {
            serialPort1.Write(CMDtoMCU, 0, 1);
            button4.Enabled = false;
            button3.Enabled = false;
            button1.Enabled = false;
            timer1.Enabled = true;


            
        }

        /*
        * shut down the lazer
        */
        private void button3_Click(object sender, EventArgs e)
        {
            serialPort1.Write(CMDtoMCU, 1, 1);
            button4.Enabled = false;
            button3.Enabled = false;
            button1.Enabled = false;
            timer1.Enabled = true;
        }

        /*
         * restart
         */
        private void button1_Click(object sender, EventArgs e)
        {
            serialPort1.Write(CMDtoMCU, 2, 1);
            button4.Enabled = false;
            button3.Enabled = false;
            button1.Enabled = false;
            timer1.Enabled = true;
        }


        int timer = 0;
        private void timer1_Tick(object sender, EventArgs e)
        {
            timer++;
            if (timer > 3)
            {
                timer = 0;
                timer1.Enabled = false;

                button1.Enabled = true;
                button3.Enabled = true;
                button4.Enabled = true;
            }


        }

        private void button5_Click(object sender, EventArgs e)
        {
            richTextBox1.SelectAll();
            richTextBox1.Copy();
        }
   
    }

}
