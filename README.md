# **Compte rendu TPs de réseau**

Auteurs : Mollandin Rémi, Peuckert Théo

## Socket
Pour le socket, nous avons principalement repris le squelette du cours et l'avons complété. Ainsi, on peu échanger des packets de données entre le server et le client.
## Envoi/reception

Afin de structurer et d'améliorer la ***lisibilité et l'efficacité*** du code, nous avons crée des ***fonctions envoi et reception***.

La fonction **sendMsg** prent en paramètre ***le socket et la chaîne de charactères*** que l'on souhaite envoyer. Il envoie ensuite ce message au travers du socket.
```c
void sendMsg(int D_Socket_Com, char *msg){
  send(D_Socket_Com,msg,strlen(msg),0);
  printf("Debug send buffer : %s\n", msg);
  sleep(0.03);
}
```
La fonction **rcvMsg** est ***bloquante*** : une fois appelée elle ***attends la reçeption d'un message*** puis le renvois sous forme de chaîne de charactères.
```c
char *rcvMsg(int D_Socket_Com){
  char *rcvBuffer=malloc(1024);
  memset(rcvBuffer,0,1024);
  recv(D_Socket_Com,rcvBuffer,1024,0);
  sleep(0.03);
  return rcvBuffer;
}
```

## Fork

Dès lors qu'un client se connecte au server, le serveur va **déléguer la tâche de gérer ce client en créant un fils**. Ce fils va ensuite prendre en charge le client et donc permettre au server de continuer à pouvoir receptionner de nouveaux clients.

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

Enfin, pour faire un questionnaire nous avons opté pour un **fichier comportant les questions**. Cela permet de pouvoir changer et/ou rajouter des questions sans pour autant avoir besoins de recompiler le code et augment la lisibilité.

Le server va donc pouvoir **envoyer les questions et vérifier la réponse** du client grâce aux données présentes dans le fichier.
```c
void readFile(int D_Socket_Com) {
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
              printf("Tringered envoi VRAI \n");
              sendMsg(D_Socket_Com,"VRAI");
            }
            else{
              sendMsg(D_Socket_Com,"FAUX");
            }
        }
    }
```
## Conclusion

Nous avons donc choisis de faire ce tp à 2 car cela nous a permis de s'aider l'un l'autre et de cumuler les compétences que nous avions chacun acquises et ainsi s'apprendre de nouveaux concepts mutuellement.

Le sujet était intéressant et l'incitation à la prise d'initiative et la liberté nous ont beaucoup plus. Nous avons aussi pu plus en accomplir en travaillant à deux.
