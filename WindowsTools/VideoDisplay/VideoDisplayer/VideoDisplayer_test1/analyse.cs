using System;

using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using ManageImage;

namespace VideoDisplayer_test1
{
    public partial class analyse : Form
    {
        ManageImage_Fish MI = new ManageImage_Fish();

        //构建窗体
        public analyse()
        {
            InitializeComponent();
        }

        private void analyse_Load(object sender, EventArgs e)
        {
            textBox1.Text = folderBrowserDialog1.SelectedPath;
            //CLP = pictureBox1.CreateGraphics();
            LoadParameters();
            LoadPhotoFile();
        }


        private void button3_Click(object sender, EventArgs e)
        {
            listBox1_SelectedIndexChanged(null, null);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Dispose();
        }

        private void analyse_FormClosing(object sender, FormClosingEventArgs e)
        {
            SaveParameters();
            this.Dispose();
        }


        private void button5_Click(object sender, EventArgs e)
        {
            SelectDirectory();
        }


        private void button2_Click(object sender, EventArgs e)
        {
            DisplayQuondamPhoto();
        }

        //选择目录
        void SelectDirectory()
        {
            string tmp;
            tmp = folderBrowserDialog1.SelectedPath;
            folderBrowserDialog1.ShowDialog();
            if (folderBrowserDialog1.SelectedPath != "")
            {
                LoadPhotoFile();
            }
            else
            {
                folderBrowserDialog1.SelectedPath = tmp;
            }
            textBox1.Text = folderBrowserDialog1.SelectedPath;
        }
        
        //显示原始图像
        void DisplayQuondamPhoto()
        {
            pictureBox2.Enabled = true;
             MI.PP.MyBitmap = new Bitmap(MI.PP.files[listBox1.SelectedIndex].FullName);

            pictureBox2.Image = MI.PP.MyBitmap;
            pictureBox2.SizeMode = PictureBoxSizeMode.Zoom;
            pictureBox2.Size = MI.PP.showSize;
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            MI.ManageImage(listBox1.SelectedIndex);
            ReloadPicture();//重构图像显示（从MyBitmip把图像刷新到picturebox）
            
            DisplayData();
           // DisplayCharacter();
            //DisplayCharacterLines();
        }
        /* CLP;
        void DisplayCharacterLines()
        {
            int x1, y1, x2, y2;
            int tmp1,tmp2;
            CLP.Clear(Color.Gray);
            
            Pen mypen = new Pen(Brushes.Green,2);

            x1 = MI.PP.pathCharacter.x;
            y1 = MI.PP.pathCharacter.z;
            tmp1 = (int)(MI.PP.pathCharacter.l1 / (Math.Sqrt((MI.PP.pathCharacter.k1 * MI.PP.pathCharacter.k1) + 1)));
            tmp2 = (int)(MI.PP.pathCharacter.l2 / (Math.Sqrt((MI.PP.pathCharacter.k1 * MI.PP.pathCharacter.k1) + 1)));
            x2 = x1 + tmp2;
            y2 = y1 + tmp1;
            CLP.DrawLine(mypen, x1, y1, x2, y2);


            CLP.DrawCurve(

            
        }*/
        //重构图像显示（从MyBitmip把图像刷新到picturebox）
        void ReloadPicture()
        {
            pictureBox2.Image = MI.PP.MyBitmap;
            pictureBox2.SizeMode = PictureBoxSizeMode.Zoom;
            pictureBox2.Image.RotateFlip(RotateFlipType.Rotate180FlipX);
            pictureBox2.Image.RotateFlip(RotateFlipType.Rotate180FlipY);
        }

        private void DisplayData()
        {
            richTextBox1.Text = "";
            richTextBox2.Text = "";
            richTextBox3.Text = "";
            richTextBox4.Text = "";
            richTextBox5.Text = "";
            richTextBox6.Text = "";
            richTextBox7.Text = "";

            richTextBox1.Text = MI.PP.displayData1;
            richTextBox2.Text = MI.PP.displayData2;
            richTextBox3.Text = MI.PP.displayData3;
            richTextBox4.Text = MI.PP.displayData4;
            richTextBox5.Text = MI.PP.displayData5;
            richTextBox6.Text = MI.PP.displayData6;
            richTextBox7.Text = MI.PP.displayData7;
        }
   
