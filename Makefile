# ########################################## #

######  #     #   ###     #       #     ######
#       ##    #  #   #                  #
#       # #   #   #      ##      ##     #
#####   #  #  #    #      #       #     #####
#       #   # #     #     #       #     #
#       #    ##  #   #    #       #     #
######  #     #   ###   #####   #####   ######

# ########################################## #

# Makefile minimaliste pour le TP IPI sur la manipulation d’images
# Pierre Tellier, ENSiiE, octobre 2017
all: Serveurv1 Clientv1

Serveurv1:
	gcc -o server Serveurv1.c -g

Clientv1:
	gcc -o client Clientv1.c -g
