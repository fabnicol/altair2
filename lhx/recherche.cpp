// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
// produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
// en vigueur à compter de l'année 2008.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions
// de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
// sur le site "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie,
// de modification et de redistribution accordés par cette licence, il n'est
// offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
// seule une responsabilité restreinte pèse sur l'auteur du programme, le
// titulaire des droits patrimoniaux et les concédants successifs.
//
// A cet égard l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant
// donné sa spécificité de logiciel libre, qui peut le rendre complexe à
// manipuler et qui le réserve donc à des développeurs et des professionnels
// avertis possédant des connaissances informatiques approfondies. Les
// utilisateurs sont donc invités à charger et tester l'adéquation du
// logiciel à leurs besoins dans des conditions permettant d'assurer la
// sécurité de leurs systèmes et ou de leurs données et, plus généralement,
// à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de la licence CeCILL, et que vous en avez accepté les
// termes.
//
//

#include "recherche.h"
using namespace std;

extern bool verbeux;

/// \file    recherche.cpp
/// \author  Fabrice Nicol
/// \brief   Ce fichier contient le code permettant de rechercher en mémoire, après décodage des bases XML,
/// un bulletin de paye particulier correspondant à un matricule, un mois et une année. Il contient aussi le code
/// permettant l'itération de cette fonctionnalité sur des intervalles temporels.

vector<string>  recherche (const vector<info_t> &Info, const string& annee, const string& mois, const string& matricule)
{
    // Bulletins à extraire
    vector<string> bulletins;

    auto matr = (const xmlChar*) matricule.c_str();
    int m = stoi (mois);
    int a = stoi (annee);

    // Parcourt l'ensemble des données de paye, pour tous les fils d'exécution, après décodage
    // Il faut donc que lhx se soit complètement exécuté préalablement

    // Boucle sur les fils d'exécution

    for (unsigned int i = 0; i < Info[0].nbfil; ++i)
        {

            // Boucle sur les données extraites pour un fil donné

            for (vector<vector<xmlChar*>>::const_iterator  it = Info[i].Table.begin(); it != Info[i].Table.end(); ++it)
                {
                    // On restreint la recherche à l'année, au mois et au matricule donnés
                    // On pourrait aller plus vite avec une table de hachage, mais l'expérience montre que ce n'est pas nécessaire
                    // it correspond à la partie de la Table pour un agent donné

                    if (atoi ((const char*) it->at (Annee)) == a
                            && atoi ((const char*) it->at (Mois)) ==  m
                            && xmlStrcmp (it->at (Matricule), matr) == 0)
                        {
                            // index correspond au rang de l'agent dans la Table (0 <= index <= NCumAgentXml)

                            long long index = it - Info[i].Table.begin();

                            // le vecteur ligne_debut a été construit lors de l'extraction : il permet de situer la ligne de début
                            // du code XML correspondant à l'agent de rang index dans le fichier et donnant le début de son propre bulletin
                            // de paye au format XML
                            // le vecteur ligne_fin donne la ligne de fin de ce code-bulletin

                            array<uint64_t, 3> debut = Info[i].ligne_debut.at (index);
                            array<uint64_t, 2> fin   = Info[i].ligne_fin.at (index);

                            // Lancer la fonction extraire_ligne qui copie les lignes XML entre la ligne de début et la ligne de fin

                            const string fichier = extraire_lignes (Info[i], debut, fin);

                            // Cela ne suffit pas à donner un fichier XML syntaxiquement correct.
                            // A cette fin, rajouter un préambule et une fin de fichier en accord avec ce préambule.
                            // Ce qui permettra éventuellement de "repasser" ce bulletin XML artificiellement créé dans lhx
                            // pour extraire ce bulletin particulier au format CSV

                            const string preambule =
                                "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<DocumentPaye>\n\
 <IdVer V=\"1.0\"/>\n\
 <Annee V=\"" + annee + "\"/>\n\
 <Mois V=\"" + mois + "\"/>\n\
 <Budget>\n\
  <Libelle V=\"" + string ((const char*)it->at (Budget)) + "\"/>\n\
  <Code V=\"\"/>\n\
 </Budget>\n\
 <Employeur>\n\
  <Nom V=\"" + string ((const char*) it->at (Employeur)) + "\"/>\n\
  <Siret V=\"" + string ((const char*) it->at (Siret)) + "\"/>\n\
 </Employeur>\n\
 <DonneesIndiv>\n";

                            const string coda = " </DonneesIndiv>\n</DocumentPaye>\n";

                            // Empiler le tout dans le vecteur bulletins

                            if (! fichier.empty())
                                bulletins.emplace_back (preambule + fichier + coda);
                        }
                }

        }

    return bulletins;
}

