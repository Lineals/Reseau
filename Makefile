# ########################################## #

######  #     #   ###     #       #     ######
#       ##    #  #   #                  #
#       # #   #   #      ##      ##     #
#####   #  #  #    #      #       #     #####
#       #   # #     #     #       #     #
#       #    ##  #   #    #       #     #
######  #     #   ###   #####   #####   ######

# ########################################## #

# Makefile minimaliste pour le TP Réseau
# Pas Pierre Tellier, ENSiiE, Decembre 2017
all: Serveurv1 Clientv1

Serveurv1:
	gcc -o server Serveurv1.c -g

Clientv1:
	gcc -o client Clientv1.c -g
