
// Auteur : Fabrice Nicol
// Copyright Fabrice Nicol
// Contributeur :
// Fabrice Nicol, années 2008 à 2012, 2017
// fabrnicol@gmail.com
//
// Ce logiciel est régi par les dispositions du code de la propriété
// intellectuelle (CPI).

// L'auteur se réserve le droit d'exploitation du présent logiciel, 
// et notamment de reproduire et de modifier le logiciel, conformément aux 
// dispositions de l'article L.122-6 du même code. L'auteur se réserve le droit
// de modifier et de corriger les erreurs du logiciel, conformément aux termes 
// du deuxième alinéa du I de l'article L.122-6-1 du même code.

// Le présent logiciel doit être regardé comme un "Module Externe", pour 
// l'application des stipulations de la licence CeCILL, lorsque le code source, 
// ou une bibliothèque compilée à partir du présent code, sont utilisés pour 
// compiler tout logiciel faisant usage du présent code.

// Tout fichier contenant le présent code, ou des versions antérieures du 
// présent code, et faisant apparaître des termes contraires aux stipulations
// précédentes doit être regardé comme faisant partie d'actes préparatoires à
// l'élaboration du présent logiciel et non comme le logiciel lui-même, ou 
// comme des erreurs matérielles dans la copie des licences. Seules les 
// stipulations précédentes doivent être regardées comme s'appliquant au 
// présent logiciel.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de ces stipulations et que vous en avez accepté les
// termes.

// Sans préjudice des dispositions du CPI, une autorisation d'usage et de
// reproduction du présent code est donnée à tout agent employé par les
// juridictions financières pour l'exercice de leurs fonctions publiques.
// Le code ainsi mis à disposition ne peut être transmis à d'autres utilisateurs.
//

#ifndef TEMPLATES_H
#define TEMPLATES_H
#include <QHash>

/// Crée une table de hachage entre deux listes de même longueur. Si les listes ne sont pas de même longueur, tronque la table à la dimension de la plus courte
/// \param H Table de hachage
/// \param L liste des clés de type T, où T est le premier argument du \e template
/// \param M Liste des valeurs de type U, où U est le second argument du \e template
/// \details \b Exemple
/// \code createHash<QString, QString>(H, L, M); \endcode
/// \sa  FListWidget,  FComboBox
template <typename T, typename U>
void createHash(QHash<T, U > &H, const QList<T> *L, const QList<U> *M)
{
    if ((L == nullptr) || (M == nullptr)) return;
    QListIterator<T> i(*L);
    QListIterator<U> j(*M);

    while ((j.hasNext()) && (i.hasNext()))
        H[i.next()]=j.next();
}

#endif // TEMPLATES_H
