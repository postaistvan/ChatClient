/*
 * JsonHandler.cpp
 *
 *  Created on: Sep 5, 2017
 *      Author: istvan
 */

#include "JsonHandler.h"
#include "Logger.h"
#include <iostream>

using namespace jsoncons;

JsonHandler::JsonHandler() :
	log_tag("JsonHelper")
{
	// TODO Auto-generated constructor stub
}

std::string JsonHandler::get_json_string() {
	return json_obj.to_string();
}

void JsonHandler::clear() {
	json_obj.clear();
}

void JsonHandler::add_element(	const std::string& element,
								const std::string& value)
{
	json_obj[element] = value;
}

bool JsonHandler::construct_json(const std::string& json_text) {
	try {

		json_obj = json::parse(json_text);
		return true;

	} catch (const parse_error& e) {

		log_error(e.what());
		return false;

	}
}

std::string JsonHandler::get_element_value(const std::string& element) {
	if (json_obj.has_key(element)) 	return json_obj[element].as_string();
	else							return "";
}

void JsonHandler::print_json() {
	std::cout << jsoncons::pretty_print(json_obj) << std::endl;
}

void JsonHandler::log_info(const std::string& msg) {
	Logger::console_info(log_tag, msg);
}

void JsonHandler::log_success(const std::string& msg) {
	Logger::console_success(log_tag, msg);
}

void JsonHandler::log_warning(const std::string& msg) {
	Logger::console_warning(log_tag, msg);
}

void JsonHandler::log_error(const std::string& msg) {
	Logger::console_error(log_tag, msg);
}

JsonHandler::~JsonHandler() {
	// TODO Auto-generated destructor stub
}

