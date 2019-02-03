# Création du login et du mot de passe
echo "Les noms d'utilisateur (login) doivent commencer par une lettre minuscule et seuls des lettres minuscules, des chiffres, des tirets bas (_), ou des tirets peuvent suivre. Aucun caractère blanc ou spécial n'est autorisé. Ils sont limités à 16 caractères"
echo "Entrer le nom d'utilisateur du compte (exemple :  section_1) : "
read compte_login
echo "Le compte utilisateur $compte_login va être créé. Confirmer (entrer Oui en toutes lettres) :"

read rep

if test x$rep != xOui; then
 echo "Fin de la procédure."
 exit 1
fi

userdel "$compte_login" 1>&2 2>/dev/null
useradd   -G users "$compte_login"

if test $? != 0; then
 echo "Le login n'est pas conforme. Vérifier qu'il ne contient que des caractères autorisés (a-z-A-Z-0-9, ni caractère blanc, ni caractère accentué ni caractère spécial)"
 exit 1
else
 echo "Utilisateur $compte_login créé."
fi

passwd -d "$compte_login" 2>/dev/null

echo "Entrer le mot de passe (pas de caractère blanc) : "
passwd "$compte_login" 

# Vérification de la création des comptes

if test $? = 0; then
  echo "Le compte $compte_login a été créé."
else
  echo "La création du compte $compte_login a échoué."
  exit 1
fi

# Tests sur le fichier $HOME

if test -d "/home/$compte_logini/Dev/altair"; then
  echo "Le répertoire /home/$compte_login/Dev/altair doit être remplacé par un lien symbolique."
  echo "Acceptez-vous de l'écraser (Oui/Non) ?"
  read rep

  if test x$rep = x"Oui"; then
    
    rm -rf /home/$compte_login/Dev/altair
    if ! test -e /home/$compte_login/Dev/altair; then
      echo "Répertoire effacé."
    else
      echo "Le répertoire /home/$compte_login/Dev/altair n'a pas pu être écrasé."
      echo "Reprendre la procédure après l'avoir écrasé manuellement."
      exit 1
    fi

  else
    echo "Répertoire conservé."
    echo "Le compte $compte_login n'a pas été créé."
    userdel $compte_login 2>/dev/null
    echo "Sauvergarder les données et reprendre la procédure ultérieurement."
    exit 1
  fi
  
  if test -h /home/$compte_login/Dev/altair; then
    rm /home/$compte_login/Dev/altair
  fi

fi

# Création du lien symbolique de $HOME vers /home/jf

if ! test -d /home/$compte_login/Dev; then
  mkdir /home/$compte_login/Dev
fi

ln -s /home/jf/Dev/altair /home/$compte_login/Dev/altair

if test -h "/home/$compte_login/Dev/altair"; then
  echo "Le compte utilisateur /home/$compte_login/Dev/altair a été créé (par lien symbolique vers /home/jf/Dev/altair)."
else
  echo "La procédure de création de lien symbolique de /home/$compte_login/Dev/altair vers /home/jf/Dev/altair"
  echo "a échoué. Reprendre la procédure manuellement : avec les droits d'administration"
  echo "entrer la commande ln -s /home/jf /home/$compte_login/Dev/altair"
  echo "Fin de la procédure"
  exit 1
fi

donnees_xhl=/home/$compte_login/Dev/altair/Tests/Exemple/Donnees/xhl/$compte_login

echo "Création du répertoire de données de paye"

mkdir -p $donnees_xhl

if test $? = 0; then
  echo "Répertoire créé: $donnees_xhl"
  chown  $compte_login $donnees_xhl
  chmod -R 0700 $donnees_xhl
else
  echo "Le répertoire $donnees_xhl n'a pas pu être créé automatiquement."
  echo "Il faudra le créer manuellement."
fi

echo "Création du répertoire .local/share/Altair"

mkdir -p /home/$compte_login/.local/share
cp -rf /home/fab/.local/share/Altair /home/$compte_login/.local/share
chown -R $compte_login /home/$compte_login/.local/share

if test -d /home/$compte_login/.local/share/Altair; then
 echo "Répertoire local créé."
fi 

echo "Exportation de la configuration RStudio"

chmod -R 0777 /home/jf/.rstudio-desktop
cp -rf /home/jf/.rstudio-desktop  /home/$compte_login
chown -R $compte_login /home/$compte_login/.rstudio-desktop
chmod -R 0777 /home/jf/Dev/altair/.Rproj.user

if test -d /home/$compte_login/.rstudio-desktop; then
 echo "Configuration exportée."
else
 echo "Configuration non exportée, à recréer manuellement."
fi 


echo "Exportation des icones"
cp -f /home/jf/Desktop/*.desktop /home/$compte_login/Desktop
sed -i s/jf/$compte_login/g /home/$compte_login/Desktop/*.desktop
cp -f /home/jf/Desktop/*.pptx /home/$compte_login/Desktop
rm -f /home/$compte_login/Desktop/Get*.desktop
rm -f /home/$compte_login/Desktop/Donate*.desktop
rm -f /home/$compte_login/Desktop/Récupérer*.desktop
chown $compte_login /home/$compte_login/Desktop/*.desktop

ls /home/$compte_login/Desktop/*.*

# Fin

echo "Fin de la procédure."
exit 0
