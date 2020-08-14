

cleanup <- function() {
  
  if (! keep_md) {
    unlink("altair.ansi_pdf", recursive=TRUE)
    unlink("altair.md")
    unlink("altair_files", recursive = TRUE)  
	unlink("temp.tex")
  }
}
  
ajuster_chemins_odt <- function(f) {
  

    con <- file(f, open = "r", encoding = encodage.code.source)
    con2 <- file("temp.tex", open = "w", encoding = encodage.code.source)
    V <- readLines(con, encoding = encodage.code.source)
	V <- gsub("\\href{Bases", "\\href{../Bases", V, fixed = TRUE)
	V <- gsub("\\href{Docs", "\\href{../Docs", V, fixed = TRUE)
    writeLines(V, con2)
    
    close(con2)
    close(con)
 }

#' Générer les fichiers docx et odt
#' @param pandoc Chemin de pandoc
#' infile Nom du fichier latex, par défaut altair.tex
#' outfile Nom du fichier docx de sortie, par défaut altair.docx
#' outfile2 Nom du fichier odt de sortie, par défaut altair.odt
#' args Arguments à passer à pandoc comme options de forme
#' @export

generer_docx_odt <- function(pandoc, 
                             infile = "altair.tex", 
                             outfile = "altair.docx", 
							 outfile2 = "altair.odt",
							 args = c("-V", "papersize=A4",  "-V", "geometry:top=2cm,bottom=1.5cm,left=2cm,right=1.5cm", "-V", "urlcolor=cyan", "--highlight-style", "tango")) {
  
  system2(
      pandoc,
      c(infile, 
	    "+RTS",
	    "-K512m",
        "-RTS",
	    "--to",
        "docx",
	     args, 
         "--output",
        outfile))
  
  ajuster_chemins_odt(infile)
  
  system2(
      pandoc,
      c("temp.tex",
	    "+RTS",
		"-K512m",
		"-RTS",
 		"--to",
        "odt",
	    args,
 		"--output",
        outfile2))
  
  cleanup()
  
  file.copy(c(outfile, outfile2), chemin.cle)

  if (ouvrir.document) {
    if (setOSWindows) {
      
      shell(paste("start winword.exe",  file.path(chemin.cle, outfile)))
      
    } else {
      
      system(paste("lowriter",  file.path(chemin.cle, outfile2)))
    }
  }
}
