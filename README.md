# exploradome_gigowatt

## Prérequis

Pour fonctionner le code n'a besoin des éléments suivants :
* Fichier **pitches.h** qui comprend les définitions des diverses notes de musiques
* Librairie **StringSplitter.h** qui permet la gestion de découper une chaine de caratère

> Pour que la librairie StringSplitter puisse fonctionner correctement, il faut modifier le fichier StringSplitter.h.
> La valeur à modifier est *static const unsigned int MAX* qu'il faut mettre à 15
> Cette valeur correspond au nombre maximum de split pouvant être réaliser par la class. Dans notre cas de figure nous pouvons aller jusqu'à 15

## Description du code

### Organisation des PIN

Dans ce code seul les PIN analogique sont utilisés et sont regroupé en 6 catégories:
* 2 - 16 : Controle des lumières pour les bouttons métier
* 17 - 21 : Controle des lumières pour les bouttons action
* 22 - 36 : Controle de la pression pour les bouttons métier
* 37 - 41 : Controle de la pression pour les bouttons action
* 42 : Haut parleur

| PIN ID | TYPE   | GROUPE      |
|:------:|:------:|:-----------:|
| 0      | n/a    | n/a         |
| 1      | n/a    | n/a         |
| 2      | OUTPUT | WorkLight   | 
| 3      | OUTPUT | WorkLight   |
| 4      | OUTPUT | WorkLight   |
| 5      | OUTPUT | WorkLight   |
| 6      | OUTPUT | WorkLight   |
| 7      | OUTPUT | WorkLight   |
| 8      | OUTPUT | WorkLight   |
| 9      | OUTPUT | WorkLight   |
| 10     | OUTPUT | WorkLight   |
| 11     | OUTPUT | WorkLight   |
| 12     | OUTPUT | WorkLight   |
| 13     | OUTPUT | WorkLight   |
| 14     | OUTPUT | WorkLight   |
| 15     | OUTPUT | WorkLight   |
| 16     | OUTPUT | WorkLight   |
| 17     | OUTPUT | ActionLight |
| 18     | OUTPUT | ActionLight |
| 19     | OUTPUT | ActionLight |
| 20     | OUTPUT | ActionLight |
| 21     | OUTPUT | ActionLight |
| 22     | INPUT  | WorkPush    |
| 23     | INPUT  | WorkPush    |
| 24     | INPUT  | WorkPush    |
| 25     | INPUT  | WorkPush    |
| 26     | INPUT  | WorkPush    |
| 27     | INPUT  | WorkPush    |
| 28     | INPUT  | WorkPush    |
| 29     | INPUT  | WorkPush    |
| 30     | INPUT  | WorkPush    |
| 31     | INPUT  | WorkPush    |
| 32     | INPUT  | WorkPush    |
| 33     | INPUT  | WorkPush    |
| 34     | INPUT  | WorkPush    |
| 35     | INPUT  | WorkPush    |
| 36     | INPUT  | WorkPush    |
| 37     | INPUT  | ActionPush  |
| 38     | INPUT  | ActionPush  |
| 39     | INPUT  | ActionPush  |
| 40     | INPUT  | ActionPush  |
| 41     | INPUT  | ActionPush  |
| 42     | OUTPUT | SPEAKER     |


### Schéma de branchement

### Variables

#### PIN variables
Le code comprend un ensemble de variable servant divers but. Toutes les variables débutant par le terme **pin** sont des entier  correspondent à un identifiant de PIN sur la carte Arduino mega.

#### push2Light variable
La variable push2Light est un tableau contenant une correspondance entre les PIN de type Push et les PIN de type Light qui doivent être activé en cas d'appuie. Il s'agit là du coeur de l'application qui assure la cohérence des actions réalisé par l'utilisateur.

L'index du tableau correspond à l'identifiant de la PIN sur la carte Arduino mega
Les valeurs correspondent à une liste d'identifiant devant être allumé en cas de pression. Ces valeurs sont séparé par un underscore "_", c'est la raison pour laquelle nous devons inclure la librairie **StringSplitter**

#### DEBUG variable
Cette variable de type boolean sert à afficher dans le terminal les informations complémentaire pour comprendre l'état du programme

#### pitches constant
Le fichier pitches.h contient les définitions de l'ensemble des notes de musiques disponibles nativement via l'Arduino Mega. Seulement 5 valeurs sont utilisées pour ce programme.

### Méthodes

Un programme Arduino est principalement constitué de 2 méthodes **setup()** et **loop()**. En complément de ces fonctions de références nous avons instancier plusieurs autres dont voici la liste :

* [void] **ring()** : Envoye un signal de modulation vers un haut parleur afin de faire sonner la note passé en paramètre.
* [void] **initInputLight()** : Initialise l'ensemble des PIN des 2 groupes de controle de la lumière (PIN 2 à 21)
* [void] **initInputButton()** : Initialise l'ensemble des PIN des 2 groupes de controle de la pression (PIN 22 à 41) 
* [void] **getButtonPushed()** : Récupére l'identifiant de la PIN du premier boutton pressé
* [void] **resetButtonState()** : Réinitialise l'ensemble des états des 2 groupes de controle de la pression (PIN 22 à 41)
* [void] **initLightsOn()** : Active les lumières de l'ensemble des 2 groupes de controle de la lumière (PIN 2 à 21)



#### Methode setup()

Dans cette méthode nous appelons simplement les méthodes **initInputLight()** et **iniInputButton()** pour initialiser l'ensemble des PIN.


#### Methode loop()

Dans cette méthode nous débuttons par allumer l'ensemble des lumières via la méthode **initLightOn()**

Ensuite nous récupérons l'identifiant du boutton poussé s'il y en à un, via la méthode **getButtonPushed()**.
Si aucun boutton n'est poussé nous terminons la boucle courrante.

Si un button est poussé nous entrons dans la condition if (~ligne 180).
Dans cette section nous débuttons par éteindre la lumière associé au boutton préssé. Pour chaque PIN de type Push il y a une PIN de type Light dont l'identifiant correspond à la formule ci-dessous :

>
> ID light  = ID push - 20
>

A présent nous récupérons la liste de toutes les lumières qui doivent être allumés suite à la pression du boutton. Pour cela on appel la variable push2Light avec l'identifiant du boutton préssé. La valeur récupéré étant une liste nous allons la séparer via la classe StringSplitter pour ainsi pouvoir itérrer sur chaque valeur.

Nous allons maintenant parcourir l'ensemble des valeurs récupéré précédement. Pour chaque valeur nous allons allumer la lumière associé puis faire sonner une note si l'identifiant courant correspond à l'un des bouttons Action.

Une fois l'ensemble des iterrations terminées, nous temporisons 500ms afin d'éviter une fréquence de boucle trop élevé.

Enfin nous réinitialisons l'état de l'ensemble des bouttons pressions.

