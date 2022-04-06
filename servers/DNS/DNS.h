//
// Created by joaoa on 05/04/2022.
//
// Dynamo's DNS servers provide a list of known addresses.
// They are the first point of contact of any new node attempting
// to connect to the network.
//
// Once a new node is started, it sends a DNS query to one of the
// predefined DNS servers in the Dynamo network. If that name server
// happens to be down, the node will attempt to establish a connection
// 3 more times before connecting to a new one.
//
// Once the connection is successful, the node sends the DNS query to
// the server. The server responds by sending its list of known hosts
// and stores the new node's IP in the host list.
//
// After updating its known hosts list, the other name servers have to sync
// their list with it. The DNS server will then send a `sync` request to all
// the other name servers.
//
// If the default nameserver happens to be busy, meaning the server's backlog
// is full, it can shift its workload to one of the other default name servers.
// The connection is redirected automatically after the `redirect [IP]` command
// is issued to the incoming nodes (where [IP] is the IP address of the other NS).
//
// Sometimes, on startup, a node may have a list of already known hosts. If that is
// the case, the node will send a list of all the hosts it knows, and the server will
// only respond with a list of the remaining existing nodes. Keep in mind that at this
// point, the request doesn't need to be handled by an actual name server, since the
// node already knows some hosts, it can prompt the other hosts for an updated host list.
// Although the nodes will always prioritise the responses coming from the name servers.
//
// All the nodes known hosts lists must be updated systematically, as well as the name
// server's. All the registries inside the host lists have a TTL (time to live) of 60
// minutes, meaning that after 60 minutes of no interaction, the name server will ping
// the node to check if it is up. If the node responds, the timestamp of the registry is
// updated, therefore resetting the clock. If it doesn't respond, the registry is deleted
// from the list, and all the name servers update their list.
//
// This workload can be distributed over all the name servers, meaning that a single name
// server doesn't need to be responsible for pinging every node once the TTL ends. If there
// are 2 name servers, the list will be split into 2, and so on...

#ifndef DEV_DYNAMO_DNS_H
#define DEV_DYNAMO_DNS_H

#include <map>

#include "../../src/system/threading/ThreadPool.h"
#include "../../src/system/time/Time.h"
#include "../../src/networking/sockets/common/Socket.h"
#include "../../src/networking/server/servers.h"
#include "../../src/util/uuid/uuid.h"

namespace servers
{
    class DNS: private net::BasicServer
    {
    private:
        /* MEMBER VARIABLES */
        // Dictionary of all the known hosts
        //    - Key: UUID of the node
        //    - Object: Socket object of the known hosts
        std::map<boost::uuids::uuid, sockaddr_in> known_hosts;

        // Dictionary for timestamping connections
        //    - Key: UUID of the node
        //    - Object: Timestamp of the last connection made
        std::map<boost::uuids::uuid, std::time_t> connection_table;

        // Variables regarding the server
        int domain;
        int service;
        int protocol;
        int port;
        unsigned long iface;
        int backlog;

        // Multi threading and thread pool instance
        int number_of_threads;
        sys::ThreadPool* tp;

        /* VIRTUAL FUNCTION OVERRIDES */
        // TODO: Define the virtual functions
        void accepter() override {};
        // The handler is used in a multi threaded environment to handle incoming connections
        void handler() override {};
        void responder() override {};

    public:
        /* CONSTRUCTOR */
        // The constructor for the class takes in the expected parameters to create a basic server
        // defined in BasicServer.h, as well as the number of threads the server will have in the thread
        // pool. If the number of threads isn't specified, the default value is used.
        DNS(int domain, int service, int protocol, int port, unsigned long iface, int bklg, int thread_count = DEFAULT_THREAD_COUNT);

        /* GETTERS */
        std::map<boost::uuids::uuid, sockaddr_in> get_known_hosts();
        std::map<boost::uuids::uuid, std::time_t> get_connection_table();
    };
}


#endif //DEV_DYNAMO_DNS_H
