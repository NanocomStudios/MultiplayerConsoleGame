#include <iostream>
#include "server.h"
#include "client.h"

using namespace std;

int main(int argc, char** argv) {
	if (argc == 2) {
		if (*argv[1] == 'c') {
			client();
		}
		else if (*argv[1] == 's') {
			server();
		}
	}
}