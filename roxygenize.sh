cd altair.linux/R; 
for i in *; do iconv -t UTF-8 -f ISO-8859-15 $i -o $i.2 &&  mv $i.2 $i; done
cd ..
for i in *; do iconv -t UTF-8 -f ISO-8859-15 $i -o $i.2 &&  mv $i.2 $i; done
sed -i 's/latin1/UTF-8/' DESCRIPTION
cd ..
Rscript roxygenize.R
cd altair.linux/R
for i in *; do iconv -f UTF-8 -t ISO-8859-15 $i -o $i.2 &&  mv $i.2 $i; done
cd ..
for i in *; do iconv -f UTF-8 -t ISO-8859-15 $i -o $i.2 &&  mv $i.2 $i; done
sed -i 's/UTF-8/latin1/' DESCRIPTION
cd ..

