#ifndef LIBXML3_H
#define LIBXML3_H
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>
#include <unordered_set>
#include <unordered_map>

using namespace std;

static constexpr const int  NTags = 20;   /* nombre de champs bulletins natifs (+2 créés)*/
static constexpr const int  NTags0 = 16; /* l'indice du cut entre bulletins et lignes pour les champs mappés */
static constexpr const int  NTags1 = 14; /* l'indice de séparation dans les champs bulletins d'origine entre valeurs char et numériques */

/*
 * Annee                0
 * Mois                 1
 * Nom                  2
 * Prenom               3
 * Matricule            4
 * Service              12
 * Nb.Enfants           6
 * Statut               7
 * Temps.de.travail     14
 * Heures.Sup.          16
 * Heures               15
 * Indice               11
 * Brut                 17
 * Net                  18
 * Net.a.Payer          19
 * NBI                  13
 * Libelle              L0
 * Code                 L1
 * Base                 L2
 * Taux                 L3
 * Nb.Unite             L4
 * Montant              L5
 * Type                 20
 * Emploi               8
 * Grade                9
 * Echelon              10
 * Categorie            21
 * Nir                  5              */


static constexpr const char* entete = "Annee;Mois;Nom;Prenom;\
Matricule;Service;Nb.Enfants;Statut;Temps.de.travail;Heures.Sup.;\
Heures;Indice;Brut;Net;Net.a.Payer;NBI;Libelle;Code;Base;Taux;\
Nb.Unite;Montant;Type;Emploi;Grade;Echelon;Categorie;Nir\n";

static constexpr const array<int, NTags + 2> map_ind = {0, 1, 2, 3, 4, 12, 6, 7, 14, 16, 15, 11, 17, 18, 19, 13, 20, 8, 9, 10, 21, 5};

static const array<string, NTags>& Tags = {
              "Annee", "Mois", "Nom", "Prenom", "Matricule", "NIR", "NbEnfants", "Statut",
              "EmploiMetier", "Grade", "Echelon", "Indice",  "Service", "NBI", "QuotiteTrav",
              "NbHeureTotal", "NbHeureSup", "MtBrut", "MtNet", "MtNetAPayer"   } ;

static constexpr const int  MaxLTags = 6; /* nombre de champs lignes */
//static constexpr const int  MaxNTypes = 12;

static const array<string, MaxLTags> LTags = {"Libelle", "Code", "Base", "Taux", "NbUnite", "Mt"};

static unordered_set<string> Types = {
                                                   "TraitB", //"TraitBrut",
                                                   "IndemR", //"IndemResid",
                                                   "SupFam", // "SupFam",
                                                   "Avanta", //"AvantageNature",
                                                   "Indemn", //"Indemnite",
                                                   "RemDiv", //"RemDivers",
                                                   "Deduct", // "Deduction",
                                                   "Acompt", // "Acompte"
                                                   "Rappel", // "Rappel",
                                                   "Retenu", // "Retenue",
                                                   "Cotisa",  // "Cotisation"
                                                   //"Comm" // "Commentaire"
                                                   };

static array<string, NTags + 2> Bulletins;

static vector<string> Lignes;

/*
"   &quot;
&	&amp;
€	&euro;
<	&lt;
>	&gt;
Space	&nbsp;
'	&acute;  */

static const unordered_map<string, char>& aconv = {{"&amp;", '&'},
                                                   {"&apos;", '\''},
                                                   {"&acute;", '\''},
                                                   {"&lt;", '<'},
                                                   { "&gt;", '>'},
                                                   {"&nbsp;", ' '}};


static unordered_map<string, string> codetype = {{"TraitB", "T"},
                                                   {"IndemR", "IR"},
                                                   {"SupFam", "S"},
                                                   {"Avanta", "AV"},
                                                   {"Indemn", "I"},
                                                   {"RemDiv", "A"},
                                                   {"Deduct", "D"},
                                                   {"Acompt", "AC"},
                                                   {"Rappel", "R"},
                                                   {"Retenu", "RE"},
                                                   {"Cotisa", "C"}};

