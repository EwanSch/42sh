# 42sh

Projet Epitech consistant à recréer un shell Unix inspiré de **tcsh**.

## Prérequis

* GCC
* Make
* Linux / WSL recommandé

## Récupération du projet

```bash
git clone <url-du-repo>
cd 42sh
```

## Compilation

```bash
make
```

Le binaire `42sh` est généré à la racine du projet.

## Lancer le shell

```bash
./42sh
```

## Exécuter les commandes souhaiter

```bash
./42sh 'commandes'
```

## Nettoyage

```bash
make clean
make fclean
```

## Recompiler entièrement

```bash
make re
```

## Fonctionnalités

* Exécution de commandes
* Builtins (`cd`, `env`, `setenv`, `unsetenv`, `exit`, ...)
* Pipes (`|`)
* Redirections (`>`, `>>`, `<`, `<<`)
* Opérateurs logiques (`&&`, `||`)
* Gestion des variables d'environnement
* Historique
* Alias
* Parsing avancé

## Structure du projet

```text
src/
├── builtins/
```

## Tests

```bash
make tests_run
```

## Auteurs
EwanSch
AisakaPMD
coco-epi05
BenjaminHess99
romainmnieto-maker


Projet réalisé dans le cadre du module PSU d'Epitech.
