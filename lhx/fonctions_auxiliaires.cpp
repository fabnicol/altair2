/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
 */

#include <mutex>
#include <cstring>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include "fonctions_auxiliaires.hpp"
#include "tags.h"

#ifdef MMAP_PARSING
#ifdef __linux__
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <sys/stat.h>
#include <assert.h>
#else
#error "La compilation MMAP ne peut se faire que sous unix."
#endif
#endif


static const char* entete_char[]={"R", u8"Année", "Mois", "Budget", "Etablissement", "Siret", "Nom", u8"Prénom", "Matricule", "Service", "Nb.Enfants", "Statut", "Temps.de.travail",
                                  "Heures.Sup.", "Heures", "Indice", "Brut", "Net", u8"Net.à.Payer", "NBI", u8"Libellé", "Code",
                                  "Base", "Taux", u8"Nb.Unité", "Montant", "Type", "Emploi", "Grade", "Nir"};

static const char* entete_char_bulletins[]={"R", u8"Année", "Mois", "Budget", "Etablissement", "Siret", "Nom", u8"Prénom", "Matricule", "Service", "Nb.Enfants", "Statut", "Temps.de.travail",
                                  "Heures.Sup.", "Heures", "Indice", "Brut", "Net", u8"Net.à.Payer", "NBI", "Emploi", "Grade", "Nir"};

#ifdef __WIN32__
#include <direct.h>
#include <string>
#include <windows.h>

// The directory path returned by native GetCurrentDirectory() no end backslash
std::string getexecpath()
{
    const unsigned long maxDir = 260;
    wchar_t currentDir[maxDir];
    GetCurrentDirectory(maxDir, currentDir);
    std::wstring ws(currentDir);
    std::string str(ws.begin(), ws.end());
    return str;
}

#else
#if defined(__linux__)

#include <unistd.h>
#define GetCurrentDir getcwd
std::string getexepath()
{

    char szTmp[32];
    sprintf(szTmp, "/proc/%d/exe", getpid());
    int bytes = MIN(readlink(szTmp, pBuf, len), len - 1);
    if(bytes >= 0)
        pBuf[bytes] = '\0';
    return bytes;
}

#endif
#endif

void ecrire_log(const info_t& info, std::ofstream& log, int diff)
{
    if (! info.chemin_log.empty())
    {

        if (log.good())
        #define P  " | "
        log << "Année " << P
            << info.Table[info.NCumAgentXml][Annee] << P
            << "Mois "  << std::setw(2) << info.Table[info.NCumAgentXml][Mois] << P
            << "Matricule " << std::setw(6) <<  info.Table[info.NCumAgentXml][Matricule] << P
            << "Rang global " << std::setw(6) <<  info.NCumAgentXml << P
            << "Rang dans fichier " << std::setw(5) <<  info.NAgent[info.fichier_courant] << P
            << "Analyseur C " << std::setw(6) << info.NLigne[info.NCumAgentXml] << P
            << "Xml " << std::setw(6) << info.NLigne[info.NCumAgentXml] - diff << P
            << "Différence " << std::setw(4) << diff << "\n";
        #undef P
    }
}



#if 0
 char* ecrire_chemin_base(const char* chemin_base, int rang_fichier_base)
{
    int s = strlen(chemin_base);
    char* chemin = new char[s + 1 + 3]();   // chemin_base + _ + 3 chiffres
    int cut = s - strlen(CSV);
    strncpy(chemin, chemin_base, cut);

    /*  si rang_fichier_base == 0, base monolithique
        si rang_fichier_base compris entre 1 et nbType, base par catégorie
        si rang_fichier_base supérieur à nbType, base par année (les années sont très supérieures au nombre de type maximum ! */
    int test = (int) (rang_fichier_base + nbType -1) / nbType;

    switch (test)
    {
        /* rang_ fichier_base == 0  -->   Table.csv */
        case 0 :
                  sprintf(chemin + cut, "%s", CSV);
                  break;

        /* 1 <= rang_ fichier_base <= nbType  --> Table_TraitBrut.csv etc. */
        case 1 :
                  sprintf(chemin + cut, "_%s%s", type_remuneration[rang_fichier_base-1], CSV);
                  break;

        /* nbType < rang_fichier_base   --> Table_2008.csv etc. */
        default :
                  sprintf(chemin + cut, "_%d%s", rang_fichier_base - nbType - 1, CSV);
    }

    return(chemin);
}
#endif
 
