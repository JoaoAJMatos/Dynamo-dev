//
// Created by joaoa on 21/04/2022.
//

#include "NodeServer.h"

#include <utility>

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

    return "";
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
int NodeServer::isKnownHost(std::string ip, int port)
{
    for (auto& host : *known_hosts)
    {
        if (host.first == ip && host.second == port) return 1;
    }

    return 0;
}

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

        std::string ip(nodeIP);
        int servPort = this->packet->headers().serverPort;
        std::string destination = this->packet->headers().destination;

        // Check if the incomming node is in the known hosts list, if not, add it
        if (destination != std::string("all") && servPort > 0 && (!isKnownHost(ip, servPort)))
        {
            known_hosts->push_back({ip, servPort});
        }
    }
    catch(const std::exception& e)
    {
        std::cout << "[ERROR] Unable to create packet from buffer: " << e.what() << std::endl;
    }

    return;
}

void NodeServer::responder() // After responding to the incoming message the responder should set the broadcast flag of the node to true
{                            // in order for the node to know when to broadcast the message to all the other nodes
    std::string payload;

    using namespace msgpack11;
    MsgPack msgpack;

    DTP::Packet* response;

    if (this->packet->getIndicator() == DTP_INDICATOR)
    {
        if (this->packet->headers().type == BLOCKCHAIN_REQUEST_PACKET)
        {
            // Send packet informing the File transfer will begin
            std::string payload = Blockchain::toString(*this->blockchain);
            response = new DTP::Packet(BLOCKCHAIN_DATA_PACKET, std::string(this->uuid), std::string(nodeIP), this->port, nodePort, payload);
            send(new_socket, response->buffer().c_str(), BUFFER_SIZE, 0);
        }
        else if (this->packet->headers().type == BLOCKCHAIN_DATA_PACKET)
        {
            // Create a new Blockchain from the incomming packet payload
            // validate it and replace the current blockchain with the new one
        
            std::cout << "Receiving chain data..." << std::endl;

            Blockchain new_chain(this->packet->getPayload());
            if(this->blockchain->replaceChain(new_chain) == 0)
            {
                std::cout << "[INFO] Blockchain instance updated to " << nodeIP << " node state" << std::endl;
            }

            return;

            // TODO: Implement DFTP for blockchain data
            
            /*std::cout << "Updating blockchain" << std::endl;

            receive_file(new_socket, "temp.txt");

            std::string chainString = fromFile();
            Blockchain new_chain(chainString);
            
            if(this->blockchain->replaceChain(new_chain) == 0)
            {
                std::cout << "[INFO] Blockchain instance updated to " << nodeIP << " node state" << std::endl;
            }*/
        }
        else if (this->packet->headers().type == NEW_BLOCK_PACKET)
        {
            std::cout << "Received new block packet: " << std::endl;

            // Create a new Block from the incomming packet payload and add it to the chain, then, try to replace the chain
            Block new_block = Block(this->packet->getPayload());
            Blockchain temp_chain(Blockchain::toString(*this->blockchain));
            temp_chain.chain.push_back(&new_block);
            
            if(this->blockchain->replaceChain(temp_chain) == 0)
            {
                std::cout << "[INFO] Blockchain instance updated to " << nodeIP << " node state" << std::endl;
                this->transactionPool->clearBlockchainTransactions(this->blockchain);
            }
        }
        else if (this->packet->headers().type == TRANSACTION_POOL_REQUEST_PACKET)
        {
            std::string payload = std::to_string(this->transactionPool->getPool().size());
            response = new DTP::Packet(TRANSACTION_POOL_DATA_PACKET, std::string(this->uuid), std::string(nodeIP), this->port, nodePort, payload);
            response->show();
            send(new_socket, response->buffer().c_str(), BUFFER_SIZE, 0);
        }
        else if (this->packet->headers().type == TRANSACTION_PACKET)
        {
            // Accept the new transaction and add it to the transaction pool
            Transaction* transaction = new Transaction(this->packet->getPayload());

            this->transactionPool->setTransaction(transaction);

            if (transaction->getOutputMap().recipient.c_str() == this->address) // Show a notification if the transaction is for this node's wallet
            {
                // TODO: Fix this notification thingy
                this->notification_buffer->assign("Transaction received");
            }
        }
        else if (this->packet->headers().type == FTP_READY)
        {
            if (this->packet->getPayload() == std::string("1"))
            {
                // Send the blockchain to the client
                std::string serializedChain = Blockchain::toString(*this->blockchain);

                // Send the blockchain file
                ftp_transfer(serializedChain);
            }

            if (this->packet->getPayload() == std::string("2"))
            {
                std::string serializedPool = TransactionPool::toString(this->transactionPool);

                std::cout << "Sending pool: " << serializedPool << std::endl;

                // Send the transaction-pool file
                ftp_transfer(serializedPool);
            }
        }
        else
        {
            std::cout << "[ERROR] Unknown packet type: " << this->packet->headers().type << std::endl;
        }

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
    while(1)
    {
        char data[PACKET_SIZE] = {0};
        int nread = fread(data, 1, PACKET_SIZE, fp);

        if(nread > 0)
        {
            send(sockfd, data, nread, 0);
        }
        if(nread < PACKET_SIZE)
        {
            if(!feof(fp)) return -1; // Return error if there was an error reading the file
            break;
        }
    }

    return 0;
}

int NodeServer::receive_file(int sockfd, char* name)
{
    int n;
    FILE* fp;
    char* filename = filename;
    char buf[PACKET_SIZE];
    int bytesReceived = 0;

    fp = fopen(filename, "ab");
    if (fp == nullptr)
    {
        std::cout << "[ERROR] (At NodeServer::receive_file(1)): Failed to open file for writing" << std::endl;
        return -1;
    }

    long double sz = 1;

    while((bytesReceived = read(sockfd, buf, PACKET_SIZE)) > 0)
    {
        sz++;
        gotoxy(0, 4);
        printf("Received: %Lf Mb", (sz/PACKET_SIZE));
        fflush(stdout);
        fwrite(buf, 1, bytesReceived, fp);
    }

    if (bytesReceived < 0)
    {
        std::cout << "[ERROR] (At NodeServer::receive_file(2)): Failed to read from socket" << std::endl;
        return -1;
    }

    return 0;
}

int NodeServer::ftp_transfer(std::string payload)
{
    toFile(std::move(payload)); // Write the contents of the serialized blockchain to a temp file
    FILE* fp = fopen("temp.txt", "rb");

    int res;

    if (fp != nullptr)
    {
        res = send_file(fp, new_socket); // Send the file to the client
    }

    return res;
}

void NodeServer::set_working_blockchain(Blockchain* chain)
{
    this->blockchain = chain;
    std::cout << "[INFO] Working Blockchain instance set" << std::endl;
}

void NodeServer::set_working_transaction_pool(TransactionPool* pool)
{
    this->transactionPool = pool;
    std::cout << "[INFO] Working Transaction Pool instance set" << std::endl;
}

void NodeServer::set_node_uuid(char* node_id)
{
    this->uuid = node_id;
    std::cout << "[INFO] Node UUID set" << std::endl;
}

void NodeServer::set_known_hosts(std::vector<std::pair<std::string, int>>* known_hosts_list)
{
    this->known_hosts = known_hosts_list;
    std::cout << "[INFO] Known hosts set" << std::endl;
}

int NodeServer::getPort() const
{
    return this->port;
}

void NodeServer::set_notification_buffer(std::string* buf)
{
    this->notification_buffer = buf;
}

void NodeServer::set_address(char* addr)
{
    this->address = addr;
}