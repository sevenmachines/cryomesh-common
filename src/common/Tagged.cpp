/*
 * Tagged.cpp
 *
 *  Created on: 8 Jul 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "Tagged.h"

namespace cryomesh {

namespace common {

boost::uuids::random_generator Tagged::random_generator;

 boost::uuids::uuid Tagged::getRandomUUID() {
	return Tagged::random_generator();
}

Tagged::Tagged() :
uuid(Tagged::getRandomUUID()) {

}

Tagged::Tagged(const Tagged & obj) {
	this->uuid = obj.uuid;
}

Tagged::~Tagged() {

}

boost::uuids::uuid Tagged::getUUID() const {
	return uuid;
}

std::string Tagged::getUUIDString() const {
	std::stringstream ss;
	ss << (this->getUUID());
	return ss.str();
}

std::string Tagged::getUUIDSummary() const {
	std::stringstream ss;
	ss << (this->getUUID());
	std::string summary = ss.str().substr(0, 8);
	return summary;
}

boost::uuids::uuid Tagged::getKey() const {
	return this->getUUID();
}

std::string Tagged::getKeyString() const {
	return this->getUUIDString();
}

boost::uuids::uuid Tagged::getTag() const {
	return this->getUUID();
}

std::string Tagged::getTagString() const {
	return this->getUUIDString();
}

Tagged & Tagged::operator=(const Tagged & obj) {
	if (&obj == this) {
		return *this;
	}
	this->uuid = obj.uuid;
	return *this;
}
}//NAMESPACE
}//NAMESPACE
