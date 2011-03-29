/*
 * ICommunication.h
 *
 *  Created on: 28 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef ICOMMUNICATION_H_
#define ICOMMUNICATION_H_

#include <string>

namespace cryo {

namespace networking {

class ICommunication {
public:
	ICommunication();
	virtual ~ICommunication();

	void sendMessage(const std::string & message) const;
	std::string receiveMessage() const;
};

}

}

#endif /* ICOMMUNICATION_H_ */
