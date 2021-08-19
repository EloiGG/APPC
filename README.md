# APPC
 Contrôle APPC via ordinateur/réseau

## Installation simple (pour utilisation)

Télécharger et lancer l'installateur correspondant à la plateforme de l'ordinateur depuis le repos APPC_INSTALL
(https://github.com/EloiGG/APPC_INSTALL). Des instructions supplémentaires seront diponibles sur le repos. 

## Installation pour développement

 - Etape 1 : télécharger JUCE (https://juce.com/get-juce/download)
 - Etape 2 : déplacer le dossier JUCE dans le dossier de votre choix
 - Etape 3 : dans ce dossier, lancer Projucer.exe
 - Etape 4 : dans Projucer : File>Global Paths... renseigner le chemin d'accès au dossier JUCE
 - Etape 5 : coller le fichier "SEVEN SEGMENT.ttf" dans le dossier C:\Windows\Fonts
 - Etape 6 (pour Windows) : télécharger et installer Visual Studio, intaller le module "développement desktop C++" dans Visual Studio Installer
 
### Ouvrir le projet

Depuis Projucer : File>open>"APPC.jucer"
Le projet peut maintenant être généré sur différentes plateformes en cliquant sur l'icone en haut à droite.
Il s'ouvre ensuite dans l'IDE et peut être compilé ou modifié depuis ce dernier

### Conditions de test

- Windows 10
- Visual Studio 2019 Community Edition
- JUCE v6.0.8
- Asus ExpertBook P2451FA-EK0029R

## Edition de l'installateur

L'installateur a été créé avec le logiciel InstallForge (https://installforge.net/download/). Pour éditer l'installateur,
ouvrir InstallForge et ouvrir les projets "InstallForgeProject.ifp" ou "InstallForgeProject_32.ifp".
Attention : dans installforge les chemins d'accès sont absolus. Pour rebuild l'installateur depuis une autre machine il faudra
donc redéfinir les chemins d'accès (dans "Files", "Commands" et "Build").