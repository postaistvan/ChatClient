/*
 * ClientService.cpp
 *
 *  Created on: Sep 4, 2017
 *      Author: istvan
 */

#include "ClientService.h"
#include "Logger.h"

#include <iostream>
#include <memory>
#include <thread>

ClientService::ClientService():
	stream(nullptr),
	connected(false),
	username("user1"),
	password("1234"),
	log_tag("ClientService")
{

}

bool ClientService::connect() {
	try {

		log_info("Connecting...");
		stream = DlibConnectionStream(new dlib::iosockstream("192.168.0.105:1234"));
		log_success("Connected");

		connected = connection_handshake();

		if (connected)
			log_success("Handshake successful");

		else {
			stream = nullptr;
			log_error("Handshake unsuccessful");
		}

	} catch (std::exception& e) {

		log_error(e.what());
		connected = false;
		log_error("Not connected");
	}

	return connected;
}

bool ClientService::connection_handshake() {
	send_login_data();
	return process_login_response();
}

void ClientService::log_info(const std::string& msg) {
	Logger::console_info(log_tag, msg);
}

void ClientService::log_success(const std::string& msg) {
	Logger::console_success(log_tag, msg);
}

void ClientService::log_warning(const std::string& msg) {
	Logger::console_warning(log_tag, msg);
}

void ClientService::log_error(const std::string& msg) {
	Logger::console_error(log_tag, msg);
}

void ClientService::send_login_data() {
	std::string login_data;

	json_handler.add_element("type", "login");
	json_handler.add_element("username", username);
	json_handler.add_element("password", password);

	login_data = json_handler.get_json_string();
	send(login_data);
}

bool ClientService::process_login_response() {

	if (stream != nullptr) {
		std::string resp;

		*stream >> resp;

		log_info("Server response: " + resp);

		json_handler.clear();
		json_handler.construct_json(resp);

		if (json_handler.get_element_value("type") == "login") {

			if (json_handler.get_element_value("result") == "valid")
				return true;
			else
				return false;

		} else
			return false;

	} else
		return false;
}

ClientService::~ClientService() {
	// TODO Auto-generated destructor stub
}

void ClientService::receiver() {
	std::string msg;


	while (true) {
		getline(*stream, msg);

		json_handler.clear();
		json_handler.construct_json(msg);

		message_cb("server", json_handler.get_element_value("text"));
	}
}

void ClientService::start_receiver(MessageCallback message_cb) {
	this->message_cb = message_cb;

	std::thread t(&ClientService::receiver, this);
	t.detach();
}

void ClientService::send(const std::string& message) {
	log_info("Sending message: " + message);
	if (stream != nullptr)
		*stream << message << endl;
	else
		log_error("Can't send the message because the client is not connected");
}
