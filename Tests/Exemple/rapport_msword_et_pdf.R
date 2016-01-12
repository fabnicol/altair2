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
  
  system(
    paste(
      file.path(Sys.getenv("R_HOME"), "../RStudio/bin/pandoc/pandoc.exe"),
      "-V papersize=A4 +RTS -K512m -RTS altair.utf8.md --to latex --from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --output altaïr.pdf --template ",
      file.path(
        Sys.getenv("R_HOME"),
        "..",
        "lib/rmarkdown/rmd/latex/default.tex"
      ),
      " --highlight-style tango --latex-engine ",
      file.path(
        Sys.getenv("R_HOME"),
        "..",
        "texlive/miktex/bin/pdflatex.exe"
      ),
      " --variable geometry:margin=1in"
    )
  )
  shell("start acrord32.exe altaïr.pdf")
  
} else {
  setwd("Tests/Exemple")
  source("syspaths.R", encoding = encodage.code.source)
  library(knitr)
  opts_chunk$set(echo = FALSE, warning = FALSE, message = FALSE, results = 'asis')
  source("prologue.R", encoding = encodage.code.source)
  writeLines(iconv(readLines("altair.R"), from = encodage.code.source, to = "UTF-8"),
             "altair.utf8.R")
  
  library(rmarkdown)
  #spin("altair.utf8.R", knit=FALSE)
  rmarkdown::knitr_options_html(fig_width=8, fig_height=4, fig_retina=6, keep_md=FALSE)
  rmarkdown::knitr_options_pdf(fig_width=8, fig_height=4, fig_crop=TRUE)
  
  rmarkdown::render("altair.utf8.R", clean = FALSE,  "html_document")
  
  system(paste("/usr/bin/pandoc",
                "altair.utf8.html  --to odt --from  html --output altaïr.odt --highlight-style tango"))
  
  system(paste("/usr/local/bin/wkhtmltopdf", 
               "--page-size A4 --quiet altair.utf8.html altaïr.pdf")) 
  
  system("/usr/bin/lowriter altaïr.odt")

  system("/usr/bin/okular altaïr.pdf")
}

#unlink("figure", recursive = TRUE)
setwd(initwd)
