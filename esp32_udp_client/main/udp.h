#pragma once

int init_udp(int timeout_seconds);
int update_sender(char* ip_addr, int port);
int update_receiver(int port);
struct sockaddr_storage get_receive_info();
void close_udp();

int send_data(char* payload, int payload_length);
int receive_data(char* buffer, int buffer_length);