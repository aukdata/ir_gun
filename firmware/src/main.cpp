/* Name: main.cpp
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include "port.hpp"

int main()
{
    gb7::port_writable<gb7::Port::PortB> pb;

    for(;;)
    {
        pb.write(~pb.read());
    }
    return 0;   /* never reached */
}
