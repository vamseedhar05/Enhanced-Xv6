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
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        perror("Socket creation error");
        exit(1);
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind error");
        close(server_socket);
        exit(1);
    }

    while (1) {
        int total_chunks;
        if (recvfrom(server_socket, &total_chunks, sizeof(total_chunks), 0, (struct sockaddr *)&client_addr, &client_len) == -1) {
            perror("Recvfrom error");
            close(server_socket);
            exit(1);
        }

        struct Packet ArrayP[total_chunks];
        int k=total_chunks;
        int i=0;
        while(k) {
            struct Packet packet;
            if (recvfrom(server_socket, &packet, sizeof(packet), 0, (struct sockaddr *)&client_addr, &client_len)){
                ArrayP[i++] = packet;
                k--;
            }
            sendto(server_socket, &packet.sequence_number, sizeof(packet.sequence_number), 0, (struct sockaddr *)&client_addr,client_len);
        }

        qsort(ArrayP, total_chunks, sizeof(struct Packet), compare);

        char received_text[MAX_CHUNK_SIZE * total_chunks + 1];
        received_text[0] = '\0';
        for (int j = 0; j < total_chunks; j++) {
            strcat(received_text, ArrayP[j].data);
        }

        received_text[strlen(received_text)] = '\0';

        printf("Client: %s", received_text);

        char text[2560];
        fgets(text, sizeof(text), stdin);

        int text_length = strlen(text);
        int Sendchunks = (text_length + MAX_CHUNK_SIZE - 1) / MAX_CHUNK_SIZE;
        int sequence_number = 0;

        sendto(server_socket, &Sendchunks, sizeof(Sendchunks), 0, (struct sockaddr *)&client_addr,client_len);
        int ACKArray[Sendchunks];

        for(int i=0;i<Sendchunks;i++){
            ACKArray[i]=0;
        }
        while(1){
            int flag=0;
            for (int i = 0; i < Sendchunks; i++){
                if(!ACKArray[i]){
                    struct Packet packet2;
                    int ACK;
                    packet2.sequence_number = sequence_number++;

                    int chunk_size = MAX_CHUNK_SIZE;
                    strncpy(packet2.data, text + (i * MAX_CHUNK_SIZE), chunk_size);

                    sendto(server_socket, &packet2, sizeof(packet2), 0, (struct sockaddr *)&client_addr, client_len);
                    if(recvfrom(server_socket, &ACK, sizeof(ACK), 0, (struct sockaddr *)&client_addr, &client_len)){
                        ACKArray[ACK]=1;
                        printf("Acknowlwdgement Recieved for chunk %d\n",ACK);
                    }
                    flag=1;
                }
            }
            if(!flag)
                break;
        }
    }

    close(server_socket);

    return 0;
}
