#pragma once

int init_udp(int timeout_seconds);
int update_sender(char* ip_addr, int port);
void close_udp();

int send_data(char* payload, int payload_length);
int recieve_data(char* buffer, int buffer_length);