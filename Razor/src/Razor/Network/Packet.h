#pragma once

#include "Protocol.h"

class Packet
{
    struct Login
    {
        std::string username, password;

        void write(Protocol::Buffer& buffer)
        {
            Protocol::write_string(buffer, username);
            Protocol::write_string(buffer, password);
        }

        void read(Protocol::Buffer& buffer)
        {
            Protocol::read_string(buffer, &username);
            Protocol::read_string(buffer, &password);
        }
    };

};

