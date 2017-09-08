/*
 * ClientService.h
 *
 *  Created on: Sep 4, 2017
 *      Author: istvan
 */

#ifndef CLIENTSERVICE_H_
#define CLIENTSERVICE_H_

#include <dlib/iosockstream.h>
#include <memory>
#include "JsonHandler.h"

typedef std::unique_ptr<dlib::iosockstream> DlibConnectionStream;
typedef std::function<void (const std::string&, const std::string&)> MessageCallback;

class ClientService {

private:
	DlibConnectionStream 	stream;
	JsonHandler				json_handler;
	bool					connected;

	std::string		username;
	std::string		password;

	const std::string log_tag;

	MessageCallback message_cb;

	/**
	 * Receiver thread for all kind of messages from the server
	 */
	void receiver();

	void send_login_data();
	bool process_login_response();

public:
	ClientService();

	bool connect();

	void log_info(const std::string& msg);
	void log_success(const std::string& msg);
	void log_warning(const std::string& msg);
	void log_error(const std::string& msg);

	bool connection_handshake();

	void start_receiver(MessageCallback message_cb);
	void send(const std::string& message);

	virtual ~ClientService();
};

#endif /* CLIENTSERVICE_H_ */
