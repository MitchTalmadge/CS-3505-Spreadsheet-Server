/*
server.cpp

Startup the server. Specifically, create and start the TCP Server, which will create
and start other control/model parts of the program. Once this is done, listen for user
input to handle input from the console.
*/

#include <iostream>
#include "tcp_server.h"
#include <string>

/*
Start the server, and setup different threads to handle user input and networking code.
*/
int main()
{
  std::cout << "Starting Server." << std::endl;
  
  // Create and start our TCP server which creates and starts controllers/models.
  tcp_server server;
  server.startup();

  std::string input;

  // Start our I/O loop that handles user input.
  while(true) {

    std::cin >> input;

    // Handle the input.
    if (input == "shutdown") {
      // Shut everything down cleanly.
      server.shut_down();
      return 0;
    } else {
      // Default unsupported message.
      std::cout << "Unsupported action: \"" << input << "\"" << std::endl;
    }
  }
}
