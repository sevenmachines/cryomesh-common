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
#include <vector>

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
	std::string getOptionValue(const std::string & key)const;
	int getIntegerFormattedOptionValue(const std::string & key)const;

	std::map<std::string, int> getIntegerFormattedOptions() const ;
	std::string getRawEntry() const;
	std::pair<std::string, std::string> extractCommand(const std::string & rawtext);
		std::pair<std::string, std::string> extractOptionPair(const std::string & rawtext);
		std::pair<std::string, std::string> extractOption(const std::string & rawtext);
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

		static std::vector<int> toIntegerMultipleValues(const std::vector<std::string> &values);
		static std::vector<std::string> tokenizeMultipleValueString(const std::string & values) ;
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
