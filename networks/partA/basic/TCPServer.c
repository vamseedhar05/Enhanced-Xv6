#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[1024];
    int bytes_received;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
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

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listen error");
        close(server_socket);
        exit(1);
    }

    printf("Server listening on port 8888...\n");

    // Accept connection from client
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket == -1) {
        perror("Accept error");
        close(server_socket);
        exit(1);
    }

    // Receive data from client
    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received == -1) {
        perror("Receive error");
        close(client_socket);
        close(server_socket);
        exit(1);
    }

    buffer[bytes_received] = '\0';
    printf("Received from client: %s\n", buffer);

    // Send a response back to the client
    const char *response = "Hello, client!";
    if (send(client_socket, response, strlen(response), 0) == -1) {
        perror("Send error");
        close(client_socket);
        close(server_socket);
        exit(1);
    }

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}
