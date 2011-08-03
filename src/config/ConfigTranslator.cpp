/*
 * ConfigTranslator.cpp
 *
 *  Created on: 24 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

//#define CONFIGTRANSLATOR_DEBUG
#include "ConfigTranslator.h"

namespace cryomesh {

namespace config {
ConfigTranslator::ConfigTranslator(std::istream & is) {
	// open file
	this->readRawEntries(is);
}
ConfigTranslator::ConfigTranslator(const std::string & filename) {
	// open file
	std::ifstream ifs(filename.c_str());
	if (ifs.is_open() == true) {
		this->readRawEntries(ifs);
		ifs.close();
	} else {
		std::cout << "ConfigTranslator::~ConfigTranslator: " << "ERROR: File " << "'" << filename << "'"
				<< "could not be opened... " << std::endl;
	}
}

ConfigTranslator::~ConfigTranslator() {
}

ConfigEntry ConfigTranslator::getConfigEntryByCommand(const std::string & com) const {
	std::list<ConfigEntry>::const_iterator it_entries = entries.begin();
	const std::list<ConfigEntry>::const_iterator it_entries_end = entries.end();
	while (it_entries != it_entries_end) {
		if (it_entries->getCommand() == com) {
			return *it_entries;
		}
		++it_entries;
	}

	ConfigEntry tempent("");
	return tempent;
}

void ConfigTranslator::readRawEntries(std::istream & fs) {
	std::string line;
	while (fs.good() == true) {
		getline(fs, line);
#ifdef CONFIGTRANSLATOR_DEBUG
		std::cout << "ConfigTranslator::readRawEntries: " << line << std::endl;
#endif
		// ignore comment lines
		size_t first_char_pos = line.find_first_not_of(' ');
		if (first_char_pos != std::string::npos) {
			std::string first_char = line.substr(first_char_pos, 1);
			if (first_char != "#") {
				entries.push_back(ConfigEntry(line));
			}
		}
	}
}

std::ostream &  ConfigTranslator::writeRawEntries(std::ostream & os) {
	// forall in entries
	{
		std::list<ConfigEntry>::const_iterator it_entries = entries.begin();
		const std::list<ConfigEntry>::const_iterator it_entries_end = entries.end();
		while (it_entries != it_entries_end) {
			os << *it_entries;
			if (it_entries != it_entries_end) {
				os << std::endl;
			}
			++it_entries;
		}
	}
	return os;
}

const std::list<ConfigEntry> & ConfigTranslator::getEntries() const {
	return entries;
}

std::ostream& operator<<(std::ostream & os, const ConfigTranslator & obj) {
	// forall in entries
	{
		std::list<ConfigEntry>::const_iterator it_entries = obj.getEntries().begin();
		const std::list<ConfigEntry>::const_iterator it_entries_end = obj.getEntries().end();
		while (it_entries != it_entries_end) {
			os << *it_entries << std::endl;
			++it_entries;
		}
	}
	return os;
}

} //NAMESPACE

} //NAMESPACE
