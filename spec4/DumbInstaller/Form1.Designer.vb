<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.InstallJava = New System.Windows.Forms.LinkLabel
        Me.Label1 = New System.Windows.Forms.Label
        Me.StatusStrip1 = New System.Windows.Forms.StatusStrip
        Me.ToolStripStatusLabel1 = New System.Windows.Forms.ToolStripStatusLabel
        Me.PictureBox1 = New System.Windows.Forms.PictureBox
        Me.InstallSpanner = New System.Windows.Forms.LinkLabel
        Me.InstallLapack = New System.Windows.Forms.LinkLabel
        Me.InstallOO = New System.Windows.Forms.LinkLabel
        Me.InstallLP = New System.Windows.Forms.LinkLabel
        Me.StatusStrip1.SuspendLayout()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SuspendLayout()
        '
        'InstallJava
        '
        Me.InstallJava.AutoSize = True
        Me.InstallJava.BackColor = System.Drawing.SystemColors.ControlLightLight
        Me.InstallJava.Location = New System.Drawing.Point(161, 145)
        Me.InstallJava.Name = "InstallJava"
        Me.InstallJava.Size = New System.Drawing.Size(72, 13)
        Me.InstallJava.TabIndex = 0
        Me.InstallJava.TabStop = True
        Me.InstallJava.Text = "1. Install Java"
        Me.InstallJava.VisitedLinkColor = System.Drawing.Color.Blue
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(303, 196)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(70, 13)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "Etm4QBn7gd" & Global.Microsoft.VisualBasic.ChrW(9)
        '
        'StatusStrip1
        '
        Me.StatusStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.ToolStripStatusLabel1})
        Me.StatusStrip1.Location = New System.Drawing.Point(0, 264)
        Me.StatusStrip1.Name = "StatusStrip1"
        Me.StatusStrip1.Size = New System.Drawing.Size(392, 22)
        Me.StatusStrip1.SizingGrip = False
        Me.StatusStrip1.Stretch = False
        Me.StatusStrip1.TabIndex = 2
        Me.StatusStrip1.Text = "StatusStrip1"
        '
        'ToolStripStatusLabel1
        '
        Me.ToolStripStatusLabel1.Name = "ToolStripStatusLabel1"
        Me.ToolStripStatusLabel1.Size = New System.Drawing.Size(111, 17)
        Me.ToolStripStatusLabel1.Text = "ToolStripStatusLabel1"
        '
        'PictureBox1
        '
        Me.PictureBox1.Image = Global.DumbInstaller.My.Resources.Resources.w_splash_3
        Me.PictureBox1.Location = New System.Drawing.Point(0, -1)
        Me.PictureBox1.Name = "PictureBox1"
        Me.PictureBox1.Size = New System.Drawing.Size(390, 262)
        Me.PictureBox1.TabIndex = 3
        Me.PictureBox1.TabStop = False
        '
        'InstallSpanner
        '
        Me.InstallSpanner.AutoSize = True
        Me.InstallSpanner.BackColor = System.Drawing.SystemColors.ControlLightLight
        Me.InstallSpanner.Location = New System.Drawing.Point(173, 162)
        Me.InstallSpanner.Name = "InstallSpanner"
        Me.InstallSpanner.Size = New System.Drawing.Size(89, 13)
        Me.InstallSpanner.TabIndex = 4
        Me.InstallSpanner.TabStop = True
        Me.InstallSpanner.Text = "2. Install Spanner"
        '
        'InstallLapack
        '
        Me.InstallLapack.AutoSize = True
        Me.InstallLapack.BackColor = System.Drawing.SystemColors.ControlLightLight
        Me.InstallLapack.Location = New System.Drawing.Point(186, 179)
        Me.InstallLapack.Name = "InstallLapack"
        Me.InstallLapack.Size = New System.Drawing.Size(85, 13)
        Me.InstallLapack.TabIndex = 5
        Me.InstallLapack.TabStop = True
        Me.InstallLapack.Text = "3. Install Lapack"
        '
        'InstallOO
        '
        Me.InstallOO.AutoSize = True
        Me.InstallOO.BackColor = System.Drawing.SystemColors.ControlLightLight
        Me.InstallOO.Location = New System.Drawing.Point(200, 196)
        Me.InstallOO.Name = "InstallOO"
        Me.InstallOO.Size = New System.Drawing.Size(96, 13)
        Me.InstallOO.TabIndex = 6
        Me.InstallOO.TabStop = True
        Me.InstallOO.Text = "4. Install OO Driver"
        '
        'InstallLP
        '
        Me.InstallLP.AutoSize = True
        Me.InstallLP.BackColor = System.Drawing.SystemColors.ControlLightLight
        Me.InstallLP.Location = New System.Drawing.Point(213, 213)
        Me.InstallLP.Name = "InstallLP"
        Me.InstallLP.Size = New System.Drawing.Size(92, 13)
        Me.InstallLP.TabIndex = 7
        Me.InstallLP.TabStop = True
        Me.InstallLP.Text = "5. Install LightPilot"
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(392, 286)
        Me.Controls.Add(Me.InstallLP)
        Me.Controls.Add(Me.InstallOO)
        Me.Controls.Add(Me.InstallLapack)
        Me.Controls.Add(Me.InstallSpanner)
        Me.Controls.Add(Me.StatusStrip1)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.InstallJava)
        Me.Controls.Add(Me.PictureBox1)
        Me.MaximizeBox = False
        Me.MaximumSize = New System.Drawing.Size(400, 320)
        Me.MinimizeBox = False
        Me.MinimumSize = New System.Drawing.Size(400, 320)
        Me.Name = "Form1"
        Me.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide
        Me.Text = "Form1"
        Me.StatusStrip1.ResumeLayout(False)
        Me.StatusStrip1.PerformLayout()
        CType(Me.PictureBox1, System.ComponentModel.ISupportInitialize).EndInit()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents InstallJava As System.Windows.Forms.LinkLabel
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents StatusStrip1 As System.Windows.Forms.StatusStrip
    Friend WithEvents ToolStripStatusLabel1 As System.Windows.Forms.ToolStripStatusLabel
    Friend WithEvents PictureBox1 As System.Windows.Forms.PictureBox
    Friend WithEvents InstallSpanner As System.Windows.Forms.LinkLabel
    Friend WithEvents InstallLapack As System.Windows.Forms.LinkLabel
    Friend WithEvents InstallOO As System.Windows.Forms.LinkLabel
    Friend WithEvents InstallLP As System.Windows.Forms.LinkLabel

End Class
