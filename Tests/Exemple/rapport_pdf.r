# encoder ce script en windows-1252

source("prologue_rapport.R",  encoding = "ISO-8859-1")

render("altair.R",
       encoding = encodage.code.source,
       output_format = output_format(knitr_options(opts_chunk = list(fig.width = 7.5, 
                                                                     fig.height = 5,
                                                                     dpi = 300,
                                                                     echo = FALSE,
                                                                     warning = FALSE,
                                                                     message = FALSE,
                                                                     results = 'asis')),
                                     keep_md = FALSE, clean_supporting = TRUE,
                                     pandoc = pandoc_options(to = "latex",
                                                             from = "markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures",
                                                             args=c("-V", 
                                                                    "papersize=A4",
                                                                    "-V",
                                                                    "geometry:top=2cm,bottom=1.5cm,left=2cm,right=1.5cm",
                                                                    "--highlight-style",
                                                                    "tango"))),
   
       output_file = "altair.pdf")

file.rename("altair.pdf", "altaïr.pdf")

if (setOSWindows) {
  shell("start acrord32.exe altaïr.pdf")
} else {
  system("okular altaïr.pdf")
}


setwd(initwd)
