
render("altair.R",
       encoding = encodage.code.source,
       output_format = output_format(knitr_options(opts_chunk = list(fig.width = 7.5, 
                                                                     fig.height = 5,
                                                                     dpi = dpi,
                                                                     echo = FALSE,
                                                                     warning = FALSE,
                                                                     message = FALSE,
                                                                     results = 'asis')),
                                     keep_md = FALSE, clean_supporting = TRUE,
                                     pandoc = pandoc_options(to = ifelse(setOSWindows, "docx", "odt"),
                                                             from = "markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures",
                                                             args=c("-V", 
                                                                    "papersize=A4" ))),
       output_file = ifelse(setOSWindows, "altaïr.docx", "altaïr.odt"))

if (setOSWindows) {
  shell("start winword altaïr.docx")
} else {
  system("lowriter altaïr.odt")
}


setwd(initwd)