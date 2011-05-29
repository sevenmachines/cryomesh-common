/*
 * ActivityGrid.h
 *
 *  Created on: 26 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef ACTIVITYGRID_H_
#define ACTIVITYGRID_H_

#include <boost/multi_array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include "BoundingBox.h"
#include "Point.h"
#include "Coordinates.h"

#include "common/Maths.h"
#include <vector>

namespace cryomesh {

namespace spacial {

class ActivityGrid {

public:

#ifdef CUTE_DEBUG
	friend class ActivityGridTest;
#endif
	friend class BoundingBox;

	typedef Coordinates<int> CoordinatesIntegers;
	typedef Coordinates<double> CoordinatesDoubles;
	typedef boost::multi_array<double, 3> SpacialArray;
	typedef typename SpacialArray::index SpacialIndex;
	typedef SpacialArray::extent_gen SpacialExtent;
	typedef std::map<CoordinatesIntegers, double> GridContainer;

	enum ActivityModifier {
		ACTIVITY_MODIFIER_ADDITION = 1, ACTIVITY_MODIFIER_MULTIPLY = 2, ACTIVITY_MODIFIER_INVERT = 4
	};

	enum GridShape {
		CIRCULAR_GRID, SQUARE_GRID
	};
	enum DIMENSION_TAGS {
		X_AXIS, Y_AXIS_Z_AXIS
	};
	ActivityGrid(const int x, const int y, const int z, const int sc = 1);

	virtual ~ActivityGrid();

	virtual int getScale() const ;
	virtual void setScale(const int sc);

	virtual void setBoundingBox(const CoordinatesIntegers & new_size, double val = 0.0);

	virtual double getInterpolatedActivity(const Point & external_point, int depth = 1,
			const BoundingBox::InterpolationStyle style = BoundingBox::InterpolationStyle::INVERSE_R);

	/**
	 * Set all grid points to be within a random range
	 *
	 * @param double
	 * 	Minimum random value for a point
	 * @param double
	 * 	Maximum random value for a point
	 */
	virtual void randomise(const double min = 0.0, const double max = 1.0);

	/**
	 * Clear grid to an optional value
	 *
	 * @param double
	 * 	Value to clear all grid points to
	 */
	virtual void clearGrid(const double val = 0.0);

	virtual void applyGridActivityModifier(const double val_modifier = 0.5,
			int activity_modifier = ACTIVITY_MODIFIER_MULTIPLY);

	const CoordinatesIntegers & getBoundingUnitCoordinates() const ;
	const CoordinatesIntegers getScaledBoundingUnitCoordinates() const ;

	/**
	 * Check there are a specified tuple of points in each dimension
	 *
	 * @param Coordinates
	 * 	The number of points in each dimension
	 *
	 * @return bool
	 * 	True if point count in each dimension x, y, z
	 */
	bool checkGridPointCount(const CoordinatesIntegers & dimension_point_count);

	/**
	 * Check if all grid point values fall within a range
	 *
	 * @param double
	 * 	Minimum required value
	 * @param double
	 * 	Maximum required value
	 *
	 * @return bool
	 * 	True if all values fall within range, false otherwise
	 */
	bool checkGridPointValues(const double min, const double max);

	boost::shared_ptr<BoundingBoxElement> getBoundingBoxElement(Point element, Point reference) const ;

	const boost::shared_ptr<BoundingBox> getBoundingBox(Point reference, int depth = 1);

	virtual void setNearestActivity(const Point & point, const double & activity);

	double getGridPointActivity(const CoordinatesIntegers & coords) const ;
	double getGridPointActivity(const int x, const int y, const int z) const ;

	const std::map<CoordinatesIntegers, double> & getActivityGrid() const;

	double getActivityDecay() const ;

	void setActivityDecay(double d);

	void growDimensions(const CoordinatesIntegers & coords, double val = 0.0);
	void shrinkDimensions(const CoordinatesIntegers & coords);

protected:
	const std::map<CoordinatesIntegers, double>::const_iterator
	findGridCoordinate(const CoordinatesIntegers & coords) const;
	std::map<CoordinatesIntegers, double>::iterator findMutableGridCoordinate(const CoordinatesIntegers & coords);
	std::map<CoordinatesIntegers, double>::iterator setGridCoordinate(const CoordinatesIntegers & coords, double val);

private:
	CoordinatesIntegers boundingUnitCoordinates;
	double activityDecay;

	/**
	 * Scale of points to unit distances
	 *
	 * @val int
	 */
	int scale;
	const int MIN_SCALE;
	const int MAX_SCALE;
	GridContainer activityGrid;
};

}

}

#endif /* ACTIVITYGRID_H_ */
