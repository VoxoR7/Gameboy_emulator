# VoxoR Gameboy Emulator

## Prérequis (installation)

### Windows

Aucun prérequis. Le jeu est prêt à être lancé !

### Linux

Testé sur :

- [x] Ubuntu 20.04.3

3 librairies supplémentaire sont nécessaires :

- libmodplug1
- libfluidsynth2
- libopusfile0

(~1Mb pour les 3 libraries)

Afin d'installer ces 3 libraries, ouvrir un terminal dans le dossier parent puis taper les commandes suivantes :

```
sudo chmod 500 ./install.sh
sudo ./install.sh
```

### MacOs

Non testé.

## Avant de commencer

Récuperer les roms GameBoy voulu et les placer dans le dossier /rom. 
L'émulateur se chargera de récuperer tous les fichiers portant l'extension .gb (en minuscule) dans le dossier /rom et les sous-dossier, puis les proposera pour être chargé dans l'émulateur.
Il est important de ne pas mettre trop de fichier et de dossier dans le dossier /rom.

## Démarrer l'émulateur

Pour démarrer, double cliquer sur l'éxécutable (le seul fichier avec l'extension .exe) dans le dossier /bin.
Sur l'écran d'acceuil, deux options sont présente :
Le volume de la musique peut-être ajuster avec les touches Z (augmenter le volume) et S (diminuer le volume)
La rom voulu peut-être choisis avec les touches D (rom suivante) et Q (rom précédente)
Une fois les options choisis, appuyer sur ENTER pour émuler le fichier choisis.

## En jeu

Une fois dans le jeu, les touches Z, Q, S et D représente les fléches directionnelles,
les touches P et L représente respectivement le bouton "A" et "B" d'une gameboy,
enfin la touche ENTER représente la touche ENTER de la gameboy et BACKSPACE représente la touche SELECT d'une gameboy.

## Travaux future

- [ ] ROM Tetris
  - [x] Jouer à Tetris
  - [ ] La majorité des bugs génant sont corrigé ! quelques bugs subsiste encore.
- [ ] Vérifier les instructions et finir d'implémenter les instructions
- [ ] Autres Roms
- [ ] Gestion de la (vrai) musique