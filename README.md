# DroneCloud

L'objectif du projet est de créer un cloud de drône qui ne depend que de lui meme pour vivre et pas du sol. Qui peut interagir avec d'autres unités, prendre des decisions, réaliser un objectif seul.


## Objectif 1:

Faire communiquer 2 programmes sur 2 terminals différents.

### Solution :

- Créer 2 drones fictifs (id, position aléatoire)
- Ajouter une api de communication (envoyer sa position / recevoir une position)
- Implementer l api de com pour chaque drone

#### Protocole v0.1

Transport : UDP sur localhost.

Chaque drone envoie toutes les 5 secondes un message de statut à l'autre drone.

Format du message :

`STATUS;<id>;<x>;<y>;<timestamp>`

Règles :
- un message commence toujours par un type ;
- pour la v1, le seul type supporté est `STATUS` ;
- `id` identifie le drone émetteur ;
- `x` et `y` représentent une position simulée ;
- `timestamp` représente le moment d'envoi ;
- si un drone ne reçoit plus de message depuis 10 secondes, il considère l'autre drone comme perdu.


### Test

Un programme dans 2 terminals différents chaque drone. Chaque drone imprime dans le terminal toutes les 5 secondes la position de l autre drone.

