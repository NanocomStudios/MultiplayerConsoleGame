#include <iostream>
#include "server.h"
#include "client.h"

using namespace std;

int main(int argc, char** argv) {
	if (argc >= 2) {
		if (*argv[1] == 'c') {
			client(argv[1]);
		}
		else if (*argv[1] == 's') {
			server();
		}
	}
}