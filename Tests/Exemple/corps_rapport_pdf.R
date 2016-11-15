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
  
  # Ne pas utiliser / ici
  
  system("xcopy /Y altaïr.pdf Donnees\\R-Altaïr")
  system("mkdir Donnees\\R-Altaïr\\Docs")
  system("xcopy /E /Y Docs Donnees\\R-Altaïr\\Docs")
  
} else {
  
  system("cp -rf Docs Donnees/R-Altair")
  system("cp -f  altaïr.pdf Donnees/R-Altair")
  
}


if (setOSWindows) {
  
  shell("start acrord32.exe Donnees/R-Altaïr/altaïr.pdf")
  
} else {
  
  system("okular Donnees/R-Altair/altaïr.pdf")
}
