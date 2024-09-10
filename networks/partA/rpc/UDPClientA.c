#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char choice[10];
    char result[1024];
    char ANchoice[10];

    // Create socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        perror("Socket creation error");
        exit(1);
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888); // Change the port number as needed
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change the IP address as needed

    while (1) {
        // Get user's choice
        printf("Client A, enter your choice (Rock, Paper, Scissors): ");
        fgets(choice, sizeof(choice), stdin);
        sendto(client_socket, choice, strlen(choice), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        // Receive game result from the server
        recvfrom(client_socket, result, sizeof(result), 0, NULL, NULL);
        printf("Game result: %s\n", result);

        // Ask if the user wants to play again
        printf("Play again? (yes/no): ");
        
        // Get user's response
        fgets(ANchoice, sizeof(ANchoice), stdin);
        sendto(client_socket, ANchoice, strlen(ANchoice), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        recvfrom(client_socket, choice, sizeof(choice), 0, NULL, NULL);
        if (ANchoice[0] == 'n' || choice[0] == 'n') {
            break; // Exit the loop if the user doesn't want to play again
        }
    }

    // Close the socket
    close(client_socket);

    return 0;
}
