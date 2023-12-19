/********************************************************
 * * nom : serverDaytime.c
 * * Auteur : OpenAI (adapté par ChatGPT)
 * * version : 0.2
 * * descr : Serveur DAYTIME en mode non connecté
 * * licence : GPL
*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        return 1;
    }

    // Déclaration des variables
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE];
    time_t rawtime;
    struct tm *timeinfo;

    // Création de la socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("Erreur lors de la création de la socket");
        return 1;
    }

    // Initialisation de la structure sockaddr_in
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Liaison de la socket à l'adresse et au port spécifiés
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur lors de la liaison de la socket");
        close(sockfd);
        return 1;
    }

    printf("Serveur DAYTIME en écoute sur le port %s\n", argv[1]);

    // Boucle principale du serveur
    while (1) {
        // Attente de la réception d'un datagramme
        int bytes_received = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (bytes_received < 0) {
            perror("Erreur lors de la réception des données");
            continue;  // Revenir à l'attente d'un nouveau datagramme
        }

        // Obtenir la date et l'heure actuelles
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        // Formater la date et l'heure
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

        // Envoyer la réponse au client en utilisant l'adresse du client
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&client_addr, client_addr_len) < 0) {
            perror("Erreur lors de l'envoi de la réponse");
        }
    }

    // Fermeture de la socket (cette partie ne sera jamais atteinte dans cet exemple)
    close(sockfd);

    return 0;
}
