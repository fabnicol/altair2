/// 
/// \mainpage Sommaire
/// La documentation du projet comprend :
/// - \subpage   page_modules "la présentation de l'organisation modulaire du logiciel"
/// - \subpage   page_alt "la documentation des paramètres de l'interface graphique et des projets Altaïr d'extension .alt"
/// - \subpage   page_lhx "la documentation de la ligne de commande de l'application-noyau lhx"
/// - \subpage   page1 "la documentation de l'algorithme d'analyse des noeuds Remuneration"
/// - \subpage   page_compil  "la documentation de la procédure de compilation sous Linux"
/// - \subpage   page_recover "la procédure de récupération"   
/// - \subpage   page_clone    "la procédure de clonage des plateforme GNU/Linux dédiées"   
/// - \ref todo "la liste des choses à faire"
/// - l'onglet <a href="namespaces.html"><b>Espaces de nommage</b></a>
/// - l'onglet <a href="annotated.html"><b>Classes</b></a>
/// - et l'onglet <a href="files.html"><b>Fichiers</b></a>
/// \n\n
/// \note Dans toute la documentation, un petit triangle bleu en marge gauche indique un lien dynamique. \n
/// Les éléments suivants sont accessibles en cliquant sur ces liens : \n
/// <ul>
/// <li> graphe d'héritage de classes</li>
/// <li> graphe de collaboration de classes</li>
/// <li> graphe des appelants d'une fonction</li>
/// <li> graphe d'appel d'une fonction</li>
/// <li> liste des types, connecteurs, signaux, fonctions et attributs hérités par une classe.</li></ul> \n
/// Il est possible de naviguer dans les graphes en utilisant la fonctionnalité de navigation (+/- pour zoomer, flèches de direction) en haut à droite des graphes.


/// \page page_clone La procédure de clonage
/// \section a Matériel utilisé
/// - une clé USB d'au moins 16 Go, de préférence de type USB3
/// - une version du [logiciel Clonezilla](http://www.clonezilla.org), au moins 2.5.4, entreposée à fins d'archivage sur le dépôt gitlabjf [à cette adresse](https://gitlabjf.ccomptes.fr/fnicol/clonezilla/blob/master/clonezilla.zip) 
/// \section b Procédure de création de l'image de la plateforme
/// A partir d'une plateforme fonctionnelle, créer une image Clonezilla.
/// A cette fin, récupérer tout d'abord le paquet zip du logiciel clonezilla standard sous ~/clonezila.zip   
/// Puis :   
/// 1. Repérer la lettre de *device* de la clé grâce à l'instruction :
///    <pre># fdisk -l</pre>
///    dans une console d'administration.
///    Créer deux partitions EXT4 sur la clé USB sous une plateforme Linux auxiliaire (Live CD ou autre), soit /dev/sdb1 et /dev/sdb2 (remplacer b par la lettre adéquate).    
/// La première partition doit être d'au moins 300 Mo. La deuxième doit être d'au moins 9 Go. Il est possible d'utiliser des logiciels comme gparted (Gnome) ou partitionmanager (KDE).
/// 2. Décompresser le logiciel Clonezilla entreposé dans un paquet zip sur la première partition /dev/sdb1 dans une console d'administration:
///    <pre># mount /dev/sdb1 /mnt</pre>
///    <pre># unzip -o ~/clonezilla.zip /mnt   </pre>
/// 3. Remplacer le fichier /mnt/syslinux/syslinux.cfg de cette première partition par le fichier clonage/création/syslinux.cfg du dépôt GIT (branche master-jf).
/// 4. Sur la plateforme Linux auxiliaire, exécuter le fichier utils/makeboot.sh comme suit dans une console d'administration :
///    <pre># cd /mnt/utils/linux</pre>
///    <pre># ./makeboot.sh /dev/sdb1 </pre> 
/// 5. Démarrer la plateforme Linux fonctionnelle sur la clé USB (utiliser F12 au démarrage pour choisir la clé plutôt que le disque dur principal).  
/// 6. Le PC s'éteindra à l'achèvement du clonage. La seconde partition /dev/sdb2 de la clé USB contient alors un répertoire image/ contenant l'ensemble des fichiers de clonage.
/// 7. Remplacer le fichier /mnt/syslinux/syslinux.cfg par le fichier clonage/copie/syslinux.sfg du dépôt GIT.    
///    La clé ainsi paramétrée est un dispositif autonome complet pour la création d'un clone de la plateforme Altaïr sur le PC cible.   
/// 8. Pour reproduire aisément ce dispositif sur d'autres clés, il est conseillé de préserver une image de clonage :   
///    <pre># dd if=/dev/sdb of=image.img bs=1G count=11 status=progress   </pre>
///    qui pourra être reproduite sur une autre clé /dev/sdc comme suit :
///    <pre># dd if=image.img of=/dev/sdc bs=1G count=11 status=progress </pre>
///
/// \section c Procédure de clonage de la plateforme
/// Une fois la clé de clonage produite par la procédure décrite ci-dessus, il suffit de démarrer un PC cible sur cette clé et d'attendre l'extinction.
/// Le PC est opérationnel dès le redémarrage.  
///
/// \section d Références
///  La documentation originale du site de l'application est archivée : \ref clonezilla_doc
///


