#include <iostream>
#include <csignal>
#include <ctime>
#include <cstdio>
#include <cstdlib>

#include "TCPServer.h"

TCPServer tcp;
pthread_t msg1[MAX_CLIENT];
int num_message = 0;

void close_app(int s) {
	tcp.closed();
	exit(0);
}

void * received(void * m)
{
        pthread_detach(pthread_self());
	vector<descript_socket*> desc;

	FILE* fp = std::fopen("/tmp/log.txt", "w+");
	if (!fp)
	{
		std::perror("File opening failed");
		return 0;
	}

	while(1)
	{
		desc = tcp.getMessage();
		for(unsigned int i = 0; i < desc.size(); i++) {
			if( desc[i] )
			{
				std::fputs(desc[i]->message.c_str(), fp);
				tcp.clean(i);
			}
		}
		usleep(1000);
	}
	std::fclose(fp);
	return 0;
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cerr << "Usage: ./server port" << endl;
		return 0;
	}

	std::signal(SIGINT, close_app);

	pthread_t msg;
        vector<int> opts = { SO_REUSEPORT, SO_REUSEADDR };

	if( tcp.setup(atoi(argv[1]),opts) == 0) {
		if( pthread_create(&msg, NULL, received, (void *)0) == 0)
		{
			while(1) {
				tcp.accepted();
			}
		}
	}
	else
		cerr << "Error setup" << endl;
	return 0;
}
