for i in 2009
do  
  echo $i...
  sed -i -e 's/\(Prenom V=\"\)\(.*\)\(\"\)/\1XXX\3/g'  -e 's/\(Nom V=\"\)\(.*\)\(\"\)/\1YYY\3/g'     -e 's/\(NIR V=\"\)\(.....\)\(.*\)\(\"\)/\1\2ZZZ\4/g'     -e 's/\(Adr. V=\"\)\(.....\)\(.*\)\(\"\)/\1\2YYY\4/g'    -e 's/\(Ville V=\"\)\(.*\)\(\"\)/\1YYY\3/g'  -e 's/\(CP V=\"\)\(.*\)\(\"\)/\1YYY\3/g'  -e 's/\(TitCpte V=\"\)\(.*\)\(\"\)/\1YYY\3/g' -e 's/\(NumUrssaf V=\"\)\(.*\)\(\"\)/\1YYY\3/g'  -e 's/\(Siret V=\"\)\(.*\)\(\"\)/\1YYY\3/g'    $i/*.xhl 
done
