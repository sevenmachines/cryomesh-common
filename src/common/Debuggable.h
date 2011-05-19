/*
 * Debuggable.h
 *
 *  Created on: 11 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef DEBUGGABLE_H_
#define DEBUGGABLE_H_

namespace cryomesh {

namespace common {

/**
 * Simple class to aid setting debuggable output on a class
 */
class Debuggable {
public:
	/**
	 * Default constructor
	 */
	Debuggable() :
		debug(false) {
	}

	/**
	 * Default destructor
	 */
	virtual ~Debuggable() {
	}

	/**
	 * Get debug status
	 *
	 * @return bool
	 * 	True if debugging on, false otherwise
	 */
	bool getDebug() const{
		return debug;
	}

	/**
	 * Get debug status
	 *
	 * @return bool
	 * 	True if debugging on, false otherwise
	 */
	bool isDebugOn() const{
		return this->getDebug();
	}

	virtual void enableDebug(bool b)=0;

	virtual bool checkConstraints()const{
			return false;
		}
protected:

	/**
	 * Set debug on or off
	 *
	 * @param bool
	 * 	True to set debugging on, false otherwise
	 */
	virtual void setDebug(bool b) {
		debug = b;
	}

private:
	/**
	 * Bool representing debugging on or off
	 *
	 * @var bool
	 */
	bool debug;
};

}

}

#endif /* DEBUGGABLE_H_ */
