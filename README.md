# Project for Computer Network Architectures and Protocols.

Compile the server:
	gcc haloserver.c logger.c -o haloserver

Compile the client:
	gcc haloclient.c -o haloclient

Usage: 
	./haloserver number_of_plays maximum_of_number_to_guest

	./haloclient ip_addr
