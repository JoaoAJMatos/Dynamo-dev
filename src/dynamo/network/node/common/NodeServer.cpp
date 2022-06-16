//
// Created by joaoa on 21/04/2022.
//

#include "NodeServer.h"

/* HELPER FUNCTIONS */
void toFile(std::string content)
{
    std::ofstream tempFile;
    tempFile.open("temp.txt");
    tempFile << content;
    tempFile.close();
}

std::string fromFile()
{
    std::string line;
    std::ifstream myfile("temp.txt");
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            myfile.close();
            return line;
        }
    }
}

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

            // Send packet informing the File transfer will begin
            response = new DTP::Packet(BLOCKCHAIN_DATA_PACKET, std::string(this->uuid), std::string(nodeIP), nodePort, std::string(""));
            send(new_socket, response->buffer().c_str(), BUFFER_SIZE, 0);
            
            // Wait for the client to acknowledge the packet and send an FTP Ready packet
            recv(new_socket, buffer, BUFFER_SIZE, 0);
            delete response;
            response = new DTP::Packet(buffer);

            if (response->getIndicator() == DTP_INDICATOR && response->headers().type == FTP_READY)
            {
                // Send the blockchain to the client
                ftp_transfer(payload);
            }
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

int NodeServer::send_file(FILE* fp, int sockfd)
{
    char data[PACKET_SIZE] = {0};

    while(fgets(data, PACKET_SIZE, fp) != NULL)
    {
        if(send(sockfd, data, sizeof(data), 0) < 0)
        {
            std::cout << "[ERROR] (At NodeServer::send_file(2)): Failed to send file" << std::endl;
            return -1;
        }
        bzero(data, PACKET_SIZE);
    }

    return 0;
}

int NodeServer::receive_file(int sockfd)
{
    int n;
    FILE* fp;
    char* filename = "temp.txt";
    char buffer[PACKET_SIZE];

    fp = fopen(filename, "w");
    if (fp == nullptr)
    {
        std::cout << "[ERROR] (At NodeServer::receive_file(1)): Failed to open file for writing" << std::endl;
        return -1;
    }

    while(true)
    {
        n = recv(sockfd, buffer, PACKET_SIZE, 0);
        if (n <= 0) break;
        fprintf(fp, "%s", buffer);
        bzero(buffer, PACKET_SIZE);
    }

    return 0;
}

int NodeServer::ftp_transfer(std::string payload)
{
    toFile(payload); // Write the contents of the serialized blockchain to a temp file
    FILE* fp;
    fp = fopen("temp.txt", "r");

    int res;
    DTP::Packet* resp;

    if (fp != nullptr)
    {
        int tries = 0;
        while(true)
        {
            res = send_file(fp, new_socket); // Send the file to the client
            tries++;

            if (res == 0) // If the file was successfully sent, wait for an ACK from the client
            {
                recv(new_socket, buffer, BUFFER_SIZE, 0);
                resp = new DTP::Packet(buffer);

                if ((resp->getIndicator() == DTP_INDICATOR) && (resp->headers().type == ACK)) break;
                if (tries == 10)
                {
                    std::cout << "[WARNING] The server was unable to ensure the file has been received by the client" << std::endl;
                    return -1;
                }
            }
        }
    }

    return 0;
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