/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
 */

#include <mutex>
#include <cstring>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <iterator>
#include "fonctions_auxiliaires.hpp"
#include "tags.h"

#ifdef __WIN32__
#include "entete-latin1.hpp"
#else
#include "entete.hpp"
#endif

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


extern bool verbeux;

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
std::string getexecpath()
{

    char szTmp[32]={0};
    char pBuf[1000]={0};
    int len = sprintf(szTmp, "/proc/%d/exe", getpid());
    int bytes = std::min((int) readlink(szTmp, pBuf, len), len - 1);
    if(bytes >= 0)
        pBuf[bytes] = '\0';
    return std::string(pBuf);
}

#endif
#endif



/* utilité d'affichage de l'environnement xhl en cas de problème de conformité des données */

errorLine_t afficher_environnement_xhl(const info_t& info, const xmlNodePtr cur)
{
    long lineN = 0;
    std::cerr << WARNING_HTML_TAG "Fichier analysé " <<  info.threads->argv[info.fichier_courant] << ENDL;
    lineN = xmlGetLineNo(cur);
    if (lineN == -1)
        {
            std::cerr << WARNING_HTML_TAG "Une balise est manquante dans le fichier." << ENDL;
        }
        else
            std::cerr << WARNING_HTML_TAG "Ligne n°" << lineN << ENDL;

    /* Tableau_entete va être en shared memory concurrent read access (no lock here) */
#if 1
    for (int l = 0; l < info.Memoire_p_ligne[info.NCumAgentXml]; ++l)
        {
          if (nullptr != info.Table[info.NCumAgentXml][l])
              std::cerr << WARNING_HTML_TAG "Balise de paye : " << Tableau_entete[l]
                        << "  " << info.Table[info.NCumAgentXml][l] << ENDL;
        }
#endif
    errorLine_t s = {lineN, std::string("Fichier : ") + std::string(info.threads->argv[info.fichier_courant])
                             + std::string(" -- Balise : ") + ((cur)? std::string((const char*)cur->name) : std::string("NA"))};
    return s;
}


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
        if (i != Budget +1 &&  i != Employeur + 1 && i != Siret +1 && i != Etablissement + 1)
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
        chemin_base = info.chemin_bulletins
        #if defined(__WIN32__) && defined (USE_ICONV)
                + ".temp"
        #endif
                + std::string(CSV);
    }
    else
    {

        chemin_base = info.chemin_base
        #if defined(__WIN32__) && defined (USE_ICONV)
                + ".temp"
        #endif
                ;      // sans l'extension csv

        switch (type) // OK en C++14
        {
           case BaseType::MONOLITHIQUE:
              chemin_base = chemin_base + CSV;
            break;

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
            chemin_base = chemin_base + std::string("-") + types_extension[rang-1] + CSV;
            break;

           case BaseType::TOUTES_CATEGORIES:
            ++rang;
            chemin_base = chemin_base + std::string("-") + types_extension[rang-1] + CSV;
        }


    }

    base.open(chemin_base);
    base.seekp(0);
    if (! base.good())
    {
        std::cerr << ERROR_HTML_TAG "Impossible d'ouvrir le fichier de sortie " << chemin_base << ENDL;
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
            std::cerr << ERROR_HTML_TAG "" << c_str << ": pas un décimal" ENDL;
        }
        else if (sl > INT32_MAX)
        {
            std::cerr << ERROR_HTML_TAG "" <<  sl << " entier excédant la limite des entiers à 16 bits" ENDL;
        }
        else if (sl < 0)
        {
            std::cerr << ERROR_HTML_TAG "" << sl <<". L'entier doit être positif" ENDL;
        }
        else
        {
            return((uint32_t)sl);
        }
        return(-1);
    }
    else
    {
        std::cerr << ERROR_HTML_TAG "Préciser le nombre de bulletins mensuels attendus (majorant du nombre)." ENDL;
        return(-1);
    }
}

void calculer_maxima(const std::vector<info_t> &Info, std::ofstream* LOG)
{
    uint32_t maximum[2] = {0, 0};

    for (int i = 0; i < Info[0].nbfil; ++i)
    {
        for (unsigned j = 0; j < Info[i].threads->argc; ++j)
            if (Info[i].NAgent[j] > maximum[0])
                maximum[0] = Info[i].NAgent[j];

        for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
            if (Info[i].NLigne[agent] > maximum[1])
                maximum[1] = Info[i].NLigne[agent] + 1;
    }

/* TODO: Eclaircir le off-by-one sur NLigne */

        std::cerr <<  STATE_HTML_TAG "Maximum de lignes : " << maximum[1] << ENDL
                   << STATE_HTML_TAG "Maximum d'agents  : " << maximum[0] << ENDL;
        if (LOG != nullptr &&  LOG->good())
        {
           *LOG  <<  STATE_HTML_TAG "Maximum de lignes : " << maximum[1] << ENDL
                 << STATE_HTML_TAG "Maximum d'agents  : " << maximum[0] << ENDL;
        }

}


template <typename Allocator = std::allocator<char>>
auto read_stream_into_string(
    std::ifstream& in,
    Allocator alloc = {})
{
  std::basic_ostringstream<char, std::char_traits<char>, Allocator>
    ss(std::basic_string<char, std::char_traits<char>, Allocator>(std::move(alloc)));

  if (!(ss << in.rdbuf()))
    throw std::ios_base::failure{"[ERR] Erreur d'allocation de lecture de fichier.\n"};

  return ss.str();
}

