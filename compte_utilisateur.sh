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
      echo "Reprendre la procédure après l'avori écrasé manuellement."
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

# Fin

echo "Fin de la procédure."
exit 0
