//
// Created by joaoa on 21/04/2022.
//

#include "NodeServer.h"

/* CONSTRUCTOR */
NodeServer::NodeServer(int domain, int service, int protocol, int port, unsigned long iface, int bklg,
                       int thread_count) : net::BasicServer(domain, service, protocol, port, iface, bklg)
{
    // Set Node server variables
    this->domain = domain;
    this->service = service;
    this->protocol = protocol;
    this->port = port;
    this->iface = iface;
    this->backlog = bklg;
    this->number_of_threads = thread_count;

    // Call the constructor for the thread pool
    tp = new sys::ThreadPool(number_of_threads);
}

/* DESTRUCTOR */
NodeServer::~NodeServer()
{
    delete tp;
}

/* PUBLIC FUNCTIONS AND FUNCTION OVERRIDES */
void NodeServer::accepter()
{
    struct sockaddr_in address = get_socket()->get_address();

    // length of the address struct to be passed into the accept() function
    int addrlen = sizeof(address);

    // Store the new socket descriptor
    //fcntl(get_socket()->get_sock(), F_SETFL, fcntl(get_socket()->get_sock(), F_GETFL) | O_NONBLOCK);
    new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);

    net::Socket::test_connection(new_socket);

    nodeIP = inet_ntoa(address.sin_addr);
    nodePort = htons(address.sin_port);

    // Read the incoming message and store it in the buffer
    recv(new_socket, buffer, BUFFER_SIZE, 0);

    return;
}

void NodeServer::handler() // The handler will attempt to create a DTP packet instance from the incomming buffer 
{                          // It will also set the broadcast buffer of the node to the incoming packet, so that the message can be broadcasted to all the other nodes
    try
    {
        this->packet = new DTP::Packet(buffer);
    }
    catch(const std::exception& e)
    {
        std::cout << "[ERROR] Unable to create packet from buffer: " << e.what() << std::endl;
    }

    return;
}


void NodeServer::responder() // After responding to the incoming message the responder should set the broadcast flag of the node to true
{                            // in order for the node to know when to broadcast the message to all the other nodes

    using namespace msgpack11;
    MsgPack msgpack;
    std::string payload;

    DTP::Packet* response;

    if (this->packet->getIndicator() == DTP_INDICATOR)
    {
        if (this->packet->headers().type == BLOCKCHAIN_REQUEST_PACKET)
        {
            // Send the blockchain to the client
            msgpack = Blockchain::serialize(*this->blockchain);
            payload = msgpack.dump();

            //response = new DTP::Packet(BLOCKCHAIN_DATA_PACKET, std::string(this->uuid), std::string(nodeIP), nodePort, payload);
            DTP::Packet rsp(BLOCKCHAIN_DATA_PACKET, std::string(this->uuid), std::string(nodeIP), nodePort, payload);

            std::cout << std::endl << "Payload: " << rsp.getPayload() << std::endl;

            send(new_socket, rsp.getPayload().data(), rsp.getPayload().size(), 0);

            /*std::cout << "Packet Buffer: " << rsp.buffer() << std::endl;

            char* toSend1 = new char[rsp.buffer().length()];

            std::string test;
            std::cout << "Here: ";

            for (int i = 0; i < rsp.buffer().length(); i++)
            {
                toSend1[i] = rsp.buffer()[i];
                if (rsp.buffer()[i] == '\0')
                {
                    std::cout << "Found null at: " << i << std::endl;
                }
                std::cout << toSend1[i];
                test.append(&toSend1[i]);
            }*/

            /*std::cout << std::endl << "Packet Buffer2: " << std::string(toSend) << std::endl;
            std::cout << "Test: " << test << std::endl;
            std::cout << "Test c_str(): " << test.c_str() << std::endl;

            DTP::Packet response2(std::string(test.c_str()));

            Blockchain chainTest(response2.getPayload());
            chainTest.printChain();*/

            /*std::cout << std::endl << "Test: " << test << std::endl;

            const char* toSend = test.c_str();
            const int toSendLen = test.length();

            std::string toSendBuffer = std::string(toSend, toSendLen);

            std::cout << std::endl << "To send buffer: " << toSendBuffer << std::endl;

            DTP::Packet response2(toSendBuffer);

            Blockchain chainTest(response2.getPayload());
            chainTest.printChain();*/

            //send(new_socket, toSendBuffer.c_str(), toSendBuffer.length(), 0);
        }
        else if (this->packet->headers().type == BLOCKCHAIN_DATA_PACKET)
        {
            // Create a new Blockchain from the incomming packet payload
            // validate it and replace the current blockchain with the new one
        
            Blockchain new_chain(this->packet->getPayload());
            if(this->blockchain->replaceChain(new_chain) == 0)
            {
                std::cout << "[INFO] Blockchain instance updated to " << nodeIP << " node state" << std::endl;
            }
        }
        else if (this->packet->headers().type == TRANSACTION_POOL_REQUEST_PACKET)
        {
            // Send the transaction pool to the client
            msgpack = TransactionPool::serialize(this->transactionPool);
            payload = msgpack.dump();

            response = new DTP::Packet(TRANSACTION_POOL_DATA_PACKET, std::string(this->uuid), std::string(nodeIP), nodePort, payload);
        }
        else if (this->packet->headers().type == TRANSACTION_PACKET)
        {
            // Accept the new transaction and add it to the transaction pool
            Transaction transaction(this->packet->getPayload());
            this->transactionPool->setTransaction(&transaction);

            std::cout << "[INFO] New transaction received from " << this->nodeIP << ":" << this->nodePort << " with ID " << transaction.getID() << std::endl;
        }
        else
        {
            std::cout << "[ERROR] Unknown packet type: " << this->packet->headers().type << std::endl;
        }

        /*char buf[response->buffer().length()];

        strcpy(buf, response->buffer());
        std::cout << "Buffer sent: " << buf << std::endl;

        int bytes = send(new_socket, buf, 3000000, 0);*/

        //std::cout << "[INFO] Sent " << bytes << " bytes to " << this->nodeIP << ":" << this->nodePort << std::endl;
        close(new_socket);
        return;
    }

    send(new_socket, "null", strlen("null"), 0);
    close(new_socket);
    return;
}

/* PUBLIC FUNCTIONS */
// This function launches the server by adding work to the threading pool work queue
void NodeServer::launch()
{
    std::cout << "[INFO] Node server launched" << std::endl;

    tp->do_work([&]() {
        while(true)
        {
            accepter();
            handler();
            responder();
        }
    });
}

void NodeServer::set_working_blockchain(Blockchain* blockchain)
{
    this->blockchain = blockchain;
    std::cout << "[INFO] Working Blockchain instance set" << std::endl;
}

void NodeServer::set_working_transaction_pool(TransactionPool* transactionPool)
{
    this->transactionPool = transactionPool;
    std::cout << "[INFO] Working Transaction Pool instance set" << std::endl;
}

void NodeServer::set_node_uuid(char* uuid)
{
    this->uuid = uuid;
    std::cout << "[INFO] Node UUID set" << std::endl;
}