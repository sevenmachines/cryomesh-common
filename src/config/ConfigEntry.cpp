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
const std::map<std::string, std::string> & ConfigEntry::getOptions() const {
	return options;
}

std::string ConfigEntry::getRawEntry() {
	return rawEntry;
}

void ConfigEntry::parseEntry() {
	if (rawEntry.size()>0 && (rawEntry.find_first_not_of(' ') != std::string::npos ) ){
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
	}else{
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

}//NAMESPACE

}//NAMESPACE
