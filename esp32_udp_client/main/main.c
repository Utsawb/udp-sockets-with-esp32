/* WiFi station Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
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

#include "wifi.h"
#include "udp.h"


/* Configure the udp server's ip and port */
#define HOST_IP_ADDR "192.168.12.10"
#define PORT 1128

void app_main(void)
{
    char buffer[32];
    init_wifi();
    init_udp(10);
    update_sender(HOST_IP_ADDR, PORT);
    if (update_receiver(PORT) < 0)
    {
        printf("Could not bind to port\n");
        return;
    }

    while (1)
    {
        struct sockaddr_storage dest_store;
        if (receive_data(buffer, 32) >= 0)
        {
            send_data("Got data\n", 9);
            memset(buffer, 0, 32);
        }
    }

    close_udp();
}