rem get the custom action propery (this could just as well be a command line, I guess)
rem win_path = "C:\windows\"
win_path = Session.Property ("CustomActionData")

rem Execute script from INSTALLER vb
rem http://stackoverflow.com/questions/98778/executing-a-script-file-from-a-windows-installer-custom-action

Set WshShell = CreateObject( "WScript.Shell" )
command = win_path & "system32\rundll32.exe setupapi,InstallHinfSection DefaultInstall 132 " & _
  win_path & "inf\ooi_usb.inf"

rem command = "pause"
msgbox command

ret = WshShell.Run ("cmd /C " & """" & command & """", 1, true)
Set WshShell = Nothing

msgbox "script returned " & ret

rem this is a terrible hack but if I use 'quit' in the success case
rem the installer still thinks it's a failure ... 
if (ret <> 0) then
  Quit(ret)
end if

