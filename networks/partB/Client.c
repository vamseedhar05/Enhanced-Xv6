#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_CHUNK_SIZE 20

// Define a packet structure
struct Packet {
    int sequence_number;
    char data[MAX_CHUNK_SIZE];
};

int compare(const void *a, const void *b) {
    const struct Packet *packetA = (const struct Packet *)a;
    const struct Packet *packetB = (const struct Packet *)b;

    return packetA->sequence_number - packetB->sequence_number;
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);

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
        char text[2560];
        fgets(text, sizeof(text), stdin);

        int text_length = strlen(text);
        int total_chunks = (text_length + MAX_CHUNK_SIZE - 1) / MAX_CHUNK_SIZE;
        int sequence_number = 0;

        sendto(client_socket, &total_chunks, sizeof(total_chunks), MSG_DONTWAIT, (struct sockaddr *)&server_addr, sizeof(server_addr));

        int ACKArray[total_chunks];
        for(int i=0;i<total_chunks;i++){
            ACKArray[i]=0;
        }
        while(1){
            int flag=0;
            for (int i = 0; i < total_chunks; i++) {
                if(!ACKArray[i]){
                    struct Packet packet;
                    int ACK;
                    packet.sequence_number = sequence_number++;

                    int chunk_size = MAX_CHUNK_SIZE;
                    strncpy(packet.data, text + (i * MAX_CHUNK_SIZE), chunk_size);

                    sendto(client_socket, &packet, sizeof(packet), 0, (struct sockaddr *)&server_addr, server_len);
                    if(recvfrom(client_socket, &ACK, sizeof(ACK), 0, (struct sockaddr *)&server_addr, &server_len)){
                        ACKArray[ACK]=1;
                        printf("Acknowlwdgement Recieved for chunk %d\n",ACK);
                    }
                    flag=1;
                }
            }
            if(!flag)
                break;
        }

        // Receive a reply from the server
        int ReciveChuncks;
        recvfrom(client_socket, &ReciveChuncks, sizeof(ReciveChuncks), 0, (struct sockaddr *)&server_addr, &server_len);
        struct Packet ArrayP[ReciveChuncks];

        int k=ReciveChuncks;
        int i=0;

        while(k){
            struct Packet packet2;
            if (recvfrom(client_socket, &packet2, sizeof(packet2), 0, (struct sockaddr *)&server_addr, &server_len) ) {
                ArrayP[i++] = packet2;
                k--;
            }
            sendto(client_socket, &packet2.sequence_number, sizeof(packet2.sequence_number), 0, (struct sockaddr *)&server_addr, server_len);
        }

        qsort(ArrayP, ReciveChuncks, sizeof(struct Packet), compare);

        char received_text[MAX_CHUNK_SIZE * ReciveChuncks + 1];
        received_text[0] = '\0';
        for (int j = 0; j < ReciveChuncks; j++) {
            strcat(received_text, ArrayP[j].data);
        }

        received_text[strlen(received_text)] = '\0';

        printf("Server: %s", received_text);
    }

    // Close the socket
    close(client_socket);
    

    return 0;
}