void convertir(string::iterator&  c, string& s)
{
    int n = 0;

    while (*(c + ++n) != ';')
    {
        if  (n == 7) return;
    }

    unordered_map<string, char>::const_iterator it;

    if ((it = aconv.find(string(c, c + n + 1))) != aconv.end())
    {
        *c = it->second;
        string::iterator iter = c;
        while (iter + n + 1 != s.end()) *++iter = *(iter + n);
        s.erase(iter + 1, s.end());
    }
}

string sanitize_lex(string& s)
{
        if (s.empty()) return "NA";

        string::iterator c = s.begin();
        while (c != s.end())
        {
            switch(*c)
            {
             case '&':
                        convertir(c, s);
                        break;


             case ';':
                        *c = '_';
                        break;

             case '\n':
                        *c = ' ';
                        break;
            }

            ++c;
        }

        return s;
}

string sanitize_num(string& s)
{

        if (s.empty()) return "NA";

        string::iterator c = s.end();
#if 0
        while (c != s.begin())
        {
            --c;

            if (*c == '0')
                *c = '\0';
            else
                break;
        }
#endif

        while (true)
        {
            if (*c == '.')
            {
                *c = ',';
                break;
            }

            if (c == s.begin())
                break;
            else
                --c;
        }

        return s;
}


bool filtre_table (string&& tag, string&& valeur, const bool rem)
{

    /* Le tag <Agent> entraîne la bufferisation des données bulletin
     * Les tags <Traitement>...<Deduction>...entraînent l'export immédiat des valeurs et un saut de ligne après export répété des données bufférisées. */

    /*Le buffer de données bulletins est local et limité aux champs concernés : */


if (rem)
{

    /* Limiter à un signal <Remuneration>, entre l'uouverture et la fermeture */

    static int last;
    int repeat = 0;

       do
       {
            for (int j = last; j < MaxLTags; ++j)
            {
                if (LTags.at(j) == tag)
                {
                    Lignes[Lignes.size() - MaxLTags + j]
                            = std::move((j < 2)? sanitize_lex(valeur) : sanitize_num(valeur));

                    last = j + 1;
                    if (last == MaxLTags) last = 0;
                    return true;
                }
            }

            last = 0;
            ++repeat;

       } while (repeat < MaxLTags);

        /* last-ditch */

    return true;

}
    /* dès le suivant */
    int repeat = 0;
    static int last;
    do
    {
        for (int j = last; j < NTags; ++j)
        {
           if (Tags.at(j) == tag)
           {
               Bulletins[j].clear();
               Bulletins[j] = std::move((j < NTags1)? sanitize_lex(valeur) : sanitize_num(valeur));
               last = j + 1;
               return true;
           }
        }

        last = 0;
        ++repeat;

    } while (repeat < NTags);

    return false;

}


vector<string> extraire_xml(const string& st, bool (*filtre)(const string&, const string&, const string&, vector<string>&), void (*IO)(size_t))
{
    vector<string> out;
    const size_t taille = st.size();
    out.reserve((size_t) taille / 5);
    string::const_iterator iter = st.begin();

    size_t i = 0;
    const size_t pas = taille / 20;
    size_t n = 0;

    while (*iter != '<' && ++iter != st.end()) continue;

    if (*(iter + 1) == '?')
    {
        string::const_iterator iter1 = iter;
        while (++iter != st.end() && *iter != '>') continue;
        out.emplace_back(string(iter1, iter + 1) + "\n");
    }

    start:
    while (iter != st.end())
    {
        while (++iter != st.end()
               && *iter != '<') { ++i; continue;}

        if (iter == st.end()) break;

        if (*(iter + 1) == '/') /* </TAG>  */
        {
            string::const_iterator iter1 = iter;
            while (++iter != st.end() && *iter != '>') { ++i; continue;}
            out.emplace_back(string(iter1, iter + 1) + "\n");
            continue;
        }

        if (iter == st.end()) break;

        if (i > n * pas)
        {
            IO(i);
            ++n;
        }

        string::const_iterator iter1 = iter;
        string tag;

        while (++iter != st.end())
        {
            ++i;
            if (*iter == ' ')  /* <TAG ...> */
            {
              tag = string(iter1 + 1, iter);
              break;
            }

            if (*iter == '>')  /* <TAG> */
            {
                out.emplace_back("<" + string(iter1 + 1, iter) + ">\n");
                goto start;
            }

        }

        if (iter == st.end()) break;

        while (++iter != st.end())
        {
            ++i;

            if (*iter == 'V')
            {
               while (++iter != st.end() && *iter == ' ') { ++ i; continue; }

               if (*iter == '=')
               {
                 break; /*<TAG  V  =... /> */
               }
               else continue; /*<TAG  x ... /> */
            }
            else /*<TAG  x... /> */
            if (*iter == '>')
            {
                out.emplace_back(string(iter1, iter +1) + "\n");
                ++iter;
                ++i;
                goto start;
            }
            else   /*<TAG  x... /> */
            continue;
        }

        if (iter == st.end()) break;

         /*<TAG  V  =... /> */

        while (++iter != st.end())
        {
            ++i;
            if (*iter == '\"') break;
        }

        if (iter == st.end()) break;

        iter1 = iter;

        while (++iter != st.end())
        {
            ++i;
            if (*iter == '\"') break;
        }

        if (iter == st.end()) break;

        const string& valeur = string(iter1 + 1, iter);
#if 0
        Q((tag + " " + valeur).c_str());
#endif
        auto iter2 = iter;

        while (++iter != st.end())
        {
            ++i;
            if (*iter == '>') break;
        }

        if (iter == st.end()) break;

        const string& reste = string(iter2 + 1, iter + 1);

        if (filtre(tag, valeur, reste, out))
        {
            ++iter;
            ++i;
            continue;
        }

        out.emplace_back("<" + tag + " V = \"" + valeur + "\"" + reste + "\n");

    }

    return(out);
}



