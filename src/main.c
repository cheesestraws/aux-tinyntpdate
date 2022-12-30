/*
 *
 * (C) 2014 David Lettier.
 *
 * http://www.lettier.com/
 *
 * NTP client.
 *
 * Compiled with gcc version 4.7.2 20121109 (Red Hat 4.7.2-8) (GCC).
 *
 * Tested on Linux 3.8.11-200.fc18.x86_64 #1 SMP Wed May 1 19:44:27 UTC 2013 x86_64 x86_64 x86_64 GNU/Linux.
 *
 * To compile: $ gcc main.c -o ntpClient.out
 *
 * Usage: $ ./ntpClient.out
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "modern_types.h"
#include "ntp.h"

void error(char* msg) {
	perror(msg);  
	exit(1);
}

int main(int argc, char* argv[]) {
	int sockfd, ret;
	
	int portno = 123; // NTP UDP port number.
	char* host_name = "pool.ntp.org"; // NTP server hostname
	struct hostent *host_info;      // NTP server name lookup results
	
	struct sockaddr_in serv_addr = { 0 }; // NTP server network address
	ntp_packet packet = { 0 };
	
	struct timeval tp = { 0 };

	time_t txTm;

	// Set the first byte's bits to 00,011,011 for li = 0, vn = 3, and mode = 3.
	*((char *)&packet) = 0x1b;

	// Create and "connect" the UDP socket.  Normal socket dance.
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd < 0) {
		error("socket");
	}

	host_info = gethostbyname( host_name ); // Convert URL to IP.
	if (!host_info) {
		error("gethostbyname");
	}

	serv_addr.sin_family = AF_INET;
	bcopy((char*)host_info->h_addr, (char*)&serv_addr.sin_addr.s_addr, host_info->h_length);
	serv_addr.sin_port = htons(portno);

	ret = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) ;
	if (ret < 0) {
		error("connect");
	}

	ret = write(sockfd, (char*)&packet, sizeof(ntp_packet));
	if ( ret < 0 ) {
		error("write");
	}
	
	// Wait and receive the packet back from the server.
	ret = read(sockfd, (char*)&packet, sizeof(ntp_packet));
	if (ret < 0) {
		error("read");
	}

	// These two fields contain the time-stamp seconds as the packet left the NTP server.
	// The number of seconds correspond to the seconds passed since 1900.

	packet.txTm_s = ntohl(packet.txTm_s); // Time-stamp seconds.
	packet.txTm_f = ntohl(packet.txTm_f); // Time-stamp fraction of a second.

	// Extract the 32 bits that represent the time-stamp seconds (since NTP epoch) from 
	// when the packet left the server.  NTP epoch is 1900; UNIX epoch is 1970.  Subtract
	// the difference.
	txTm = (time_t)(packet.txTm_s - NTP_TIMESTAMP_DELTA);

	// Print the time we got from the server, accounting for timezone
	printf("time: %s", ctime((const time_t*)&txTm));

	tp.tv_sec = txTm;
	ret = settimeofday(&tp, 0);
	if (ret < 0) {
		error("settimeofday");
	}
	
	return 0;
}
