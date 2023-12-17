# UDP Client-Server Implementation in C

This project demonstrates a basic implementation of a UDP client-server communication using C programming language in a Unix/Linux environment.

## Overview

The project consists of two programs:

- **clientUDP.c**: A client program that sends a randomly generated number to the server and displays the response received.
- **UDPServer.c**: A server program that receives a number from the client, generates random numbers, and sends them back to the client.

## Features

- **UDP Protocol**: Utilizes User Datagram Protocol (UDP) for communication between client and server.
- **Random Number Generation**: Generates random numbers on the server and sends them to the client based on the received number.

## Usage

### Running the Server

To start the server, compile `UDPServer.c` and run the executable with a specified port:

```bash
./UDPServer <port>
