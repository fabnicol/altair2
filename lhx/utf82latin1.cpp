
#include <iostream>
#include <fstream>
#ifdef MMAP_PARSING
#ifdef __linux__
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#else
#error "La compilation MMAP ne peut se faire que sous unix."
#endif
#endif

using namespace std;

/// Efface le premier caractère d'une chaîne et translate la chaîne d'un caractère vers la gauche
/// \param c chaine de caractères libXml2 à modifier par pointeur

static inline void effacer_char (unsigned char * c)
{
   if (c == nullptr) return;
   int j = 0;

   do
      {
          * (c + j) = * (c + j + 1);
          ++j;
      } while (* (c + j));
}

static inline off_t convert2utf8(char* path)
{

    int fd = open (path, O_RDONLY);

    assert (fd != -1);

    struct stat sb;
    if (fstat(fd, &sb) == -1) throw runtime_error("Impossible d'obtenir la taille du fichier.");

    off_t file_size = sb.st_size;

    /* MADV_SEQUENTIAL
    *    The application intends to access the pages in the specified range sequentially, from lower to higher addresses.
    *   MADV_WILLNEED
    *    The application intends to access the pages in the specified range in the near future. */


    unsigned char* s = (unsigned char*) mmap(NULL, file_size, PROT_READ | PROT_WRITE,
                MAP_PRIVATE, fd, 0);
    if (s == MAP_FAILED)
    {
        throw runtime_error ("problème sur mmap, fonctions_auxiliaires.cpp");
    }

    unsigned char* s0 = s;

   while (s - s0 <  file_size )
    {

    switch (*s)
            {


//   Gros hack de pseudo-conversion UTF-8 vers Latin-1, qui permet d'économiser les 40 %
//   de surcoût d'exécution lié à l'utilisation d'iconv pour retraiter les fichiers de
//   sortie (fonction convertir(const char*))
//   Ce hack est presque sans coût. Il se base sur les hypothèses suivantes :
//       a) pas de caractères spéciaux multioctets
//       b) seuls sont convertis : à, â, ç, è, é, ê, ë, î, ï, ô, û ... et les majuscules
//         correspondantes càd
//   dont le code UTF-8 commence par 0xC3. Il suffit d'ajouter 0x40 sur les quatre bits
//   hauts de l'octet.

            case 0xC3:
                *s = ((* (s + 1) & 0xF0) + 0x40) | (* (s + 1) & 0x0F);

                effacer_char (s + 1);

                break;

            case 0xC2:

                *s = * (s + 1);
//    Le caractère ° (degré) est bien codé en Latin-1 comme 0xB0, mais il y a un
//    problème avec le paquet texlive inputenc pour la conversion pdf.
//    On remplace donc par (0x65). Apparemment plus nécessaire */
//    if (info.Table[info.NCumAgentXml][l][i] == 0xB0)
//            info.Table[info.NCumAgentXml][l][i] = 0x65;

                effacer_char (s + 1);

                break;

            }

        ++s;
    }

  close(fd);
  remove(path);
  ofstream out;
  out.open(path);
  out << s0 << endl;
  out.close();

return file_size;
}



int main(int argc, char **argv)
{

    if (argc < 2) return -1;

    off_t size = 0;

    for (int i = 1; i < argc; ++i)
    {
        off_t s = convert2utf8(argv[i]);
        cerr << "Fichier : " << argv[i] << " Taille : " << s << endl;
        size += s;
    }

    cerr << "Total: " << size << endl;

    return 0;
}

