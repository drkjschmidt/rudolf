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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(Form1))
        Me.InstallJava = New System.Windows.Forms.LinkLabel
        Me.StatusStrip1 = New System.Windows.Forms.StatusStrip
        Me.ToolStripStatusLabel1 = New System.Windows.Forms.ToolStripStatusLabel
        Me.PictureBox1 = New System.Windows.Forms.PictureBox
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
        Me.InstallJava.Location = New System.Drawing.Point(178, 162)
        Me.InstallJava.Name = "InstallJava"
        Me.InstallJava.Size = New System.Drawing.Size(72, 13)
        Me.InstallJava.TabIndex = 0
        Me.InstallJava.TabStop = True
        Me.InstallJava.Text = "1. Install Java"
        Me.InstallJava.VisitedLinkColor = System.Drawing.Color.Blue
        '
        'StatusStrip1
        '
        Me.StatusStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.ToolStripStatusLabel1})
        Me.StatusStrip1.Location = New System.Drawing.Point(0, 260)
        Me.StatusStrip1.Name = "StatusStrip1"
        Me.StatusStrip1.Size = New System.Drawing.Size(384, 22)
        Me.StatusStrip1.SizingGrip = False
        Me.StatusStrip1.Stretch = False
        Me.StatusStrip1.TabIndex = 2
        Me.StatusStrip1.Text = "StatusStrip1"
        '
        'ToolStripStatusLabel1
        '
        Me.ToolStripStatusLabel1.Name = "ToolStripStatusLabel1"
        Me.ToolStripStatusLabel1.Size = New System.Drawing.Size(121, 17)
        Me.ToolStripStatusLabel1.Text = "ToolStripStatusLabel1"
        '
        'PictureBox1
        '
        Me.PictureBox1.Image = CType(resources.GetObject("PictureBox1.Image"), System.Drawing.Image)
        Me.PictureBox1.Location = New System.Drawing.Point(0, -1)
        Me.PictureBox1.Name = "PictureBox1"
        Me.PictureBox1.Size = New System.Drawing.Size(390, 262)
        Me.PictureBox1.TabIndex = 3
        Me.PictureBox1.TabStop = False
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
        Me.InstallLapack.Text = "2. Install Lapack"
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
        Me.InstallOO.Text = "3. Install OO Driver"
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
        Me.InstallLP.Text = "4. Install LightPilot"
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(384, 282)
        Me.Controls.Add(Me.InstallLP)
        Me.Controls.Add(Me.InstallOO)
        Me.Controls.Add(Me.InstallLapack)
        Me.Controls.Add(Me.StatusStrip1)
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
    Friend WithEvents StatusStrip1 As System.Windows.Forms.StatusStrip
    Friend WithEvents ToolStripStatusLabel1 As System.Windows.Forms.ToolStripStatusLabel
    Friend WithEvents PictureBox1 As System.Windows.Forms.PictureBox
    Friend WithEvents InstallLapack As System.Windows.Forms.LinkLabel
    Friend WithEvents InstallOO As System.Windows.Forms.LinkLabel
    Friend WithEvents InstallLP As System.Windows.Forms.LinkLabel

End Class
