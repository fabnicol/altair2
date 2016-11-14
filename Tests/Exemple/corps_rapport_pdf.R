source("rendre.R", encoding = encodage.code.source)

rendre(to = "latex",
       args=c("-V", 
              "papersize=A4",
              "-V",
              "geometry:top=2cm,bottom=1.5cm,left=2cm,right=1.5cm",
              "--highlight-style",
              "tango"),
       output_file = "altair.pdf")

file.rename("altair.pdf", "altaïr.pdf")


if (setOSWindows) {
  
  system("xcopy /E /Y Docs Donnees/R-Altair")
  system("copy /Y altaïr.pdf Donnees/R-Altaïr")
  
} else {
  
  system("cp -rf Docs Donnees/R-Altair")
  system("cp -f  altaïr.pdf Donnees/R-Altair")
  
}


if (setOSWindows) {
  
  shell("start acrord32.exe Donnees/R-Altaïr/altaïr.pdf")
  
} else {
  
  system("okular Donnees/R-Altair/altaïr.pdf")
}
