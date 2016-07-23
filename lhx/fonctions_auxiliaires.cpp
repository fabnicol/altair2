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
#include <sys/stat.h>
#include "fonctions_auxiliaires.hpp"
#include "tags.h"

using namespace std;
extern bool verbeux;

#ifdef __WIN32__
#include "entete-latin1.hpp"
#else
#include "entete.hpp"
#include <unistd.h>
#endif

#ifdef MMAP_PARSING
#ifdef __linux__
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>
#else
#error "La compilation MMAP ne peut se faire que sous unix."
#endif
#endif



ostringstream help()
{
ostringstream out;
out <<  "**Usage** :  lhx OPTIONS fichiers.xhl  " << "\n\n"
          <<  "**OPTIONS :**  " << "\n\n"
          <<  "**-n** *argument obligatoire* : nombre maximum de bulletins mensuels attendus [calcul exact par défaut]  " << "\n\n"
          <<  "**-N** *argument obligatoire* : nombre maximum de lignes de paye attendues [calcul exact par défaut]  " << "\n\n"
          <<  "**-t** *argument optionnel*   : type de base en sortie, soit 'standard', soit 'bulletins' [défaut bulletins].  " << "\n\n"
          <<  "**-T** *argument obligatoire* : nombre de lignes maximum par base .csv [défaut illimité]. Au plus 999 tables seront générées.  " << "\n\n"
          <<  "**-T AN**                   : générer une table par année  " << "\n\n"
          <<  "**-T A/AC/AV/C/D/I/IR/RE/S/T** : générer une table pour chaque catégorie de ligne :    \n\n"
          <<  "      A rémunérations diverse  \n\n"
          <<  "      AC acompte  \n\n"
          <<  "      AV avantage en nature  \n\n"
          <<  "      C cotisation  \n\n"
          <<  "      D déduction  \n\n"
          <<  "      I indemnités  \n\n"
          <<  "      IR indemnité de résidence  \n\n"
          <<  "      RE retenue  \n\n"
          <<  "      S supplément familial  \n\n"
          <<  "      T traitement brut  \n\n"
          <<  "      X toutes catégories     \n\n\n"
          <<  "**-o** *argument obligatoire* : fichier.csv, chemin complet du fichier de sortie [défaut 'Table.csv' avec -t].  " << "\n\n"
          <<  "**-D** *argument obligatoire* : répertoire complet du fichier de sortie [défaut '.' avec -t].  " << "\n\n"
          <<  "**-d** *argument obligatoire* : séparateur décimal [défaut ',' avec -t].  " << "\n\n"
          <<  "**-s** *argument obligatoire* : séparateur de champs [défaut ';' avec -t]. Ne pas utiliser '_'.  " << "\n\n"
          <<  "**-j** *argument obligatoire* : nombre de fils d'exécution (1 à 10).  " << "\n\n"
          <<  "**-l** *sans argument*        : générer une colonne de numéros de ligne intitulée 'R'.  " << "\n\n"
          <<  "**-M** *sans argument*        : ne pas libérer la mémoire réservée en fin de programme.   " << "\n\n"
          <<  "**-m** *sans argument*        : calculer les maxima d'agents et de lignes de paye.  " << "\n\n"
          <<  "**-L** *argument obligatoire* : chemin du log d'exécution du test de cohérence entre analyseurs C et XML.  " << "\n\n"
          <<  "**-R** *argument obligatoire* : expression régulière pour la recherche des élus (codés : ELU dans le champ Statut.  " << "\n\n"
          <<  "**-S** *sans argument*        : exporter les champs Budget, Employeur, Siret, Etablissement.  " << "\n\n"
          <<  "**-E** *sans argument*        : exporter le champ Echelon.  " << "\n\n"
          <<  "**-q** *sans argument*        : limiter la verbosité.  " << "\n\n"
          <<  "**-f** *argument obligatoire* : la ligne de commande est dans le fichier en argument, chaque élément à la ligne.  " << "\n\n"
          <<  "**--xhlmem** *arg. oblig.*    : taille des fichiers à analyser en octets.  " << "\n\n"
          <<  "**--memshare** *arg. oblig.*  : Part de la mémoire vive utilisée, en points de pourcentage.  " << "\n\n"
          <<  "**--segments** *arg. oblig.*  : nombre minimum de segments de base.  " << "\n\n"
          <<  "**--pretend**                 : exécution sans traitement des fichiers.  " << "\n\n"
          <<  "**--verifmem**                : seulement vérifier la consommation mémoire.  " << "\n\n"
          <<  "**--hmarkdown**               : aide en format markdown.  " << "\n\n"
          <<  "**--pdf**                     : aide en format pdf.  " << "\n\n";
    #ifdef GENERATE_RANK_SIGNAL
              out  <<  "**-rank** *argument optionnel* : générer le fichier du rang de la base de paye en cours dans le fichier ";
             // out  <<  "ou à défaut dans " USERPROFILE "/" LOCALDATA ".\n\n";
    #endif
   return out;
}

