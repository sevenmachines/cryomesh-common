/*
 * Tagged.h
 *
 *  Created on: 19 Jan 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 *
 *      Class to add uuid tagged functionality to classes
 */

#ifndef TAGGED_H_
#define TAGGED_H_
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <string>
#include <sstream>

namespace cryomesh {

namespace common {

/**
 * Tagged class implements uuid tagging of classes
 * 
 * 		Tagged class should be inherited by functions who want to be uuid tagged
 * and have the related functionality.
 */
class Tagged {
public:

	//DEPRECATED
	/**
	 * Get a new ID, not guaranteed unique
	 *
	 * @return unsigned long int
	 * 	The new id
	 *
	 static unsigned long int getNewID() {
	 ++Tagged::ids;
	 return ids;
	 }*/

	//DEPRECATED
	/**
	 * Variable for getting new id'd
	 *
	 static unsigned long int ids;
	 */

	/**
	 * Constructor
	 *
	 * 		Constructor for Tagged
	 */
	Tagged() ;

	Tagged(const Tagged & obj);

	Tagged(const boost::uuids::uuid obj_uuid) ;

	/**
	 * Destructor
	 *
	 * 		Destructor for Tagged
	 */
	virtual ~Tagged() ;

	/**
	 * Get the uuid of object
	 *
	 * @return boost::uuids::uuid
	 * 		The unique uuid tag of this object
	 */
	boost::uuids::uuid getUUID() const;

	/**
	 * Get the uuid of object as a string
	 *
	 * @return std::string
	 * 		The unique uuid tag of this object as a string
	 */
	std::string getUUIDString() const ;

	/**
	 * Get the short summary of the uuid of object as a string
	 *
	 * @return std::string
	 * 		The short summary uuid tag of this object as a string
	 */
	std::string getUUIDSummary() const ;

	/**
	 * Get the key of object
	 *
	 * @return boost::uuids::uuid
	 * 		The unique uuid tag of this object
	 */
	boost::uuids::uuid getKey() const ;

	/**
	 * Get the key string of object
	 *
	 * @return std::string
	 * 		The unique uuid tag string of this object
	 */
	std::string getKeyString() const ;
	/**
	 * Get the tag of object
	 *
	 * @return boost::uuids::uuid
	 * 		The unique uuid tag of this object
	 */
	boost::uuids::uuid getTag() const;

	/**
	 * Get the tag string of object
	 *
	 * @return std::string
	 * 		The unique uuid tag string of this object
	 */
	std::string getTagString() const ;

	//DEPRECATED
	/**
	 * Get the ID of object
	 *
	 * @return unsigned long ing
	 * 		The not unique id tag of this object
	 *
	 unsigned long int getID() const {
	 return id;
	 }*/

	/**
	 * Assignment operator
	 *
	 * @param const Tagged & obj
	 * 		RHS assignment
	 *
	 * @return Tagged &
	 * 		This object after assignment
	 */
	Tagged & operator=(const Tagged & obj);

	static boost::uuids::uuid  getRandomUUID();

protected:
	/*
	 * Unique uuid of this object
	 *
	 * @var boost::uuids::uuid
	 */
	 boost::uuids::uuid uuid;

	//DEPRECATED
	/**
	 * Compatibility id, not guaranteed unique
	 *
	 * @var unsigned long int
	 *
	 unsigned long int id;
	 */

	static boost::mt19937 randomMersenne;
	static boost::uuids::basic_random_generator<boost::mt19937> randomUUID;
};

}

}

#endif /* TAGGED_H_ */
