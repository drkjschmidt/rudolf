Imports System.Text.RegularExpressions
Imports System.IO

Module Module1

    Sub FixVersion(ByVal SrcName As String, ByVal DstName As String)
        Dim InFile As New StreamReader(SrcName)
        Dim OutFile As New StreamWriter(DstName)
        Dim InLine As String

        ' Dim MyRe As New Regex("(#define\s+)(\w+)(\s+""?)(\d+)([ ,]+)(\d+)([ ,]+)(\d+)([ ,]+)(\d+)(\\0"")?")
        Dim MyRe As New Regex("(#define\s+)(\w+)(\s+""?[^\d]*)(\d+)([ ,\.]+)(\d+)([ ,\.]+)(\d+)([ ,]+|\s+Build\s+)(\d+)(\\0""|"")?")
        Dim MyMC As MatchCollection

        Do While InFile.Peek() <> -1
            InLine = InFile.ReadLine()

            ' Console.WriteLine(InLine)
            If MyRe.IsMatch(InLine) Then
                MyMC = MyRe.Matches(InLine)
                ' Console.WriteLine("MATCH: ")
                ' For ii = 0 To 11
                ' Console.WriteLine(Format(ii) + ": '" + MyMC(0).Groups(ii).Value + "'")
                ' Next

                For ii = 1 To 11
                    If ii = 10 Then
                        Console.Write(Format(Val(MyMC(0).Groups(ii).Value) + 1))
                        OutFile.Write(Format(Val(MyMC(0).Groups(ii).Value) + 1))
                    Else
                        Console.Write(MyMC(0).Groups(ii).Value)
                        OutFile.Write(MyMC(0).Groups(ii).Value)
                    End If
                Next
                Console.WriteLine()
                OutFile.WriteLine()
            End If
        Loop
        OutFile.Close()
    End Sub

    Sub Main()
        If Len(Command$) > 0 Then
            Console.WriteLine("Asked to update version in '" + Command$() + "'")

            Dim MyFI As New FileInfo(Command$)
            Dim MyTF As New FileInfo(Command$() + ".temp")

            If MyFI.Exists Then
                MyTF.Delete()
                MyFI.CopyTo(MyTF.FullName)
                MyTF.Refresh()
                If (MyTF.Exists) Then
                    Console.WriteLine("Reading file '" + MyTF.Name + "'")
                    FixVersion(MyTF.FullName, MyFI.FullName)
                Else
                    Console.WriteLine("Temp file creation failed!")
                End If

            Else
                Console.WriteLine("File not found '" + Command$() + "'")
            End If
        Else
            Console.WriteLine("No Args!")
        End If
    End Sub

End Module
