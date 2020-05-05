#include "udpServer.h"


/**
 * @brief initialize the sock in a port
 * @param port int
 * @param sockfd int*
 * @retval none
 * **/
void initServerUDP(int port, int *sockfd)
{
    struct sockaddr_in si_me;
    *sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (*sockfd < 0) {
        perror("opening socket");
    }

    memset(&si_me, '\0', sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(port);
    si_me.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(*sockfd, (struct sockaddr*)&si_me, sizeof(si_me)) < 0) {
        perror("Binding");
    }
}

/**
 * @brief will receive the data from the socket udp and print the addresse's, port's and data's client
 * function pointed for thread
 * @param p_data void* data parse by the thread
 * @retval none
 * **/
void *receiveFromUDP(dataUDP *my_data)
{ 
    int index = my_data->index;
    uint8_t *buffer = my_data->buffer;
    int port = my_data->port;
    int *sockfd = my_data->sockfd;
    socklen_t addr_size;
    struct sockaddr_in si_other;
    int size;
    int buffer_size =3*14;
    addr_size=sizeof(si_other);

    // Receive data from UDP
    size = recvfrom(*sockfd, buffer, buffer_size, 0, (struct sockaddr*)&si_other, &addr_size);
    if (size < 0) {
        perror("recvfrom");
    }
    my_data->buffer = buffer;

        //TODO: CALL FONCTION TO SERIALIZE HERE
        /*
        printf("[+]Data Received: ");
        for (int i = 0; i<buffer_size; i++) {
            printf("0x%02x ", buffer[i]);
        }
        printf("\n");
        printf("------------------------------------------\n");
        */

       
    
}