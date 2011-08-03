/*
 * ConfigTranslator.h
 *
 *  Created on: 24 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef CONFIGTRANSLATOR_H_
#define CONFIGTRANSLATOR_H_

#include "ConfigEntry.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <string>
#include <map>
#include <list>

namespace cryomesh {

namespace config {

/**
 * Class to read in/write out  config files and translate them into collections
 * of ConfigEntry
 */
class ConfigTranslator {
public:
	ConfigTranslator( std::istream & is);
	ConfigTranslator(const std::string & filename);
	virtual ~ConfigTranslator();
	const std::list<ConfigEntry> & getEntries() const;

	ConfigEntry getConfigEntryByCommand(const std::string & com) const;

	/**
	 * To stream operator
	 *
	 *	@param std::ostream & os
	 *		The output stream
	 *	@param const ConfigTranslator & obj
	 *		The object to stream
	 *
	 *	@return std::ostream &
	 *		The output stream
	 */
	friend std::ostream& operator<<(std::ostream & os, const ConfigTranslator & obj);
protected:
	/**
	 * Read in raw entries from a stream
	 */
	void readRawEntries(std::istream & os);

	/**
	 * write out raw entries to a stream
	 */
	std::ostream & writeRawEntries(std::ostream & os);

private:
	/**
	 * List all the config entries
	 */
	std::list<ConfigEntry> entries;
};

}

}

#endif /* CONFIGTRANSLATOR_H_ */
