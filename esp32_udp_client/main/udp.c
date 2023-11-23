#include "udp.h"

#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

static struct sockaddr_in dest_addr;
static struct sockaddr_in in_addr;
static struct sockaddr_storage source_addr;
static socklen_t socklen;
static int sock;

int init_udp(int timeout_seconds)
{
    struct timeval timeout;
    timeout.tv_sec = timeout_seconds;
    timeout.tv_usec = 0;
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);
    return sock < 0 ? -1 : 0;
}

int update_receiver(int port)
{
    in_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    in_addr.sin_family = AF_INET;
    in_addr.sin_port = htons(port);
    int t = 0;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &t, sizeof t);
    setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &t, sizeof t);
    return bind(sock, (struct sockaddr *)&in_addr, sizeof(in_addr));
}

int update_sender(char* ip_addr, int port)
{
    dest_addr.sin_addr.s_addr = inet_addr(ip_addr);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    return 0;
}

int send_data(char* payload, int payload_length)
{
    return sendto(sock, payload, payload_length, 0, (struct sockaddr*) &dest_addr, sizeof dest_addr);
}

int receive_data(char * buffer, int buffer_length)
{
    socklen = sizeof source_addr;
    int len = recvfrom(sock, buffer, buffer_length - 1, 0, (struct sockaddr*) &source_addr, &socklen);
    if (len < 0)
    {
        return -1;
    }
    buffer[buffer_length] = '\0';
    return len;
}

struct sockaddr_storage get_receive_info()
{
    return source_addr;
}

void close_udp()
{
    shutdown(sock, 0);
    close(sock);
}