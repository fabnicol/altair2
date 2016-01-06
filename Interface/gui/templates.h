#ifndef TEMPLATES_H
#define TEMPLATES_H
#include <QHash>


template <typename T, typename U> void createHash(QHash<T, U > &H, const QList<T> *L, const QList<U> *M)
{
    if ((L == nullptr) || (M == nullptr)) return;
    QListIterator<T> i(*L);
    QListIterator<U> j(*M);

    while ((j.hasNext()) && (i.hasNext()))
        H[i.next()]=j.next();
}

#endif // TEMPLATES_H
