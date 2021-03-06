/* Gnomescroll, Copyright (c) 2013 Symbolic Analytics
   Licensed under GPLv3 */
#pragma once

//#include <net_lib/common/sequencer.h>
//#include <net_lib/client/pviz.h>
//#include <net_lib/common/sequencer.h>
//#include <net_lib/common/python_channel.hpp>

#include <net_lib/common/types.hpp>
#include <net_lib/common/packet_buffer.hpp>

#include <net_lib/enet/enet.h>

#include <time.h>

class NetMessageArray; //forward declaration

struct PacketSequence
{
    class NetMessageArray* nma;   //pointer to buffer
    short read_index;  //starting index in buffer
    short messages_n;    //number of packets

    short seq;
    short ack;
    int time;

    //int retry_attempts; //can only use for reliable, ordered packet sequences
};

struct PacketSequence2
{
    short seq;
    short received;
};

/*
NetPeer
*/


//struct ENetPeer;

const int NET_PEER_MAP_MESSAGE_BUFFER_DEFAULT = 4096;

class NetPeer
{
    private:

    public:
        ClientID client_id;
        int connected;
        int version;
        DisconnectType disconnect_code;

        ENetPeer* enet_peer;

        //uint32_t ip;
        //uint16_t port;

    void flush_to_net();

    class NetMessageManager reliable_message_manager;
    class NetMessageManager unreliable_message_manager;
    //class NetMessageManager python_message_manager;

/*
    void push_unreliable_message(class NetMessage* nm) __attribute__((always_inline));
    void push_reliable_message(class NetMessage* nm) __attribute__((always_inline));
    void push_python_message(class NetMessage* nm) __attribute__((always_inline));
*/

    void push_unreliable_message(class NetMessage* nm);
    void push_reliable_message(class NetMessage* nm);
    //void push_python_message(class NetMessage* nm);

    bool version_match()
    {   // will return true if version has not been set yet
        return (this->version == 0 || this->version == GS_VERSION);
    }

    bool force_disconnected()
    {
        return (this->disconnect_code == DISCONNECT_FORCED);
    }

    bool full()
    {
        return (this->disconnect_code == DISCONNECT_FULL);
    }

    bool should_disconnect()
    {   // returns true if the disconnect code was set to a value that should indicate force disconnect
        return (this->disconnect_code != DISCONNECT_NONE && this->disconnect_code != DISCONNECT_TIMEOUT);
    }

    void flush_map_messages();
    void resize_map_message_buffer(size_t size_min);

    char* map_message_buffer;
    size_t map_message_buffer_index;
    size_t map_message_buffer_max;
    /*
        packet sequencer
    */

    //x to y: x state
    struct PacketSequence packet_sequence_buffer[256];
    int packet_sequence_number;
    //x to y: y state
    struct PacketSequence2 seqbuff[256];
    int highest_packet_sequence_number;

    void init() {}

    NetPeer();
    ~NetPeer();

};