void ecrire_entete_bulletins(const info_t &info, std::ofstream& base)
{
  ecrire_entete0(info, base, entete_char_bulletins, sizeof(entete_char_bulletins)/sizeof(char*));
}

void ecrire_entete_table(const info_t &info, std::ofstream& base)
{
  ecrire_entete0(info, base, entete_char, sizeof(entete_char)/sizeof(char*));
}

void ecrire_entete0(const info_t &info, std::ofstream& base, const char* entete[], int N)
{
  int i;
  if (info.select_siret)
    for (i = !info.generer_rang; i < N - 1; ++i)
      base << entete[i] << info.separateur;
  else
    for (i = !info.generer_rang; i < N - 1; ++i)
    {
        if (i != Budget +1 && i!= Siret +1 && i != Employeur + 1)
            base << entete[i] << info.separateur;
    }

  base << entete[i] << "\n";
}

void ouvrir_fichier_bulletins(const info_t &info, std::ofstream& base)
{
    ouvrir_fichier_base0(info, BaseCategorie::BULLETINS, BaseType::MONOLITHIQUE, base);
}

void ouvrir_fichier_base(const info_t &info, BaseType type, std::ofstream& base)
{
    ouvrir_fichier_base0(info, BaseCategorie::BASE, type, base);
}

void ouvrir_fichier_base0(const info_t &info, BaseCategorie categorie, BaseType type, std::ofstream& base)
{
    std::string chemin_base = "";
    std::string  index = "-";

    static int rang;
    std::array<std::string, 12> types_extension = { "traitements",
                                                     "indemnités-résidence",
                                                     "sft",
                                                     "avantages-nature",
                                                     "indemnités",
                                                     "divers",
                                                     "déductions",
                                                     "acompte",
                                                     "rappels",
                                                     "retenues",
                                                     "cotisations",
                                                     "commentaires" };

    if (categorie == BaseCategorie::BULLETINS)
    {
        chemin_base = info.chemin_bulletins + std::string(CSV);
    }
    else
    {

        chemin_base = info.chemin_base;      // sans l'extension csv

        switch (type) // OK en C++14
        {
           case BaseType::PAR_ANNEE:
               index = index + std::to_string(++rang) +  std::string(CSV);
               chemin_base = chemin_base + index;
               break;

           case BaseType::PAR_COMMENTAIRE:

           case BaseType::PAR_COTISATION:
             ++rang;

           case BaseType::PAR_RETENUE:
            ++rang;

           case BaseType::PAR_RAPPEL:
            ++rang;

           case BaseType::PAR_ACOMPTE:
             ++rang;

           case BaseType::PAR_DEDUCTION:
             ++rang;

           case BaseType::PAR_REM_DIVERSES:
             ++rang;

           case BaseType::PAR_INDEMNITE:
             ++rang;

           case BaseType::PAR_AVANTAGE_NATURE:
             ++rang;

           case BaseType::PAR_SFT:
             ++rang;

           case BaseType::PAR_INDEMNITE_RESIDENCE:
             ++rang;

           case BaseType::PAR_TRAITEMENT:
             ++rang;
            break;

           case BaseType::TOUTES_CATEGORIES:
                    ++rang;
        }

        chemin_base = chemin_base + std::string("-") + types_extension[rang-1] + CSV;
    }

    base.open(chemin_base);
    base.seekp(0);
    if (! base.good())
    {
        std::cerr << ERROR_HTML_TAG "Impossible d'ouvrir le fichier de sortie " << chemin_base << "\n";
        exit(-1000);
    }

    if (categorie == BaseCategorie::BASE)
        ecrire_entete_table(info, base);
    else
        ecrire_entete_bulletins(info, base);

    return;
}