void exporter(vector<string>& out)
{
    /* dépiler les précédents */

    const int S = Lignes.size();
    int n = 0;

    /* sanitiser les bull */

    while (n < S - MaxLTags)
    {
        for (int i = 0; i < NTags0; ++i)
        {
            out.push_back(Bulletins.at(map_ind.at(i)));
            out.emplace_back(";");
        }

        /* sanitiser les lignes */
        for (int j = 0; j < MaxLTags; ++j)
        {
            out.emplace_back(Lignes.at(n + j));
            out.emplace_back(";");
        }

        n += MaxLTags;

        for (int i = NTags0; i < NTags + 1; ++i)
        {
            out.push_back(Bulletins.at(map_ind.at(i)));
            out.emplace_back(";");
        }
        out.push_back(Bulletins.at(map_ind.at(NTags + 1)));
        out.emplace_back("\n");

    }

        for (int i = 0; i < NTags0; ++i)
        {
            out.emplace_back(Bulletins.at(map_ind.at(i)));
            out.emplace_back(";");
        }

        if (n)
        {
            /* sanitiser les lignes */
            for (int j = 0; j < MaxLTags; ++j)
            {
                out.emplace_back(Lignes.at(n + j));
                out.emplace_back(";");
            }
        }
        else
        for (int j = 0; j < MaxLTags; ++j)
        {
            out.emplace_back("NA;");
        }

        for (int i = NTags0; i < NTags + 1; ++i)
        {
            out.emplace_back(Bulletins.at(map_ind.at(i)));
            out.emplace_back(";");
        }

        out.emplace_back(Bulletins.at(map_ind.at(NTags + 1)));

        out.emplace_back("\n");

    Lignes.clear();
    Bulletins.fill("NA");
}



