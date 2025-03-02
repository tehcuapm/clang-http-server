#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(void) {

  struct sockaddr_in server_addr = {
    .sin_family = AF_INET,
    .sin_port   = htons(8080),
    .sin_addr   = { INADDR_ANY },
    .sin_zero   = { 0 }
  };

  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  };

  // On attend une requête avant de renvoyer ECONNREFUSED
  if (listen(sockfd, 1) < 0) {
    perror("listen failed");
    exit(EXIT_FAILURE);
  };

  int client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
  if (client_fd == -1) {
    perror("Erreur lors de l'acceptation (accept)");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  // A améliorer
  char request[1000];
  int bytes_received = recv(client_fd, request, 1000, 0);
  if (bytes_received < 0) {
    perror("recv");
    exit(EXIT_FAILURE);
  };

  request[bytes_received] = '\0';

  char method[10];
  char uri[50];
  char version[20];

  if (sscanf(request, "%s %s %s", method, uri, version) < 3) {
    printf("Erreur lors du parsing.\n");
  }

  printf("Connexion acceptée de %s:%d\nMethod: %s\nUri: %s\nVersion: %s\n",
        inet_ntoa(client_addr.sin_addr),
        ntohs(client_addr.sin_port),
        method, uri, version);

  char *response = "HTTP/1.1 200 OK\n\nContent-type: text/html; charset=utf-8\r\n\r\n<html><body><h1>HELLO</h1></body></html>";
  ssize_t bytes_sent = send(client_fd, response, strlen(response), 0);
  if (bytes_sent < 0) {
    perror("Erreur lors de l'envoi de la réponse");
    close(client_fd);
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  close(client_fd);
  close(sockfd);

  return 0;
}