/// \page clonezilla_doc Documentation technique : la ligne de commande de clonezilla
/// ### The boot parameters for Clonezilla live    
/// Cette page a été archivée en février 2018 à partir du site clonezilla(http://www.clonezilla.org/fine-print-live-doc.php?path=clonezilla-live/doc/99_Misc/00_live-boot-parameters.doc)   \n   
/// Kernel boot parameters are text strings which are interpreted by the system to change specific behaviors and enable or disable certain features. Different boot loaders use different config files for the boot     parameters:    \n
/// For Clonezilla live booting from CD on a MBR machine, isolinux is the boot loader. Therefore the config file is /syslinux/isolinux.cfg.    \n
/// For Clonezilla live booting from USB flash drive on a MBR machine, syslinux is the boot loader. Therefore the config file is /syslinux/syslinux.cfg.   \n
/// For Clonezilla live booting from PXE on a MBR machine, pxelinux is the boot loader. The config file is on the PXE server, and is configured by the system administrator. it could be something like /tftpboot/nbi_img/pxelinux.cfg/default, or different file.    \n
/// For Clonezilla live booting from a uEFI machine, grub2 is used. Therefore the config file is /EFI/boot/grub.cfg.   \n
/// We now describe the kernel boot parameters related to Clonezilla live. If you want to modify the boot parameters, make sure you edit the right config file.    \n
/// Clonezilla live is based on Debian live with clonezilla installed. Therefore there are 2 kinds of boot parameters:    \n
/// Boot parameters from Debian live-boot and live-config. You can refer to the manual of live-boot and manual of live-config..   \n
/// Boot parameters specially for Clonezilla. All of them are named as "ocs_*", e.g. ocs_live_run, ocs_live_extra_param, ocs_live_batch.   \n
/// ocs_live_run is intended for saving/restoring an image or cloning disk/partition. It is the main program to be run in Clonezilla live. Available program: ocs-live-general, ocs-live-restore or any command you write.   \n
/// e.g. ocs_live_run="ocs-live-general"   \n
/// //NOTE// You might have to use "sudo" command inside your own script, or you can assign it like: ocs_live_run="sudo bash /my-clonezilla"\n
/// ocs_live_extra_param will be used only when ocs_live_run=ocs-live-restore (not for ocs-live-general or any other), then it will be passed to ocs-sr. Therefore these parameters are actually those of ocs-sr.\n
/// e.g. ocs_live_extra_param="--batch -c restoredisk sarge-r5 hda"\n
/// To preset the keyboard layout, use "keyboard-layouts" from live-config.\n
/// e.g. keyboard-layouts=NONE (won't change the default layout, which is US keyboard)\n
/// keyboard-layouts=fr (French keyboard)\n
/// //NOTE// The ocs_live_keymap used in Clonezilla live 1.x is deprecated. The following description is only for reference.\n
/// ocs_live_keymap is for keymap used in Clonezilla live. Man install-keymap for more details.\n
/// e.g. ocs_live_keymap="NONE" (won't change the default layout)\n
/// ocs_live_keymap="/usr/share/keymaps/i386/azerty/fr-latin9.kmap.gz" (French keyboard)\n
/// batch mode or not (yes/no), if no, will run interactively.\n
/// e.g. ocs_live_batch="no"\n
/// To assign the image repository via URI (Uniform Resource Identifier), use "ocs_repository".\n
/// URI supported in Clonezilla live:\n
/// [dev|smb|ssh|nfs|nfs4|http|https]:[//[user:password@]host[:port]][/]path\n
/// There are 5 types of repository can be assigned via "ocs_repository":\n
/// local device: dev:///dev/partition\n
/// nfs server: nfs|nfs4://host/path\n
/// samba server: smb://[domain;user:password@]host/path\n
/// ssh server: ssh://[user@]host[:port]/path (No password can be assigned in URI)\n
/// webdav server: http|https://host[:port]/path (No username and password can be assigned in URI)\n
/// E.g.\n
/// ocs_repository="dev:///dev/sdf1"\n
/// ocs_repository="nfs://192.168.100.254/home/partimag/"\n
/// ocs_repository="nfs4://192.168.100.254/partimag/"\n
/// ocs_repository="smb://administrator:mypasswd@192.168.100.175/images/"\n
/// ocs_repository="smb://my_domain;jack:mypasswd@192.168.1.1:445/images/"\n
/// ocs_repository="ssh://jack@192.168.100.211/home/partimag/"\n
/// ocs_repository="http://192.168.100.180/share"\n
/// Then when Clonezilla live boots, the image repository assigned by ocs_repository will be automatically mounted, and if authentication is required, it will prompt you.\n
/// //NOTE//\n
/// Remember to set the network configuration either by the option "ocs_prerun" to run "dhclient" or the option "ip" if your image repository is a network repository.\n
/// You can also use "ocs_prerun" to use your own command to mount your image repository if you like, and that's more flexible if you want to assign more parameters for your mount command.\n
/// To fetch tarall/zip/sh files from http(s), ftp, tftp, and local URL then extract to /opt/ in the Clonezilla live environment, use "ocs_preload*", i.e.\n
/// Type 1, fetch tarball/zip/shell script file:\n
/// ocs_preload=[http|https|ftp|tftp|file]://[HOST_NAME_or_IP_ADD]/path/to/your_tarball_or_script\n
/// Support file format: tar.gz, tgz, tar.bz2, tbz2, tar.xz, txz, zip, .sh\n
/// Type 2, fetch files on a cifs/nfs directory using mount command:\n
/// ocs_preload="mount -t cifs //YOUR_CIFS_SERVER/path/to/ /path/to/mounting_point"\n
/// ocs_preload="mount -t nfs YOUR_CIFS_SERVER:/path/to/ /path/to/mounting_point"\n
/// Support network file system: cifs (samba) and nfs2/3/4\n
/// E.g. You can put the following in the boot parameter:\n
/// ocs_preload=tftp://192.168.100.254/my-custom.tgz\n
/// ocs_preload=http://192.168.200.254/my-custom.tar.xz\n
/// ocs_preload=file:///lib/live/mount/medium/my-custom.tar.bz2\n
/// ocs_preload=tftp://192.168.100.254/my-custom.sh\n
/// ocs_preload="mount -t cifs //192.168.120.2/images/ /tmp/cifs -o user=administrator,password=yourpasswd"\n
/// ocs_preload="mount -t nfs 192.168.120.254:/home/partimag/script/ /tmp/nfs -o ro"\n
/// Multiple ocs_preload* are available, just append a number after that. E.g.\n
/// ocs_prealod=... ocs_preload1=... ocs_preload2=...\n
/// Then when Clonezilla live boots, the file assigned by ocs_preload will be downloaded and extracted to /opt. Its mode will be set automatically, too. i.e. set as mode 755 and Unix format script.\n
/// Besides, if /opt/{overwrite-all-boot-param,overwrite-part-boot-param} exists (Downloaded from the file assigned by ocs_preload), it can be used to overwrite the boot parameters. File overwrite-all-boot-param\n is to overwrite the whole /proc/cmdline, while overwrite-part-boot-param only overwrites part of the variables in /proc/cmdline. Especially those "ocs_*" parameters. This is useful when you want to customize\n the boot parameters while you do not want to or can not modify the boot parameter file (isolinux.cfg, for example). \n
/// For example, the contents of "overwrite-all-boot-param" can be like: boot=live union=overlay username=user config components quiet noswap edd=on nomodeset nodmraid locales=en_US.UTF-8 keyboard-layouts=en ocs_live_run="ocs-sr -x -um beginner --batch -p reboot -scs -scr -sfsck -senc" ocs_live_extra_param="" ocs_live_batch=no vga=788 ip= net.ifnames=0 nosplash i915.blacklist=yes radeonhd.blacklist=yes nouveau.blacklist=yes vmwgfx.enable_fbdev=1.\n
/// The content of "overwrite-part-boot-param" can be like: locales=ja_JP.UTF-8 keyboard-layouts=fr ocs_live_run="ocs-sr -x -scr -scs" ocs_live_batch="no"\n
/// //NOTE//\n
/// Remember to set the network configuration either by the option "ocs_prerun" to run "dhclient" or the option "ip" if your files are on the network repository.\n
/// You can also use "ocs_prerun" to use your own command to fetch your files and put in Clonezilla live environment if you like, and that's more flexible if you want to assign more parameters for your own command.\n
/// To preset the language, use "locales" from live-config.\n
/// e.g. locales=en_US.UTF-8\n
/// //NOTE// The "ocs_lang" used in Clonezilla live 1.x is deprecated.\n
/// To preset the font name and font size for KMS mode, use "ocs_fontface" and "ocs_fontsize".\n
/// e.g. ocs_fontface="TerminusBold"\n
/// ocs_fontsize="24x12"\n
/// Check file /etc/default/console-setup and the console-setup(5) manual page on Debian or Ubuntu system.\n
/// ocs_debug (or ocs-debug) is for you to enter command line prompt before any clonezilla-related action is run. This is easier for you to debug.\n
/// ocs_daemonon, ocs_daemonoff, ocs_numlk, ocs_capslk.\n
/// Ex. for the first 2 parameters, ocs_daemonon="ssh", then ssh service will be turned on when booting. For the last 2 parameters, use "on" or "off", e.g. ocs_numlk=on to turn on numberlock when booting.\n
/// ocs_prerun, ocs_prerun1, ocs_prerun2... is intended for running a command before an operation of saving/restoring an image or cloning disk/partition is started. E.g. ocs_prerun="df -h" (For running the system\n command "df -h", or ocs_prerun="/lib/live/mount/medium/myscript.sh" (For running the script myscript.sh in the root path of the live USB media or CD, i.e. after booting into Clonezilla live, the root path of live USB media or CD is mounted as /lib/live/mount/medium/). If you have more commands to run, you can assign them in the order: ocs_prerun=..., ocs_prerun1=..., ocs_prerun2=.... If more than 10 parameters,\n remember to use ocs_prerun01, ocs_prerun02..., ocs_prerun11 to make it in order.\n
/// ocs_postrun, ocs_postrun1, ocs_postrun2... is intended for running a command after an operation of saving/restoring an image or cloning disk/partition has been completed. E.g. ocs_postrun="/live/image/myscript.sh". \nIf you have more commands to run, you can assign them in the order: ocs_postrun=..., ocs_postrun1=..., ocs_postrun2=.... If more than 10 parameters, remember to use ocs_postrun01, ocs_postrun02..., ocs_postrun11 to make it in order. //NOTE// \nIf you use this parameter, remember not to use "-p reboot" or "-p poweroff" option of ocs-sr. Use "-p true" instead because if "-p reboot" or "-p poweroff" is used, it will reboot or shutdown the system before ocs_postrun related commands are executed. For more info, check this discussion.\n
/// ocs_savedisk_prerun, ocs_saveparts_prerun, ocs_restoredisk_prerun, and ocs_restoreparts_prerun. These four boot parameters can be used to run a command right before the action savedisk, saveparts, restoredisk,\n and restoreparts, respectively. E.g. ocs_restoredisk_prerun="echo running ocs_restoredisk_prerun" means the command "echo running ocs_restoredisk_prerun" will be run right before "restoredisk" action is run.\n
/// ocs_savedisk_postrun, ocs_saveparts_postrun, ocs_restoredisk_postrun, and ocs_restoreparts_postrun. These four boot parameters can be used to run a command right after the action savedisk, saveparts,\n restoredisk, and restoreparts, respectively. E.g. ocs_restoredisk_postrun="echo running ocs_restoredisk_postrun" means the command "echo running ocs_restoredisk_postrun" will be run right after "restoredisk" action is run.\n
/// ocs_overwrite_postaction. This boot parameter has higher priority than the action assigned by option "-p" in ocs-sr, and "-pa" in ocs-onthefly. The value for ocs_overwrite_postaction is: [choose|reboot|poweroff]-on-[restoredisk|restoreparts|savedisk|saveparts|clone] E.g., choose-on-restoredisk means the postaction for restoredisk will always be "choose" no matter what is assigned in -p of ocs-sr or -pa of ocs-onthefly.\n
/// echo_ocs_prerun and echo_ocs_postrun are used to echo the commands of prerun and postrun. By default the command assigned in ocs_prerun or ocs_postrun will be echoed. By using "no" the command won't be\n echoed. This is useful when you want to hide some commands.\n
/// e.g. echo_ocs_prerun="no" (Will not show the commands assigned in boot parameter "ocs_prerun").\n
/// ocs_live_run_tty. This option allows you to specify the tty where $ocs_live_run is run. By default $ocs_live_run is run on /dev/tty1 only. If you want to make $ocs_live_run use on ttyS0, for example, use ocs_live_run_tty="/dev/ttyS0".\n
/// //NOTE//\n
/// The kernel boot parameter "console" (e.g. console=ttyS0,38400n81) is for the GNU/Linux system, while ocs_live_run_tty is for $ocs_live_run. They are different. Therefore you can make GNU/Linux system output\n the messages on tty1, while make $ocs_live_run run on ttyS1 like: "console=tty1,38400n81 ocs_live_run_tty=/dev/ttyS1". To make all the output on serial console ttyS1, use: "console=ttyS1,38400n81 ocs_live_run_tty=/dev/ttyS1"\n
/// It's recommended to assign locales and keyboard-layouts in the boot parameters too.\n
/// ip, this option allows you to specify the network parameters for network card. In Clonezilla live a patched live-initramfs is used, which is different from the original live-initramfs so that you can assign\n DNS server, too. Its format is: ip=ethernet port,IP address, netmask, gateway, DNS. E.g. If you want to assing eth0 with IP address 10.0.100.1, netmask 255.255.255.0, gateway 10.0.100.254, DNS server 8.8.8.8, you can assign the following in the boot parameter:\n
/// ip=eth0:10.0.100.1:255.255.255.0:10.0.100.254:8.8.8.8\n
/// If more than one network card, you can use "," to separate them, e.g.:\n
/// ip=eth0:10.0.100.1:255.255.255.0:10.0.100.254:8.8.8.8,eth1:192.168.120.1:255.255.255.0:192.168.120.254::\n
/// Besides, two parameters could be used to assign the network card for PXE booting, "live-netdev" (yes, not ocs_live_netdev) and "nicif" can be used when using PXE booting,\n
/// For "live-netdev", you can force to assign the network device by its ethernet device name on GNU/Linux, e.g. eth0, eth1, to get filesystem.squashfs. This is useful when there are two or more NICs are linked.\n E.g. live-netdev="eth1" allows you to force the live-initramfs to use eth1 to fetch the root file system filesystem.squashfs.\n
/// For "nicif", you can force to assign the network device by its MAC address, e.g. 00:aa:bb:cc:dd:ee, to get filesystem.squashfs. This is useful when there are two or more NICs are linked. E.g.\n nicif="00:aa:bb:cc:dd:ee", allows you to force the live-initramfs to use the ethernet card with MAC address "00:aa:bb:cc:dd:ee" to fetch the root file system filesystem.squashfs.\n
/// You can find some examples about using these boot parameters in the [Clonezilla Live Doc](http://www.clonezilla.org/clonezilla-live-doc.php).\n