#ifdef __WIN32__
#include <direct.h>
#include <string>
#include <windows.h>


// The directory path returned by native GetCurrentDirectory() no end backslash
string getexecpath()
{
    const unsigned long maxDir = 260;
    wchar_t currentDir[maxDir];
    GetCurrentDirectory(maxDir, currentDir);
    wstring ws(currentDir);
    string str(ws.begin(), ws.end());
    return str;
}

#else
#if defined(__linux__)

#include <unistd.h>
#define GetCurrentDir getcwd
string getexecpath()
{

    char szTmp[32]={0};
    char pBuf[1000]={0};
    int len = sprintf(szTmp, "/proc/%d/exe", getpid());
    int bytes = min((int) readlink(szTmp, pBuf, len), len - 1);
    if(bytes >= 0)
        pBuf[bytes] = '\0';
    return string(pBuf);
}

#endif
#endif



/* utilité d'affichage de l'environnement xhl en cas de problème de conformité des données */

errorLine_t afficher_environnement_xhl(const info_t& info, const xmlNodePtr cur)
{
    long lineN = 0;
    cerr << WARNING_HTML_TAG "Fichier analysé " <<  info.threads->argv[info.fichier_courant] << ENDL;
    lineN = xmlGetLineNo(cur);
    if (lineN == -1)
        {
            cerr << WARNING_HTML_TAG "Une balise est manquante dans le fichier." << ENDL;
        }
        else
            cerr << WARNING_HTML_TAG "Ligne n°" << lineN << ENDL;

    /* Tableau_entete va être en shared memory concurrent read access (no lock here) */
#if 1
    for (int l = 0; l < info.Memoire_p_ligne[info.NCumAgentXml]; ++l)
        {
          if (nullptr != info.Table[info.NCumAgentXml][l])
              cerr << WARNING_HTML_TAG "Balise de paye : " << Tableau_entete[l]
                        << "  " << info.Table[info.NCumAgentXml][l] << ENDL;
        }
#endif
    errorLine_t s = {lineN, string("Fichier : ") + string(info.threads->argv[info.fichier_courant])
                             + string(" -- Balise : ") + ((cur)? string((const char*)cur->name) : string("NA"))};
    return s;
}

off_t taille_fichier(const string& filename)
{
#ifndef __linux__
    struct __stat64 stat_buf;
    int rc = __stat64(filename.c_str(), &stat_buf);
 #else
    struct stat64 stat_buf;
    int rc = stat64(filename.c_str(), &stat_buf);
 #endif
    return rc == 0 ? stat_buf.st_size : -1;
}

#ifdef __linux__
    size_t getTotalSystemMemory()
    {
        long pages = sysconf(_SC_PHYS_PAGES);
        long page_size = sysconf(_SC_PAGE_SIZE);
        return pages * page_size;
    }

   #include <sys/sysinfo.h>


    size_t getFreeSystemMemory()
    {
        struct sysinfo info;
        sysinfo( &info );
        return (size_t)( info.freeram + info.bufferram) * (size_t)info.mem_unit;
        //info.freeram-info.bufferram
    }



#else
#include <windows.h>
    size_t getTotalSystemMemory()
    {
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return status.ullTotalPhys;
    }

    size_t getFreeSystemMemory()
    {
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return status.ullAvailPhys;
    }

#endif



/* *******************************************************************************************************************************************************************************
 * Author:  David Robert Nadeau
 * Site:    http://NadeauSoftware.com/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/deed.en_US
 */

