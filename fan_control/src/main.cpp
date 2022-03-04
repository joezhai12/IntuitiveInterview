#include <iostream>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <getopt.h>
#include <netdb.h>
#include <limits.h>
#include <csignal>
#include <cstring>
#include <chrono>
#include <thread>
#include "fan_manager.hpp"
#include "fan_config_reader.hpp"

#define MAX_SUBS 256 // maximum subsystems defined as uint8_t

#define USAGE                                                        \
  "usage:\n"                                                         \
  "  echoserver [options]\n"                                         \
  "options:\n"                                                       \
  "  -p                  Port (Default: 10823)\n"                    \
  "  -h                  Show this help message\n"

/* OPTIONS DESCRIPTOR ====================================================== */
static struct option g_long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"port", required_argument, NULL, 'p'},
    {NULL, 0, NULL, 0}};

constexpr auto CYCLE_TIME = std::chrono::microseconds(1000000LL); // 1 Hz

void sigintHandler(int signum){
  printf("Terminating subsystem program...");
  exit(signum);
}

int main(int argc, char* argv[]){
  std::cout << "Starting subsystem..." << std::endl;

  std::signal(SIGINT, sigintHandler);

  int option_char;
  int portno = 10823; /* port to listen on */

  // Parse and set command line arguments
  while ((option_char =
              getopt_long(argc, argv, "hx:p:", g_long_options, NULL)) != -1) {
    switch (option_char) {
      case 'p':  // listen-port
        portno = atoi(optarg);
        break;
      case 'h':  // help
        fprintf(stdout, "%s ", USAGE);
        exit(0);
        break;
      default:
        fprintf(stderr, "%s ", USAGE);
        exit(1);
    }
  }

  setbuf(stdout, NULL);  // disable buffering

  if ((portno < 1025) || (portno > 65535)) {
    fprintf(stderr, "%s @ %d: invalid port number (%d)\n", __FILE__, __LINE__,
            portno);
    exit(1);
  }

  int sock_fd;
  struct sockaddr_in servaddr, cliaddr;

  // Creating socket file descriptor
  if ( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Server
  servaddr.sin_family      = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port        = htons(portno);

  // Bind the socket with the server address
  if ( bind(sock_fd, (const struct sockaddr *)&servaddr,
        sizeof(servaddr)) < 0 )
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // parse fan configuration file and set max pwm counts
  std::vector<std::pair<uint32_t, std::string>> fan_config;
  read_fan_config("./config/fan_configuration.csv", fan_config);

  unsigned char buffer[MAX_SUBS*sizeof(float)];
  FanManager fan_manager(fan_config);

  while(1){
    auto start_time = std::chrono::system_clock::now();

    socklen_t len = sizeof(cliaddr);

    int n = recvfrom(sock_fd, (char *)buffer, sizeof(buffer),
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &len);
    if(n < 0){
      std::cout << "Fan Controller: Error receiving subsystem temperatures..." << std::endl;
      exit(EXIT_FAILURE);
    }
    else if(n == 0){
      std::cout << "Fan Controller: No data received..." << std::endl;
      continue;
    }

    fan_manager.cycle(&buffer[0], n);

    auto end_time = std::chrono::system_clock::now();

    auto delta = end_time - start_time;

    std::this_thread::sleep_for(CYCLE_TIME - delta);
  }

  close(sock_fd);
  return 0;
}