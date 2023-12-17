#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 5005

void displayMenu() {
    printf("\nMenu des services:\n");
    printf("1. Obtenir la date et l'heure du serveur\n");
    printf("2. Obtenir la liste des fichiers d'un répertoire\n");
    printf("3. Obtenir le contenu d'un fichier\n");
    printf("4. Obtenir la durée de la connexion\n");
  
}

int authenticate() {
    char username[50];
    char password[50];
    char storedUsername[] = "user123"; 
    char storedPassword[] = "pass123"; 

    printf("Authentification requise\n");
    printf("Nom d'utilisateur : ");
    scanf("%s", username);
    printf("Mot de passe : ");
    scanf("%s", password);

    if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
        printf("Authentification réussie.\n");
        return 1; 
    } else {
        printf("Échec de l'authentification.\n");
        return 0; 
    }
}

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    int choice;
    int authenticated = 0;

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur création socket !");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("Adresse invalide/ Adresse non supportée");
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connexion echouée");
        exit(EXIT_FAILURE);
    }
    
    while (!authenticated) {
        authenticated = authenticate();
        if (authenticated == 1) 
        {
        do {
        displayMenu();
        printf("Choisissez un service: ");
        scanf("%d", &choice);

        write(client_socket, &choice, sizeof(int));

        switch (choice) {
            case 1: {
                char dateTime[100];
                read(client_socket, dateTime, sizeof(dateTime));
                printf("Date et heure du serveur : %s\n", dateTime);
                break;
            }
            case 2: {
                char fileList[1024];
                read(client_socket, fileList, sizeof(fileList));
                printf("Liste des fichiers :\n%s\n", fileList);
                break;
            }
            case 3: {
                char filename[256];
                printf("Entrez le nom du fichier : ");
                scanf("%s", filename);
                write(client_socket, filename, sizeof(filename));

                char fileContent[1024];
                read(client_socket, fileContent, sizeof(fileContent));
                printf("Contenu du fichier :\n%s\n", fileContent);
                break;
            }
            case 4: {
                char duration[50];
                read(client_socket, duration, sizeof(duration));
                printf("Durée depuis la connexion : %s\n", duration);
                break;
            }
            default:
                break;
        }
    } while (choice != 0);
    }
    }

    close(client_socket);
    return 0;
}
