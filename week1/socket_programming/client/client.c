#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sock{
  sa_family_t sun_family;
  char sun_path[108];
}sockaddr_un;

int main(){
  int client, connection, addr_len;
  char recv_data[128];

  client = socket(AF_UNIX, SOCK_STREAM, 0);
  if (client == -1){
    printf("소켓 생성 실패\n");
    return -1;
  }

  struct sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, "/tmp/sock");
  addr_len = sizeof(addr);


  while(1){
    connection = connect(client, (const struct sockaddr *)&addr, addr_len);
    if (connection == -1){
      printf("연결 실패\n");
      perror("connect");
      sleep(3);
      continue;
    }else{
      recv(client, recv_data, 128, 0);
      printf("%s", recv_data);
      send(client, "icewall", strlen("icewall"), 0);
      break;
    }
  }
  close(client);
}