#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void) {

  int sockclt = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_addr = {
    .sin_family = AF_INET,
    .sin_port   = htons(8080),
    .sin_addr.s_addr = inet_addr("127.0.0.1"),
    .sin_zero   = { 0 }
  };

  socklen_t server_addr_len = sizeof(server_addr);

  if (connect(sockclt, (struct sockaddr *)&server_addr, server_addr_len) < 0) {
    perror("connect");
    exit(EXIT_FAILURE);
  };

  if (write(sockclt, "hello", 5) < 0) {
    perror("write");
    exit(EXIT_FAILURE);
  };

  char buffer[100];
  int bytes_received = recv(sockclt, buffer, 100, 0);
  if (bytes_received < 0) {
    perror("recv");
    exit(EXIT_FAILURE);
  }
  buffer[bytes_received] = '\0';
  printf("%s\n", buffer);
  close(sockclt);

  return 0;
}