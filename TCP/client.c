#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024


void displayMenu() {
    printf("\nMenu des services:\n");
    printf("1. Obtenir la date et l'heure du serveur\n");
    printf("2. Obtenir la liste des fichiers d'un répertoire\n");
    printf("3. Obtenir le contenu d'un fichier\n");
    printf("4. Obtenir la durée de la connexion\n");
    printf("5. Terminer la session\n");
    printf("Choisissez un service: ");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <serveur_address> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *serveur_address = argv[1];
    const int port = atoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Erreur lors de la création de la socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serveur_addr;
    serveur_addr.sin_family = AF_INET;
    serveur_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, serveur_address, &serveur_addr.sin_addr) <= 0) {
        perror("Erreur lors de la conversion de l'adresse");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&serveur_addr, sizeof(serveur_addr)) == -1) {
        perror("Erreur lors de la connexion au serveur");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    char username[MAX_BUFFER_SIZE];
    char password[MAX_BUFFER_SIZE];

    // Interface d'authentification
    printf("Authentification requise\n");
    printf("Nom d'utilisateur: ");
    scanf("%s", username);
    printf("Mot de passe: ");
    scanf("%s", password);

    // Envoyer l'authentification au serveur
    char authMessage[MAX_BUFFER_SIZE];
    snprintf(authMessage, sizeof(authMessage), "AUTH %s %s", username, password);
    send(sockfd, authMessage, strlen(authMessage), 0);

    // Attendre la réponse du serveur
    char authResponse[MAX_BUFFER_SIZE];
    recv(sockfd, authResponse, sizeof(authResponse), 0);

    char trimmedResponse[MAX_BUFFER_SIZE];
    int i = strlen(authResponse) - 1;

    while (isspace(authResponse[i])) {
    i--;
    }

    authResponse[i + 1] = '\0';
    strcpy(trimmedResponse, authResponse);

    if (strcmp(trimmedResponse, "AUTH_SUCCESS") != 0) {
        printf("Authentification échouée. Terminaison du client.\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Authentification réussie.\n");

    // Interface utilisateur pour les services
    while (1) {
        displayMenu();

        char choice[MAX_BUFFER_SIZE];
        scanf("%s", choice);

        // Envoyer la demande de service au serveur
        send(sockfd, choice, strlen(choice), 0);

        // Quitter si l'utilisateur choisit "Fin"
        if (strcmp(choice, "Fin") == 0) {
            break;
        }

        // Attendre la réponse du serveur
        char serviceResponse[MAX_BUFFER_SIZE];
        recv(sockfd, serviceResponse, sizeof(serviceResponse), 0);

        printf("Résultat du service:\n%s\n", serviceResponse);
    }



    // Fermer la connexion
    close(sockfd);

    return 0;
}