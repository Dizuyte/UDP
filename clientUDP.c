/********************************************************
 * * nom : clientUDP.c
 * * Auteur : Milan Galvani / NetCo
 * * version : 0.2
 * * descr : Client DAYTIME en mode non connecté
 * * licence : GPL

     Compile using : gcc clientUDP.c -o clientUDP
*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s hostname port\n", argv[0]);
        return 1;
    }

    // Déclaration des variables
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char buffer[MAX_BUFFER_SIZE];

    // Création de la socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Erreur lors de la création de la socket");
        return 1;
    }

    // Résolution du nom de domaine
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "Erreur lors de la résolution du nom de domaine\n");
        return 1;
    }

    // Initialisation de la structure sockaddr_in
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr = *((struct in_addr *)server->h_addr);

    // Envoi de données au serveur
    const char *message = "Hello, serveur!";
    sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Attente de la réponse du serveur
    int bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
    if (bytes_received < 0) {
        perror("Erreur lors de la réception des données");
        close(sockfd);
        return 1;
    }

    // Affichage de la réponse du serveur
    printf("Réponse du serveur : %.*s\n", bytes_received, buffer);

    // Fermeture de la socket
    close(sockfd);

    return 0;
}
