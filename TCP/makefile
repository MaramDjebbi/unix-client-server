# Compilateur et drapeaux
CC = gcc
CFLAGS = -Wall

# Fichiers sources et exécutables cibles
CLIENT_SRC = clientTCP.c
SERVER_SRC = serveurTCP.c
CLIENT_EXE = client
SERVER_EXE = serveur

	

all:  $(CLIENT_EXE) $(SERVER_EXE)


$(CLIENT_EXE): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT_EXE) $(CLIENT_SRC)


$(SERVER_EXE): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_EXE) $(SERVER_SRC)

.PHONY: clean run 

run_server: $(SERVER_EXE) 
	#gnome-terminal -e ./$(SERVER_EXE) 
	#gnome-terminal -e ./$(CLIENT_EXE) 
	gnome-terminal -- $(SHELL) -c "./serveur; exec $SHELL"
run_client:$(CLIENT_EXE)
	gnome-terminal -- $(SHELL) -c "./client && sleep 5; exec $SHELL"



clean:
	rm -f $(CLIENT_EXE) $(SERVER_EXE)


