// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser
// les fichiers de paye produits au format spécifié par l'annexe de la
// convention-cadre nationale de dématérialisation en vigueur à compter de
// l'année 2008.
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
// //////////////////////////////////////////////////////////////////////////

/// \file matricules.cpp
/// \author Fabrice Nicol
/// \brief Implémentation de la classe MatriculeInput, qui construit un dialogue d'extraction des bulletins

#include "matricules.h"

MatriculeInput::MatriculeInput (int width, int height)
{
    QGridLayout* q = new QGridLayout;

    QLabel *l = new QLabel ("Matricules");
    QLabel *m = new QLabel ("Matricules");
    QLabel *n = new QLabel ("Matricules");

    QToolButton *lb = new QToolButton;
    QToolButton *mb = new QToolButton;
    QToolButton *nb = new QToolButton;
    QToolButton *ab = new QToolButton;

    lb->setIcon (QIcon (":/images/edit-clear.png"));
    mb->setIcon (QIcon (":/images/edit-clear.png"));
    nb->setIcon (QIcon (":/images/edit-clear.png"));
    ab->setIcon (QIcon (":/images/include.png"));

    lb->setToolTip ("Effacer");
    mb->setToolTip ("Effacer");
    nb->setToolTip ("Effacer");
    ab->setToolTip ("Ajouter une ligne de saisie");

    const char* tip = "Pour extraire le bulletin d'un agent, rentrer son matricule, suivi d'un tiret,\n"
                      "puis le numéro du mois (de 1 à 12), suivi d'un tiret, et l'année.\n"
                      "Utiliser ... pour indiquer une plage de valeurs.\n"
                      "Insérer un point-vigule avant la demande suivante, sans espace.\n"
                      "Exemple :\n\n"
                      "\t1058 N-3-2010;1010 B-7...9-2012...2014\n\n"
                      "pour extraire les bulletins des agents de matricule :\n\n"
                      "\t1058 N en mars 2010\n"
                      "\t1010 B en juillet, août, septembre 2012 à 2014.\n"
                      "Lorsque les années sont données sur deux chiffres au lieu de 4, elles sont\n"
                      "réputées être postérieures à l'an 2000.\n"
                      "Il est possible de regrouper plusieurs matricules, pour une même valeur\n"
                      "du ou des mois et années. Les matricules sont alors séparés par des virgules.\n"
                      "Exemple :\n\n"
                      "\t1059,1092,1011-1...3-15...17\n\n"
                      "pour extraire les bulletins des agents de matricule :\n\n"
                      "\t1059, 1092 et 1011 en janvier, février et mars 2015, 2016 et 2017.\n";


    l->setToolTip (tip);
    m->setToolTip (tip);
    n->setToolTip (tip);

    matrLineEdit = new FLineEdit ("",
                                  "Matricules",
                                  {"Impression", "Format Matricule-Mois...-Année(s)...;"},
                                   "%bulletins");

    matrLineEdit2 = new FLineEdit ("",
                                   "MatriculesB",
                                   {"Impression", "Format Matricule-Mois...-Année(s)...;"},
                                    "%bulletins");

    matrLineEdit3 = new FLineEdit ("",
                                   "MatriculesC",
                                   {"Impression", "Format Matricule-Mois...-Année(s)...;"},
                                    "%bulletins");

    matrLineEditList = QList<FLineEdit*>();
    matrLineEditList << matrLineEdit << matrLineEdit2 << matrLineEdit3;

    eraseButtonList = QList<QToolButton*>();
    eraseButtonList  << lb << mb << nb;

    QGroupBox* dBox = new QGroupBox ("Exportation");
    dirpath = common::path_access (DONNEES "Bulletins");

    dossier = new FLineFrame ({"Matricules", "Répertoire"},
                              dirpath,
                              "dossierBulletins",
                              {0, 0},
                              nullptr,
                              "%dossier-bulletins",
                              flags::directory::checkEmptyness,
                              flags::flineframe::isDirectoryPath);

    if (! QFileInfo (dirpath).isDir())
        QDir().mkpath (dirpath);

    dBox->setLayout (dossier->getLayout());

    // format d'input: par exemple : 1058N-3-2010;1010B-7...9-2012..2014 :
    // agents 1058N en mars 2010 et 1010B en juillet,août,septembre 2012 à 2014

    closeButton = new QDialogButtonBox (QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    closeButton->button (QDialogButtonBox::Ok)->setText ("Accepter");
    closeButton->button (QDialogButtonBox::Cancel)->setText ("Annuler");

    q->addWidget (l, 0, 2);
    q->addWidget (m, 3, 2);
    q->addWidget (n, 6, 2);

    q->setRowMinimumHeight (10, height * 2 / 8);
    q->setRowMinimumHeight (9,  height * 5 / 16);
    q->setRowMinimumHeight (8,  height * 1 / 8);
    q->setRowMinimumHeight (5,  height * 1 / 8);
    q->setRowMinimumHeight (2,  height * 1 / 8);

    q->addWidget (matrLineEdit,  1, 2);
    q->addWidget (lb,  1, 3);
    q->addWidget (matrLineEdit2, 4, 2);
    q->addWidget (mb,  4, 3);
    q->addWidget (matrLineEdit3, 7, 2);
    int pos = 7;
    q->addWidget (nb,  pos, 3);
    q->addWidget (dBox, pos + 2, 2);
    q->addWidget (closeButton, pos + 3, 2);
    q->addWidget (ab, pos + 3, 3);

    setFixedWidth (width);
    setMinimumHeight (height);

    setWindowTitle ("Extraction des bulletins");

    eraseButtonList = QList<QToolButton*>();

    eraseButtonList << lb << mb << nb;

    connect(ab, &QToolButton::clicked, [this, q, ab] {

            static int rank;
            ++rank;
            FLineEdit* ligne = new FLineEdit ("",
                                              "Matricules" + QString::number(rank),
                                              {"Impression", "Format Matricule-Mois...-Année(s)...;"},
                                               "%bulletins");
            matrLineEditList << ligne;

            QToolButton* button = new QToolButton;
            eraseButtonList << button;
            button->setIcon (QIcon (":/images/edit-clear.png"));
            button->setToolTip("Effacer");

            int pos = 1 + 3 * (3 + rank);
            q->addWidget(new QLabel("Matricules"), pos - 1, 2);
            q->addWidget(ligne, pos, 2);
            q->addWidget(button, pos, 3);
            QRect rec = QApplication::desktop()->availableGeometry();
            int h = rec.height();
            h = h/4 + rank * h / 25;
            setMinimumHeight(h);

            QList<QToolButton*>::Iterator b = eraseButtonList.begin();
            QList<FLineEdit*>::Iterator it = matrLineEditList.begin();

            while (b != eraseButtonList.end())
            {
                connect(*b,
                        &QToolButton::clicked,
                        [this, it]
                        {
                          (*it)->clear();
                        });
                ++b;
                ++it;
            }
    });

    QList<QToolButton*>::Iterator b = eraseButtonList.begin();
    QList<FLineEdit*>::Iterator it = matrLineEditList.begin();

    while (b != eraseButtonList.end())
    {
        connect(*b,
                &QToolButton::clicked,
                [this, it]
                {
                  (*it)->clear();
                });
        ++b;
        ++it;
    }

    connect (closeButton,
             &QDialogButtonBox::accepted,
             [this]
            {
                bool res = true;
                for (FLineEdit* ligne : matrLineEditList)
                {
                    res &= checkInput (ligne);
                    if (! ligne->text().isEmpty())
                       matricules += matricules.isEmpty() ? "" : ";" + ligne->text();
                }

                emit(updateProject(true));

                if (res) accept();
                else
                    Q("Le format des entrants est inexact.\nMerci de respecter les conventions de formatage :\n  matricule1,...,matriculeN-mois1...moisN-année1...annéeN")
            });

    connect (closeButton,
             &QDialogButtonBox::rejected,
             [this]
    {
        reject();
    });

    setLayout (q);
}

bool MatriculeInput::checkInput (FLineEdit* l)
{
    bool res = true;

    QString s = l->text();
    QList<QString> L;

    for (auto &&S : s.split (';', QString::SkipEmptyParts))
        {
            QList<QString> sublist = S.split ('-', QString::SkipEmptyParts);

            if (sublist.size() != 3)
                {
                    QMessageBox::warning (nullptr, "Format de la requête " + S,
                                          "Utiliser le format suivant : matricule-mois-année.<br>"
                                          "Un intervalle peut être utilisé pour le mois et l'année.<br>"
                                          "Exemple :<br>\t 1012B-1...7-2011...2013", QMessageBox::Ok);
                    S.clear();
                }

            int cut = 0, cut2 = 0;
            const QString s1 = sublist.at (1);
            bool range = false;

            if (s1.contains ("..."))
                {
                    cut  = s1.split ("...", QString::SkipEmptyParts).at (0).toInt();
                    cut2 = s1.split ("...", QString::SkipEmptyParts).at (1).toInt();
                    range = true;
                }

            if (! s1.contains (QRegularExpression ("[0-9]+(?:\\.\\.\\.[0-9]+|)"))
                    || (range && (cut > 12 || cut < 1) && (cut2 > 12 || cut2 < 1)) || (! range  && (s1.toInt() > 12 || s1.toInt() < 1)))
                {
                    QMessageBox::warning (nullptr, "Format de la requête " + S, "Le mois doit être compris entre 1 et 12.", QMessageBox::Ok);
                    S.clear();
                }

            if (! sublist.at (2).contains (QRegularExpression ("[0-9]{4}(?:\\.\\.\\.[0-9]{4}|)")) || (! range  && sublist.at (2).toInt() < 2007))
                {
                    QMessageBox::warning (nullptr, "Format de la requête " + S, "L'année doit être numérique, à compter de 2008.", QMessageBox::Ok);
                    S.clear();
                }

            if (! S.isEmpty())
                L << S;
            else
                res = false;
        }

    l->setText (L.join (";"));
    return res;
}