vector<string> extraire_xml_s_cut(const string& st, bool (*filtre)(string&&, string&&, const bool), void (*IO)(size_t))
{

    vector<string> out;
    const uint64_t taille = st.size();

    out.reserve((uint64_t) taille / 5);
    bool remuneration = false;

    Bulletins.fill("NA");

    uint64_t i = 0;
    const uint64_t pas = taille / 20;
    uint64_t n = 0;
    string::const_iterator iter = st.begin();

start:
    while (iter != st.end())
    {
        while (++iter != st.end()
               && *iter != '<') { ++i; continue;}

        if (iter == st.end()) break;

        if (*(iter + 1) == '/') /* </TAG>  */
        {
            iter += 2;

            string::const_iterator iter1 = iter;
            string::const_iterator iter2 = iter;

            if (*iter1 == 'P' && *++iter1 == 'a' && *++iter1 == 'y' && *++iter1 == 'e' && *++iter1 == 'I') // PayeIndivMensuel
            {
                exporter(out);
            }
            else
            if (*iter2 == 'R' && *++iter2 == 'e' && *++iter2 == 'm' && *++iter2 == 'u' && *++iter2 == 'n') // Rémunération
                remuneration = false;

            /* If TAG == Remuneration fermer signal correspondant */

            while (++iter != st.end() && *iter != '>') { ++i; continue;}

            continue;
        }

        if (iter == st.end()) break;

        if (i > n * pas)
        {
            IO(i);
            ++n;
        }

        const string::const_iterator iter1 = iter + 1;

        while (++iter != st.end())
        {
            ++i;

            if (*iter == ' ')  /* <TAG ...> */
            {
              break;
            }
            else
            if (*iter == '>') /* <TAG> */
            {
              ++iter;
              ++i;

              unordered_set<string>::iterator type_it;

              if ( (type_it = Types.find(string(iter1, iter1 + 6))) != Types.end())
              {
                  for (int i = 0; i < MaxLTags; ++i)
                      Lignes.push_back("NA");

                  Bulletins[NTags] = codetype[*type_it];
                  remuneration = true;
                  //noPayeI = false;
              }

              /* If TAG == Remuneration envoyer signal correspondant */

              goto start;
            }

        }

        const string::const_iterator iter2 = iter;

        while (++iter != st.end())
        {
            ++i;

            if (*iter == 'V')
            {
               while (++iter != st.end() && *iter == ' ') { ++ i; continue; }

               if (*iter == '=')
               {
                 break; /*<TAG  V  =... /> */
               }
               else continue; /*<TAG  x ... /> */
            }
            else  /*<TAG  x... /> */
            if (*iter == '>')
            {
                ++iter;
                ++i;
                goto start;
            }
            else   /*<TAG  x... /> */
            continue;
        }

        if (iter == st.end()) break;

         /*<TAG  V  =... /> */

        while (++iter != st.end())
        {
            ++i;
            if (*iter == '\"') break;
        }

        if (iter == st.end()) break;

        const string::const_iterator iter3 = iter + 1;

        while (++iter != st.end())
        {
            ++i;
            if (*iter == '\"') break;
        }

        if (iter == st.end()) break;

        filtre(/* tag = */ string(iter1, iter2),
               /* valeur = */ string(iter3, iter),
               remuneration);

        ++iter;
        ++i;

    }

    return(out);
}


void extraire_xml_s(const string& st, bool (*filtre)(string&&, string&&, const bool), void (*IO)(size_t))
{
    vector<string> vst;
    const uint64_t taille = st.size();
    vst.reserve(taille / 10000);
    string::const_iterator iter = st.begin();

    while (*iter != '<' && ++iter != st.end()) continue;

    if (*(iter + 1) == '?')
    {
        while (++iter != st.end() && *iter != '>') continue;
    }

    while (iter != st.end())
    {
        while (++iter != st.end()
               && *iter != '<')  continue;

        if (iter == st.end()) return;

        string::const_iterator iter1 = ++iter;

        if (*iter1 == 'P' && *++iter1 == 'a' && *++iter1 == 'y' && *++iter1 == 'e' && *++iter1 == 'I') // PayeIndivMensuel
        {
          while(iter != st.end())
           {
            while (++iter != st.end()
                   && *iter != '/')  continue;

            string::const_iterator iter2 = ++iter;

            if (*iter == 'P' && *++iter == 'a' && *++iter == 'y' && *++iter == 'e' && *++iter == 'I') // PayeIndivMensuel
            {
                while (++iter != st.end()
                       && *iter != '<')  continue;

                vst.emplace_back([&iter, &iter2] {
                                       string t;
                                       t.resize(iter - iter2);
                                       string::const_iterator d = t.begin();
                                       for (auto && c = iter2; c != iter; ++d, ++c)
                                          d = c;

                                       return t;
                }());
            }
            else
            if (*iter == 'N' && *++iter == 'o' && *++iter == 'm' && *++iter == 'e' && *++iter == 'n') // Nomenclature
            {
                goto end;
            }
          }
        }
    }

    end:

    /* exporter ici dans le fichier au retour des threads */
    for (auto&& v : vst) extraire_xml_s_cut(v, filtre, IO);

}

#endif // LIBXML3_H
