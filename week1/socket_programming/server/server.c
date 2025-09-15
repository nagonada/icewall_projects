#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct sock{
  sa_family_t sun_family;
  char sun_path[128];
}sockaddr_un;

int main(){
  setvbuf(stdout, NULL, _IOLBF, 0);
  int socket_fd, binded, acc, addr_size;
  char* data = "console.log(\"icewall\");\n";
  char recv_data[128];
  unlink("/tmp/sock");
  socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (socket_fd == -1){
    printf("소켓 생성 실패");
    fflush(stdout);
    return -1;
  }

  struct sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, "/tmp/sock");
  addr_size = sizeof(addr);
  binded = bind(socket_fd, (const struct sockaddr *)&addr, addr_size);
  if(binded == -1){
    printf("소켓 bind 실패\n");
    fflush(stdout);
    perror("bind");
    return -1;
  }

  if(listen(socket_fd, 10) == -1){
    printf("listen 전환 실패\n");
    fflush(stdout);
    return -1;
  }

  fprintf(stderr, "accept 시도\n");
  fflush(stdout);
  while(1){
    acc = accept(socket_fd, (struct sockaddr *)&addr, &addr_size);
    if (acc == -1){
      printf("accept 실패\n");
      fflush(stdout);
      continue;
    }else{
      fprintf(stderr, "accept 성공\n");
      fflush(stdout);
      send(acc, data, strlen(data)+1, 0);
      recv(acc, recv_data, 128, 0);
      fprintf(stderr, "%s\n", recv_data);
      fflush(stdout);
      close(acc);
    }
  }
