/*
data_container.h

Data container holds shared data between the network controller and the model.
This is meant to consolidate the areas where multiple threads will need to be 
able to access shared data.
 */

#ifndef PIGRAMMERS_SPREADSHEET_SERVER_DATA_CONTAINER
#define PIGRAMMERS_SPREADSHEET_SERVER_DATA_CONTAINER

#include <queue>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <mutex>
#include <model/packet/inbound/inbound_packet.h>
#include <model/packet/outbound/outbound_packet.h>
#include <boost/optional/optional.hpp>

class data_container {

private:

  /**
   * Map from spreadsheet name to incoming message queue.
   */
  std::queue<inbound_packet *> inbound_packets_;
  std::mutex inbound_packets_mutex_;

  /**
   * Map from socket_id_ to outgoing message queue.
   */
    std::map<int, std::queue<outbound_packet *> > outbound_packets_;
    std::mutex outbound_packets_mutex_;

    /**
     * Know who disconnected so spreadsheet_controller can learn of it.
     */
    std::set<int> disconnected_clients_;
    std::mutex disconnected_clients_mutex_;

    /**
     * Private constructor for singleton pattern.
     */
    data_container() = default;

    /**
     * Private destructor for singleton pattern.
     */
    ~data_container();

public:

    /**
     * @return The singleton instance of this container.
     */
    static data_container &get_instance();

    /**
     * Marks a socket ID as having been connected.
     * @param socket_id The ID of the registered socket.
     */
    void register_socket(int socket_id);

    /**
     * Marks a socket ID as having been disconnected.
     * @param socket_id The ID of the disconnected socket.
     */
    void remove_socket(int socket_id);

    /**
     * Deleted copy constructor since this is a singleton.
     */
    data_container(data_container const &) = delete;

    /**
     * Deleted assignment operator since this is a singleton.
     */
    void operator=(data_container const &)  = delete;

    /**
     * Get a message from the inbound queue. Called by the spreadsheet controller.
     */
    inbound_packet * get_inbound_packet();

    /**
     * Adds a new packet to the inbound packet queue, to be retrieved one-at-a-time later.
     */
    void new_inbound_packet(inbound_packet &packet);

    /**
     * Get outbound message from the given socket's queue.
     */
    outbound_packet * get_outbound_packet(int socket_id);

    /**
     * Send new message to outbound queue for given socket.
     * @return if queued successfully. False indicates the socket is no longer
     * active and can be removed.
     */
    bool new_outbound_packet(int socket_id, outbound_packet &packet);

};

#endif
