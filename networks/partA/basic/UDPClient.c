#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];
    int bytes_received;

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

    // Send data to server
    const char *message = "Hello, server!";
    if (sendto(client_socket, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Send error");
        close(client_socket);
        exit(1);
    }

    // Receive data from server
    bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0, NULL, NULL);
    if (bytes_received == -1) {
        perror("Receive error");
        close(client_socket);
        exit(1);
    }

    buffer[bytes_received] = '\0';
    printf("Received from server: %s\n", buffer);

    // Close the socket
    close(client_socket);

    return 0;
}
