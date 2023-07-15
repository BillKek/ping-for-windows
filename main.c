#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include "timer1.h"
#include "params.h"

#define MAX_BUFFER_SIZE 1024

// ICMP header structure
typedef struct icmp_hdr
{
  unsigned char type;     // ICMP message type
  unsigned char code;     // Type sub-code
  unsigned short checksum; // Checksum
  unsigned short id;      // Identifier
  unsigned short seq;     // Sequence number
} ICMP_HDR;

// Calculate the checksum for the ICMP packet
unsigned short
calculateChecksum (unsigned short *buffer, int size)
{
  unsigned long cksum = 0;

  while (size > 1)
    {
      cksum += *buffer++;
      size -= sizeof(unsigned short);
    }

  if (size)
    {
      cksum += *(unsigned char*) buffer;
    }

  cksum = (cksum >> 16) + (cksum & 0xffff);
  cksum += (cksum >> 16);

  return (unsigned short) (~cksum);
}

int
main (int argc, char *argv[])
{

  parse (argc, argv); // -> str_ip4 ->str_webser
  if (str_ip4 == 0)
    exit (2);

  WSADATA wsaData;
  SOCKET sock;
  int bytesReceived;
  struct sockaddr_in destAddr;
  struct icmp_hdr *icmpHeader;
  int packetSize;
  const char *destIP = str_ip4; // Destination IP address for ping

  timer1_init ();

  while (1)
    {
      // Initialize Winsock
      if (WSAStartup (MAKEWORD(2, 2), &wsaData) != 0)
	{
	  fprintf (stderr, "Failed to initialize winsock\n");
	  return 1;
	}

      // Create raw socket for ICMP
      sock = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP);
      if (sock == INVALID_SOCKET)
	{
	  fprintf (stderr, "Failed to create socket\n");
	  return 1;
	}

      // Установка максимального времени ожидания ???
      int timeoutMillis = 15000;  // 15 секунд
      struct timeval timeout;
      timeout.tv_sec = timeoutMillis / 1000;
      timeout.tv_usec = 0;
      //timeout.tv_usec = (timeoutMillis % 1000) * 1000;

      if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char*) &timeout,
		      sizeof(timeout)) == SOCKET_ERROR)
	{
	  fprintf (stderr, "Failed to set recv timeout\n");
	}

      // Set destination address
      memset (&destAddr, 0, sizeof(destAddr));
      destAddr.sin_family = AF_INET;
      inet_pton (AF_INET, destIP, &(destAddr.sin_addr));

      char buffer[MAX_BUFFER_SIZE];
      // Prepare the ICMP packet
      icmpHeader = (struct icmp_hdr*) buffer;
      icmpHeader->type = 8; // ICMP Echo Request type
      icmpHeader->code = 0;
      icmpHeader->id = htons (GetCurrentProcessId ());
      icmpHeader->seq = htons (1);
      icmpHeader->checksum = 0;
      icmpHeader->checksum = calculateChecksum ((unsigned short*) icmpHeader,
						sizeof(ICMP_HDR));

      packetSize = sizeof(ICMP_HDR);

      //long restart_recv = 0;
      double interv = -1;
      //do // цикл для того, чтобы пользователь успел поработать с фаерфолом
	{

	  interv = -1;
	  // Send the ICMP packet
	  timer1_t1 ();
	  if (sendto (sock, buffer, packetSize, 0, (struct sockaddr*) &destAddr,
		      sizeof(destAddr)) == SOCKET_ERROR)
	    { // popup firewall message async
	      fprintf (stderr, "Failed to send ICMP packet\n");
	      closesocket (sock);
	      WSACleanup ();
	      return 1;
	    }

	  // Receive the ICMP reply
	  bytesReceived = recv (sock, buffer, MAX_BUFFER_SIZE, 0); // waiting long
	  double interv_tmp = timer1_t2_res_us ();
	  if (bytesReceived == SOCKET_ERROR)
	    {
	    }
	  else if (bytesReceived == 28) // ???
	    {
	      interv = interv_tmp;
	      //printf("bytesReceived %li  packetSize %li \n",bytesReceived , packetSize);
	    }
	}
//      while ((restart_recv < 30 // 60 * 2
//      ) && (restart_recv > 0));

      if ((bytesReceived == SOCKET_ERROR) && (bytesReceived != 28))
	{
	  fprintf (stderr, "Failed to receive ICMP reply\n");
	  closesocket (sock);
	  WSACleanup ();

	  printf ("Ping %s [%s] = timeout \n", str_ip4, str_webser);
	}
      else
	printf ("Ping %s [%s] = %li us\n", str_ip4, str_webser,
		(long) (interv + 0.5));

      // Clean up
      closesocket (sock);
      WSACleanup ();

      Sleep (1000);
    }

  return 0;
}
