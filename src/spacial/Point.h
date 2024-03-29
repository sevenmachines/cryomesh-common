/*
 * Point.h
 *
 *  Created on: 31 Mar 2010
 *      Author: niall
 */

#ifndef POINT_H_
#define POINT_H_

#include <iostream>

namespace cryomesh {

namespace spacial {

template<class T>
class Coordinates;
class SphericalPoint;

class Point {
public:
	Point();
	Point(const Coordinates<int> & coords);
	Point(const Coordinates<double> & coords);

	Point(double new_x, double new_y, double new_z);
	Point(const Point & sp);
	Point(const SphericalPoint & sp);

	virtual ~Point();
	double getDistance(const Point & start = Point(0, 0, 0)) const;
	void setToMinimumDistances(double min);

	/**
	 * Scale point by value
	 *
	 * @param double
	 * 	factor to scale by
	 *
	 * @return
	 * 	This scaled point
	 */
	Point getMagnitudes() const;
	Point getScaled(double factor) const;
	Point getRounded() const;
	double getX() const;
	double getY() const;
	double getZ() const;

	void setX(double d);
	void setY(double d);
	void setZ(double d);

	/*
	 * get  a normalised vector
	 */
	Point getNormalisedVector() const;
	// hazy equals for doubles
	bool equals(const Point & obj) const;
	bool operator==(const Point & obj) const;
	Point & operator=(const Point & obj);
	Point operator+(const Point & obj) const;
	Point operator-(const Point & obj) const;
	Point operator*(double d) const;
	Point operator/(double d) const;
	friend std::ostream & operator<<(std::ostream& os, const Point & obj);

private:
	double x;
	double y;
	double z;

};

}

}

#endif /* POINT_H_ */
