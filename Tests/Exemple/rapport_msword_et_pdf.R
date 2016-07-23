# encoder ce script en windows-1252

source("prologue_rapport.R", encoding = encodage.code.source)

source("corps_rapport_pdf.R", encoding = encodage.code.source)  

  ## d�but de hack ## 
  
  # Hack relativement laid mais tr�s efficace qui �vite de refaire tourner render pour la fabrication d'un .md
  # en recalculant les m�mes choses afin de produire le nouveau format.
  # On utilise le .md produit par keep_md = TRUE plus haut et les images conserv�es gr�ce � clean_supporting = FALSE
  # Mais il faut r�ins�rer un nouveau YAML header pour effacer les noms, dates etc. standard
  # et aussi effacer un FALSE ind�sirable produit par render en t�te de .md (qui correspond au traitement du premier YAML header du pdf)
  # Les autres alternatives ont �t� essay�es et �chouent. La production directe de .md par render �choue sur la question
  # des liens URL comportant un caract�re non-ascii.
  
  # attention encodage UTF-8 obligatoire quelle que soit la plateforme
  
  con <- file("altair.2.md", open = "a", encoding = "UTF-8")
  
  writeLines(text = "\n---\ntitle: false\nauthor: false\ndate: false\n---\n", con)     
  
  writeLines(readLines("altair.md", encoding = "UTF-8")[-c(1, 2)], con)
  
  close(con)
  file.rename("altair.2.md", "altair.md")
  
  # A r��valuer en fonction de l'�volution de la biblioth�que rmarkdown et de pandoc
  
  ## fin de hack ##
  
  system(
    paste(
      ifelse(setOSWindows, file.path(Sys.getenv("R_HOME"), "../RStudio/bin/pandoc/pandoc.exe"), "/usr/bin/pandoc"),
      "altair.md +RTS -K512m -RTS --to",
      ifelse(setOSWindows, "docx", "odt"),
      "--from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --highlight-style tango --output",
      ifelse(setOSWindows, "alta�r.docx", "alta�r.odt"))
    )
  
  if (setOSWindows) shell("start winword alta�r.docx") else system("/usr/bin/lowriter alta�r.odt")
  
  if (! keep_md) {
    unlink("altair.ansi_pdf", recursive=TRUE)
    unlink("altair.md")
  }
  
  unlink("altair_files", recursive = TRUE)  


setwd(initwd)
