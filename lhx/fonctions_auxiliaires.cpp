/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
 */




#include <cstring>
#include <cstdint>
#include <iostream>
#include "fonctions_auxiliaires.hpp"
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

#if 0
 char* ecrire_chemin_base(const char* chemin_base, int rang_fichier_base)
{
    int s = strlen(chemin_base);
    char* chemin = new char[s + 1 + 3];   // chemin_base + _ + 3 chiffres
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

void ecrire_entete(const info_t &info, std::ofstream& base)
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
        if (i != Budget +1 && i!= Siret +1 && i != Etablissement + 1)
            base << entete[i] << info.separateur;
    }

  base << entete[i] << "\n";
}


void ouvrir_fichier_bulletins(const info_t &info, std::ofstream& base)
{
    return ouvrir_fichier_base0(info, 0, BULLETINS, base);
}


void ouvrir_fichier_base(const info_t &info, int rang, std::ofstream& base)
{
    return ouvrir_fichier_base0(info, rang, BASE, base);
}

void ouvrir_fichier_base0(const info_t &info, int rang, int type, std::ofstream& base)
{
    std::string chemin_base = "";
    if (type == BASE)
        chemin_base = info.chemin_base;
    else
        chemin_base = info.chemin_bulletins;


    base.open(chemin_base);
    base.seekp(0);
    if (! base.good())
    {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier de sortie.\n";
        exit(-1000);
    }

    if (type == BASE)
        ecrire_entete(info, base);
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
            std::cerr << "Erreur : " << c_str << ": pas un décimal\n";
        }
        else if (sl > INT32_MAX)
        {
            std::cerr << "Erreur : " <<  sl << " entier excédant la limite des entiers à 16 bits\n";
        }
        else if (sl < 0)
        {
            std::cerr << "Erreur : " << sl <<". L'entier doit être positif\n";
        }
        else
        {
            return((uint32_t)sl);
        }
        return(-1);
    }
    else
    {
        std::cerr << "Erreur : Préciser le nombre de bulletins mensuels attendus (majorant du nombre).\n";
        return(-1);
    }
}

int calculer_memoire_requise(info_t& info)
{
    errno = 0;

    // Attention reserve() ne va pas initialiser les membres à 0 sous Windows. Utiliser resize() ici.

#ifdef PREALLOCATE_ON_HEAP

    /* C++ style vector allocation *//

#define tab info.NLigne
        info.NLigne.resize(info.threads->argc * MAX_NB_AGENTS);
#else

  /* C style vector allocation */

    uint16_t tab[info.threads->argc * MAX_NB_AGENTS];
   // memset(tab, 0, info.threads->argc * MAX_NB_AGENTS) peut éventuellement être utile pour certains compilateurs anciens.

#endif

    char d = 0;

    std::cerr << "Premier scan des fichiers pour déterminer les besoins mémoire ... \n";

    /* par convention  un agent avec rémunération non renseignées (balise sans fils) a une ligne */
    for (unsigned i = 0; i < info.threads->argc; ++i)
    {
        errno = 0;
        
#ifdef FGETC_PARSING
        
        std::ifstream c;
        c.open(info.threads->argv[i]);
        if (c.good())
            c.seekg(0, c.beg);
        else 
        {
            perror("Erreur : Erreur : Ouverture Fichiers.");    // cautious no-op
            std::cerr << info.threads->argv[i] << std::endl;
            exit(-120);
        }

        if (errno)
        {
            perror("Erreur : Fichier .xhl");
            exit(-122);
        }
        
        while (! c.eof())
        {
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
            if  (c.get()  == '/')
            {
                tab[info.NCumAgent] = 1;
                ++info.NCumAgent;
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
        }
        c.clear();
        c.close();

#ifdef PREALLOCATE_ON_HEAP
#undef tab
#else
        info.NLigne.assign(tab, tab+info.NCumAgent);
#endif

#endif
#ifdef MMAP_PARSING

        //std::cerr << "Mappage en mémoire de " << info.threads->argv[i] << "...\n";
        struct stat st;
        stat(info.threads->argv[i], &st);
        const size_t file_size =  st.st_size;
        void *dat;
        int fd = open(info.threads->argv[i], O_RDONLY);
       // std::cerr << "Taille : " << file_size << std::endl;
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

