#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <csignal>
#include <chrono>
#include <thread>
#include <cstring>
#include "subsystem.hpp"

#define USAGE                                                          \
  "usage:\n"                                                           \
  "  echoclient [options]\n"                                           \
  "options:\n"                                                         \
  "  -s                  Server (Default: localhost)\n"                \
  "  -p                  Port (Default: 10823)\n"                      \
  "  -n                  Number of Subsystems (Default: 1)\n"          \
  "  -h                  Show this help message\n"

/* OPTIONS DESCRIPTOR ====================================================== */
static struct option g_long_options[] = {
    {"server", required_argument, NULL, 's'},
    {"port", required_argument, NULL, 'p'},
    {"subsystems", required_argument, NULL, 'n'},
    {"help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}};

constexpr auto CYCLE_TIME = std::chrono::microseconds(1000000LL); // 1 Hz

void sigintHandler(int signum){
  printf("Terminating subsystem program...");
  exit(signum);
}

int main(int argc, char* argv[]){
  std::cout << "Starting subsystem..." << std::endl;

  std::signal(SIGINT, sigintHandler);

  uint16_t portno = 10823;
  int option_char = 0;
  std::string hostname = "localhost";
  uint8_t num_subsystems = 1;

  // Parse and set command line arguments
  while ((option_char =
              getopt_long(argc, argv, "p:s:n:hx", g_long_options, NULL)) != -1) {
    switch (option_char) {
      default:
        fprintf(stderr, "%s", USAGE);
        exit(1);
      case 's':  // server
        hostname = optarg;
        break;
      case 'p':  // listen-port
        portno = atoi(optarg);
        break;
      case 'n': // number of subsystems
        if(optarg == 0){
          std::cout << "Number of subsystems cannot be 0" << std::endl;
          exit(1);
        }
        num_subsystems = atoi(optarg);
        break;
      case 'h':  // help
        fprintf(stdout, "%s", USAGE);
        exit(0);
        break;
    }
  }

  setbuf(stdout, NULL);  // disable buffering

  if ((portno < 1025) || (portno > 65535)) {
    fprintf(stderr, "%s @ %d: invalid port number (%d)\n", __FILE__, __LINE__,
            portno);
    exit(1);
  }

  if (hostname.empty()) {
    fprintf(stderr, "%s @ %d: invalid host name\n", __FILE__, __LINE__);
    exit(1);
  }

  int sock_fd;
  struct sockaddr_in servaddr;

  // Creating socket file descriptor
  if ( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(portno);
  servaddr.sin_addr.s_addr = INADDR_ANY;

  // Initialize subsystems and buffer
  unsigned char buffer[sizeof(float) * num_subsystems];
  SubsystemManager sub_manager(num_subsystems);

  while(1){
    auto start_time = std::chrono::system_clock::now();

    // send/receive data
    socklen_t len = sizeof(servaddr);

    sub_manager.cycle(&buffer[0]);

    sendto(sock_fd, (const char *)buffer, sizeof(buffer),
          MSG_CONFIRM, (const struct sockaddr *) &servaddr,
          len);

    printf("Temperature sent.\n");

    auto end_time = std::chrono::system_clock::now();

    auto delta = end_time - start_time;

    std::this_thread::sleep_for(CYCLE_TIME - delta);
  }

  close(sock_fd);
  return 0;
}