#if defined(__WIN32__)
    #include <windows.h>
    #include <psapi.h>

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    #include <stdio.h>

#endif



/**
 * Returns the current resident set size (physical memory use) measured
 * in bytes, or zero if the value cannot be determined on this OS.
 */

size_t getCurrentRSS( )
{
#if defined(__WIN32__)
    /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
    return (size_t)info.WorkingSetSize;


#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    /* Linux ---------------------------------------------------- */
    long rss = 0L;
    FILE* fp = NULL;
    if ( (fp = fopen( "/proc/self/statm", "r" )) == NULL )
        return (size_t)0L;		/* Can't open? */
    if ( fscanf( fp, "%*s%ld", &rss ) != 1 )
    {
        fclose( fp );
        return (size_t)0L;		/* Can't read? */
    }
    fclose( fp );
    return (size_t)rss * (size_t)sysconf( _SC_PAGESIZE);

#endif
}

// End of Creative commons license


void ecrire_log(const info_t& info, ofstream& log, int diff)
{
    if (! info.chemin_log.empty())
    {

        if (log.good())
        #define P  " | "
        log << "Année " << P
            << info.Table[info.NCumAgentXml][Annee] << P
            << "Mois "  << setw(2) << info.Table[info.NCumAgentXml][Mois] << P
            << "Matricule " << setw(6) <<  info.Table[info.NCumAgentXml][Matricule] << P
            << "Rang global " << setw(6) <<  info.NCumAgentXml << P
            << "Rang dans fichier " << setw(5) <<  info.NAgent[info.fichier_courant] << P
            << "Analyseur C " << setw(6) << info.NLigne[info.NCumAgentXml] << P
            << "Xml " << setw(6) << info.NLigne[info.NCumAgentXml] - diff << P
            << "Différence " << setw(4) << diff << "\n";
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
    int test = (int) (rang_fichier_base + nbType - 1) / nbType;

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

void ecrire_entete_bulletins(const info_t &info, ofstream& base)
{
  ecrire_entete0(info, base, entete_char_bulletins, sizeof(entete_char_bulletins)/sizeof(char*));
}

void ecrire_entete_table(const info_t &info, ofstream& base)
{
  ecrire_entete0(info, base, entete_char, sizeof(entete_char)/sizeof(char*));
}

void ecrire_entete0(const info_t &info, ofstream& base, const char* entete[], int N)
{
  int i;
  if (info.select_echelon)
  {
      if (info.select_siret)
        for (i = !info.generer_rang; i < N - 1; ++i)
          base << entete[i] << info.separateur;
      else
        for (i = !info.generer_rang; i < N - 1; ++i)
        {
            if (i != Budget +1 &&  i != Employeur + 1 && i != Siret +1 && i != Etablissement + 1)
                base << entete[i] << info.separateur;
        }
  }
  else
  {
      if (info.select_siret)
        for (i = !info.generer_rang; i < N - 1; ++i)
        {
          if (entete[i][0] != 'E' || entete[i][1] != 'c')  // Pour "Echelon"
              base << entete[i] << info.separateur;
        }
      else
        for (i = !info.generer_rang; i < N - 1; ++i)
        {
            if (i != Budget +1 &&  i != Employeur + 1 && i != Siret +1 && i != Etablissement + 1
                 && (entete[i][0] != 'E' || entete[i][1] != 'c'))  // Pour "Echelon"
                base << entete[i] << info.separateur;
        }
  }

  base << entete[i] << "\n";
}

void ouvrir_fichier_bulletins(const info_t &info, ofstream& base, int segment)
{
    ouvrir_fichier_base0(info, BaseCategorie::BULLETINS, BaseType::MONOLITHIQUE, base, segment);
}

void ouvrir_fichier_base(const info_t &info, BaseType type, ofstream& base, int segment)
{
    ouvrir_fichier_base0(info, BaseCategorie::BASE, type, base, segment);
}

void ouvrir_fichier_base0(const info_t &info, BaseCategorie categorie, BaseType type, ofstream& base, int segment)
{

    string chemin_base = "";

    int Type = 0;

    if (categorie == BaseCategorie::BULLETINS)
    {
        chemin_base = info.chemin_bulletins
        #if defined(__WIN32__) && defined (USE_ICONV)
                + ".temp"
        #endif
                + string(CSV);
    }
    else
    {

        string  index = "-";
        static int rang;
        static int segment_ancien;
        bool nouveau_segment = (segment != segment_ancien);
        int increment = 1;

        if (segment_ancien > 0 && nouveau_segment)
        {
                if  (type != BaseType::MAXIMUM_LIGNES
                    && type != BaseType::PAR_ANNEE
                    && type != BaseType::MAXIMUM_LIGNES_PAR_ANNEE)
                    {
                        rang = 0;
                    }
                increment = 0;
        }

        chemin_base = info.chemin_base
        #if defined(__WIN32__) && defined (USE_ICONV)
                + ".temp"
        #endif
                ;      // sans l'extension csv

        switch (type) // OK en C++14. Il faut donc ajouter la directive de compilation -std=c++14
        {
           case BaseType::MONOLITHIQUE:
              chemin_base = chemin_base + CSV;
            break;

           case BaseType::MAXIMUM_LIGNES:
           case BaseType::PAR_ANNEE:
           case BaseType::MAXIMUM_LIGNES_PAR_ANNEE:
               rang += increment;
               index = index + to_string(rang) +  string(CSV);
               chemin_base = chemin_base + index;
             break;


           case BaseType::NA:
            ++Type;

           case BaseType::PAR_COMMENTAIRE:
             ++Type;

           case BaseType::PAR_COTISATION:
             ++Type;

           case BaseType::PAR_RETENUE:
            ++Type;

           case BaseType::PAR_RAPPEL:
            ++Type;

           case BaseType::PAR_ACOMPTE:
             ++Type;

           case BaseType::PAR_DEDUCTION:
             ++Type;

           case BaseType::PAR_REM_DIVERSES:
             ++Type;

           case BaseType::PAR_INDEMNITE:
             ++Type;

           case BaseType::PAR_AVANTAGE_NATURE:
             ++Type;

           case BaseType::PAR_SFT:
             ++Type;

           case BaseType::PAR_INDEMNITE_RESIDENCE:
             ++Type;

           case BaseType::PAR_TRAITEMENT:
            chemin_base = chemin_base + index + types_extension[Type] + CSV;
            break;

           default: break;
        }

       segment_ancien = segment;
    }

    base.open(chemin_base, ofstream::out | ofstream::app);

    if (! base.good())
    {
        cerr << ERROR_HTML_TAG "Impossible d'ouvrir le fichier de sortie "<< chemin_base << " de type " << Type << ENDL;
        exit(-1000);
    }

    bool insert_header = (taille_fichier(chemin_base) == 0);

    if (insert_header)
    {
        if (categorie == BaseCategorie::BASE)
            ecrire_entete_table(info, base);
        else
            ecrire_entete_bulletins(info, base);
    }


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
            cerr << ERROR_HTML_TAG "" << c_str << ": pas un décimal" ENDL;
        }
        else if (sl > INT32_MAX)
        {
            cerr << ERROR_HTML_TAG "" <<  sl << " entier excédant la limite des entiers à 16 bits" ENDL;
        }
        else if (sl < 0)
        {
            cerr << ERROR_HTML_TAG "" << sl <<". L'entier doit être positif" ENDL;
        }
        else
        {
            return((uint32_t)sl);
        }
        return(-1);
    }
    else
    {
        cerr << ERROR_HTML_TAG "Préciser le nombre de bulletins mensuels attendus (majorant du nombre)." ENDL;
        return(-1);
    }
}

