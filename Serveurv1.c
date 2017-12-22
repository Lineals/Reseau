#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

struct client{
  char* nom;
  int* reponses;
  int resultat;
};
struct question{
  char question[100];
  char* reponse1;
  char* reponse2;
  char* reponse3;
  char* reponse4;
  int reponse;
};
void Questionnaire(struct client monclient , int D_Socket_Com);

void sendMsg(int D_Socket_Com, char *msg);

char *rcvMsg(int D_Socket_Com);

int main(){

  int D_Socket_Ecoute, D_Socket_Com;
  char buffer[1024];
  struct sockaddr_in Struct_Addr_Serv;
  struct sockaddr_in Struct_Addr_Rec_Cli;
  socklen_t addr_size;

  D_Socket_Ecoute  = socket(PF_INET, SOCK_STREAM, 0);
  Struct_Addr_Serv.sin_family = AF_INET;
  Struct_Addr_Serv.sin_port = htons(7891);
  Struct_Addr_Serv.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(Struct_Addr_Serv.sin_zero, '\0', sizeof(Struct_Addr_Serv.sin_zero));

  int yes = 1;
    if(setsockopt(D_Socket_Ecoute, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      printf("Error setsockopt");
      return 0;
    }

  bind(D_Socket_Ecoute, (struct sockaddr *) &Struct_Addr_Serv, sizeof(Struct_Addr_Serv));

  if(listen(D_Socket_Ecoute,5)==0) printf("Listening\n");
  else printf("Error\n");

  addr_size = sizeof Struct_Addr_Rec_Cli;
  int pid;
  int compteur=1;
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

  return 0;
}

void sendMsg(int D_Socket_Com, char *msg){
  char sendBuffer[1024];
  memset(sendBuffer,0,1024);
  strcpy(sendBuffer,msg);
  send(D_Socket_Com,sendBuffer,strlen(sendBuffer),0);
  printf("Debug send buffer : %s\n", sendBuffer);
  sleep(0.03);
}

char *rcvMsg(int D_Socket_Com){
  char *rcvBuffer=malloc(1024);
  memset(rcvBuffer,0,1024);
  recv(D_Socket_Com,rcvBuffer,1024,0);
  sleep(0.03);
  return rcvBuffer;
}


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
            char index;
            char *token = strtok(line, ",");
            while (token != NULL) {
              if (loop==0){
                printf("Bonne reponse server side %s\n", token);
                index=token;
                token = strtok(NULL, ",");
              } else {
                sendMsg(D_Socket_Com, token);
                token = strtok(NULL, ",");
              }
              loop++;
            }
            sendMsg(D_Socket_Com,"waiting");
            if(strcmp(rcvMsg(D_Socket_Com),index)==0){sendMsg(D_Socket_Com,vrai);}
            else{ sendMsg(D_Socket_Com,faux);}
        }
    }
    sendMsg(D_Socket_Com,"exit");

    close(D_Socket_Com);
}

void Questionnaire(struct client monclient, int D_Socket_Com){
  char vrai[4]="VRAI\n";
  char faux[4]="FAUX\n";
  sendMsg(D_Socket_Com,"Ringo starr est \n1) -le batteur des beatles \n2) -le chancelier allemand en 1935");
  if(rcvMsg(D_Socket_Com)=="1"){sendMsg(D_Socket_Com,vrai);}
  else{ sendMsg(D_Socket_Com,faux);}
  sendMsg(D_Socket_Com,"Emilio est un \n1) -Ninja \n2) -Homme avec bien trop d'énergie");
  if(rcvMsg(D_Socket_Com)=="2"){sendMsg(D_Socket_Com,vrai);}
  else{ sendMsg(D_Socket_Com,faux);}
  sendMsg(D_Socket_Com,"Pourquoi \n1) -Oui \n2) -Non");
  if(rcvMsg(D_Socket_Com)=="2"){sendMsg(D_Socket_Com,vrai);}
  else{ sendMsg(D_Socket_Com,faux);}

  //envoie fin
  sendMsg(D_Socket_Com,"exit");

  close(D_Socket_Com);
}
