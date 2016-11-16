
notice <- function(infile, outfile) {
  system(paste("pandoc -V papersize=A4  -V  geometry:top=2cm,bottom=1.5cm,left=2cm,right=1.5cm -V urlcolor=cyan -t latex -o",
               outfile,
               infile))
}
