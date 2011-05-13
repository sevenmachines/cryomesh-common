/*
 * ConfigEntry.cpp
 *
 *  Created on: 24 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "ConfigEntry.h"
#include<iostream>
#include<boost/tokenizer.hpp>
#include<string>
#include <cstdlib>

namespace cryomesh {

namespace config {

ConfigEntry::ConfigEntry(const std::string & rawline) :
	rawEntry(rawline) {
	parseEntry();
}

ConfigEntry::~ConfigEntry() {
}

std::string ConfigEntry::getCommand() const {
	return command;
}
std::string ConfigEntry::getOptionValue(const std::string & key) const {
	std::map<std::string, std::string>::const_iterator it_found = options.find(key);
	std::string val = "";
	if (it_found != options.end()) {
		val = it_found->second;
	}
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
	if (rawEntry.size() > 0 && (rawEntry.find_first_not_of(' ') != std::string::npos)) {
		// break into space seperated
		typedef boost::tokenizer<boost::char_separator<char> > char_tokenizer;
		boost::char_separator<char> sep(" ");
		char_tokenizer tokens(rawEntry, sep);
		//for (char_tokenizer::iterator beg = tokens.begin(); beg != tokens.end(); ++beg) {
		//	std::cout << *beg << "\n";
		//}

		char_tokenizer::const_iterator it_tokens = tokens.begin();
		const char_tokenizer::const_iterator it_tokens_end = tokens.end();
		// 1st is command
		if (it_tokens != it_tokens_end) {
			command = *it_tokens;
		}
		++it_tokens;
		//seperate out option pairs
		while (it_tokens != it_tokens_end) {
			boost::char_separator<char> sep_equals("=");
			char_tokenizer option(*it_tokens, sep_equals);
			char_tokenizer::const_iterator it_option = option.begin();
			char_tokenizer::const_iterator it_option_end = option.end();
			std::string first = "";
			std::string second = "";
			if (it_option != it_option_end) {
				first = *it_option;
			}
			++it_option;
			if (it_option != it_option_end) {
				second = *it_option;
			}
			options.insert(std::pair<std::string, std::string>(first, second));

			++it_tokens;
		}
	} else {
		command = "";
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
}//NAMESPACE

}//NAMESPACE
