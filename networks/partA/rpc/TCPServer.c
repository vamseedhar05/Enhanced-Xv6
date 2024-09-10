#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket, server_socket2,client_socket1, client_socket2;
    struct sockaddr_in server_addr, server_addr2,client_addr1, client_addr2;
    socklen_t client_len1 = sizeof(client_addr1);
    socklen_t client_len2 = sizeof(client_addr2);
    char buffer1[1024];
    char buffer2[1024];
    
    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation error");
        exit(1);
    }

    server_socket2 = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket2 == -1) {
        perror("Socket2 creation error");
        exit(1);
    }
    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8888); // Change the port number as needed

    memset(&server_addr2, 0, sizeof(server_addr2));
    server_addr2.sin_family = AF_INET;
    server_addr2.sin_addr.s_addr = INADDR_ANY;
    server_addr2.sin_port = htons(9999); // Change the port number as needed

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind error");
        close(server_socket);
        exit(1);
    }
    // Listen for incoming connections
    if (listen(server_socket, 1) == -1) {
        perror("Listen error");
        close(server_socket);
        exit(1);
    }

    if (bind(server_socket2, (struct sockaddr *)&server_addr2, sizeof(server_addr2)) == -1) {
        perror("Bind error");
        close(server_socket2);
        exit(1);
    }

    if (listen(server_socket2, 1) == -1) {
        perror("Listen error");
        close(server_socket2);
        exit(1);
    }
    printf("TCP Server listening on port 8888 and 9999...\n");

    // Accept connection from client 1
    client_socket1 = accept(server_socket, (struct sockaddr *)&client_addr1, &client_len1);
    if (client_socket1 == -1) {
        perror("Accept error");
        close(server_socket);
        exit(1);
    }
    
    // Accept connection from client 2
    client_socket2 = accept(server_socket2, (struct sockaddr *)&client_addr2, &client_len2);
    if (client_socket2 == -1) {
        perror("Accept error");
        close(server_socket2);
        exit(1);
    }

    while (1) {
        // Receive decisions from client 1
        recv(client_socket1, buffer1, sizeof(buffer1), 0);
        buffer1[strlen(buffer1) - 1] = '\0'; // Remove newline character
        
        // Receive decisions from client 2
        recv(client_socket2, buffer2, sizeof(buffer2), 0);
        buffer2[strlen(buffer2) - 1] = '\0'; // Remove newline character

        printf("Client 1 chose: %c\n", buffer1[0]);
        printf("Client 2 chose: %c\n", buffer2[0]);

        // Logic to determine the game result (simplified)
        char* result;
        if (buffer1[0]==buffer2[0]) {
            result = "Draw";
        } 
        else if((buffer1[0] == 'r' && buffer2[0] == 's') ||
                (buffer1[0] == 'p' && buffer2[0] == 'r') ||
                (buffer1[0] == 's' && buffer2[0] == 'p') ){
            result = "Client 1 wins!";
        } else {
            result = "Client 2 wins!";
        }

        // Send the result to both clients
        send(client_socket1, result, strlen(result), 0);
        send(client_socket2, result, strlen(result), 0);

        printf("Game result: %s\n", result);
        
        // Ask clients if they want to play again
        // char play_again[100] = "Play again? (yes/no): ";
        // send(client_socket1, play_again, strlen(play_again), 0);
        // send(client_socket2, play_again, strlen(play_again), 0);
        
        // Receive responses from clients
        recv(client_socket1, buffer1, sizeof(buffer1), 0);
        recv(client_socket2, buffer2, sizeof(buffer2), 0);
        
        send(client_socket1, buffer2, strlen(buffer2), 0);
        send(client_socket2, buffer1, strlen(buffer1), 0);
        if (buffer1[0] == 'y' && buffer2[0] == 'y') {
            continue;
        } else {
            break;
        }
    }

    // Close sockets
    close(client_socket1);
    close(client_socket2);
    close(server_socket);
    close(server_socket2);

    return 0;
}
