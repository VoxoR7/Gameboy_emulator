# VoxoR Gameboy emulator

## Avant de commencer

Récuperer les roms GameBoy voulu et les placer dans le dossier /rom. 
L'émulateur se chargera de récuperer tous les fichiers portant l'extension .gb dans le dossier /rom et les sous-dossier, puis les proposera pour être chargé dans l'émulateur.
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
enfin la touche BACKSPACE représente la touche ENTER de la gameboy et ENTER représente la touche SELECT d'une gameboy.

## Système d'exploitation

- [x] Windows
- [ ] Linux
- [ ] MacOs

## Travaux future

- [x] Gestion du temps réel (avoir 59,7 fps comme sur une vrai Gameboy)
- [ ] ROM Tetris
  - [x] Jouer à Tetris
  - [ ]  Encore des bugs à corriger comme des crash de l'émulateur ou un mauvais affichage du niveau...
- [ ] Vérifier les instructions et finir d'implémenter les instructions
- [ ] Autres Roms
- [ ] Gestion de la (vrai) musique
- [ ] Linux support