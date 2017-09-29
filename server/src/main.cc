#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>

const unsigned port = 2500;

struct {
  int socket;
  struct sockaddr_in addr;
} server;

struct {
  socklen_t len;
  int socket;
  struct sockaddr_in addr;
} client;

char buffer[256];

int main(int argc, char *argv[]) {

  server.socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server.socket < 0) {
    fprintf(stderr, "FAILED TO CREATE SOCKET\n");
    return -1;
  }

  int enable = 1;
  if (setsockopt(server.socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
    fprintf(stderr, "FAILED TO REUSE SOCKET\n");
  }

  bzero((char*)&server.addr, sizeof(server.addr));

  server.addr.sin_family = AF_INET;
  server.addr.sin_addr.s_addr = INADDR_ANY;
  server.addr.sin_port = htons(port);

  if (bind(server.socket, (struct sockaddr*)&server.addr, sizeof(server.addr)) < 0) {
    fprintf(stderr, "FAILED TO BIND\n");
    return -1;
  }

  listen(server.socket, 5);
  client.len = sizeof(client.addr);

  printf("WAIRING FOR CONNECTION\n");
  client.socket = accept(server.socket, (struct sockaddr *)&client.addr, &client.len);
  printf("CLIENT CONNECTED\n");
  
  if (client.socket < 0) {
    fprintf(stderr, "FAILED TO ACCEPT\n");
    return -1;
  }

  bzero(buffer, 256);
  
  printf("WAITING FOR CLIENT TO SEND DATA\n");
  if (recv(client.socket, buffer, 256, 0) < 0) {
    fprintf(stderr, "RECV FAILED\n");
  }

  bzero(buffer, 256);
  sprintf(buffer, "SENDING DATA");

  printf("SENDING DATA TO CLIENT\n");
  if (send(client.socket, buffer, 256, 0) < 0) {
    fprintf(stderr, "SEND FAILED\n");
  }

  return 0;
}
