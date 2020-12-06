# Projet 1 LINFO1252
## Structure du dossier
Ce dossier contient tous les fichiers réalisés par Amine Mounzer (69161800) et Andru Onciul (69721800) pour le projet 1 de Systèmes informatiques (LINFO1252).
Le projet est divisé en deux parties disctinctes, repris dans les dossiers "part1" et "part2". Les fichiers hors de ces dossiers sont: le Makefile pour lancer les test, les evaluations.sh qui effectuent les tests (bash) et le plot.py qui réalise les graphes (python).
Notre dossier contient :
* Le dossier part1 contient nos 3 algorithmes multi-thread compilable et exécutable: philosophe, reader-consumer, producer-consumer.

* Le dossier part2 contient: les algorithmes test and set et test and test and set compilables et exécutables, les 3 algortihmes multi-thread modifiés pour fonctionner avec notre propre interface sémpahores et un fichier .h permettant d'importer notre interface sémpahore dans tous nos fichiers.

* Le dossier perf_results_part1 contient les fichiers csv dans lesquels les résultats des tests de performances des algorithmes de la partie 1 sont stockés

* Le dossier perf_results_part2 contient les fichiers csv dans lesquels les résultats des tests de performances des algorithmes de la partie 2 sont stockés

* Le dossier plots dans lequel tous nos plot crées par le script python sont sauvegardés

## Note
L'abréviation 'tas' sera utilsée dans nos noms de variables/dossiers/fichiers pour signifier que c'est des valeurs obtenues grâce au test and set (et double_tas = test and test and set)

## Instructions pour le makefile
Voici les instructions pour compiler et exécuter les tests via le Makefile:
* l'instruction ``` make ``` nettoie les fichiers de tous les fichiers .o et .csv, ensuite il compile tous les algortihmes (philo, prod-cons, read-write, test-and-set, test-and-test-and-set, philo-tas, prod-cons-tas, read-write-tas)

* l'instruction ``` make test ``` permet de lancer les fichiers "evaluation.sh" qui vont lancer tous les tests (5 mesures de perfs pour chaque configuration de 1 à N thread) sur les algo des deux différentes parties

* l'instruction ``` make clean ``` permet de nettoyer simplement les fichiers .o du dossier

* l'instruction ``` make clean_all ``` permet de nettoyer les fichiers .o et .csv du dossier (les csv qui se trouvent dans les dossiers de perf)
