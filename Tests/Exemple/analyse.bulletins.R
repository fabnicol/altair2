
Bulletins.paie.nir.total.hors.élus <- unique(Bulletins.paie[Année == fin.période.sous.revue
                                                            & Mois == 12
                                                            & Statut != "ELU",
                                                            c(clé.fusion, "Nir"), with=FALSE], by = NULL)

Bulletins.paie.nir.fonctionnaires  <- unique(Bulletins.paie[Année == fin.période.sous.revue
                                                            & Mois  == 12
                                                            & (Statut == "TITULAIRE" |
                                                                 Statut == "STAGIAIRE"),
                                                            c(clé.fusion, "Nir"), with=FALSE], by = NULL)

Bulletins.paie.nir.nontit  <- unique(Bulletins.paie[Année == fin.période.sous.revue
                                                    & Mois  == 12
                                                    & Statut == "NON_TITULAIRE",
                                                    c(clé.fusion, "Nir"), with=FALSE], by = NULL)


Bulletins.paie.nir.permanents  <- unique(Bulletins.paie[Année == fin.période.sous.revue
                                                        & Mois  == 12
                                                        & (Statut == "NON_TITULAIRE" | Statut == "STAGIAIRE" | Statut == "TITULAIRE"),
                                                        c(clé.fusion, "Nir"), with=FALSE], by = NULL)

names(Bulletins.paie.nir.total.hors.élus) <- c(clé.fusion, "Nir")

# Age au 31 décembre de l'exercice dernier.exerciceal de la période sous revue
# ne pas oublier [ ,...] ici:

cut_16_18 <- function(x) x[x >= 16 & x <= 68]

années.fonctionnaires   <- cut_16_18(extraire.nir(Bulletins.paie.nir.fonctionnaires, fin.période.sous.revue))

années.total.hors.élus  <- cut_16_18(extraire.nir(Bulletins.paie.nir.total.hors.élus, fin.période.sous.revue))

années.total.permanents <- cut_16_18(extraire.nir(Bulletins.paie.nir.permanents, fin.période.sous.revue))

années.total.nontit     <- cut_16_18(extraire.nir(Bulletins.paie.nir.nontit, fin.période.sous.revue))

Bulletins.paie.nir.total.hors.élus.début <- unique(Bulletins.paie[Année == début.période.sous.revue
                                                                  & Mois == 12
                                                                  & Statut != "ELU",
                                                                  c(clé.fusion, "Nir"), with=FALSE], by = NULL)

Bulletins.paie.nir.fonctionnaires.début  <- unique(Bulletins.paie[Année == début.période.sous.revue
                                                                  & Mois  == 12
                                                                  & (Statut == "TITULAIRE" |
                                                                       Statut == "STAGIAIRE"),
                                                                  c(clé.fusion, "Nir"), with=FALSE], by = NULL)

Bulletins.paie.nir.nontit.début          <- unique(Bulletins.paie[Année == début.période.sous.revue
                                                                  & Mois == 12
                                                                  & Statut == "NON_TITULAIRE"   , .(Matricule, Nir)], by = NULL)

Bulletins.paie.nir.permanents.début      <- unique(Bulletins.paie[Année == début.période.sous.revue
                                                                  & Mois  == 12
                                                                  & (Statut == "NON_TITULAIRE" | Statut == "STAGIAIRE" | Statut == "TITULAIRE"),
                                                                  c(clé.fusion, "Nir"), with=FALSE], by = NULL)


names(Bulletins.paie.nir.total.hors.élus.début) <- c(clé.fusion, "Nir")


# Age au 31 décembre de l'exercice dernier.exerciceal de la période sous revue
# ne pas oublier [ ,...] ici:

années.fonctionnaires.début   <- cut_16_18(extraire.nir(Bulletins.paie.nir.fonctionnaires.début, début.période.sous.revue))

années.total.hors.élus.début  <- cut_16_18(extraire.nir(Bulletins.paie.nir.total.hors.élus.début, début.période.sous.revue))

années.total.nontit.début     <- cut_16_18(extraire.nir(Bulletins.paie.nir.nontit.début, début.période.sous.revue))

années.total.permanents.début <- cut_16_18(extraire.nir(Bulletins.paie.nir.permanents.début, début.période.sous.revue))
