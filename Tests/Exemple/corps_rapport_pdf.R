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
  shell("start acrord32.exe altaïr.pdf")
} else {
  system("okular altaïr.pdf")
}

