# encoder ce script en windows-1252
encodage.code.source <- "ISO-8859-1"
initwd <- getwd()
setOSWindows  <- Sys.info()["sysname"] != "Linux"


if (setOSWindows) {
  setwd(file.path(Sys.getenv("R_HOME"), "../Tests/Exemple"))
  source("syspaths.R", encoding = encodage.code.source)
  knitr::opts_chunk$set(fig.width=8, fig.height=4, echo = FALSE, warning = FALSE, message = FALSE, results = 'asis')
  source("prologue.R", encoding = encodage.code.source)
  writeLines(
    iconv(readLines("altair.R"), from = encodage.code.source, to = "WINDOWS-1252"),
    "altair.ansi.R"
  )
  library(knitr)
  spin("altair.ansi.R")
  writeLines(iconv(
    readLines("altair.ansi.md"),
    from = "WINDOWS-1252",
    to = "UTF-8"
  ),
  "altair.utf8.md",
  useBytes = TRUE)
  system(
    paste(
      file.path(Sys.getenv("R_HOME"), "../RStudio/bin/pandoc/pandoc.exe"),
      "altair.utf8.md --to docx --from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --output altaïr.docx --highlight-style tango"
    )
  )
  shell("start winword altaïr.docx")
  unlink("figure", recursive = TRUE)
  
} else {
  setwd("Tests/Exemple")
  source("syspaths.R", encoding = encodage.code.source)
  knitr::opts_chunk$set(echo = FALSE, warning = FALSE, message = FALSE, results = 'asis')
  source("prologue.R", encoding = encodage.code.source)
  writeLines(iconv(readLines("altair.R"), from = encodage.code.source, to = "UTF-8"),
             "altair.utf8.R")
  library(rmarkdown)
  
  rmarkdown::render("altair.utf8.R", clean = FALSE,  html_document(fig_retina = 6))
  # produit altair.utf8.html
  
  system(
    paste(
      "/usr/bin/pandoc",
      "altair.utf8.html  --to odt --from  html --output altaïr.odt --highlight-style tango"
    )
  )
  
# fallback :  
#  system(
#    paste(
#      "/usr/bin/pandoc",
#      "altair.utf8.knit.md --to odt --from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --output altaïr.odt --highlight-style tango"
#    )
 # )
  
  system("/usr/bin/lowriter altaïr.odt")
  unlink("altair.utf8_files", recursive = TRUE)  
}


setwd(initwd)
