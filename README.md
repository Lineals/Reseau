# **Compte rendu TPs de réseau**


## Socket

## Envoi/reception

```c
void sendMsg(int D_Socket_Com, char *msg){
  send(D_Socket_Com,msg,strlen(msg),0);
  printf("Debug send buffer : %s\n", msg);
  sleep(0.03);
}
char *rcvMsg(int D_Socket_Com){
  char *rcvBuffer=malloc(1024);
  memset(rcvBuffer,0,1024);
  recv(D_Socket_Com,rcvBuffer,1024,0);
  sleep(0.03);
  return rcvBuffer;
}
```

## Fork

```c
for(;;)
{
        D_Socket_Com = accept(D_Socket_Ecoute, (struct sockaddr *) &Struct_Addr_Rec_Cli, &addr_size);

        if ((pid = fork()) == -1)
        {
            close(D_Socket_Com);
            continue;
        }
        else if(pid > 0)
        {
            close(D_Socket_Com);
            printf("Lancement du fils n°%d\n",compteur++);
            continue;
        }
        else if(pid == 0)
        {

          //demande nom
          sendMsg(D_Socket_Com,"Nom ?");
          //recois nom

          struct client monclient= {.nom = rcvMsg(D_Socket_Com), .reponses=malloc(1024*sizeof(int)), .resultat=0};
          readFile(D_Socket_Com);
          break;
        }
}
```
## lecture des questions

```c

void readFile(int D_Socket_Com) {
  char vrai[4]="VRAI\n";
  char faux[4]="FAUX\n";
  FILE *fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  fp = fopen("/home/lineal/Bureau/questions","r");

  if(fp){
    while ((read = getline(&line, &len, fp)) != -1 && read != NULL) {
            int loop=0;
            int index;
            char *token = strtok(line, ",");
            while (token != NULL) {
              if (loop==0){
                printf("Bonne reponse server side %s\n", token);
                index=atoi(token);
                token = strtok(NULL, ",");
              } else {
                sendMsg(D_Socket_Com, token);
                token = strtok(NULL, ",");
              }
              loop++;
            }
            sendMsg(D_Socket_Com,"waiting");
            if(atoi(rcvMsg(D_Socket_Com))==index){
              sendMsg(D_Socket_Com,vrai);
            }
            else{
              sendMsg(D_Socket_Com,faux);
            }
        }
    }
    sendMsg(D_Socket_Com,"exit");

    close(D_Socket_Com);
}
```

Le filtre médian permet **d'éléminer les valeurs extremes** de l'image en attribuant a chaque pixel la valeur de la médiane de la liste de tous ses voisins triés par ordre croissant. Pour cela on va faire un fonction qui prend un paramètre N étant la racine du nombre de voisins que l'on va prendre en compte pour calculer la valeur d'un pixel.
Voici un extrait du coeur de la fonction, c'est a dire l'opération que l'on effectue pour chaque pixel:
