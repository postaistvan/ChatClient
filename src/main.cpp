/*
 * main.cpp
 *
 *  Created on: Sep 4, 2017
 *      Author: istvan
 */

#include "ClientService.h"

#include <iostream>

using namespace std;

ClientService client;

void receiver(const std::string& username, const std::string& message) {
	cout << '[' << username << "] " << message << endl;
}

void start_sender() {
	std::string message;

	while (true) {
		cout << ':';
		cin >> message;

		client.send(message);
	}
}

int main() {

	if (client.connect()) {
		client.start_receiver(receiver);
		start_sender();
	}

	return 0;
}
