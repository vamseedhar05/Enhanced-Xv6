#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];
    int bytes_received;

    // Create socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Socket creation error");
        exit(1);
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8888); // Change the port number as needed

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind error");
        close(server_socket);
        exit(1);
    }

    printf("UDP Server listening on port 8888...\n");

    // Receive data from client
    bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
    if (bytes_received == -1) {
        perror("Receive error");
        close(server_socket);
        exit(1);
    }

    buffer[bytes_received] = '\0';
    printf("Received from client: %s\n", buffer);

    // Send a response back to the client
    const char *response = "Hello, client!";
    if (sendto(server_socket, response, strlen(response), 0, (struct sockaddr *)&client_addr, client_len) == -1) {
        perror("Send error");
        close(server_socket);
        exit(1);
    }

    // Close the socket
    close(server_socket);

    return 0;
}
