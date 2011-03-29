/*
 * ConfigEntry.h
 *
 *  Created on: 24 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef CONFIGENTRY_H_
#define CONFIGENTRY_H_

#include <istream>
#include <ostream>
#include <fstream>
#include <string>
#include <map>
#include <list>

namespace cryomesh {

namespace config {

/** Base class for all config entries that can be read by ConfigTranslator,
 * specialisations will overwrite the required elements
 */
class ConfigEntry {
public:
	ConfigEntry(const std::string & rawline="");
	virtual ~ConfigEntry();
	std::string getCommand()  const;
	const std::map<std::string, std::string> & getOptions() const;
	std::string getRawEntry();

	/**
		 * To stream operator
		 *
		 *	@param std::ostream & os
		 *		The output stream
		 *	@param const ConfigEntry & obj
		 *		The object to stream
		 *
		 *	@return std::ostream &
		 *		The output stream
		 */
		friend std::ostream& operator<<(std::ostream & os, const ConfigEntry & obj);

protected:
	void parseEntry();
private:
	std::string command;
	std::map<std::string, std::string> options;

	std::string rawEntry;
};

}

}

#endif /* CONFIGENTRY_H_ */
