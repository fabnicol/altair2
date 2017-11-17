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

#include "recherche.hpp"
using namespace std;



/// \file    recherche.cpp
/// \author  Fabrice Nicol
/// \brief   Ce fichier contient le code permettant de rechercher en mémoire, après décodage des bases XML,
/// un bulletin de paye particulier correspondant à un matricule, un mois et une année. Il contient aussi le code
/// permettant l'itération de cette fonctionnalité sur des intervalles temporels.

vector<string>  recherche(const vector<info_t> &Info, const string& annee, const string& mois, const string& matricule)
{
  vector<string> bulletins;
  
  auto matr = (const xmlChar*) matricule.c_str();  
  int m = stoi(mois);
  int a = stoi(annee);
  
  for (unsigned int i = 0; i < Info[0].nbfil; ++i)
  {
    for (vector<vector<xmlChar*>>::const_iterator  it = Info[i].Table.begin(); it != Info[i].Table.end(); ++it)
    {
        if (atoi((const char*) it->at(Annee)) == a
            && atoi((const char*) it->at(Mois)) ==  m
            && xmlStrcmp(it->at(Matricule), matr) == 0)
        {
            long long index = it - Info[i].Table.begin();
            array<uint64_t, 3> debut = Info[i].ligne_debut.at(index);
            array<uint64_t, 2> fin   = Info[i].ligne_fin.at(index);
            
            // trouver la ligne debut. lire jusqu'à fin dans le fichier F à déterminer (GUI)
            const string fichier = extraire_lignes(Info[i], debut, fin);
            
            const string preambule =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<DocumentPaye>\n\
 <IdVer V=\"1.0\"/>\n\
 <Annee V=\"" + annee + "\"/>\n\
 <Mois V=\"" + mois + "\"/>\n\
 <Budget>\n\
  <Libelle V=\"" + string((const char*)it->at(Budget)) + "\"/>\n\
  <Code V=\"\"/>\n\
 </Budget>\n\
 <Employeur>\n\
  <Nom V=\"" + string((const char*) it->at(Employeur)) + "\"/>\n\
  <Siret V=\"" + string((const char*) it->at(Siret)) + "\"/>\n\
 </Employeur>\n\
 <DonneesIndiv>\n";
            
            const string coda = " </DonneesIndiv>\n</DocumentPaye>\n";     
            
            if (! fichier.empty())
               bulletins.emplace_back(preambule + fichier + coda);
        }
    }

  }
    
    return bulletins;
}

const string extraire_lignes(const info_t& info, const array<uint64_t, 3>& debut, const array <uint64_t, 2>& fin)
{

 if (fin[1] < debut[1]) return "";

 string xhl = info.threads->in_memory_file.at(debut[2]);

 string tab = xhl.substr(debut[1], fin[1] - debut[1] + 1);

 tab += "\n";

 return tab;
}

bool bulletin_paye(const string& chemin_repertoire, const vector<info_t> &Info, const string& matricule, const string& mois, const string& annee)
{
    cerr << ENDL;
    cerr << PROCESSING_HTML_TAG << "Extraction des bulletins..." << ENDL;

    auto bulletins = recherche(Info, annee, mois, matricule);
    int rang = 0;
    bool res = true;
    
    for (auto &&bulletin : bulletins)
    {
        ++rang;
        string nom_bulletin =  annee + string("_") + mois + string("_") + matricule
                              + (rang > 1 ? "_" + to_string(rang) : "") + string(".xml");
                
        ofstream f;
        
        f.open(chemin_repertoire + "/" + nom_bulletin, std::ofstream::out | std::ofstream::trunc);
        
        if (f.is_open())
            f << bulletin;
        else
        {
           cerr << ERROR_HTML_TAG "Echec de la génération du bulletin pour le matricule "
                << matricule << " Année : " << annee << " Mois : " << mois << ENDL;
           res = false;
           continue;
        }
        
        f.close();
               
        cerr << STATE_HTML_TAG "Bulletin extrait pour le matricule " << matricule;

        if (rang > 1)
        {
            cerr << " Rang local : " << rang;
        }

        cerr << " Année : " << annee << " Mois : " << mois << ENDL;
    }
    
    return res;
 }
