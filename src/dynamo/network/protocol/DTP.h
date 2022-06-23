/**
 * @file dtp.h
 * @author Joao Matos
 * 
 * @brief The structure of the Dynamo Transmition Protocol. 
 * 
 * @version 0.1
 * @date 2022-06-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __DTP_H__
#define __DTP_H__

#define DTP_INDICATOR 10
#define FTP_READY 5
#define ACK -1
#define BLOCKCHAIN_DATA_PACKET 0
#define BLOCKCHAIN_REQUEST_PACKET 1
#define TRANSACTION_PACKET 2
#define TRANSACTION_POOL_DATA_PACKET 3
#define TRANSACTION_POOL_REQUEST_PACKET 4
#define NEW_BLOCK_PACKET 7

#include <iostream>

#include "../../../../libs/msgpack11/msgpack11.hpp"

namespace DTP
{
    struct header
    {
        int type;                   // The type of the packet (-1 - ACK ,0 - BLOCKCHAIN_DATA, 1 - BLOCKCHAIN_REQUEST, 2 - TRANSACTION, 3 - TRANSACTION_POOL_DATA, 4 - TRANSACTION_POOL_REQUEST)
        std::string origin;         // IP address of the sender
        std::string destination;    // IP address of the receiver
        int serverPort;             // Port of the sender's server instance
        int port;                   // Port of the receiver
    };

    class Packet
    {
    private:
        /* MEMBER VARIABLES */
        int indicator;              // The indicator of the packet (specifies the protocol)
        
        header packetHeader;
        
        int payloadSize;            // The size of the payload
        std::string payload;        // The payload of the packet

        msgpack11::MsgPack packer; // The MsgPack object to serialize the packet

    public:
        /* CONSTRUCTOR */
        Packet(int type, std::string origin, std::string destination, int serverPort, int port, std::string payload);
        
        explicit Packet(const std::string& buffer);

        /* PUBLIC FUNCTIONS */
        /**
         * @brief Gets the packet buffer to be sent over the network.
         * 
         * @return std::string 
         */
        std::string buffer();

        /**
         * @brief Prints the packet to the std output.
         * 
         */
        void show();

        /* GETTERS */
        header headers();

        int getIndicator() const;

        std::string getPayload();
        int getPayloadSize() const;
    };
}

#endif // __DTP_H__