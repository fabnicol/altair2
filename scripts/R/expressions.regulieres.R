# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, annees 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
#
# Ce logiciel est un programme informatique servant à extraire et analyser
# les fichiers de paye produits au format spécifié par l'annexe de la
# convention-cadre de dématérialisation en vigueur à partir de 2008.
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

# ESPACES DE VALEURS LICITES POUR CERTAINS CHAMPS (modalités)

########## Problématique ##############

# expressions régulières

#\\bxyz veut aussi dire cde.xyz(...), à éviter

# --- NBI
# Schémas recherchés :
#    nbi, n.b.., n(o*).? *b(o*).? *i(nd*).?
# ex: NBI, N.B.I, NOUV. BON. IND., N BON IND
# ex: PFI, P.F.I, PR. FONCT. INFO., IND F INF

expression.rég.traitement     <- "(?:\\s|^)trait.*\\b"
expression.rég.nbi        <- "(?:\\s|^)nbi|(?:n(?:(?=o)o\\w*|)\\.?\\s*\\b)?b(?:(?=o)o\\w*|)\\.?\\s*\\bi(?:(?=n)nd\\w*|)\\.?\\b"
expression.rég.astreintes <- "astr.*"
expression.rég.nas        <- "log.*(:?n\\.?\\s*\\ba\\.?\\s*\\bs|n..?c.*ser)"

# --- FPH

expression.rég.primespecifique <- "pr\\.?\\w*\\b\\s*\\bsp..?ci.*"
expression.rég.ift <- "(?:\\bind\\.?\\w*\\s*forf\\.?\\w*\\s*(?:et\\s*|)te.*)"
expression.rég.primedeservice <- "\\bpr\\.?\\w*\\b\\s*(?:de\\s*)\\bs\\w+v"
expression.rég.primedetechnicite <- "\\bpr\\.?\\w*\\b\\s*\\b(?:de\\s*|)tech.*"
expression.rég.médecin <- "(?:\\bm..?d(?:\\.|ecin)\\b\\s+|prat\\.?\\w*\\b\\s*\\bhos).*"

# --- Prime de fonctions informatiques
# Schémas recherchés :
#    pfi, p.f.i., p(r*).? *f(o*).? *inf(o*).?, i(nd*).? *f(o*).? *inf(o*).?
# ex: PFI, P.F.I, PR. FONCT. INFO., IND F INF

expression.rég.pfi        <- "(?:^|\\s)pfi|(?:p(?:(?=r)r\\w*|)|i(?:(?=n)nd\\w*|))\\.?\\s*(?:\\bf(?:(?=o)o\\w*|)\\.?\\s*)?\\binf(?:(?=o)o\\w*|)\\.?\\b"
expression.rég.vacataires <- "HOR"

expression.rég.heures.sup <- "(?:^|\\s)iht?[s]|i(?:(?=n)n\\w*|)\\.?\\s*\\bh(?:(?=o)o\\w*|)\\.?.*\\bs(?:(?=u)u\\w*|)\\.?\\b|h(?:(?=e)e\\w*|)\\.?\\s*\\b(?:(?=s)s\\w*)\\.?\\b"
expression.rég.iat        <- "(?:^|\\s)i\\.?a\\.?t|i(?:(?=n)n\\w*|)\\.?\\s*\\ba(?:(?=d)d\\w*|)\\.?\\s*\\b(?:et\\s*\\b)?t(?:(?=e)e\\w*|)\\.?\\b"
expression.rég.ifts       <- "(?:^|\\s)i\\.?f\\.?t\\.?s|i(?:(?=n)n\\w*|)\\.?\\s*\\bf\\w*\\.?\\s*\\bt(?:(?=r)r\\w*|)\\.?\\s*\\bs(?:(?=u)u\\w*|)\\.?\\b"
expression.rég.pfr        <- "(?:^|\\s)p\\.?f\\.?r|(?:p(?:(?=r)r\\w*|))\\.?\\s*\\bfo?\\w*\\.?.*\\s+.*\\br\\w*s\\w*\\.?\\s*\\b"
expression.rég.iss        <- "(?:^|\\s)i\\.?s\\.?s|(?:i(?:(?=n)n\\w*|))\\.?\\s*\\bsu?\\w*\\.?.*\\s+.*\\bsp?\\w*\\.?\\s*\\b"
expression.rég.ifse       <- "(?:^|\\s)i\\.?f\\.?s\\.?e|(?:i(?:(?=n)n\\w*|))\\.?\\s*\\bfo?\\w*\\.?.*\\s+.*\\bs\\w*\\.?\\s*\\be.*"
expression.rég.iemp       <- "(?:^|\\s)i\\.?e\\.?m\\.?p|(?:i(?:(?=n)n\\w*|))\\.?\\s*\\bex?\\w*\\.?.*\\s+.*\\bm\\w*\\.?\\s*\\bpr.*"
expression.rég.psr        <- "(?:^|\\s)p\\.?s\\.?r|(?:p(?:(?=r)r\\w*|))\\.?\\s*\\bse?\\w*\\.?.*\\s+.*\\br\\w*n\\w*\\.?\\s*\\b"
expression.rég.ipf        <- "(?:^|\\s)i\\.?p\\.?f|(?:i(?:(?=n)n\\w*|))\\.?\\s*\\bpe?\\w*\\.?.*\\s+.*\\bf\\w*n\\w*\\.?\\s*\\b"

expression.rég.population <- "\\bASS(\\b|A).*"
expression.rég.élus       <- "ind.*[é,e]lu"

expression.rég.attaché    <- "(?:^|\\s)att\\w*\\.?\\b|se\\w*\\.?\\bma\\w*\\.?\\b"
expression.rég.attaché.p  <- "(?:^|\\s)att\\w*\\.?\\bpr\\w*\\.?\\b|dir\\w*\\.?\\b"
expression.rég.admin      <- "(?:^|\\s)adm\\w*\\.?\\b"
expression.rég.admin.hc   <- "(?:^|\\s)adm\\w*\\.?\\bh\\w*\\.?\\s?c\\w*\\.?\\b"
expression.rég.admin.g    <- "(?:^|\\s)adm\\w*\\.?\\b\\s?g\\w*\\.?\\b"