/// \page page_recover Procédure de récupération
/// \section connexion_blank En cas d'impossibilité de se connecter
/// Si l’écran graphique de connexion aux comptes (login) n’apparaît pas, le système est sérieusement compromis, ce qui peut arriver en cas de redémarrage forcé au cours d'une actualisation automatique non terminée.\n
/// Il faut recloner l'ensemble du système.\n
/// 
/// \section connexion_ok Cas général
/// Si cet écran apparaît, ce qui est le plus souvent le cas, mais qu’il y a seulement un problème de connexion à un ou plusieurs comptes (login), procéder comme suit :\n
/// **EtapeA**\n   
/// 1. Préalablement, télécharger sous Windows le paquet altair.zip à l’[adresse](https://www.dropbox.com/s/35696fw5w0upleo/altair.zip?dl=0) ou sur toute adresse réseau indiquée par le gestionnaire du logiciel.\n
/// 2. Copier ce paquet sur la clé USB3 préalablement complètement nettoyée.<pre></pre>
/// 
/// **Etape B**\n   
/// 3. Aller sur le PC linux.
/// 4. Déconnecter tous les périphériques éventuels (autres clés, cdrom, etc.)
/// 5. Démarrer sans la clé USB3 dans son port USB. Attendre que l’écran de login apparaisse. Puis taper ensemble les touches :  **Ctrl + Alt + F1**
/// 6. Puis au login : \n
///       fabpc login : \n
///       rentrer : root \n
///       Puis rentrer le mot de passe root \n
///       On obtient l’invite  <pre style="color:red;">fabpc ~#</pre>
/// 
/// **Etape C**\n
/// 7. Une fois l’invite en ligne de commande comme ci-dessus on est en mode administrateur root.
/// 8. Insérer alors la clé USB3 dans un port USB de façade.
/// 9. La monter : \n
///       <span style="color:red;">fabpc ~#</span> mkdir /mnt \n
///       <span style="color:red;">fabpc ~#</span> mount –o nonempty /dev/sdb1  /mnt
/// 10. Vérifier si altair.zip est bien reconnue sur la clé :\n
///       <span style="color:red;">fabpc ~#</span> ls /mnt    
/// 11. Aller sur la clé :\n 
///       <span style="color:red;">fabpc ~#</span> cd /mnt
/// 12. Décompresser le paquet :\n
///       <span style="color:red;">fabpc ~#</span> unzip –o altair.zip
/// 13. Détruire le répertoire altair déficient :\n
///       <span style="color:red;">fabpc ~#</span> rm –rvf /home/fab/Dev/altair
/// 14. Copier les fichiers de la clé vers le PC :\n
///       <span style="color:red;">fabpc ~#</span> cp –rvf altair /home/fab/Dev
/// 15. Eteindre : \n 
///       <span style="color:red;">fabpc ~#</span> shutdown –h now <pre></pre> 
/// 
/// **Etape D**\n
/// 16. Enlever la clé.
/// 17. Connecter le port Ethernet (vérifier la connexion préalablement sous Windows etc.).
/// 18. Redémarrer. Le PC est alors en mode d’actualisation automatique.\n Le laisser impérativement s’éteindre seul.
/// 19. Redémarrer lorsqu’il est éteint et surtout pas avant.
/// 20. Tester. <pre></pre>
/// 
/// **Etapes optionnelles**\n
/// En cas d’échec à l’actualisation du PC.
/// 21. Répéter B.\n
/// 22. Insérer la clé. Répéter les deux premières lignes de commande de C.\n
/// 23. Copier le fichier version.log sur la clé :\n
/// <span style="color:red;">fabpc ~#</span> cp  /home/Public/version.log   /mnt \n
/// Ce fichier est un log des procédures d’actualisation. \n
/// 24. Pour vérifier qu’une actualisation automatique est en cours (ou non) :\n
/// <span style="color:red;">fabpc ~#</span> tail  /home/Public/version.log   \n
/// Si oui s’affiche un message :\n
///  *  Actualisation de l’application Altaïr…\n
///  *  Actualisation du dépôt fab… \n
/// Puis du dépôt jf puis quelques autres messages.\n
/// Sinon s’affiche :\n
/// "***" Pas de mise à jour "***"
/// 25. Copier le fichier log sur la clé :\n
/// <span style="color:red;">fabpc ~#</span> cp  /home/Public/log   /mnt \n
/// Ce fichier est un log des connexions.
/// 26. Copier le fichier VERSION sur la clé :\n
/// <span style="color:red;">fabpc ~#</span> cp  /home/Public/VERSION   /mnt \n
/// Ce fichier est  le numéro de version normalement téléchargé depuis le dépôt Gitlabjf.
/// S’il n’est pas aligné, cela signifie que la connexion internet est dysfonctionnelle.
/// 
/// \section connexion_desktop Si la connexion au compte d'administration est possible
/// Il est alors possible d'utiliser les fonctionnalités de KDE pour simplifier les manipulations précédentes.\n
/// 1. Mettre le paquet `altair.zip` sur la clé USB.
/// 2. Avec la clé connectée au port USB, se connecter au compte **fab** (administrateur)
/// 3. Ouvrir le gestionnaire de données et allez dans **Clé USB3**.
/// 4. Copier-coller `altair.zip` de **Clé USB3** dans `/home/fab/Dev` (=répertoire Dev dans Dossier personnel). On peut faire **Ctrl+C** puis **Ctrl+V** comme sous windows.
/// 5. Ecraser `/home/fab/Dev/altair` (**Shift + suppr + confirmer**)
/// 6. Décompresser `/home/fab/Dev/altair.zip` ce qui donnera un nouveau `/home/fab/Dev/altair` \n
///    Il suffit pour cela de faire clic droit sur **altair.zip > Extraire > Extraire l’archive ici, détecter le sous-dossier**.
/// 7. Refaire une actualisation automatique sans clé, sans connexion à aucun compte.



