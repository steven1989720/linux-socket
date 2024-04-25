#include <iostream>
#include <signal.h>
#include "TCPClient.h"

TCPClient tcp;

void sig_exit(int s)
{
	tcp.exit();
	exit(0);
}

int main(int argc, char *argv[])
{
	if(argc != 4) {
		cerr << "Usage: ./client name ip port period" << endl;
		return 0;
	}
	signal(SIGINT, sig_exit);

	tcp.setup(argv[2],atoi(argv[3]));
	while(1)
	{
		// [yyyy-mm-dd hh:mm:ss.ms] "client_name"
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		int hour = now->tm_hour;
		int min  = now->tm_min;
		int sec  = now->tm_sec;
		std::string message = "[" +
			    to_string(now->tm_year + 1900) + "-" +
			    to_string(now->tm_mon + 1)     + "-" +
			    to_string(now->tm_mday)        + " " +
			    to_string(hour)                + ":" +
			    to_string(min)                 + ":" +
			    to_string(sec)                 + "] ";
			    to_string(argv[1])             + "\r\n";

		tcp.Send(message);

		sleep(atoi(argv[4]));
	}
	return 0;
}
