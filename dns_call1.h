/*
 * dns_call1.h
 *
 *  Created on: 16 июл. 2023 г.
 *      Author: User
 */

#ifndef DNS_CALL1_H_
#define DNS_CALL1_H_

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>


static char* dns_call(char* str_webser1)
{
long ip4_res = 0;
char* alloc_ip4_str=0;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "Failed to initialize Winsock.\n");
        exit(11);
    }

    struct addrinfo hints, *results, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;      // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP socket

    int status = getaddrinfo(str_webser1, "80", &hints, &results);
    if (status != 0) {
        fprintf(stderr, "Failed to resolve hostname: %s\n", gai_strerror(status));
        WSACleanup();
        exit(12);
    }

    char ip[INET6_ADDRSTRLEN];
    for (result = results; result != NULL; result = result->ai_next) {
        void* addr;
        char* ip_version;

        if (result->ai_family == AF_INET) { // IPv4
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)result->ai_addr;
            addr = &(ipv4->sin_addr);
            ip_version = "IPv4";
            ip4_res = 1;
        } else { // IPv6
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)result->ai_addr;
            addr = &(ipv6->sin6_addr);
            ip_version = "IPv6";
        }

        inet_ntop(result->ai_family, addr, ip, sizeof(ip));
        printf("Resolved %s: %s\n", ip_version, ip);
        if (ip4_res)
          {
            alloc_ip4_str = strdup(ip);
          }

    }

    freeaddrinfo(results);
    WSACleanup();


    return alloc_ip4_str;

}

#endif /* DNS_CALL1_H_ */
