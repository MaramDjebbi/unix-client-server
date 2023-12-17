#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <fcntl.h>

#define MAX_BUFFER_SIZE 1024

// Fonction pour envoyer la date et l'heure actuelles
void sendDateTime(int client_fd) {
    time_t rawtime;
    struct tm *info;
    char buffer[MAX_BUFFER_SIZE];

    time(&rawtime);
    info = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "Date: %x\nHeure: %X", info);
    send(client_fd, buffer, strlen(buffer), 0);
}

// Fonction pour envoyer la liste des fichiers d'un répertoire
void sendFileList(int client_fd, const char *directory_path) {
    DIR *dir;
    struct dirent *ent;
    char buffer[MAX_BUFFER_SIZE];

    if ((dir = opendir(directory_path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            snprintf(buffer, sizeof(buffer), "%s\n", ent->d_name);
            send(client_fd, buffer, strlen(buffer), 0);
        }
        closedir(dir);
    } else {
        perror("Erreur lors de l'ouverture du répertoire");
    }
}

// Fonction pour envoyer le contenu d'un fichier
void sendFileContent(int client_fd, const char *file_path) {
    int file = open(file_path, O_RDONLY);
    if (file == -1) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = read(file, buffer, sizeof(buffer))) > 0) {
        send(client_fd, buffer, bytesRead, 0);
    }

    close(file);
}

// Fonction pour envoyer la durée déjà écoulée depuis le début de la connexion du client courant
void sendElapsedTime(int client_fd, time_t start_time) {
    time_t current_time;
    time(&current_time);
    double elapsed_time = difftime(current_time, start_time);

    char buffer[MAX_BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "Temps écoulé depuis le début de la connexion: %.2f secondes", elapsed_time);
    send(client_fd, buffer, strlen(buffer), 0);
}

/////////////////////////////

const char *valid_username = "user123";
const char *valid_password = "pass123";

int authenticateClient(int client_socket, char *username, char *password) {
    char authMessage[MAX_BUFFER_SIZE];
    recv(client_socket, authMessage, sizeof(authMessage), 0);

    printf("Received Auth Message: %s\n", authMessage); // Debugging print

    char auth_username[MAX_BUFFER_SIZE];
    char auth_password[MAX_BUFFER_SIZE];

    sscanf(authMessage, "AUTH %s %s", auth_username, auth_password);

    printf("Received Username: %s\n", auth_username); // Debugging print
    printf("Received Password: %s\n", auth_password); // Debugging print
    printf("Valid Username: %s\n", username); // Debugging print
    printf("Valid Password: %s\n", password); // Debugging print

    if (strcmp(auth_username, username) == 0 && strcmp(auth_password, password) == 0) {
        send(client_socket, "AUTH_SUCCESS", strlen("AUTH_SUCCESS"), 0);
        return 1; // Authentication successful
    } else {
        send(client_socket, "AUTH_FAILURE", strlen("AUTH_FAILURE"), 0);
        return 0; // Authentication failed
    }
}
//////////////////////////////////

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const int port = atoi(argv[1]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Erreur lors de la création de la socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serveur_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    serveur_addr.sin_family = AF_INET;
    serveur_addr.sin_port = htons(port);
    serveur_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serveur_addr, sizeof(serveur_addr)) == -1) {
        perror("Erreur lors de la liaison de la socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) == -1) {
        perror("Erreur lors de la mise en écoute");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Serveur TCP en attente sur le port %d...\n", port);

    while (1) {
    int client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd == -1) {
        perror("Erreur lors de l'acceptation de la connexion");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    time_t start_time;
    time(&start_time);

    if (authenticateClient(client_fd, valid_username, valid_password)) {
        printf("Authentification réussie\n");

        char choice[MAX_BUFFER_SIZE];
        // Receive the choice from the client
        recv(client_fd, choice, sizeof(choice), 0);

        printf("received choice %s\n", choice);
        // Perform actions based on the received choice
        if (strcmp(choice, "1") == 0) {
            sendDateTime(client_fd);
        } else if (strcmp(choice, "2") == 0) {
            sendFileList(client_fd, "/path/to/directory");
        } else if (strcmp(choice, "3") == 0) {
            sendFileContent(client_fd, "/path/to/file.txt");
        } else if (strcmp(choice, "4") == 0) {
            sendElapsedTime(client_fd, start_time);
        } else if (strcmp(choice, "Fin") == 0) {
            printf("Client has chosen to end the session.\n");
        } else {
            printf("Invalid choice received from client.\n");
        }
    } else {
        printf("Authentication failed for client.\n");
        // Close the connection for failed authentication
        close(client_fd);
    }
}



    close(sockfd);

    return 0;
}