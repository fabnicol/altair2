# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, années 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
# produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
# en vigueur à compter de l'année 2008.
# 
# Ce logiciel est régi par la licence CeCILL soumise au droit français et
# respectant les principes de diffusion des logiciels libres. Vous pouvez
# utiliser, modifier et/ou redistribuer ce programme sous les conditions
# de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
# sur le site "http://www.cecill.info".
# 
# En contrepartie de l'accessibilité au code source et des droits de copie,
# de modification et de redistribution accordés par cette licence, il n'est
# offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
# seule une responsabilité restreinte pèse sur l'auteur du programme, le
# titulaire des droits patrimoniaux et les concédants successifs.
# 
# A cet égard l'attention de l'utilisateur est attirée sur les risques
# associés au chargement, à l'utilisation, à la modification et/ou au
# développement et à la reproduction du logiciel par l'utilisateur étant
# donné sa spécificité de logiciel libre, qui peut le rendre complexe à
# manipuler et qui le réserve donc à des développeurs et des professionnels
# avertis possédant des connaissances informatiques approfondies. Les
# utilisateurs sont donc invités à charger et tester l'adéquation du
# logiciel à leurs besoins dans des conditions permettant d'assurer la
# sécurité de leurs systèmes et ou de leurs données et, plus généralement,
# à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
# 
# Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
# pris connaissance de la licence CeCILL, et que vous en avez accepté les
# termes.
# 
# 

extraire_paye <- function(an, L) {
  
  if (! is.null(L)) return(unique(Bulletins.paie[Année == an
                                  & Mois == 12
                                  & Statut != "ELU"
                                  & Statut %chin% L,
                                  c(clé.fusion, "Nir"), with=FALSE], by = NULL))
  
  
  return(unique(Bulletins.paie[Année == an
                               & Mois == 12
                               & Statut != "ELU",
                               c(clé.fusion, "Nir"), with=FALSE], by = NULL))
}


produire_pyramides <- function(Filtre_bulletins, titre, versant = "") {

  
  année.fin.comp <- if (versant != "") {
                      max(début.période.sous.revue, min(altair::année_comparaison(versant)$année, fin.période.sous.revue))
                  } else fin.période.sous.revue
  
  
  
  Bulletins.début.psr <- extraire_paye(début.période.sous.revue, Filtre_bulletins)
  
  Bulletins.fin.psr   <- extraire_paye(fin.période.sous.revue, Filtre_bulletins)
  
       ages.début.psr <- extraire.nir(Bulletins.début.psr, début.période.sous.revue)
  
         ages.fin.psr <- extraire.nir(Bulletins.fin.psr, fin.période.sous.revue)
  
  
  source("pyramides.R", local = TRUE, encoding = encodage.code.source)
  
  pyramides(Bulletins.début.psr, 
            Bulletins.fin.psr,
            ages.début.psr,
            ages.fin.psr,
            titre,
            versant)
   
  stub <- gsub(" ", "-", sub("â", "a", titre)) %+% "_"
  
  nom.fichier.avant <- stub %+% début.période.sous.revue
  nom.fichier.après <- stub %+% fin.période.sous.revue
  

  Sauv.base(file.path(chemin.dossier.bases, "Effectifs"),  "ages.début.psr", nom.fichier.avant, environment = environment())
  Sauv.base(file.path(chemin.dossier.bases, "Effectifs"),  "ages.fin.psr", nom.fichier.après, environment = environment())
  
}


