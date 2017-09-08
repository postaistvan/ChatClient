/*
 * JsonHandler.h
 *
 *  Created on: Sep 5, 2017
 *      Author: istvan
 */

#ifndef JSONHANDLER_H_
#define JSONHANDLER_H_

#include "jsoncons/json.hpp"

class JsonHandler {

private:

	const std::string 	log_tag;
	jsoncons::json 		json_obj;

public:

	JsonHandler();

	/**
	 * Constructing the json object
	 */
	void add_element(const std::string& element, const std::string& value);
	bool construct_json(const std::string& json_text);

	std::string get_json_string();

	/**
	 * Deconstructing the json object
	 */
	std::string get_element_value(const std::string& element);
	void clear();

	void print_json();

	/**
	 * Logging to console
	 */
	void log_info(const std::string& msg);
	void log_success(const std::string& msg);
	void log_warning(const std::string& msg);
	void log_error(const std::string& msg);

	virtual ~JsonHandler();
};

#endif /* JSONHANDLER_H_ */
