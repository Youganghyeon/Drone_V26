namespace GCS
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.P_PID = new System.Windows.Forms.Panel();
            this.B_PID = new System.Windows.Forms.Button();
            this.FP_MENU = new System.Windows.Forms.FlowLayoutPanel();
            this.Menu = new System.Windows.Forms.Button();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.gMapControl1 = new GMap.NET.WindowsForms.GMapControl();
            this.FP_MENU.SuspendLayout();
            this.flowLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // P_PID
            // 
            this.P_PID.BackColor = System.Drawing.Color.Gainsboro;
            this.P_PID.Location = new System.Drawing.Point(3, 40);
            this.P_PID.Name = "P_PID";
            this.P_PID.Size = new System.Drawing.Size(1191, 167);
            this.P_PID.TabIndex = 1;
            this.P_PID.Paint += new System.Windows.Forms.PaintEventHandler(this.P_PID_Paint_2);
            // 
            // B_PID
            // 
            this.B_PID.Location = new System.Drawing.Point(3, 4);
            this.B_PID.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.B_PID.Name = "B_PID";
            this.B_PID.Size = new System.Drawing.Size(86, 29);
            this.B_PID.TabIndex = 0;
            this.B_PID.Text = "B_PID";
            this.B_PID.UseVisualStyleBackColor = true;
            this.B_PID.Click += new System.EventHandler(this.B_PID_Click);
            // 
            // FP_MENU
            // 
            this.FP_MENU.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.FP_MENU.AutoSize = true;
            this.FP_MENU.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.FP_MENU.BackColor = System.Drawing.Color.White;
            this.FP_MENU.Controls.Add(this.B_PID);
            this.FP_MENU.Controls.Add(this.P_PID);
            this.FP_MENU.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.FP_MENU.Location = new System.Drawing.Point(3, 40);
            this.FP_MENU.Name = "FP_MENU";
            this.FP_MENU.Size = new System.Drawing.Size(1197, 210);
            this.FP_MENU.TabIndex = 2;
            this.FP_MENU.Paint += new System.Windows.Forms.PaintEventHandler(this.FP_MENU_Paint);
            // 
            // Menu
            // 
            this.Menu.Location = new System.Drawing.Point(3, 3);
            this.Menu.Name = "Menu";
            this.Menu.Size = new System.Drawing.Size(1264, 31);
            this.Menu.TabIndex = 3;
            this.Menu.Text = "Menu";
            this.Menu.UseVisualStyleBackColor = true;
            this.Menu.Click += new System.EventHandler(this.Menu_Click);
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.AutoSize = true;
            this.flowLayoutPanel1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.flowLayoutPanel1.BackColor = System.Drawing.Color.White;
            this.flowLayoutPanel1.Controls.Add(this.Menu);
            this.flowLayoutPanel1.Controls.Add(this.FP_MENU);
            this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.flowLayoutPanel1.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(0, 418);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(1267, 253);
            this.flowLayoutPanel1.TabIndex = 4;
            this.flowLayoutPanel1.WrapContents = false;
            this.flowLayoutPanel1.Paint += new System.Windows.Forms.PaintEventHandler(this.flowLayoutPanel1_Paint);
            // 
            // gMapControl1
            // 
            this.gMapControl1.Bearing = 0F;
            this.gMapControl1.CanDragMap = true;
            this.gMapControl1.EmptyTileColor = System.Drawing.Color.Navy;
            this.gMapControl1.GrayScaleMode = false;
            this.gMapControl1.HelperLineOption = GMap.NET.WindowsForms.HelperLineOptions.DontShow;
            this.gMapControl1.LevelsKeepInMemmory = 5;
            this.gMapControl1.Location = new System.Drawing.Point(116, 62);
            this.gMapControl1.MarkersEnabled = true;
            this.gMapControl1.MaxZoom = 2;
            this.gMapControl1.MinZoom = 2;
            this.gMapControl1.MouseWheelZoomEnabled = true;
            this.gMapControl1.MouseWheelZoomType = GMap.NET.MouseWheelZoomType.MousePositionAndCenter;
            this.gMapControl1.Name = "gMapControl1";
            this.gMapControl1.NegativeMode = false;
            this.gMapControl1.PolygonsEnabled = true;
            this.gMapControl1.RetryLoadTile = 0;
            this.gMapControl1.RoutesEnabled = true;
            this.gMapControl1.ScaleMode = GMap.NET.WindowsForms.ScaleModes.Integer;
            this.gMapControl1.SelectedAreaFillColor = System.Drawing.Color.FromArgb(((int)(((byte)(33)))), ((int)(((byte)(65)))), ((int)(((byte)(105)))), ((int)(((byte)(225)))));
            this.gMapControl1.ShowTileGridLines = false;
            this.gMapControl1.Size = new System.Drawing.Size(1139, 353);
            this.gMapControl1.TabIndex = 5;
            this.gMapControl1.Zoom = 0D;
            this.gMapControl1.Load += new System.EventHandler(this.gMapControl1_Load);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(255)))), ((int)(((byte)(128)))));
            this.ClientSize = new System.Drawing.Size(1267, 671);
            this.Controls.Add(this.gMapControl1);
            this.Controls.Add(this.flowLayoutPanel1);
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.FP_MENU.ResumeLayout(false);
            this.flowLayoutPanel1.ResumeLayout(false);
            this.flowLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Panel P_PID;
        private System.Windows.Forms.Button B_PID;
        private System.Windows.Forms.FlowLayoutPanel FP_MENU;
        private System.Windows.Forms.Button Menu;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private GMap.NET.WindowsForms.GMapControl gMapControl1;
    }
}

