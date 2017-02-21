
#include "recherche.hpp"
using namespace std;

vector<string>  recherche(const vector<info_t> &Info, const string& annee, const string& mois, const string& matricule)
{
  vector<string> bulletins;
  
  auto matr = (const xmlChar*) matricule.c_str();  
  auto m = (const xmlChar*) mois.c_str();
  auto a = (const xmlChar*) annee.c_str();
  
  for (unsigned int i = 0; i < Info[0].nbfil; ++i)
  {
    for (vector<vector<xmlChar*>>::const_iterator  it = Info[i].Table.begin(); it != Info[i].Table.end(); ++it)
    {
        if (xmlStrcmp(it->at(Annee), a) == 0 
            && xmlStrcmp(it->at(Mois), m) == 0
            && xmlStrcmp(it->at(Matricule), matr) == 0)
        {
            long long index = it - Info[i].Table.begin();
            array<uint64_t, 3> debut = Info[i].ligne_debut.at(index);
            array<uint64_t, 2> fin   = Info[i].ligne_fin.at(index);
            
            // trouver la ligne debut. lire jusqu'à fin dans le fichier F à déterminer (GUI)
            const string fichier = extraire_lignes(Info[i], debut, fin);
            if (! fichier.empty())
               bulletins.emplace_back(fichier);
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
                              + (rang > 1 ? "_" + to_string(rang) : "") + string(".txt");
                
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