int calculer_memoire_requise(info_t& info)
{
    errno = 0;

    // Attention reserve() ne va pas initialiser les membres à 0 sous Windows. Utiliser resize() ici.

#ifdef PREALLOCATE_ON_HEAP

    /* C++ style vector allocation */

    #define tab info.NLigne

        tab.resize(info.threads->argc * MAX_NB_AGENTS);
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


    /* par convention  un agent avec rémunération non renseignées (balise sans fils) a une ligne */
    for (unsigned i = 0; i < info.threads->argc; ++i)
    {
      #ifdef GUI_TAG_MESSAGES
         #ifdef GENERATE_RANK_SIGNAL

           generate_rank_signal();
           std::cerr <<  " \n" ;

         #endif
      #endif

      #if defined(FGETC_PARSING) || defined(STRINGSTREAM_PARSING)

        std::ifstream c(info.threads->argv[i]);

        if (c.is_open())
            c.seekg(0, std::ios::beg);
        else 
        {
            if (verbeux)
                std::cerr <<  ERROR_HTML_TAG "Problème à l'ouverture du fichier *" << info.threads->argv[i] << "*" << ENDL;
            exit(-120);
        }

        errno = 0;

       #ifdef FGETC_PARSING

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
                              << ENDL "pour l'agent n°"   << info.NCumAgent + 1 << ENDL;
                    exit(0);

                    #ifndef STRICT
                      continue;
                    #else
                      exit(-100);
                    #endif
                }

            }

        #else   // STRINGSTREAM_PARSING

            std::string ss;
            ss.reserve(10000000);
            std::ostringstream   SS(ss);

            if (!(SS << c.rdbuf()))
               throw std::ios_base::failure{ "[ERR] Erreur d'allocation de lecture de fichier.\n" };

            ss = SS.str();

            SS.str("");

            std::string::iterator iter = ss.begin();

            while (iter != ss.end())
            {
                bool remuneration_xml_open = false;

                if  (*++iter != '<') continue;
                if  (*++iter != 'R') continue;
                if  (*++iter != 'e') continue;
                if  (*++iter != 'm') continue;
                if  (*++iter != 'u') continue;
                if  (*++iter != 'n') continue;

                #ifndef FULL_PREALLOCATION_TEST

                   iter += 7;

                #else

                   if  (*++iter != 'e') continue;
                   if  (*++iter != 'r') continue;
                   if  (*++iter != 'a') continue;
                   if  (*++iter != 't') continue;
                   if  (*++iter != 'i') continue;
                   if  (*++iter != 'o') continue;
                   if  (*++iter != 'n') continue;

                #endif

                remuneration_xml_open = true;

                if  (*++iter  == '/')
                {
                    tab[info.NCumAgent] = 1;
                    ++info.NCumAgent;

                    remuneration_xml_open = false;
                    continue;  // Balise simple vide
                }

                while (iter != ss.end())
                {
                    if (*++iter != '<') continue;
                    if ((d = *++iter)  != 'C')
                    {
                        if (d != '/') continue;
                        else if (*++iter  != 'R')   continue;
                        else if (*++iter  != 'e')   continue;
                        else if (*++iter  != 'm')   continue;
                        else if (*++iter  != 'u')   continue;
                        else if (*++iter  != 'n')   continue;

                        remuneration_xml_open = false;

                        if (tab[info.NCumAgent] == 0)
                            tab[info.NCumAgent] = 1;

                        ++info.NCumAgent;
                        break;
                    }
                    else
                    {
                        if (*++iter != 'o') continue;
                        else
                        {
                            if (*++iter != 'd')   continue;
                            else
                            {
                                if (*++iter != 'e')   continue;
                                else
                                {
                                    if (*++iter != ' ')   continue;
                                    ++tab[info.NCumAgent];
                                }
                            }
                        }
                    }
                }

                if (remuneration_xml_open == true)
                {
                    std::cerr << "Erreur XML : la balise Remuneration n'est pas refermée pour le fichier " << info.threads->argv[i]
                              << ENDL "pour l'agent n°"   << info.NCumAgent + 1 << ENDL;

                    #ifndef STRICT
                      continue;
                    #else
                      exit(-100);
                    #endif
                }

            }

            info.threads->in_memory_file[i] = std::move(ss);

        #endif

        c.clear();
        c.close();

#ifdef PREALLOCATE_ON_HEAP
#undef tab
#else
        info.NLigne.assign(tab, tab + info.NCumAgent);
#endif

#endif
#ifdef MMAP_PARSING

        //std::cerr << "Mappage en mémoire de " << info.threads->argv[i] << "..."ENDL;
        struct stat st;
        stat(info.threads->argv[i], &st);
        const size_t file_size =  st.st_size;
        void *dat;
        int fd = open(info.threads->argv[i], O_RDONLY);
       // std::cerr << "Taille : " << file_size << ENDL;
        assert(fd != -1);
        dat = mmap(NULL, file_size,  PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
        assert(dat != NULL);
        //write(1, dat, file_size);
        char* data = (char*) dat;
       // std::cerr << "Mapping OK"ENDL;
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