        /*-------------------------------------------------------------------------------*/
        //鼠标在图像上方时显示信息
        private void pictureBox2_MouseUp(object sender, MouseEventArgs e)
        {
            label1.Show();
            label2.Show();
            label3.Show();
        }
        private void pictureBox2_MouseMove(object sender, MouseEventArgs e)
        {
            int x, y;
          // Bitmap disBitmap = new Bitmap(pictureBox2.Image);
            Bitmap disBitmap = MI.PP.MyBitmap;
            try
            {
                x = MousePosition.X - pictureBox2.Location.X - this.Location.X;
                y = MousePosition.Y - pictureBox2.Location.Y - this.Location.Y;
                label1.Location = new Point(x, y);
                label2.Location = new Point(x, y + 10);
                label3.Location = new Point(x, y + 20);
                x -= 6;
                y -= 29;
                x = (x * MI.PP.MyBitmap.Width) / pictureBox2.Width;
                y = (y * MI.PP.MyBitmap.Height) / pictureBox2.Height;
                label1.Text = Convert.ToString(255 - MI.PP.MyBitmap.GetPixel(x, y).A);
                label2.Text = "x=" + Convert.ToString(159 - x);
                label3.Text = "z=" + Convert.ToString(119 - y);
            }
            catch
            {
            }
        }
        private void pictureBox2_MouseLeave(object sender, EventArgs e)
        {
            label1.Hide();
            label2.Hide();
            label3.Hide();
        }

   
        //载入文件列表
        private void button4_Click(object sender, EventArgs e)
        {
            LoadPhotoFile();
        }
        void LoadPhotoFile()
        {
            int i;
            int num;
            try
            {
                listBox1.Items.Clear();
                DirectoryInfo foldername = new DirectoryInfo(textBox1.Text);//可能会出错 2012-6-21
                MI.PP.files = foldername.GetFiles();
                num = MI.PP.files.Length;
                if (num > 0)
                {
                    for (i = 0; i < num; i++)
                    {
                        listBox1.Items.Add(MI.PP.files[i].Name);
                    }
                    listBox1.SelectedIndex = 0;
                }
            }
            catch
            {
                MessageBox.Show("找不到文件目录", "Error");
            }

        }

        //从Configuration中读取参数
        int ReadConfigParameter(string parameterName)
        {
            int tmp1, tmp2;
            string output;
            tmp1 = MI.PP.Configuration.IndexOf(parameterName) + parameterName.Length + 1;
            tmp2 = MI.PP.Configuration.IndexOf("\n", tmp1);
            output = MI.PP.Configuration.Substring(tmp1, (tmp2 - tmp1 - 1));
            return Convert.ToInt32(output);
        }
        //向Configuration中写入参数
        void SaveConfigParameter(string parameterName, string value)
        {
            int tmp1, tmp2;
            tmp1 = MI.PP.Configuration.IndexOf(parameterName) + parameterName.Length + 1;
            tmp2 = MI.PP.Configuration.IndexOf("\n", tmp1);
            MI.PP.Configuration = MI.PP.Configuration.Remove(tmp1, (tmp2 - tmp1 - 1));
            MI.PP.Configuration = MI.PP.Configuration.Insert(tmp1, value);
        }
        //存储参数
        void SaveParameters()
        {
            StreamWriter ConfTXT = new StreamWriter(@".\Config.txt");
            SaveConfigParameter("Threshold_SearchEdge", textBox3.Text);
            SaveConfigParameter("SkipLines", textBox4.Text);
            SaveConfigParameter("skiplineControl", textBox6.Text);
            ConfTXT.Write(MI.PP.Configuration);
            ConfTXT.Dispose();
        }
        //载入参数
        void LoadParameters()
        {
            StreamReader ConfTXT = new StreamReader(@".\Config.txt");
            MI.PP.Configuration = ConfTXT.ReadToEnd().ToString();

            MI.PP.ThresholdEdge = ReadConfigParameter("Threshold_SearchEdge");
            textBox3.Text = MI.PP.ThresholdEdge.ToString();

            MI.PP.skipLines = ReadConfigParameter("SkipLines");
            textBox4.Text = MI.PP.skipLines.ToString();

            MI.PP.skiplineControl = ReadConfigParameter("skiplineControl");
            textBox6.Text = MI.PP.skiplineControl.ToString();

            ConfTXT.Dispose();

        }


        //变更外部参数        
        private void textBox3_TextChanged(object sender, EventArgs e)
        {
            MI.PP.ThresholdEdge = Convert.ToInt32(textBox3.Text);
        }
        //变更外部参数   
        private void textBox4_TextChanged(object sender, EventArgs e)
        {
            MI.PP.skipLines = Convert.ToInt32(textBox4.Text);
        }
        //变更外部参数   
        private void textBox6_TextChanged(object sender, EventArgs e)
        {
            MI.PP.skiplineControl = Convert.ToInt32(textBox6.Text);
        }


    }
}