int32_t lire_argument(int argc, char* c_str)
{
    if (argc > 2)
    {
        char *end;
        int s = strlen(c_str);
        errno = 0;

        if (c_str[0] == '\'' && c_str[s - 1] == '\'')
         {
           c_str++;
           c_str[s - 2] ='\0';
         }

        const long sl = strtol(c_str, &end, 10);

        if (end == c_str)
        {
            std::cerr << ERROR_HTML_TAG "" << c_str << ": pas un décimal\n";
        }
        else if (sl > INT32_MAX)
        {
            std::cerr << ERROR_HTML_TAG "" <<  sl << " entier excédant la limite des entiers à 16 bits\n";
        }
        else if (sl < 0)
        {
            std::cerr << ERROR_HTML_TAG "" << sl <<". L'entier doit être positif\n";
        }
        else
        {
            return((uint32_t)sl);
        }
        return(-1);
    }
    else
    {
        std::cerr << ERROR_HTML_TAG "Préciser le nombre de bulletins mensuels attendus (majorant du nombre).\n";
        return(-1);
    }
}


int calculer_memoire_requise(info_t& info)
{
    errno = 0;

    // Attention reserve() ne va pas initialiser les membres à 0 sous Windows. Utiliser resize() ici.

#ifdef PREALLOCATE_ON_HEAP

    /* C++ style vector allocation */

    #define tab info.NLigne
        info.NLigne.resize(info.threads->argc * MAX_NB_AGENTS);
#else

  /* C style vector allocation */

    uint16_t tab[info.threads->argc * MAX_NB_AGENTS];
   // memset(tab, 0, info.threads->argc * MAX_NB_AGENTS) peut éventuellement être utile pour certains compilateurs anciens.

#endif

    char d = 0;

    /* on compte un agent par balise <Remuneration/> ou par couple valide de balise <Remuneration>...</Remuneration> (fermeture contrôlée)
     * alors :
     *   on compte un agent en plus (++info.NCumAgent) avec un nombre de ligne égal au moins à un, même si pas de ligne de paye codée.
     *   Si il existe N lignes de paye codées, alors info.NLigne[info.NCumAgent] = N. */

    std::cerr << "\n" PROCESSING_HTML_TAG "Premier scan des fichiers pour déterminer les besoins mémoire ... \n";

    /* par convention  un agent avec rémunération non renseignées (balise sans fils) a une ligne */
    for (unsigned i = 0; i < info.threads->argc; ++i)
    {
         #ifdef GENERATE_RANK_SIGNAL
           generate_rank_signal();
           std::cerr <<  " \n";
         #endif

#ifdef FGETC_PARSING

        std::ifstream c;
        c.open(info.threads->argv[i]);
        if (c.good())
            c.seekg(0, c.beg);
        else 
        {
            std::cerr <<  ERROR_HTML_TAG "Ouverture du fichier  " << info.threads->argv[i] << "\n";
            exit(-120);
        }

        errno = 0;

        while (! c.eof())
        {
            bool remuneration_xml_open = false;

            if  (c.get() != '<') continue;
            if  (c.get() != 'R') continue;
            if  (c.get() != 'e') continue;
            if  (c.get() != 'm') continue;
            if  (c.get() != 'u') continue;
            if  (c.get() != 'n') continue;

#ifndef FULL_PREALLOCATION_TEST
            c.get(),c.get(),c.get(),c.get(),c.get(),c.get(),c.get();
#else
            if  (c.get() != 'e') continue;
            if  (c.get() != 'r') continue;
            if  (c.get() != 'a') continue;
            if  (c.get() != 't') continue;
            if  (c.get() != 'i') continue;
            if  (c.get() != 'o') continue;
            if  (c.get() != 'n') continue;
#endif
            remuneration_xml_open = true;

            if  (c.get()  == '/')
            {
                tab[info.NCumAgent] = 1;
                ++info.NCumAgent;

                remuneration_xml_open = false;
                continue;  // Balise simple vide
            }

            while (! c.eof())
            {
                if (c.get() != '<') continue;
                if ((d = c.get())  != 'C')
                {
                    if (d != '/') continue;
                    else if (c.get()  != 'R')   continue;
                    else if (c.get()  != 'e')   continue;
                    else if (c.get()  != 'm')   continue;
                    else if (c.get()  != 'u')   continue;
                    else if (c.get()  != 'n')   continue;

                    remuneration_xml_open = false;

                    if (tab[info.NCumAgent] == 0)
                        tab[info.NCumAgent] = 1;

                    ++info.NCumAgent;
                    break;
                }
                else
                {
                    if (c.get() != 'o') continue;
                    else
                    {
                        if (c.get() != 'd')   continue;
                        else
                        {
                            if (c.get() != 'e')   continue;
                            else
                            {
                                if (c.get() != ' ')   continue;
                                ++tab[info.NCumAgent];
                            }
                        }
                    }
                }
            }

            if (remuneration_xml_open == true)
            {
                std::cerr << "Erreur XML : la balise Remuneration n'est pas refermée pour le fichier " << info.threads->argv[i]
                          << "\npour l'agent n°"   << info.NCumAgent + 1 << "\n";
                exit(0);
#ifndef STRICT
                continue;
#else
                exit(-100);
#endif
            }

        }

        c.clear();
        c.close();

#ifdef PREALLOCATE_ON_HEAP
#undef tab
#else
        info.NLigne.assign(tab, tab + info.NCumAgent);
#endif

#endif
#ifdef MMAP_PARSING

        //std::cerr << "Mappage en mémoire de " << info.threads->argv[i] << "...\n";
        struct stat st;
        stat(info.threads->argv[i], &st);
        const size_t file_size =  st.st_size;
        void *dat;
        int fd = open(info.threads->argv[i], O_RDONLY);
       // std::cerr << "Taille : " << file_size << "\n";
        assert(fd != -1);
        dat = mmap(NULL, file_size,  PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
        assert(dat != NULL);
        //write(1, dat, file_size);
        char* data = (char*) dat;
       // std::cerr << "Mapping OK\n";
        size_t d = 0;
        char C;
        
        while (d < file_size - 14)
        {

            if  (data[++d] != '<') continue;
            if  (data[++d] != 'R') continue;
            if  (data[++d] != 'e') continue;
            if  (data[++d] != 'm') continue;
            if  (data[++d] != 'u') continue;
            if  (data[++d] != 'n') continue;
            if  (data[++d] != 'e') continue;
            if  (data[++d] != 'r') continue;
            if  (data[++d] != 'a') continue;
            if  (data[++d] != 't') continue;
            if  (data[++d] != 'i') continue;
            if  (data[++d] != 'o') continue;
            if  (data[++d] != 'n') continue;
            if  (data[++d] == '/')
            {
                info.NLigne[info.NCumAgent]=1;
                ++info.NCumAgent;
                continue;  // Balise simple vide
            }

            while (d < file_size - 1)
            {
                if (data[++d] != '<') continue;
                if ((C = data[++d]) != 'C')
                {
                    if (C != '/') continue;
                    else if (data[++d] != 'R')   continue;
                    else if (data[++d] != 'e')   continue;
                    else if (data[++d] != 'm')   continue;
                    else if (data[++d] != 'u')   continue;
                    else if (data[++d] != 'n')   continue;

                    if (info.NLigne[info.NCumAgent] == 0) info.NLigne[info.NCumAgent] = 1;
                    //info->NAgent[i]++;
                    ++info.NCumAgent;
                    break;
                }
                else
                {
                    if (data[++d] != 'o') continue;
                    else
                    {
                        if (data[++d] != 'd')   continue;
                        else
                        {
                            if (data[++d] != 'e')   continue;
                            else
                            {
                                if (data[++d] != ' ')   continue;
                                
                                ++info.NLigne[info.NCumAgent];
                            }
                        }
                    }
                }
            }
        }
        
        
        
        munmap(data, file_size);
        close(fd);
#endif        
        
    }

    /* A ETUDIER */
#ifdef PREALLOCATE_ON_HEAP
    info.NLigne.resize(info.NCumAgent+1);
#endif

    return errno;
}

