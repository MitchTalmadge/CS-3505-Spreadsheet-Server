/*
main.cpp

Startup the server. Specifically, create and start the TCP Server, which will create
and start other control/model parts of the program. Once this is done, listen for user
input to handle input from the console.
*/

#include "main.h"
#include <controller/network/tcp_server.h>
#include <iostream>
#include <string>

/*
Start the server, and setup different threads to handle user input and networking code.
*/
int main(int argc, char **argv) {
    std::cout << "Starting Server." << std::endl;

    // Create and start our TCP server which creates and starts controllers/models.
    tcp_server server;
    bool setup = server.startup();

    if (!setup) {
      std::cout << "Shutting down server." << std::endl;
      return -1;
    }

    std::string input;

    // Start our I/O loop that handles user input.
    while (true) {

        std::cin >> input;

        if (!input.empty()) {
            // Handle the input.
            if (input == "shutdown") {
	      std::cout << "Shutting down server." << std::endl;

	      return 0;
            } else {
                // Default unsupported message.
                std::cout << "Unsupported action: \"" << input << "\"" << std::endl;
            }
        }
    }
}