/// \page page_compil La documentation de la procédure de compilation sous Linux
/// \section prereq Prérequis
/// Altaïr est compilé avec les outils suivants :
/// - une version recompilée sous forme statique des bibliothèques Qt5.7
/// - le compilateur C++ GNU GCC 6.2
/// L'ensemble des dépendances requises est livré avec la plateforme elle-même.
///
/// \section proc_qtcreator   Avec Qt creator
/// La procédure standard de compilation sous Linux (et avec quelques ajustements sous Windows) utilise l'IDE (interface de développement intégrée) Qt creator
/// de l'entreprise [Digia](https://en.wikipedia.org/wiki/Main_Page) (propriétaire des bibliothèques [Qt5](https://fr.wikipedia.org/wiki/Qt)).\n       
/// Il est possible de télécharger Qt creator sur le [site de téléchargement](https://www1.qt.io/offline-installers/) de Digia.\n     
/// L'IDE Qt creator est livrée avec le compte administrateur de la plateforme. Une icône verte (Qt) est disposée sur le bureau et permet de lancer l'IDE.\n    
/// Il est paramétré pour utiliser les bibliothèques et le compilateur indiqués dans les prérequis.\n    
/// Au lancement de l'IDE :
/// - cliquer sur Fichier > Ouvrir un fichier ou un projet > Sélectionner le fichier de projet altair.linux.pro dans le dossier `~/Dev/altair/Interface/gui`
/// - cliquer sur Fichier > Ouvrir un fichier ou un projet > Sélectionner le fichier de projet lhx.linux.pro dans le dossier `~/Dev/altair/lhx`
/// - cliquer sur Fichier > Ouvrir un fichier ou un projet > Sélectionner le fichier de projet fwidgets_lib.pro dans le dossier `~/Dev/altair/fwidgets_lib`
/// - cliquer sur Compiler > Exécuter qmake
/// - cliquer sur Compiler > Tout recompiler
/// Une barre de progression verte en bas à droite de l'IDE témoigne de la progression de la compilation.\n A la fin de cette progression, la fenêtre "Sortie de compilation"
/// en bas de l'IDE indique que les processus de compilation se sont terminés normalement (ou pas).\n     
/// L'ensemble du logiciel est alors recompilé à partir des sources.    
///
/// \section proc_other Avec les Makefile
/// Les Makefile sont générés à chaque invocation de qmake.\n    
/// Cet outil peut être utilisé manuellement comme suit :      
/// - "/home/fab/Qt5.7/5.7/Src/qtbase/bin/qmake" /home/fab/Dev/altair/Interface/gui/altair.linux.pro -r -spec linux-g++     
/// - "/home/fab/Qt5.7/5.7/Src/qtbase/bin/qmake" /home/fab/Dev/altair/lhx/lhx.linux.pro -r -spec linux-g++    
/// - "/home/fab/Qt5.7/5.7/Src/qtbase/bin/qmake" /home/fab/Dev/altair/fwidgets_lib/fwidgets_lib.pro -r -spec linux-g++ \n     
/// Puis, dans chaque répertoire contenant les fichiers .pro des trois lignes de commande ci-dessus, lancer `make -j8`.\n     
/// La compilation est alors terminée.      

/// \page page_modules L'organisation modulaire du logiciel Altaïr
/// \section sec1_modules Les modules du logiciel
/// Le logiciel comprend <b>trois modules exécutable et un module externe sous forme de bibliotèque statique</b> :\n
/// <ul>
/// <li>une <b>interface graphique utilisateur</b> qui produit une ligne de commande<br>Le code source de cette interface est distribué dans le répertoire \b Interface ;</li>
/// <li>cette interface est développée en utilisant les composants (\em widgets) fonctionnels du module externe \b fwidgets_lib, réutilisé pour consolider les fonctionnalités de l'interface graphique et réduire le volume de code propre. <br>
/// Le code source de ce module externe est distribué dans le répertoire \b fwidgets_lib ;</li>
/// <li>une <b>application-noyau</b> nommée \b \em lhx, qui :</li>
///   <ul>
///   <li>exécute cette ligne de commande ;</li>
///   <li>extrait les données de paye pour produire des bases (bulletins et lignes de paye) au format CSV, équivalentes à celles qui sont exportées par le logiciel <a href="http:///www.xemelios.org">Xemelios</a> de la DGFIP ;</li>
///   </ul>
/// Le code source de cette application-noyau est distribué dans le répertoire \b lhx.
/// </li>
/// <li>un analyseur de données, constitué d'un <b>ensemble de scripts</b> en <a href="http://www.r-project.org">langage R</a>, qui sont exécutés :
///   <ul>
///   <li>soit directement depuis l'interface graphique ;</li>
///   <li>soit en utilisant l'interface de développement <a href="http://www.rstudio.com">RStudio</a>.</li>
///   </ul>
///   En entrée, ces scripts réimportent en mémoire les bases de paye précédemment exportées au format CSV. <br>
///   En sortie, ils produisent des rapports d'analyse de la paye, aux formats Microsoft Word, Libreoffice Writer ou Adobe PDF.<br>
///   Ces analyses portent sur la démographie des structures étudiées, les rémunérations des personnels et le respect du cadre statutaire et réglementaires de la paye.<br>
///   Une annexe propose quelques indicateurs d'appréciation de la fiabilité des bases de paye et des tableaux auxiliaires en lien, qui peuvent faciliter l'analyse personnalisée de certains aspects qui ne font pas l'objet d'un traitement automatisé.\n
///   Les rapports d'analyse sont accompagnés d'un dossier documentaire, qui comprend des notices méthodologiques et des documents de référence.<br>
///   Ils contiennent des liens hypertextes vers des fichiers auxiliaires au format CSV, qui précisent les analyses statistiques et réglementaires présentées dans le rapport.<br>
/// Ces scripts d'extension .R sont distribués, pour certains, dans le répertoire racine et, pour la plupart d'entre eux, dans le répertoire \b Tests/Exemple.
/// </li>
/// </ul>
/// \section sec2_docum La documentation des modules
/// Les modules sont documentés dans le répertoire \b Docs.\n
/// En particulier :\n
/// - Pour les modules <i>interface graphique</i> et <i>application-noyau</i> la documentation \em Doxygen est générée dans le répertoire \b Docs/html
/// - la [documentation PDF](../Scripts_R/sommaire_pdf.pdf) et la [documentation html](../Scripts_R/sommaire_html.pdf) du <i>module analyseur</i> est générée dans le répertoire \b Docs/Scripts_R.      
///   Les deux sommaires doivent être lus dans ce même répertoire pour que leurs liens relatifs vers des fichiers auxiliaires puissent s'ouvrir.     
/// - le [guide utilisateurs](../Guide utilisateurs.pptx)  pour les modules <i> Interface graphique </i>et <i>application-noyau</i> (extraction des données) est le fichier <b>Docs/Guide utilisateurs.pptx</b>
/// - un guide utilisateurs relatif au module <i>analyseur</i> (rapports d'analyse) a été produit à l'automne 2017 mais n'a pas été, à ce jour, validé par le CAM ; sa diffusion est suspendue.<br>
///   Une [version provisoire](../Guide_Altaïr.docx) est enregistrée sous \b Docs/Guide_Altaïr.docx
/// - la [documentation méthodologique](../méthodologie.pdf) à caractère statistique relative au calcul des rémunérations est le fichier \b Docs/méthodologie.pdf
/// - le [rapprochement](../Guide utilisateurs.pptx) entre les requêtes réalisées par l'analyseur de données est actualisé dans le fichier <b>Docs/requetes ALTAIR.xlsx (\em Auteure : Mme Vitalia le Boudec)</b>
/// - le [guide d'installation](../projet/INSTALLATION DU LOGICIEL ALTAÏR.pptx)  est le fichier <b> Docs/projet/INSTALLATION DU LOGICIEL ALTAÏR.pptx</b>
/// - une [notice à l'attention](../projet/MANTIS-suivi des bogues.pdf) des testeurs pour le suivi des bogues est proposée dans le fichier <b> projet/MANTIS-suivi des bogues.pdf</b>
/// Le répertoire \b Docs contient par ailleurs des documents utiles :
///     - soit à la poursuite du développement : volumétrie de codage, guide de développement sous Windows (peut être utile en cas de nécessité)
///     - soit à l'interprétation des sorties : documents juridiques et statistiques de référence, copie de la documentation en ligne de la DGFIP sur le modèle de données \b Xemelios, qui est globalement suivi par le module <i>application-noyau</i>, copie de la convention-cadre nationale de dématérialisation.


