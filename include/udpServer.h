#include "config-parsing.h"
#include "struct-parsing.h"

void initServerUDP(int port, int *sockfd);
void *receiveFromUDP(dataUDP *my_data);