void calculer_maxima(const vector<info_t> &Info, ofstream* LOG)
{
    uint32_t maximum[2] = {0, 0};

    for (unsigned i = 0; i < Info[0].nbfil; ++i)
    {
        for (unsigned j = 0; j < Info[i].threads->argc; ++j)
            if (Info[i].NAgent[j] > maximum[0])
                maximum[0] = Info[i].NAgent[j];

        for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
            if (Info[i].NLigne[agent] > maximum[1])
                maximum[1] = Info[i].NLigne[agent] + 1;
    }

/* TODO: Eclaircir le off-by-one sur NLigne */

        cerr <<  STATE_HTML_TAG "Maximum de lignes : " << maximum[1] << ENDL
                   << STATE_HTML_TAG "Maximum d'agents  : " << maximum[0] << ENDL;
        if (LOG != nullptr &&  LOG->good())
        {
           *LOG  <<  STATE_HTML_TAG "Maximum de lignes : " << maximum[1] << ENDL
                 << STATE_HTML_TAG "Maximum d'agents  : " << maximum[0] << ENDL;
        }

}


template <typename Allocator = allocator<char>>
inline string read_stream_into_string(
    ifstream& in,
    Allocator alloc = {})
{
  basic_ostringstream<char, char_traits<char>, Allocator>
    ss(basic_string<char, char_traits<char>, Allocator>(move(alloc)));

  if (!(ss << in.rdbuf()))
    throw ios_base::failure{"[ERR] Erreur d'allocation de lecture de fichier.\n"};

  return ss.str();
}