/// \page page_alt Documentation des paramètres de l'interface graphique et des projets Altaïr d'extension .alt
/// \tableofcontents
/// \section edition_alt Edition d'un projet .alt
/// Pour éditer un projet :
/// - soit utiliser un éditeur de texte et de suivre les indications \e supra ;
/// - soit utiliser l'éditeur intégré de projet .alt : <b>Ctrl + E</b> ou menu Editer &rarr; Editer le projet courant &rarr; faire les modifications &rarr; menu Fichier interne à l'éditeur &rarr; Enregistrer et Quitter ou autres actions de ce menu.
/// - soit modifier les paramètres dans l'interface elle-même puis enregistrer le projet (<b>Ctrl + S</b> ou menu Fichier &rarr; Enregistrer) ou utiliser la fonctionnalité d'enregistrement automatique de l'interface (menu Configurer &rarr; Configurer l'interface &rarr; Sauvegarder le projet .alt automatiquement) ;
///
/// \section edition_defaut_alt Génération automatique du projet par défaut
/// Un projet par défaut (<i>défaut.alt</i>) est automatiquement généré, en l'absence d'autre projet créé par l'utilisateur, dans l'emplacement défini par tools::userdatadir, autrement dit :
/// - dans ~/Dev/altair/#DONNEES_XHL pour l'administrateur
/// - dans ~/Dev/altair/#DONNEES_XHL/<i>login</i> pour les autres utilisateurs.
///
/// Le répertoire de génération de <i>défaut.alt</i> est celui dans lequel sont placées les données de paye du gestionnaire de fichiers (partie gauche de l'interface).\n
/// Sauf spécification contraire de la constante de compilation #DONNEES_XHL, ce répertoire est en général :
/// - \b ~/Dev/altair/Tests/Exemple/Donnees/xhl pour l'administrateur
/// - \b ~/Dev/altair/Tests/Exemple/Donnees/xhl/<i>login</i> pour les autres utilisateurs.
///
/// \section lancement_alt Chargement automatique d'un projet Altaïr dans l'interface graphique
/// - Il est possible d'utiliser les fichiers de projet .alt pour lancer directement l'interface graphique, chargée des paramètres et données contenus dans le projet.\n
/// L'utilisateur retrouve ainsi exactement l'état de l'interface et du traitement des données qui était en cours lors d'une utilisation précédente.
/// A cette fin :
///   - vérifier que l'ensemble des dépendances de bibliothèques est dans le chemin d'exécution par défaut (sous Linux, LD_LIBRARY_PATH des fichiers .desktop est exporté en variable globale)
///   - sous Linux (KDE) : Clic droit sur un fichier .alt &rarr; Ouvrir avec &rarr; Autres &rarr; renseigner le chemin de l'interface Altaïr et cocher Mémoriser l'association...\n
/// Un clic (ou deux, selon les bureaux) sur un fichier .alt permet de charger automatiquement le projet en lançant l'interface graphique.
/// - Une autre façon de charger automatiquement le projet en ligne de commande est d'indiquer le chemin du projet en premier argument du chemin de l'interface :
/// \code
/// ~/Dev/altair/Interface_linux/gui/x64$ ./Altair ~/projet.alt
/// \endcode
/// - Une dernière façon de charger automatiquement un projet est d'utiliser le projet par défaut.\n
/// Il est possible de charger automatiquement le projet par défaut en cochant la case <b>Charger le projet par défaut au lancement</b> du dialogue de configuration.\n
/// Si la case <b>Sauvegarder le projet .alt automatiquement</b> est aussi cochée, le projet par défaut <i>défaut.alt</i> a sauvegardé l'ensemble des paramètres de l'interface décrits dans le  \ref sec_alt1 "tableau des paramètres" avant sa dernière fermeture.\n
/// Au lancement suivant, l'état antérieur de l'interface est alors automatiquement restauré.
/// \warning Ce paramétrage peut occasionner des plantages de l'interface si les fichiers de paye présents dans le projet automatiquement sauvegardé à la dernière exécution de l'interface ne sont plus présents sur le disque à l'emplacement indiqué par le projet .alt.\n
/// Dans ce cas, écraser le projet <i>défaut.alt</i>  et relancer.
///
/// \section lancement_alt_manu Chargement manuel d'un projet Altaïr dans l'interface graphique
/// Alternativement, après lancement de l'interface :
/// - menu Fichier &rarr; Ouvrir le projet .alt ou <b>Ctrl + O</b>
/// - si le projet est en fichiers récents (menu Fichier &rarr; fichiers numérotés par ordre d'apparition), sélectionner le fichier bascule l'interface sur ce projet récent
/// - il est aussi possible de désarchiver un projet archivé (menu Fichier &rarr; Désarchiver le projet)
/// - pour un nouveau projet : <b>Ctrl + N</b> ou menu Fichier &rarr; Nouveau projet.
///
/// \section cli_lhx Génération de la ligne de commande de l'application-noyau lhx
/// Par défaut la ligne de commande de \b lhx contient les options suivantes, qui sont automatiquement générées :
/// <ul><li>-m : calcul des maxima du nombre de bulletins de paye par mois et du nombre de lignes de paye par agent par bulletin</li>
/// <li>-d ',' : séparateur décimal virgule</li>
/// <li>-s ';' : séparateur de champs point-virgule</li>
/// <li>-E : Générer l'échelon</li>
/// <li>-rank tools::sharedir + "/rank" : fichier exporté dans ~/.local/share/applications/Altair (tools::sharedir par défaut) <br> indiquant l'index de la barre de progression</li>
/// <li>&ndash;&ndash;cdrom : si depuis un disque optique</li>
/// <li>-D  ~/Dev/altair/#DONNEES_SORTIE /subdir : répertoire d'exportation des bases si subdir est non vide </li>
/// <li>&ndash;&ndash;esiret Sirets séparés par blancs : si des Sirets sont excus (barrés) dans l'onglet Siret</li>
/// <li>&ndash;&ndash;ebudget budgets séparés par blancs : si des budgets sont excus (barrés) dans l'onglet Budget</li>
/// <li>&ndash;&ndash;eemployeur employeurs séparés par blancs : si des employeurs sont excus (barrés) dans l'onglet Employeur</li>
/// </ul>
/// Elle contient en outre des valeurs qui dépendent de l'état des composants (\em widgets) de l'interface graphique.\n
/// En fonction des valeurs d'un des composants identifiés dans le tableau suivant, un ou plusieurs éléments sont ajoutés à la ligne de commande de \b lhx.\n
/// En miroir, une valeur (ou une série de valeurs) est générée entre la paire de balises XML correspondantes dans le projet .alt.\n
/// Par exemple, si la case du dialogue de configuration <b> Limiter la verbosité </b> est cochée, le composant MainWindow::defaultQuietBox génère l'option de ligne de commande \b -q.\n
/// En parallèle, une paire de balises \em quiet est munie de la valeur positive \em oui dans le projet XML d'extension .alt :
/// \code
/// <quiet>
///  oui
/// </quiet>
/// \endcode
/// Si la même case est décochée, l'option \b -q n'est pas insérée en ligne de commande. La paire de balises du projet .alt est alors renseignée par \em non :
/// \code
/// <quiet>
///  non
/// </quiet>
/// \endcode
/// \n
/// \section sec_alt1 Balises XML des projets Altaïr et composants (widgets) associés de la bibliothèque fwidgets_lib
/// \note Lorsque qu'une notation {valeur1, valeur2,...} est rencontrée, comprendre qu'est choisie une valeur parmi valeur1, valeur2, etc.\n
/// La valeur est donnée soit littéralement (en extension) soit en intension (année -> 2011, 2012, etc.).
/// 
/// <div>
///
///
/// <table>
/// <tr><th>Balise XML</th><th>Attributs</th><th>Valeur</th><th>Dialogue/Onglet</th><th>Génération de ligne de commande pour <i>lhx</i></th><th>Commentaires</th><th>Composant</th></tr>
/// <tr><td style="font-weight:bold;">projet</td><td>version="..."</td><td></td><td></td><td></td><td>Début du projet et version de l'application.</td><td></td></tr>
/// <tr><td style="font-weight:bold;">data</td><td></td><td></td><td></td><td></td><td>Début des données de paye</td><td></td></tr>
/// <tr><td style="font-weight:bold;">XHL</td><td>profondeur="2"</td><td>Enchâsse les balise <i>onglet</i></td><td>Onglet central</td><td></td><td>Données de paye : 2 niveaux d'enchâssement ("onglet" et "item").</td><td>Altair::project</td></tr>
/// <tr><td style="font-weight:bold;">onglet</td><td>V="{année}"</td><td>Enchâsse les balises <i>item</i></td><td>Onglets annuels centraux</td><td></td><td>Onglet pour chaque année de paye (sauf trois derniers).</td><td></td></tr>
/// <tr><td style="font-weight:bold;">item</td><td>V="..." S="..." B="..." E="..." EM="..."</td><td>Chaîne de caractères</td><td>Elément des onglets annuels centraux</td><td>Chemin du fichier.</td><td>Chemin du fichier mensuel de paye pour le mois V, le Siret S, l'établissement E et l'employeur EM</td><td></td></tr>
/// <tr><td style="font-weight:bold;">onglet</td><td>V="Siret"</td><td></td><td>3ème onglet en partant de droite.</td><td>Si barré(s) : &ndash;&ndash;esiret <i>Siret(s) exclus(s)</i></td><td>Onglet des Siret.</td><td></td></tr>
/// <tr><td style="font-weight:bold;">item</td><td>V="" S="" B="" E="" EM=""</td><td>Chaîne de caractères du Siret.</td><td></td><td></td><td>Siret détecté (éventuellement : exclu).</td><td></td></tr>
/// <tr><td style="font-weight:bold;">onglet</td><td>V="Budget"</td><td></td><td>2ème onglet en partant de droite.</td><td>Si barré(s) : &ndash;&ndash;ebudget <i>Budget(s) exclus(s)</i></td><td>Onglet des Budgets.</td><td></td></tr>
/// <tr><td style="font-weight:bold;">item</td><td>V="" S="" B="" E="" EM=""</td><td>Chaîne de caractères du Budget.</td><td></td><td></td><td>Budget détecté (éventuellement : exclu).</td><td></td></tr>
/// <tr><td style="font-weight:bold;">onglet</td><td>V="Employeur"</td><td></td><td>1er onglet en partant de droite</td><td>Si barré(s) : &ndash;&ndash;employeur <i>Employeur(s) exclus(s)</i></td><td>Onglet des Employeurs.</td><td></td></tr>
/// <tr><td style="font-weight:bold;">item</td><td>V="" S="" B="" E="" EM=""</td><td>Chaîne de caractères de l'employeur.</td><td></td><td></td><td>Employeur détecté (éventuellement : exclu).</td><td></td></tr>
/// <tr><td style="font-weight:bold;">systeme</td><td></td><td></td><td></td><td></td><td>Paramètres système (paramétrage de l'interface, dialogues d'options et de configuration de l'interface)</td><td></td></tr>
/// <tr><td style="font-weight:bold;">projectManagerDisplay</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher le gestionnaire de projets à droite de l'interface.<br>La profondeur 0 indique une balise sans enchâssement : la valeur est lue dans le texte de la balise</td><td>MainWindow::defaultProjectManagerWidgetLayoutBox</td></tr>
/// <tr><td style="font-weight:bold;">fileManagerDisplay</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher le gestionnaire (ou explorateur ou arbre) de fichiers à gauche de l'interface.</td><td>MainWindow::defaultFileManagerWidgetLayoutBox</td></tr>
/// <tr><td style="font-weight:bold;">fullScreenDisplay</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Plein écran ("oui" ou "non")</td><td>MainWindow::defaultFullScreenLayoutBox</td></tr>
/// <tr><td style="font-weight:bold;">outputTextEdit</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher l'onglet des messages en bas de l'interface.</td><td>MainWindow::defaultOutputTextEditBox</td></tr>
/// <tr><td style="font-weight:bold;">fileToolBar</td><td> profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher la barre d'outils du menu Fichier.</td><td>MainWindow::defaultFileToolBarBox</td></tr>
/// <tr><td style="font-weight:bold;">editToolBar</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher la barre d'outils du menu Editer.</td><td>MainWindow::defaultEditToolBarBox</td></tr>
/// <tr><td style="font-weight:bold;">processToolBar</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher la barre d'outils du menu Lancer.</td><td>MainWindow::defaultProcessToolBarBox</td></tr>
/// <tr><td style="font-weight:bold;">optionsToolBar</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher la barre d'outils du menu Configurer.</td><td>MainWindow::defaultOptionsToolBarBox</td></tr>
/// <tr><td style="font-weight:bold;">aboutToolBar</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Afficher la barre d'outils du menu Aide.</td><td>MainWindow::defaultAboutToolBarBox</td></tr>
/// <tr><td style="font-weight:bold;">saveProjectBehavior</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Enregistrer les projet .alt automatiquement à chaque modification d'un paramètre.</td><td>MainWindow::defaultSaveProjectBehaviorBox</td></tr>
/// <tr><td style="font-weight:bold;">importerAuLancement</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Importer les données de paye du répertoire Données (.xhl/.xml) au lancement de l'interface.</td><td>MainWindow::importerAuLancementBox</td></tr>
/// <tr><td style="font-weight:bold;">loadProjectBehavior</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Charger le projet .alt par défaut au lancement de l'interface.</td><td>MainWindow::defaultLoadProjectBehaviorBox</td></tr>
/// <tr><td style="font-weight:bold;">limitConsoleOutput</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td></td><td>Limiter le nombre de lignes du log de l'onglet Console.</td><td>MainWindow::defaultMaximumConsoleOutputBox</td></tr>
/// <tr><td style="font-weight:bold;">TRAITEMENT</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye Traitement</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">NBI</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye NBI</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">PFI</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye PFI</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">PFR</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye PFR</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">IPF</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye IPF</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">PSR</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye PSR</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">IFTS</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye IFTS</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">IAT</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye IAT</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">IHTS</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye IHTS</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">VACATAIRES</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye VACATAIRES</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">ASTREINTES</td><td>profondeur="0"</td><td>Soit blanc soit Code(s) de paye ASTREINTES</td><td>Dialogue d'options &rarr; onglet Codes</td><td></td><td>Séparés par point-virgule. Saisi manuellement dans le dialogue d'options à l'onglet Codes.</td><td>Pointeur local <i>line</i> vers FLineEdit dans codePage::ajouterVariable</td></tr>
/// <tr><td style="font-weight:bold;">Matricules</td><td>profondeur="0"</td><td>Matricule(s), mois, année(s) saisi(s) pour extraction d'un bulletin de paye donné.<br>Argument de format : matricule-mois_début...mois_fin-année_début...année_fin;matricule2-mois_début2...mois_fin2-année_début2...année_fin2;[etc.]</td><td>Dialogue Extraire les bulletins</td><td>&ndash;&ndash;bulletins <i>argument</i></td><td>1ère ligne.</td><td>MatriculeInput::matrLineEdit</td></tr>
/// <tr><td style="font-weight:bold;">MatriculesB</td><td>profondeur="0"</td><td>Matricule(s), mois, année(s) saisi(s) pour extraction d'un bulletin de paye donné.<br> Argument de même format.</td><td>Dialogue Extraire les bulletins</td><td>&ndash;&ndash;bulletins <i>argument</i></td><td>2e ligne.</td><td>MatriculeInput::matrLineEdit2</td></tr>
/// <tr><td style="font-weight:bold;">MatriculesC</td><td>profondeur="0"</td><td>Matricule(s), mois, année(s) saisi(s) pour extraction d'un bulletin de paye donné.<br> Argument de même format.</td><td>Dialogue Extraire les bulletins</td><td>&ndash;&ndash;bulletins <i>argument</i></td><td>3e ligne.</td><td>MatriculeInput::matrLineEdit3</td></tr>
/// <tr><td style="font-weight:bold;">dossierBulletins</td><td>profondeur="0"</td><td>Chemin du dossier d'extraction des bulletins de paye individuels.</td><td>Dialogue Extraire les bulletins</td><td>&ndash;&ndash;dossier-bulletins <i>argument</i></td><td></td><td>MatriculeInput::dossier</td></tr>
/// <tr><td style="font-weight:bold;">quiet</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue de configuration</td><td>-q</td><td>Limiter la verbosité de l'onglet Console.</td><td>MainWindow::defaultQuietBox</td></tr>
/// <tr><td style="font-weight:bold;">baseType</td><td>profondeur="0"</td><td>{"", "AN", "X", "T", "I", "S", "A", "R", "AC", "AV", "IR", "C", "D", "RE"}</td><td>Dialogue d'options &rarr; onglet Format</td><td>-T <i>argument</i><td>Type de base monolithique, par année, tous types, ou par type de ligne de paye (Traitement, Indemnité, SFT, etc.)</td><td>standardPage::baseTypeWidget</td></tr>
/// <tr><td style="font-weight:bold;">maxLigne</td><td>profondeur="0"</td><td>Nombre</td><td>Dialogue d'options &rarr; onglet Format</td><td>-T <i>argument</i><td>Maximum du nombre de lignes de la base Table.csv (lignes de paye)</td><td>standardPage::maxNLigneLineEdit</td></tr>
/// <tr><td style="font-weight:bold;">genererNumLigne</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td>-l</td><td>Numéroter les lignes.</td><td>standardPage::rangCheckBox</td></tr>
/// <tr><td style="font-weight:bold;">exporterEtab</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td>-S</td><td>Exporter les colonnes Siret, Budget, Etablissement, Employeur.</td><td>standardPage::etabCheckBox</td></tr>
/// <tr><td style="font-weight:bold;">exportMode</td><td>profondeur="0"</td><td>{'Standard', 'Cumulative', 'Distributive', 'Distributive+'}</td><td>Dialogue d'options &rarr; onglet Format</td><td>&ndash;&ndash;export <i>argument</i></td><td>Mode d'exportation des bases CSV (voir guide utilisateur).</td><td>standardPage::exportWidget</td></tr>
/// <tr><td style="font-weight:bold;">FPH</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td></td><td>Activer les contrôles réglementaires FPH.</td><td>standardPage::FPHCheckBox</td></tr>
/// <tr><td style="font-weight:bold;">genererTable</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td>-t</td><td>Générer les bulletins et lignes de paye au format CSV.</td><td>standardPage::tableCheckBox</td></tr>
/// <tr><td style="font-weight:bold;">archiveTable</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td></td><td>Archiver les sorties CSV du logiciel dans un format de compression (.arch).</td><td>Pointeur local <i>archiveTableBox</i> vers FCheckBox dans le constructeur standardPage::standardPage()</td></tr>
/// <tr><td style="font-weight:bold;">exportTable</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td></td><td>Exporter les sorties CSV du logiciel vers un répertoire donné.</td><td>Pointeur local <i>exportTableBox</i> vers FCheckBox dans le constructeur standardPage::standardPage()</td></tr>
/// <tr><td style="font-weight:bold;">archiveAll</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td></td><td>Archiver l'ensemble des sorties du logiciel dans un format de compression (.arch).</td><td>Pointeur local <i>archiveAllBox</i> vers FCheckBox dans le constructeur standardPage::standardPage()</td></tr>
/// <tr><td style="font-weight:bold;">exportAll</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td></td><td>Exporter l'ensemble des sorties du logiciel vers un répertoire donné.</td><td>Pointeur local <i>exportAllBox</i> vers FCheckBox dans le constructeur standardPage::standardPage()</td></tr>
/// <tr><td style="font-weight:bold;">archiveXML</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td></td><td>Archiver les données de paye.</td><td>Pointeur local <i>archiveXhlBox</i> vers FCheckBox dans le constructeur standardPage::standardPage()</td></tr>
/// <tr><td style="font-weight:bold;">exportXML</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Format</td><td></td><td>Exporter les données de paye vers un répertoire donné.</td><td>Pointeur local <i>exportXhlBox</i> vers FCheckBox dans le constructeur standardPage::standardPage()</td></tr>
/// <tr><td style="font-weight:bold;">processType</td><td>profondeur="0"</td><td>{'1',...,'11'}</td><td>Dialogue d'options &rarr; onglet Traitement</td><td>-j <i>argument</i></td><td>Utiliser autant de fils d'exécution qu'indiqué dans le menu déroulant.</td><td>processPage::processTypeWidget</td></tr>
/// <tr><td style="font-weight:bold;">log</td><td>profondeur="0"</td><td>Chemin du fichier altair.log</td><td>Dialogue d'options &rarr; onglet Traitement</td><td>-L <i>argument</i></td><td>Chemin du log d’exécution du test de cohérence entre analyseurs C et XML.</td><td>processPage::logFrame</td></tr>
/// <tr><td style="font-weight:bold;">genererLog</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Traitement</td><td></td><td>Autoriser la génération du log.</td><td>processPage::logCheckBox</td></tr>
/// <tr><td style="font-weight:bold;">activerConsole</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Traitement</td><td></td><td>Activer les sorties Console.</td><td>processPage::consoleCheckBox</td></tr>
/// <tr><td style="font-weight:bold;">memoryUse</td><td>profondeur="0"</td><td>Nombre entre 1 et 100</td><td>Dialogue d'options &rarr; onglet Traitement</td><td></td><td>Part maximum de la RAM utilisée.</td><td>processPage::memoryUseWidget</td></tr>
/// <tr><td style="font-weight:bold;">rapportType</td><td>profondeur="0"</td><td>{'PDF', 'WORD et ODT', 'WORD, ODT et PDF'</td><td>Dialogue d'options &rarr; onglet Traitement</td><td></td><td>Activé uniquement si enchainerRapports vaut 'oui'. Rapports d'analyse de type PDF, MS Word (.docx) et Libreoffice Writer (.odt)</td><td>processPage::rapportTypeWidget</td></tr>
/// <tr><td style="font-weight:bold;">enchainerRapports</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Traitement</td><td></td><td>Lancer automatiquement les scripts R depuis l'interface graphique sans utiliser RStudio.</td><td>processPage::enchainerRapports</td></tr>
/// <tr><td style="font-weight:bold;">rapportEntier</td><td>profondeur="0"</td><td>{oui, non}</td><td>Dialogue d'options &rarr; onglet Traitement</td><td></td><td><i>Interne à l'interface graphique. Non exporté vers le fichier de projet Altaïr .alt.</i><br>Version expérimentale (code de développement).</td><td>processPage::rapportEntier</td></tr></table>
/// </div> 
///\n\n
/// \section  sec_alt2 Exemple commenté de projet .alt
/// <table style="width:35%;"><tr><th>Code XML</th><th>Commentaires</th></tr></table>
/// \code
/// <?xml version="1.0"?>                Fichier de type XML.
/// <projet version="17.12-1">           Début du projet et version de l'application.
/// <data>                               Partie "données de paye".
///  <XHL profondeur="2">                Balise XHL : 2 niveaux d'enchâssement ("onglet" et "item"). Données de paye.
///   <onglet V="2011">                  Onglet de valeur ("V") 2011 (année).
///     <item V="1"
///         S="28XXXXXXXXXXXX"
///         B="BUDGET PRINCIPAL"
///         E="ETABLISSEMENT
///         X" EM="EMPLOYEUR Y">
///     /home/fab/Dev/altair/Tests/Exemple/Donnees/xhl/2011/01.xhl
///     </item>
///                                      Fichier de paye 01.xhl dans le répertoire xhl/2011 de rang 1, correspondant au Siret 28..., à un budget principal de l'établissement X de l'employeur Y.
///   </onglet>
///   <onglet V="2012">                  Onglet de valeur ("V") 2012 (année).
///     <item V="1"
///          S="28XXXXXXXXXXXX"
///          B="BUDGET PRINCIPAL"
///          E="ETABLISSEMENT X"
///          EM="EMPLOYEUR Y">
///     /home/fab/Dev/altair/Tests/Exemple/Donnees/xhl/2012/02.xhl
///     </item>
///                                      Fichier de paye 02.xhl dans le répertoire xhl/2012 du mois 1 (janvier), correspondant au Siret 28..., à un budget principal de l'établissement X de l'employeur Y.
///     <item V="1"
///          S="28XXXXXXXXXXXX"
///          B="BUDGET ANNEXE FORMATION"
///          E="ETABLISSEMENT X"
///          EM="EMPLOYEUR Y">
///      /home/fab/Dev/altair/Tests/Exemple/Donnees/xhl/2012/03.xhl
///      </item>
///                                      Fichier de paye 03.xhl dans le répertoire xhl/2012 du mois 1 (janvier), correspondant au Siret 28..., à un budget principal de l'établissement X de l'employeur Y.
///   <onglet V="Siret">                 Onglet des Siret
///     <item V="" S="" B=""
///           E="" EM="">
///       28XXXXXXXXXXXX
///     </item>
///                                      Seul le Siret 28... est référencé.
///   </onglet>
///   <onglet V="Budget">                Onglet des Budgets
///     <item V="" S="" B=""
///           E="" EM="">
///       BUDGET ANNEXE FORMATION
///     </item>
///                                      Détection d'un budget annexe formation.
///     <item V="" S="" B=""
///           E="" EM="">
///       BUDGET PRINCIPAL
///     </item>
///                                      Détection d'un budget principal.
///   </onglet>
///   <onglet V="Employeur">             Onglet des employeurs
///     <item V="" S="" B=""
///           E="" EM="">
///       EMPLOYEUR Y
///     </item>
///                                      Détection du seul employeur Y.
///   </onglet>
///  </XHL>                              Fin des données de paye de l'onglet central
/// </data>
/// <systeme>                            Paramètres système (paramétrage de l'interface, dialogues d'options et de configuration de l'interface)
///  <projectManagerDisplay profondeur="0">  La profondeur 0 indique une balise sans enchâssement : la valeur est lue dans le texte entre <balise> et </balise>
///   oui                                Afficher le gestionnaire de projets à droite de l'interface.
///  </projectManagerDisplay>
///  <fileManagerDisplay profondeur="0">
///   oui                                Afficher le gestionnaire (ou explorateur ou arbre) de fichiers à gauche de l'interface.
///  </fileManagerDisplay>
///  <fullScreenDisplay profondeur="0">
///   non                                Pas de plein écran
///  </fullScreenDisplay>
///  <outputTextEdit profondeur="0">
///   oui                                Afficher l'onglet des messages en bas de l'interface.
///  </outputTextEdit>
///  <fileToolBar profondeur="0">
///   oui                                Afficher la barre d'outils du menu Fichier.
///  </fileToolBar>
///  <editToolBar profondeur="0">
///   oui                                Afficher la barre d'outils du menu Editer.
///  </editToolBar>
///  <processToolBar profondeur="0">
///   oui                                Afficher la barre d'outils du menu Lancer.
///  </processToolBar>
///  <optionsToolBar profondeur="0">
///   oui                                Afficher la barre d'outils du menu Configurer.
///  </optionsToolBar>
///  <aboutToolBar profondeur="0">
///   non                                Afficher la barre d'outils du menu Aide.
///  </aboutToolBar>
///  <saveProjectBehavior profondeur="0">
///   non                                Ne pas enregistrer les projet .alt automatiquement à chaque modification d'un paramètre.
///  </saveProjectBehavior>
///  <importerAuLancement profondeur="0">
///   non                                Ne pas importer les données de paye du répertoire Données (.xhl/.xml) au lancement de l'interface.
///  </importerAuLancement>
///  <loadProjectBehavior profondeur="0">
///   non                                Ne pas charger le projet .alt par défaut au lancement de l'interface.
///  </loadProjectBehavior>
///  <limitConsoleOutput profondeur="0">
///   non                                Ne pas limiter le nombre de lignes du log de l'onglet Console.
///  </limitConsoleOutput>
///  <quiet profondeur="0">
///   oui                                Limiter la verbosité de l'onglet Console.
///  </quiet>
///  <baseType profondeur="0">
///  &nbsp;                              Type de base monolithique.
///  </baseType>
///  <maxLigne profondeur="0">
///   1000000                            Nombre maximum de lignes de la base de paye : 1 million.
///  </maxLigne>
///  <genererNumLigne profondeur="0">
///   non                                Ne pas numéroter les lignes.
///  </genererNumLigne>
///  <exporterEtab profondeur="0">
///   non                                Ne pas exporter les colonnes Siret, Budget, Etablissement, Employeur.
///  </exporterEtab>
///  <exportMode profondeur="0">
///   'Standard'                         Exportation standard (avec nettoyage de la clé et sans distribution).
///  </exportMode>
///  <FPH profondeur="0">
///   non                                Ne pas activer les contrôles réglementaires FPH.
///  </FPH>
///  <genererTable profondeur="0">
///   oui                                Générer les bulletins et lignes de paye au format CSV.
///  </genererTable>
///  <archiveTable profondeur="0">
///   non                                Ne pas archiver les sorties CSV du logiciel dans un format de compression (.arch)
///  </archiveTable>
///  <exportTable profondeur="0">
///   non                                Ne pas exporter les sorties CSV du logiciel vers un répertoire donné.
///  </exportTable>
///  <archiveAll profondeur="0">
///   non                                Ne pas archiver l'ensemble des sorties du logiciel dans un format de compression (.arch)
///  </archiveAll>
///  <exportAll profondeur="0">
///   non                                Ne pas exporter l'ensemble des sorties du logiciel vers un répertoire donné.
///  </exportAll>
///  <archiveXML profondeur="0">
///   non                                Archiver les données de paye.
///  </archiveXML>
///  <exportXML profondeur="0">
///   non                                Exporter les données de paye vers un répertoire donné.
///  </exportXML>
///  <processType profondeur="0">
///   '4'                                Utiliser 4 fils d'exécution.
///  </processType>
///  <log profondeur="0">
///   /home/fab/.local/share/Altair/altair.log   Chemin du log d’exécution du test de cohérence entre analyseurs C et XML.
///  </log>
///  <genererLog profondeur="0">
///   non                                Ne pas générer le log.
///  </genererLog>
///  <activerConsole profondeur="0">
///   oui                                Activer les sorties Console.
///  </activerConsole>
///  <memoryUse profondeur="0">
///   80                                 Utiliser 80 % de la RAM au maximum.
///  </memoryUse>
///  <rapportType profondeur="0">
///   'WORD et ODT'                      Rapports d'analyse de type MS Word (.docx) et Libreoffice Writer (.odt)
///  </rapportType>
///  <enchainerRapports profondeur="0">
///   non                                Ne pas enchaîner les rapports d'analyse à la suite de l'extraction des données.
///  </enchainerRapports>
///  <TRAITEMENT profondeur="0">
///  &nbsp;                              Pas de code de paye Traitement saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </TRAITEMENT>
///  <NBI profondeur="0">
///  &nbsp;                              Pas de code de paye NBI saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </NBI>
///  <PFI profondeur="0">
///  &nbsp;                              Pas de code de paye PFI saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </PFI>
///  <PFR profondeur="0">
///  &nbsp;                              Pas de code de paye PFR saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </PFR>
///  <IPF profondeur="0"
///  &nbsp;                              Pas de code de paye IPF saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </IPF>
///  <PSR profondeur="0">
///  &nbsp;                              Pas de code de paye PSR saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </PSR>
///  <IFTS profondeur="0">
///  &nbsp;                              Pas de code de paye IFTS saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </IFTS>
///  <IAT profondeur="0">
///  &nbsp;                              Pas de code de paye IAT saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </IAT>
///  <IHTS profondeur="0">
///  &nbsp;                              Pas de code de paye IHTS saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </IHTS>
///  <VACATAIRES profondeur="0">
///  &nbsp;                              Pas de code de paye VACATAIRES saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </VACATAIRES>
///  <ASTREINTES profondeur="0">
///  &nbsp;                              Pas de code de paye ASTREINTES saisi manuellement dans le dialogue d'options à l'onglet Codes.
///  </ASTREINTES>
///  <Matricules profondeur="0">
///  &nbsp;                              Pas de matricule saisi pour extraction d'un bulletin de paye donné.
///  </Matricules>
///  <MatriculesB profondeur="0">
///  &nbsp;                              Pas de matricule saisi pour extraction d'un bulletin de paye donné. (2e ligne)
///  </MatriculesB>
///  <MatriculesC profondeur="0">
///  &nbsp;                              Pas de matricule saisi pour extraction d'un bulletin de paye donné. (3e ligne)
///  </MatriculesC>
///  <dossierBulletins profondeur="0">
///   /home/fab/Dev/altair/Tests/Exemple/Donnees/Bulletins   Dossier d'extraction des bulletins de paye individuels.
///  </dossierBulletins>
/// </systeme>                           Fin des paramètres système.
/// </projet>                            Fin du projet.
/// \endcode

