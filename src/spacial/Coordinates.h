/*
 * Coordinates.h
 *
 *  Created on: 29 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#define COORDINATES_DEBUG

#ifndef COORDINATES_H_
#define COORDINATES_H_

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>

namespace cryomesh {

namespace spacial {

template<class T>
class Coordinates {
public:
	Coordinates() {
		coords = boost::tuple<T, T, T>(0, 0, 0);
	}

	Coordinates(const Coordinates<T> & obj) {
		coords = boost::tuple<T, T, T>(obj.getX(), obj.getY(), obj.getZ());
	}
	Coordinates(const T x, const T y, const T z) {
		coords = boost::tuple<T, T, T>(x, y, z);

	}
	~Coordinates() {
	}
	/**
	 * Less than operator
	 *
	 * @param const Coordinates & obj
	 * 		RHS
	 *
	 * @return bool
	 * 		True if < than obj, false otherwise
	 */
	bool operator<(const Coordinates & obj) const {
		return this->coords <obj.coords;
	}

	/**
	 * Comparator operator
	 *
	 * @param const Coordinates & obj
	 * 		RHS object
	 *
	 * @return bool
	 * 		True if equal, false otherwise
	 */
	bool operator==(const Coordinates & obj) const {
		return (this->coords == obj.coords);
	}

	T getX() const {
		return boost::tuples::get<0>(coords);
	}
	T getY() const {
		return boost::tuples::get<1>(coords);
	}
	T getZ() const {
		return boost::tuples::get<2>(coords);
	}

	/**
	 * To stream operator
	 *
	 *	@param std::ostream & os
	 *		The output stream
	 *	@param const Coordinates & obj
	 *		The object to stream
	 *
	 *	@return std::ostream &
	 *		The output stream
	 */
	friend std::ostream& operator<<(std::ostream & os, const Coordinates & obj) {
		os << "(" << obj.getX() << ", " << obj.getY() << ", " << obj.getZ() << ")";
		return os;
	}
private:
	boost::tuple<T, T, T> coords;
};

}

}

#endif /* COORDINATES_H_ */