int calculer_memoire_requise(info_t& info)
{
    errno = 0;

    // Attention reserve() ne va pas initialiser les membres à 0 sous Windows. Utiliser resize() ici.
   memory_debug("calculer_memoire_requise_pre_tab_resize");

#ifdef PREALLOCATE_ON_HEAP

    /* C++ style vector allocation */

    #define tab info.NLigne

        tab.resize(info.threads->argc * MAX_NB_AGENTS);

        memory_debug("calculer_memoire_requise_pre");
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


      #if defined(FGETC_PARSING) || defined(STRINGSTREAM_PARSING)

        ifstream c(info.threads->argv[i]);
        if (verbeux)
            cerr << PROCESSING_HTML_TAG  "Ouverture du fichier " << info.threads->argv[i] << ENDL;

        if (c.is_open())
            c.seekg(0, ios::beg);
        else
        {
            if (verbeux)
                cerr <<  ERROR_HTML_TAG "Problème à l'ouverture du fichier *" << info.threads->argv[i] << "*" << ENDL;
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
                    cerr << "Erreur XML : la balise Remuneration n'est pas refermée pour le fichier " << info.threads->argv[i]
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

            auto ss = read_stream_into_string(c);

            string::iterator iter = ss.begin();

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
                    cerr << "Erreur XML : la balise Remuneration n'est pas refermée pour le fichier " << info.threads->argv[i]
                              << ENDL "pour l'agent n°"   << info.NCumAgent + 1 << ENDL;
                    exit(0);

                    #ifndef STRICT
                      continue;
                    #else
                      exit(-100);
                    #endif
                }

            }

            info.threads->in_memory_file[i] = move(ss);

        #endif

        c.clear();
        c.close();

        memory_debug("calculer_memoire_requise_close");

#ifdef PREALLOCATE_ON_HEAP
#undef tab
#else
        info.NLigne.assign(tab, tab + info.NCumAgent);
#endif

#endif
#ifdef MMAP_PARSING

        //cerr << "Mappage en mémoire de " << info.threads->argv[i] << "..."ENDL;
        struct stat st;
        stat(info.threads->argv[i].c_str(), &st);
        const size_t file_size =  st.st_size;
        void *dat;
        int fd = open(info.threads->argv[i].c_str(), O_RDONLY);
       // cerr << "Taille : " << file_size << ENDL;
        assert(fd != -1);

       /* MADV_SEQUENTIAL
       *    The application intends to access the pages in the specified range sequentially, from lower to higher addresses.
       *   MADV_WILLNEED
       *    The application intends to access the pages in the specified range in the near future. */

        dat = mmap(NULL, file_size,  PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
        int ret;

        ret = madvise (dat, 0, MADV_SEQUENTIAL | MADV_WILLNEED);
        if (ret < 0)
                perror ("madvise");

        assert(dat != NULL);
        //write(1, dat, file_size);
        char* data = (char*) dat;
       // cerr << "Mapping OK"ENDL;
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


        info.threads->in_memory_file[i] = move(data);
        //munmap(data, file_size);
        close(fd);
#endif

#ifdef GUI_TAG_MESSAGES
   #ifdef GENERATE_RANK_SIGNAL

     generate_rank_signal();
     cerr <<  " \n" ;

   #endif
#endif

    }

    /* A ETUDIER */
#ifdef PREALLOCATE_ON_HEAP
    info.NLigne.resize(info.NCumAgent+1);
#endif

memory_debug("calculer_memoire_requise_end");
    return errno;
}

