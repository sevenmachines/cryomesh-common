/*
 * Point.cpp
 *
 *  Created on: 31 Mar 2010
 *      Author: niall
 */

//#define POINT_DEBUG
#include "Point.h"
#include "SphericalPoint.h"
#include "Coordinates.h"
#include <math.h>
#include <boost/math/special_functions/round.hpp>

namespace cryomesh {

namespace spacial {

Point::Point() :
		x(0), y(0), z(0) {

}

Point::Point(const Coordinates<int> & coords) :
		x(coords.getX()), y(coords.getY()), z(coords.getZ()) {

}

Point::Point(const Coordinates<double> & coords) :
		x(coords.getX()), y(coords.getY()), z(coords.getZ()) {

}

Point::Point(double new_x, double new_y, double new_z) :
		x(new_x), y(new_y), z(new_z) {

}
Point::Point(const Point & sp) :
		x(sp.getX()), y(sp.getY()), z(sp.getZ()) {
}
Point::Point(const SphericalPoint & sp) :
		x(sp.getRadius() * (sin(sp.getTheta().getValue()) * cos(sp.getPhi().getValue()))), y(
				sp.getRadius() * (sin(sp.getTheta().getValue()) * sin(sp.getPhi().getValue()))), z(
				sp.getRadius() * cos(sp.getTheta().getValue())) {
}
Point::~Point() {
}
double Point::getDistance(const Point & start) const {
	double p1 = static_cast<double>( x - start.getX());
	double p2 = static_cast<double>( y - start.getY());
	double p3 = static_cast<double>(z - start.getZ());
#ifdef POINT_DEBUG
	std::cout<<"Point::getDistance: "<<"p1,p2,p3: "<< p1<<", "<<p2<<", "<<p3<<std::endl;
#endif
	double distance = sqrt((p1 * p1) + (p2 * p2) + (p3 * p3));
	return distance;
}
void Point::setToMinimumDistances(double min) {
	if (x < min) {
		x = min;
	}
	if (y < min) {
		y = min;
	}
	if (z < min) {
		z = min;
	}
}

Point Point::getMagnitudes() const {
	int scale_x = 1;
	int scale_y = 1;
	int scale_z = 1;

	if (this->x < 0) {
		scale_x = -1;
	}
	if (this->y < 0) {
		scale_y = -1;
	}
	if (this->z < 0) {
		scale_z = -1;
	}
	return Point(this->x * scale_x, this->y * scale_y, this->z * scale_z);
}

Point Point::getScaled(double factor) const {
	return Point(this->x * factor, this->y * factor, this->z * factor);
}

Point Point::getRounded() const {
	return Point(boost::math::round(this->getX()), boost::math::round(this->getY()), boost::math::round(this->getZ()));
}

double Point::getX() const {
	return x;
}
double Point::getY() const {
	return y;
}
double Point::getZ() const {
	return z;
}

void Point::setX(double d) {
	this->x = d;
}
void Point::setY(double d) {
	this->y = d;
}
void Point::setZ(double d) {
	this->z = d;
}

Point Point::getNormalisedVector() const {
	// get distance to origin
	double dist = (*this).getDistance(Point(0, 0, 0));
	double new_x = (*this).getX() / dist;
	double new_y = (*this).getY() / dist;
	double new_z = (*this).getZ() / dist;
	return Point(new_x, new_y, new_z);
}
bool Point::equals(const Point & obj) const {
	const double DELTA = 0.000001;
	bool bx = (this->x + DELTA > obj.getX()) && (this->x - DELTA < obj.getX());
	bool by = (this->y + DELTA > obj.getY()) && (this->y - DELTA < obj.getY());
	bool bz = (this->z + DELTA > obj.getZ()) && (this->z - DELTA < obj.getZ());
	return (bx && by && bz);
}
bool Point::operator==(const Point & obj) const {
	return ((this->x == obj.getX()) && (this->y == obj.getY()) && (this->z == obj.getZ()));
}
Point & Point::operator=(const Point & obj) {
	if (this != &obj) {
		this->x = obj.getX();
		this->y = obj.getY();
		this->z = obj.getZ();
	}
	return *this;
}
Point Point::operator+(const Point & obj) const {
	return Point(this->getX() + obj.getX(), this->getY() + obj.getY(), this->getZ() + obj.getZ());
}
Point Point::operator-(const Point & obj) const {
	return Point(this->getX() - obj.getX(), this->getY() - obj.getY(), this->getZ() - obj.getZ());
}
Point Point::operator*(double d) const {
	return Point(this->getX() * d, this->getY() * d, this->getZ() * d);
}

Point Point::operator/(double d) const {
	if (d == 0) {
		d = 0.00001;
	}
	return Point(this->getX() / d, this->getY() / d, this->getZ() / d);
}

std::ostream & operator<<(std::ostream& os, const Point & obj) {
	os << "Point: " << "(" << obj.getX() << ", " << obj.getY() << ", " << obj.getZ() << ")";
	return os;

}
} //NAMESPACE

} //NAMESPACE
