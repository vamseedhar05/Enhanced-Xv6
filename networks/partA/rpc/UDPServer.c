#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket;
    int server_socket2;
    struct sockaddr_in server_addr, server_addr2, client_addr1, client_addr2;
    socklen_t client_len1 = sizeof(client_addr1);
    socklen_t client_len2 = sizeof(client_addr2);
    char buffer1[1024];
    char buffer2[1024];

    // Create socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Socket1 creation error");
        exit(1);
    }
    
    server_socket2 = socket(AF_INET, SOCK_DGRAM, 0);
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

    if (bind(server_socket2, (struct sockaddr *)&server_addr2, sizeof(server_addr2)) == -1) {
        perror("Bind error");
        close(server_socket2);
        exit(1);
    }

    printf("UDP Server listening on port 8888 and 9999...\n");

    while (1) {
        // Receive decisions from client 1
        recvfrom(server_socket, buffer1, sizeof(buffer1), 0, (struct sockaddr *)&client_addr1, &client_len1);
        buffer1[strlen(buffer1) - 1] = '\0'; // Remove newline character

        // Receive decisions from client 2
        recvfrom(server_socket2, buffer2, sizeof(buffer2), 0, (struct sockaddr *)&client_addr2, &client_len2);
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
        sendto(server_socket, result, strlen(result), 0, (struct sockaddr *)&client_addr1, client_len1);
        sendto(server_socket2, result, strlen(result), 0, (struct sockaddr *)&client_addr2, client_len2);

        printf("Game result: %s\n", result);

        // Ask clients if they want to play again
        char play_again1[10];
        char play_again2[10];
        // Receive responses from clients
        recvfrom(server_socket, play_again1, sizeof(play_again1), 0, NULL, NULL);
        recvfrom(server_socket2, play_again2, sizeof(play_again2), 0, NULL, NULL);
        sendto(server_socket, play_again2, strlen(play_again2), 0, (struct sockaddr *)&client_addr1, client_len1);
        sendto(server_socket2, play_again1, strlen(play_again1), 0, (struct sockaddr *)&client_addr2, client_len2);
        // Check if both clients want to play again (respond with "yes")
        if (play_again1[0] == 'y' && play_again2[0] == 'y') {
            // Continue the loop and start a new game
            continue;
        } else {
            // Exit the loop if any client doesn't want to play again
            break;
        }
    }

    // Close the socket
    close(server_socket);
    close(server_socket2);

    return 0;
}
