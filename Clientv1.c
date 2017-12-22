#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

void sendMsg(int D_Socket_Com, char *msg);

char *rcvMsg(int D_Socket_Com);

int main(){
  int D_Socket_Client;
  char buffer[1024];
  struct sockaddr_in Struct_Addr_Serv;
  socklen_t addr_size;
  int loop = 0;
  D_Socket_Client = socket(PF_INET, SOCK_STREAM, 0);
  Struct_Addr_Serv.sin_family = AF_INET;
  Struct_Addr_Serv.sin_port = htons(7891);
  Struct_Addr_Serv.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(Struct_Addr_Serv.sin_zero, '\0', sizeof (Struct_Addr_Serv).sin_zero);
  addr_size = sizeof(Struct_Addr_Serv);

  connect(D_Socket_Client, (struct sockaddr *) &Struct_Addr_Serv, addr_size);

  //recois demande nom
  printf("%s\n",rcvMsg(D_Socket_Client));

  //envoi nom
  memset(buffer, 0, 1024);
  scanf("%s",buffer );
  sendMsg(D_Socket_Client,buffer);

  while (1){

    char *incoming=malloc(1024);
    memset(incoming,0,1024);
    //recois question/debut/fin
    incoming=rcvMsg(D_Socket_Client);
    if (strcmp(incoming,"exit")==0){
      break;
    }
    else if (strcmp(incoming,"waiting")==0){
      printf("Reponse n° : ");
      memset(incoming, 0, 1024);
      scanf("%s",incoming);
      sendMsg(D_Socket_Client,incoming);
    } else {
      printf("%s\n",incoming);
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
}

char *rcvMsg(int D_Socket_Com){
  char *rcvBuffer=malloc(1024);
  memset(rcvBuffer,0,1024);
  recv(D_Socket_Com,rcvBuffer,1024,0);
  return rcvBuffer;
}
