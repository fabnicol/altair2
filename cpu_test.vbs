' Enumerating Processor Information


On Error Resume Next

strComputer = "."
Set objWMIService = GetObject("winmgmts:" _
    & "{impersonationLevel=impersonate}!\\" & strComputer & "\root\cimv2")

Set colItems = objWMIService.ExecQuery("Select * from Win32_Processor")

Set objFSO=CreateObject("Scripting.FileSystemObject")
Set objFile = objFSO.CreateTextFile("cpu_check",True)

For Each objItem in colItems
  If (InStr(UCase(objItem.Name), "CORE2", vbTextCompare) = 0 And InStr(UCase(objItem.Name), "CELERON", vbTextCompare) = 0) Then objFile.Write "0" & vbCrLf Else objFile.Write "1" & vbCrLf
Next

objFile.Close

