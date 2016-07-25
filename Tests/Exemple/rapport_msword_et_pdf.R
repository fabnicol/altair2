# encoder ce script en windows-1252

source("prologue_rapport.R", encoding = "ISO-8859-1")

source("corps_rapport_pdf.R", encoding = encodage.code.source)  

  ## début de hack ## 
  
  # Hack relativement laid mais très efficace qui évite de refaire tourner render pour la fabrication d'un .md
  # en recalculant les mêmes choses afin de produire le nouveau format.
  # On utilise le .md produit par keep_md = TRUE plus haut et les images conservées grâce à clean_supporting = FALSE
  # Mais il faut réinsérer un nouveau YAML header pour effacer les noms, dates etc. standard
  # et aussi effacer un FALSE indésirable produit par render en tête de .md (qui correspond au traitement du premier YAML header du pdf)
  # Les autres alternatives ont été essayées et échouent. La production directe de .md par render échoue sur la question
  # des liens URL comportant un caractère non-ascii.
  
  # attention encodage UTF-8 obligatoire quelle que soit la plateforme
  
  con <- file("altair.2.md", open = "a", encoding = "UTF-8")
  
  writeLines(text = "\n---\ntitle: false\nauthor: false\ndate: false\n---\n", con)     
  
  writeLines(readLines("altair.md", encoding = "UTF-8")[-c(1, 2)], con)
  
  close(con)
  file.rename("altair.2.md", "altair.md")
  
  # A réévaluer en fonction de l'évolution de la bibliothèque rmarkdown et de pandoc
  
  ## fin de hack ##
  
  system(
    paste(
      ifelse(setOSWindows, file.path(Sys.getenv("R_HOME"), "../RStudio/bin/pandoc/pandoc.exe"), "/usr/bin/pandoc"),
      "altair.md +RTS -K512m -RTS --to",
      ifelse(setOSWindows, "docx", "odt"),
      "--from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --highlight-style tango --output",
      ifelse(setOSWindows, "altaïr.docx", "altaïr.odt"))
    )
  
  if (setOSWindows) shell("start winword altaïr.docx") else system("/usr/bin/lowriter altaïr.odt")
  
  if (! keep_md) {
    unlink("altair.ansi_pdf", recursive=TRUE)
    unlink("altair.md")
  }
  
  unlink("altair_files", recursive = TRUE)  


setwd(initwd)
