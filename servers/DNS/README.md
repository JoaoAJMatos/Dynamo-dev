# DDNS (Dynamo-DNS)

Dynamo's DNS servers provide a list of known addresses.
They are the first point of contact of any new node attempting
to connect to the network.
 
Once a new node is started, it sends a DNS query to one of the
predefined DNS servers in the Dynamo network. If that name server
happens to be down, the node will attempt to establish a connection
3 more times before connecting to a new one.

Once the connection is successful, the node sends the DNS query to
the server. The server responds by sending its list of known hosts
and stores the new node's IP in the host list.
After updating its known hosts list, the other name servers have to sync
their list with it. The DNS server will then send a `sync` request to all
the other name servers.

If the default nameserver happens to be busy, meaning the server's backlog
is full, it can shift its workload to one of the other default name servers.
The connection is redirected automatically after the `redirect [IP]` command
is issued to the incoming nodes (where [IP] is the IP address of the other NS).

Sometimes, on startup, a node may have a list of already known hosts. If that is
the case, the node will send a list of all the hosts it knows, and the server will
only respond with a list of the remaining existing nodes. Keep in mind that at this
point, the request doesn't need to be handled by an actual name server, since the
node already knows some hosts, it can prompt the other hosts for an updated host list.
Although the nodes will always prioritise the responses coming from the name servers.

All the nodes known hosts lists must be updated systematically, as well as the name
server's. All the registries inside the host lists have a TTL (time to live) of 60
minutes, meaning that after 60 minutes of no interaction, the name server will ping
the node to check if it is up. If the node responds, the timestamp of the registry is
updated, therefore resetting the clock. If it doesn't respond, the registry is deleted
from the list, and all the name servers update their list.

This workload can be distributed over all the name servers, meaning that a single name
server doesn't need to be responsible for pinging every node once the TTL ends. If there
are 2 name servers, the list will be split into 2, and so on...