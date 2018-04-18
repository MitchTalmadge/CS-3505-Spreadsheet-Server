#ifndef PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_EDIT_PACKET_H
#define PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_EDIT_PACKET_H


#include "inbound_packet.h"

class inbound_edit_packet : inbound_packet {

    /**
     * The name of the cell that is being edited.
     */
    std::string cell_name_;

    /**
     * The new contents of the cell.
     */
    std::string cell_contents_;

public:

    inbound_edit_packet(int socket_id_, const std::string &raw_contents_, const std::string &cell_name_,
                        const std::string &cell_contents_);

    const std::string &get_cell_name_() const;

    const std::string &get_cell_contents_() const;

};


#endif //PIGRAMMERS_SPREADSHEET_SERVER_INBOUND_EDIT_PACKET_H
