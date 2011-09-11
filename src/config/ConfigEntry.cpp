/*
 * ConfigEntry.cpp
 *
 *  Created on: 24 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

//#define CONFIGENTRY_DEBUG
#include "ConfigEntry.h"
#include<iostream>
#include<boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include<string>
#include <sstream>

#include <cstdlib>

namespace cryomesh {

namespace config {

ConfigEntry::ConfigEntry(const std::string & rawline) :
		command(), options(), rawEntry(rawline) {
	parseEntry();
}

ConfigEntry::~ConfigEntry() {
}

std::string ConfigEntry::getCommand() const {
	return command;
}
std::string ConfigEntry::getOptionValue(const std::string & key) const {
#ifdef CONFIGENTRY_DEBUG
	std::cout << "ConfigEntry::getOptionValue: " << "Parsing: " << "'" << key << std::endl;
#endif
	std::map<std::string, std::string>::const_iterator it_found = options.find(key);
	std::string val = "";
	if (it_found != options.end()) {
		val = it_found->second;
#ifdef CONFIGENTRY_DEBUG
		std::cout << "ConfigEntry::getOptionValue: " << "Found: " << "'" << val << "'" << std::endl;
#endif
	}
	boost::erase_all(val, "\"");
#ifdef CONFIGENTRY_DEBUG
	std::cout << "ConfigEntry::getOptionValue: " << "Trimmed: " << "'" << val << "'" << std::endl;
#endif
	return val;
}
int ConfigEntry::getIntegerFormattedOptionValue(const std::string & key) const {
	int val_int = atoi(this->getOptionValue(key).c_str());
	return val_int;
}

const std::map<std::string, std::string> & ConfigEntry::getOptions() const {
	return options;
}

std::map<std::string, int> ConfigEntry::getIntegerFormattedOptions() const {
	std::map<std::string, int> formatted_map;
	// forall in options
	{
		std::map<std::string, std::string>::const_iterator it_options = options.begin();
		const std::map<std::string, std::string>::const_iterator it_options_end = options.end();
		while (it_options != it_options_end) {
			formatted_map[it_options->first] = std::atoi(it_options->second.c_str());
			++it_options;
		}
	}
	return formatted_map;
}

std::string ConfigEntry::getRawEntry() const {
	return rawEntry;
}

void ConfigEntry::parseEntry() {
	std::string working_entry = rawEntry;
	std::pair<std::string, std::string> ret_pair = this->extractCommand(working_entry);
	command = ret_pair.first;
	working_entry = ret_pair.second;
	bool done_processing = false;
	while (done_processing == false && working_entry != "") {
		ret_pair = this->extractOptionPair(working_entry);
		std::pair<std::string, std::string> temp_opt = this->extractOption(ret_pair.first);
#ifdef CONFIGENTRY_DEBUG
		std::cout << "ConfigEntry::parseEntry: " << "pair: " << temp_opt.first << ", " << temp_opt.second << std::endl;
#endif
		if (temp_opt.first != "") {
			options.insert(temp_opt);
		}

		if (working_entry.size() == ret_pair.second.size()) {
			done_processing = true;
			//std::cout << "ConfigEntry::parseEntry: " << "Stuck in option extraction loop... exiting" << std::endl;
		}
		working_entry = ret_pair.second;
	}

}

std::ostream& operator<<(std::ostream & os, const ConfigEntry & obj) {
	os << obj.getCommand() << " ";
	const std::map<std::string, std::string> & tempmap = obj.getOptions();
	// forall in tempmap
	{
		std::map<std::string, std::string>::const_iterator it_tempmap = tempmap.begin();
		const std::map<std::string, std::string>::const_iterator it_tempmap_end = tempmap.end();
		while (it_tempmap != it_tempmap_end) {
			os << it_tempmap->first << "=" << it_tempmap->second;
			++it_tempmap;
			if (it_tempmap != it_tempmap_end) {
				os << " ";
			}
		}
	}
	return os;
}

std::vector<int> ConfigEntry::toIntegerMultipleValues(const std::vector<std::string> &values) {
	std::vector<int> intvals;

	// forall in values
	{
		std::vector<std::string>::const_iterator it_values = values.begin();
		const std::vector<std::string>::const_iterator it_values_end = values.end();
		while (it_values != it_values_end) {
			int tempval = atoi(it_values->c_str());
			intvals.push_back(tempval);
			++it_values;
		}
	}
	return intvals;
}

std::vector<std::string> ConfigEntry::tokenizeMultipleValueString(const std::string & values) {
	std::vector<std::string> tokened_values;
#ifdef CONFIGENTRY_DEBUG
	std::cout << "ConfigEntry::tokenizeMultipleValueString: " << "Parsing: " << "'" << values << "'" << std::endl;
#endif
	// break into space seperated
	typedef boost::tokenizer<boost::char_separator<char> > char_tokenizer;
	boost::char_separator<char> sep(" ");
	char_tokenizer tokens(values, sep);

	char_tokenizer::const_iterator it_tokens = tokens.begin();
	const char_tokenizer::const_iterator it_tokens_end = tokens.end();
	while (it_tokens != it_tokens_end) {
		tokened_values.push_back(*it_tokens);
		++it_tokens;
	}
	return tokened_values;
}

std::pair<std::string, std::string> ConfigEntry::extractCommand(const std::string & rawtext) {
#ifdef CONFIGENTRY_DEBUG
	std::cout << "ConfigEntry::extractCommand: " << "raw: " << rawtext << std::endl;
#endif
	// format
	//  command option1=1 option2="2 3 4"
	std::string ret_comm = "";
	std::string ret_remain = "";
	if (rawtext.size() > 0) {
		std::string temp_raw_text = boost::trim_left_copy(rawtext);
		int first_space = temp_raw_text.find_first_of(' ');
		ret_comm = temp_raw_text.substr(0, first_space);
		ret_remain = boost::trim_left_copy(temp_raw_text.substr(first_space + 1));
	}
#ifdef CONFIGENTRY_DEBUG
	std::cout << "ConfigEntry::extractCommand: " << "pair: " << ret_comm << ", " << ret_remain << std::endl;
#endif
	return std::pair<std::string, std::string>(ret_comm, ret_remain);
}
std::pair<std::string, std::string> ConfigEntry::extractOptionPair(const std::string & rawtext) {
#ifdef CONFIGENTRY_DEBUG
	std::cout << "ConfigEntry::extractOptionPair: " << "raw: " << rawtext << std::endl;
#endif
	std::string ret_opt = "";
	std::string ret_remain = "";

	std::string remaining_text = boost::trim_left_copy(rawtext);

	std::string temp_opt = "";
	size_t first_quote = rawtext.find_first_of('"');
	size_t second_quote = rawtext.find_first_of('"', first_quote + 1);
	size_t first_eq = rawtext.find_first_of('=');
	size_t second_eq = rawtext.find_first_of('=', first_eq + 1);
	size_t first_space = rawtext.find_first_of(' ');

	//std::cout << "ConfigEntry::extractOptionPair: " << "first_quote " << first_quote << "second_quote " << second_quote
	//		<< "first_eq " << first_eq << "second_eq " << second_eq << "first_space " << first_space << std::endl;

	bool get_quoted_option = (first_quote != std::string::npos) && (second_quote < second_eq || second_eq
			== std::string::npos);
	if (get_quoted_option == true) {
		ret_opt = rawtext.substr(0, second_quote + 1);
		if (second_quote + 1 == std::string::npos) {
			ret_remain = "";
		} else {
			ret_remain = rawtext.substr(second_quote + 1);
		}
	} else {
		ret_opt = rawtext.substr(0, first_space);
		ret_remain = rawtext.substr(first_space + 1);
	}
	boost::trim(ret_opt);
	boost::trim(ret_remain);
#ifdef CONFIGENTRY_DEBUG
	std::cout << "ConfigEntry::extractOptionPair: " << "pair: " << ret_opt << ", " << ret_remain << std::endl;
#endif
	return (std::pair<std::string, std::string>(ret_opt, ret_remain));

}
std::pair<std::string, std::string> ConfigEntry::extractOption(const std::string & rawtext) {
#ifdef CONFIGENTRY_DEBUG
	std::cout << "ConfigEntry::extractOption: " << "raw: " << rawtext << std::endl;
#endif
	//seperate out option pairs
	typedef boost::tokenizer<boost::char_separator<char> > char_tokenizer;
	boost::char_separator<char> sep_equals("=");
	char_tokenizer option(rawtext, sep_equals);
	char_tokenizer::const_iterator it_option = option.begin();
	char_tokenizer::const_iterator it_option_end = option.end();
	std::string first = "";
	std::string second = "";
	if (it_option != it_option_end) {
		first = *it_option;
		++it_option;
	}

	if (it_option != it_option_end) {
		second = *it_option;
	}
#ifdef CONFIGENTRY_DEBUG
	std::cout << "ConfigEntry::extractOption: " << "pair: " << first << ", " << second << std::endl;
#endif
	return (std::pair<std::string, std::string>(first, second));
}
}//NAMESPACE

}//NAMESPACE