const string extraire_lignes (const info_t& info, const array<uint64_t, 3>& debut, const array <uint64_t, 2>& fin)
{

    if (fin[1] < debut[1]) return "";

    string xhl = info.threads->in_memory_file.at (debut[2]);

    string tab = xhl.substr (debut[1], fin[1] - debut[1] + 1);

    tab += "\n";

    return tab;
}

vector<string> bulletin_paye (const string& chemin_repertoire, const vector<info_t> &Info, const string& matricule, const string& mois, const string& annee)
{
    cerr << ENDL;
    cerr << PROCESSING_HTML_TAG << "Extraction des bulletins..." << ENDL;

    auto bulletins = recherche (Info, annee, mois, matricule);
    int rang = 0;
    vector<string> chemins_bulletins;

    for (auto &&bulletin : bulletins)
        {
            ++rang;

            // Classement par Matricule - Annee - Mois
            // Cet ordre permet un lcassement automatique optimal dans les gestionnaires de fichiers des SE.
            // Dans la pratique en effet on recherche d'abord les bulletins d'un agent, en triant ensuite par exercice et par mois.

            string nom_bulletin =  "Paye-" + matricule + "-" + annee + "-" + mois + ".xml";

            ofstream f;

            string chemin_bulletin = chemin_repertoire + "/" + nom_bulletin;

            chemins_bulletins.emplace_back(chemin_bulletin);

            f.open (chemin_bulletin, std::ofstream::out | std::ofstream::trunc);

            if (f.is_open())
                f << bulletin;
            else
                {
                    cerr << ERROR_HTML_TAG "Echec de la génération du bulletin pour le matricule "
                         << matricule << " Annee : " << annee << " Mois : " << mois << ENDL;

                    continue;
                }

            f.close();

            cerr << STATE_HTML_TAG "Bulletin extrait pour le matricule " << matricule;

            if (rang > 1)
                {
                    cerr << " Rang local : " << rang;
                }

            cerr << " Annee : " << annee << " Mois : " << mois << ENDL;
        }

    return chemins_bulletins;
}

inline
vector<string> __scan_mois (const string &repertoire_bulletins,
                const vector<info_t> &Info,
                const string &matricule,
                const string &mois,
                const string &annee)
{
    size_t pos = 0;
    vector<string>chemins_bulletins_extraits;

    // Les mois peuvent être donnés en intervalles du type 02...11
    // ce qui signifie : tous les mois entre février et novembre inclus

    if ((pos = mois.find_first_of ('.')) != string::npos)
        {
            int m0 = stoi (mois.substr (0, pos));
            pos = mois.find_last_of ('.');
            int m1 = stoi (mois.substr (pos + 1));

            // Boucler entre les deux mois ainsi donnés en borne inf et max
            // et lancer la fonction bulletin_paye sur chacun de ces mois

            for (int m = m0; m <= m1; ++m)
            {
                if (verbeux) cerr << PROCESSING_HTML_TAG "Annee : " << annee << " Extraction du mois : " << m  << ENDL;

                vector<string> c = bulletin_paye (repertoire_bulletins,
                                                   Info,
                                                   matricule,
                                                   to_string (m),
                                                   annee);

                 vect_concat(chemins_bulletins_extraits, c);
            }
        }

    // Si pas d'intervalle, lancer la fonction bulletin_paye sur le seul mois donné.

    else
    {
        if (verbeux) cerr << PROCESSING_HTML_TAG "Annee : " << annee << " Extraction du mois : " << mois  << ENDL;
        chemins_bulletins_extraits = bulletin_paye (repertoire_bulletins,
                                                    Info,
                                                    matricule,
                                                    mois,
                                                    annee);

    }

    return chemins_bulletins_extraits;
}

vector<string> scan_mois (const string &repertoire_bulletins,
                const vector<info_t> &Info,
                const string &matricule,
                const string &mois,
                const string &annee)
{
size_t pos = 0;
vector<string> chemins_bulletins_extraits;

// Les matricules peuvent être séparés par des virgules

if ((pos = matricule.find_first_of (',')) != string::npos)
    {
        vector<string> matriculeList = split(matricule, ',');

        for (auto &&m : matriculeList)
        {
             vect_concat(chemins_bulletins_extraits,
                         __scan_mois (repertoire_bulletins,
                                        Info,
                                        m,
                                        mois,
                                        annee));
        }
    }
else
    chemins_bulletins_extraits = __scan_mois (repertoire_bulletins,
                                                Info,
                                                matricule,
                                                mois,
                                                annee);

return chemins_bulletins_extraits;
}

