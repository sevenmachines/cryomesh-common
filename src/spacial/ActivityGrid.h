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

	friend class ActivityGridTest;
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
	virtual double getUnitFromScaledDistance(double d) const;
	virtual double getScaledFromUnitDistance(int d) const;

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

	ActivityGrid::GridContainer::iterator getNearestGridPoint(const Point & point) ;

	/**
	 * Find the nearest grid point to a point and return its activity
	 *
	 *	@param Point
	 *		The reference point for calculation
	 *
	 * @return std::pair<CoordinatesIntegers, double >
	 * 	The nearest grid point values
	 */
	virtual const std::pair<CoordinatesIntegers, double> getNearestGridPointActivity(const Point & point) const;

	/**
	 * Find the nearest grid point to a point and set its activity
	 *
	 * @param Point
	 * 	Point reference
	 * @param double
	 * 	The activity to set the point to
	 *
	 * @return const std::pair<CoordinatesIntegers, double>
	 * 	The point that was set
	 */
	virtual void setNearestGridPointActivity(const Point & point, const double & activity);

	/**
	 * Apply an activity at a scaled point and affect nearby grid points
	 *
	 * @param Point
	 * 	Point reference
	 * @param double
	 * 	The activity to set the point to
	 */
	void applyPointActivityToGrid(const Point & point, const double & activity,
			BoundingBox::InterpolationStyle decayStyle = BoundingBox::InterpolationStyle::INVERSE_R);

	double getGridPointActivity(const CoordinatesIntegers & coords) const ;
	double getGridPointActivity(const int x, const int y, const int z) const ;

	const std::map<CoordinatesIntegers, double> & getActivityGrid() const;

	double getActivityDecay() const ;

	void setActivityDecay(double d);

	GridContainer getGridPointsByActivity(double act);
	GridContainer getGridPointsByActivity(double act_min, double act_max);
	/**
	 * Compare the grid points of this grid with another, using  ints with
	 * the count of those in the comparison grid that are greater than ours, lesser than or cant be found
	 *
	 * @param ActivityGrid
	 * 	Grid to compare with
	 * @param int
	 * 	Store for equal to count
	 * @param int
	 * 	Store for greater than count
	 * @param int
	 * 	Store for less than count
	 * @param int
	 * 	Store for not found count
	 */
	void compareGridPoints(const ActivityGrid & grid_comp, int & equal_to, int & greater_than, int & less_than,
			int & not_known) const;
	void
			compareGridPoints(const double comp_act, int & equal_to, int & greater_than, int & less_than,
					int & not_known) const;

	double getActivitySummation() const;

	void growDimensions(const CoordinatesIntegers & coords, double val = 0.0);
	void shrinkDimensions(const CoordinatesIntegers & coords);

	/**
	 * To stream operator
	 *
	 *	@param std::ostream & os
	 *		The output stream
	 *	@param const ActivityGrid & obj
	 *		The object to stream
	 *
	 *	@return std::ostream &
	 *		The output stream
	 */
	friend std::ostream& operator<<(std::ostream & os, const ActivityGrid & obj);
protected:
	const std::map<CoordinatesIntegers, double>::const_iterator
	findGridCoordinate(const CoordinatesIntegers & coords) const;
	std::map<CoordinatesIntegers, double>::iterator findMutableGridCoordinate(const CoordinatesIntegers & coords);
	std::map<CoordinatesIntegers, double>::iterator
	setGridPointActivity(const CoordinatesIntegers & coords, double val);

	/**
	 *  Apply activity from an unscaled point to another unscaled point
	 *
	 **/
	std::pair<CoordinatesIntegers, double> &
	applyPointActivityToGridPoint(const Point & source, const double source_activity,
			std::pair<CoordinatesIntegers, double> & sink,
			const ActivityModifier modifier = ACTIVITY_MODIFIER_ADDITION,
			BoundingBox::InterpolationStyle decayStyle = BoundingBox::InterpolationStyle::INVERSE_R);
private:
	/**
	 * The Coordinates that define the cube that encases all points
	 * Note they are non-inclusive
	 *
	 * @val CoordinatesIntegers
	 */
	CoordinatesIntegers boundingUnitCoordinates;

	/**
	 * Activity rate of decay per unit distance
	 * eg 2 -> activity falls by two per 1 unit of distance
	 *
	 * @val double
	 */
	double activityDecay;

	/**
	 * Scale of points to unit distances
	 *
	 * @val int
	 */
	int scale;

	const int MIN_SCALE;
	const int MAX_SCALE;

	/**
	 * Value of activity effect below which we stop calculating it
	 *
	 * @val double
	 */
	const double ACTIVITY_DECAY_CUTOFF;

	/**
	 * Container of activity grid points and activities
	 *
	 * @val GridContainer
	 */
	GridContainer activityGrid;
};

}

}

#endif /* ACTIVITYGRID_H_ */
