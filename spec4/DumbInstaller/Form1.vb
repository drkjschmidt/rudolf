
Public Class Form1
    Dim myCwd As String
    Dim myPaths(0 To 3) As String


    Private Sub myRunInstaller(ByVal cmd As String)
        Dim myFile As String
        Dim myTries As String

        myTries = ""
        For ii = 0 To UBound(myPaths)
            myFile = myPaths(ii) + "\" + cmd
            myTries = myTries + vbCrLf + myFile
            ToolStripStatusLabel1.Text = "Trying " + myFile
            If Dir(myFile) <> "" Then
                ToolStripStatusLabel1.Text = "Installing using " + myFile
                System.Diagnostics.Process.Start(myFile)
                ' Shell(myFile)
                Return
            End If
        Next
        MsgBox("No installer found for " + cmd + "!" + vbCrLf + "Tried:" + vbCrLf + myTries)
    End Sub
    '
    ' JAVA
    '
    Private Sub InstallJava_LinkClicked(ByVal sender As System.Object, ByVal e As System.Windows.Forms.LinkLabelLinkClickedEventArgs) Handles InstallJava.LinkClicked
        ToolStripStatusLabel1.Text = "Install Java"
        myRunInstaller("jre-7u2-windows-i586.exe")
    End Sub
    Private Sub InstallJava_MouseEnter(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles InstallJava.MouseEnter
        ToolStripStatusLabel1.Text = "Install Packaged Java Version"
    End Sub
    Private Sub InstallJava_MouseLeave(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles InstallJava.MouseLeave
        ToolStripStatusLabel1.Text = ""
    End Sub
    '
    ' Lapack
    '
    Private Sub InstallLapack_LinkClicked(ByVal sender As System.Object, ByVal e As System.Windows.Forms.LinkLabelLinkClickedEventArgs) Handles InstallLapack.LinkClicked
        ToolStripStatusLabel1.Text = "Install Lapack Math Library"
        myRunInstaller("lapackpp-2.5.1-setup.exe")
    End Sub
    Private Sub InstallLapack_MouseEnter(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles InstallLapack.MouseEnter
        ToolStripStatusLabel1.Text = "Install Lapack Math Library"
    End Sub
    Private Sub InstallLapack_MouseLeave(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles InstallLapack.MouseLeave
        ToolStripStatusLabel1.Text = ""
    End Sub
    '
    ' Ocean Optics
    '
    Private Sub InstallOO_LinkClicked(ByVal sender As System.Object, ByVal e As System.Windows.Forms.LinkLabelLinkClickedEventArgs) Handles InstallOO.LinkClicked
        ToolStripStatusLabel1.Text = "Install Ocean Optics Spectrometer Driver"
        'Clipboard.Clear()
        'Clipboard.SetText("Etm4QBn7gd")
        'MsgBox("This installer requires a password." + vbCrLf + _
        '       "The password is: Etm4QBn7gd" + vbCrLf + _
        '       "For your convenience the password is already in the clipboard!")

        myRunInstaller("OmniDriverSPAM-2.01-win32-net2.0-redistributable-installer.exe")
    End Sub
    Private Sub InstallOO_MouseEnter(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles InstallOO.MouseEnter
        ToolStripStatusLabel1.Text = "Install Ocean Optics Spectrometer Driver"
    End Sub
    Private Sub InstallOO_MouseLeave(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles InstallOO.MouseLeave
        ToolStripStatusLabel1.Text = ""
    End Sub
    '
    ' LightPilot
    '
    Private Sub InstallLP_LinkClicked(ByVal sender As System.Object, ByVal e As System.Windows.Forms.LinkLabelLinkClickedEventArgs) Handles InstallLP.LinkClicked
        ToolStripStatusLabel1.Text = "Install LightPilot application"
        myRunInstaller("LPInstall.msi")
    End Sub
    Private Sub InstallLP_MouseEnter(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles InstallLP.MouseEnter
        ToolStripStatusLabel1.Text = "Install LightPilot application"
    End Sub
    Private Sub InstallLP_MouseLeave(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles InstallLP.MouseLeave
        ToolStripStatusLabel1.Text = ""
    End Sub


    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Me.Name = "LightPilot 1.0 Install Helper"
        Me.Text = "LightPilot 1.0 Install Helper"

        myPaths(0) = CurDir()
        myPaths(1) = CurDir() + "\installers"
        myPaths(2) = "D:"
        myPaths(3) = "W:\wilson1"

        ToolStripStatusLabel1.Text = myPaths(0)
        Me.Name = "LightPilot 1.0 Install Helper"
        Me.Text = "LightPilot 1.0 Install Helper"
    End Sub

    Const EWX_LogOff As Long = 0
    Const EWX_SHUTDOWN = 1
    Const EWX_REBOOT = 2
    Const EWX_FORCE = 4
    Private Declare Function ExitWindows _
            Lib "User32" Alias "ExitWindowsEx" _
            (ByVal dwOptions As Long, ByVal dwReserved As Long) As Long

    Private Sub Form1_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing
        Dim ask As String
        ask = MessageBox.Show( _
            "It is strongly recommended to reboot after any changes!" + vbNewLine + _
            "Would you like to reboot now?", _
            "form1", _
            MessageBoxButtons.YesNo, _
            MessageBoxIcon.Question)
        If ask = MsgBoxResult.Yes Then
            ' reboot here
            'ExitWindows(EWX_REBOOT, &HFFFFFFFF)
            System.Diagnostics.Process.Start("shutdown", "-r -t 00")
        End If
    End Sub
End Class
