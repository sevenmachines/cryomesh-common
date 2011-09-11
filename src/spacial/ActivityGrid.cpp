/*
 * ActivityGrid.cpp
 *
 *  Created on: 27 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

//#define ACTIVITYGRID_DEBUG

#include "ActivityGrid.h"
#include <boost/math/special_functions/round.hpp>

namespace cryomesh {

namespace spacial {

ActivityGrid::ActivityGrid(const int x, const int y, const int z, const int sc) :
		boundingUnitCoordinates(), activityDecay(1), scale(sc), MIN_SCALE(1), MAX_SCALE(1000), ACTIVITY_DECAY_CUTOFF(0.01) , activityGrid(){
	common::Maths::clamp(scale, MIN_SCALE, MAX_SCALE);
	this->setBoundingBox(CoordinatesIntegers(x, y, z));
}

ActivityGrid::ActivityGrid(const Point & bounding_box, const int sc) :
		boundingUnitCoordinates(),activityDecay(1), scale(sc), MIN_SCALE(1), MAX_SCALE(1000), ACTIVITY_DECAY_CUTOFF(0.01) , activityGrid(){
	common::Maths::clamp(scale, MIN_SCALE, MAX_SCALE);
	this->setBoundingBox(CoordinatesIntegers(bounding_box.getX(), bounding_box.getY(), bounding_box.getZ()));
}

ActivityGrid::~ActivityGrid() {

}

double ActivityGrid::getInterpolatedActivity(const Point & point, int depth,
		const BoundingBox::InterpolationStyle style) {
	// get rescaled point
	Point rescaled_point = point.getScaled( static_cast<double>(this->getScale()));
	boost::shared_ptr<BoundingBox> temp_box = this->getBoundingBox(rescaled_point, depth);
	double interpolated_act = temp_box->getInterpolatedActivity(style);
#ifdef ACTIVITYGRID_DEBUG
	std::cout << "ActivityGrid::getInterpolatedActivity: " << "point: "<<point<<" level: " << depth << " interpolated_act: "
			<< interpolated_act << std::endl;
#endif
	return interpolated_act;
}

ActivityGrid::GridContainer::iterator ActivityGrid::getNearestGridPoint(const Point & point) {
	ActivityGrid::GridContainer::iterator it_found;
	if (activityGrid.size() > 0) {
		const Point rescaled_point = point.getScaled(static_cast<double>(this->getScale()));
		const Point grid_point = rescaled_point.getRounded();
		int grid_x = std::min((boundingUnitCoordinates.getX() - 1),
				std::max(0,static_cast<int>( boost::math::round(rescaled_point.getX()))));
		int grid_y = std::min((boundingUnitCoordinates.getY() - 1),
				std::max(0, static_cast<int>( boost::math::round(rescaled_point.getY()))));
		int grid_z = std::min((boundingUnitCoordinates.getZ() - 1),
				std::max(0, static_cast<int>(  boost::math::round(rescaled_point.getZ()))));
		CoordinatesIntegers coords(grid_x, grid_y, grid_z);
		it_found = this->findMutableGridCoordinate(coords);
		if (it_found == activityGrid.end()) {
			std::cout << "ActivityGrid::getNearestGridPoint: " << "Cannot find nearest point " << coords << " to point"
					<< rescaled_point << std::endl;
		}
	}

	return it_found;
}

const std::pair<ActivityGrid::CoordinatesIntegers, double> ActivityGrid::getNearestGridPointActivity(
		const Point & point) const {
	const Point rescaled_point = point.getScaled(static_cast<double>( this->getScale()));
	const Point grid_point = rescaled_point.getRounded();
	CoordinatesIntegers coords(grid_point.getX(), grid_point.getY(), grid_point.getZ());
	double grid_act = this->getGridPointActivity(coords);
	return std::pair<ActivityGrid::CoordinatesIntegers, double>(coords, grid_act);
}
void ActivityGrid::setNearestGridPointActivity(const Point & point, const double act) {
	// get rescaled point
	Point rescaled_point = point.getScaled(static_cast<double>( this->getScale()));
	Point grid_point = rescaled_point.getRounded();
	CoordinatesIntegers coords(grid_point.getX(), grid_point.getY(), grid_point.getZ());
	this->setGridPointActivity(coords, act);
}

void ActivityGrid::applyPointActivityToGrid(const Point & point, const double activity,
		BoundingBox::InterpolationStyle decayStyle) {
	// get bounding boxes and apply until the whole box falls below threshold
	Point rescaled_point = point.getScaled(static_cast<double>( this->getScale()));

	// get corner point

	//find maximum bounding box
	double max_scaled_distance;
	if (decayStyle == BoundingBox::InterpolationStyle::INVERSE_R) {
		max_scaled_distance = this->getScale() * activity / this->getActivityDecay();
	} else if (decayStyle == BoundingBox::InterpolationStyle::INVERSE_R) {
		max_scaled_distance = this->getScale() * activity / (this->getActivityDecay() * this->getActivityDecay());
	} else {
		std::cout << "ActivityGrid::applyPointActivityToGrid: "
				<< "ERROR: Interpolation style not found, defaulting..." << std::endl;
		max_scaled_distance = this->getScale() * activity / this->getActivityDecay();
	}

	const int base_corner_x = boost::math::round(rescaled_point.getX() - max_scaled_distance + 0.5);
	const int base_corner_y = boost::math::round(rescaled_point.getY() - max_scaled_distance + 0.5);
	const int base_corner_z = boost::math::round(rescaled_point.getZ() - max_scaled_distance + 0.5);
	const int base_radius = boost::math::round(max_scaled_distance - 0.5);

#ifdef ACTIVITYGRID_DEBUG
	std::cout << "ActivityGrid::applyPointActivityToGrid: " << "Point: " << point << " Rescaled: " << rescaled_point
			<< " Distance: " << max_scaled_distance << " gives base " << "(" << base_corner_x << ", " << base_corner_y
			<< ", " << base_corner_z << ")" << "r = " << base_radius << std::endl;
#endif
	int near_bounding_x_plane = std::max(0, base_corner_x);
	int far_bounding_x_plane = std::min(base_corner_x + (2 * base_radius), boundingUnitCoordinates.getX());
	int near_bounding_y_plane = std::max(0, base_corner_y);
	int far_bounding_y_plane = std::min(base_corner_y + (2 * base_radius), boundingUnitCoordinates.getY());
	int near_bounding_z_plane = std::max(0, base_corner_z);
	int far_bounding_z_plane = std::min(base_corner_z + (2 * base_radius), boundingUnitCoordinates.getZ());

#ifdef ACTIVITYGRID_DEBUG
	std::cout << "ActivityGrid::applyPointActivityToGrid: " << "Bounding Plane: " << "(" << near_bounding_x_plane
			<< ", " << near_bounding_y_plane << ", " << near_bounding_z_plane << ")" << " to " << "("
			<< far_bounding_x_plane << ", " << far_bounding_y_plane << ", " << far_bounding_z_plane << ")" << std::endl;
#endif

	for (int i = near_bounding_x_plane; i < far_bounding_x_plane; i++) {
		for (int j = near_bounding_y_plane; j < far_bounding_y_plane; j++) {
			for (int k = near_bounding_z_plane; k < far_bounding_z_plane; k++) {
				CoordinatesIntegers coords(i, j, k);
				GridContainer::iterator it_found = activityGrid.find(coords);
				if (it_found != activityGrid.end()) {
					// do apply activity
					std::pair<CoordinatesIntegers, double> temp_sink = *it_found;
					this->applyPointActivityToGridPoint(rescaled_point, activity, temp_sink, ACTIVITY_MODIFIER_ADDITION);
					it_found->second = temp_sink.second;
					//	std::cout << "ActivityGrid::applyPointActivityToGrid: do_coordinates... " << coords << " :"
					//		<< it_found->second << std::endl;
				} else {
					std::cout << "ActivityGrid::applyPointActivityToGrid: " << "ERROR: Cannot find point " << coords
							<< " in grid." << std::endl;
				}
			}
		}
	}
}

double ActivityGrid::getGridPointActivity(const ActivityGrid::CoordinatesIntegers & coords) const {
	return (this->getGridPointActivity(coords.getX(), coords.getY(), coords.getZ()));
}
double ActivityGrid::getGridPointActivity(const int x, const int y, const int z) const {
	double activity = 0.0;
	CoordinatesIntegers coords(x, y, z);
	GridContainer::const_iterator it_found = this->findGridCoordinate(coords);
	if (it_found != activityGrid.end()) {
		activity = it_found->second;
	} else {
		std::cout << "ActivityGrid::getGridPointActivity: " << "Error: Could not find coordinates (" << x << ", " << y
				<< ", " << z << ")" << std::endl;
	}
	return activity;
}

const std::map<ActivityGrid::CoordinatesIntegers, double> & ActivityGrid::getActivityGrid() const {
	return activityGrid;
}

int ActivityGrid::getScale() const {
	return scale;
}

void ActivityGrid::randomise(const double min, const double max) {

	// forall in activityGrid
	{
		GridContainer::iterator it_activityGrid = activityGrid.begin();
		const GridContainer::const_iterator it_activityGrid_end = activityGrid.end();
		while (it_activityGrid != it_activityGrid_end) {
			double temp_val = common::Maths::getRandomDouble(min, max);
			it_activityGrid->second = temp_val;
			++it_activityGrid;
		}
	}
}

void ActivityGrid::clearGrid(const double val) {
#ifdef ACTIVITYGRID_DEBUG
	std::cout << "ActivityGrid::clearGrid: " << "" << std::endl;
#endif
	// forall in activityGrid
	{
		GridContainer::iterator it_activityGrid = activityGrid.begin();
		const GridContainer::const_iterator it_activityGrid_end = activityGrid.end();
		while (it_activityGrid != it_activityGrid_end) {
			it_activityGrid->second = val;
			++it_activityGrid;
		}
	}
}

void ActivityGrid::applyGridActivityModifier(const double val_modifier, int activity_modifier) {

	bool do_invert = false;
	bool do_add = false;
	bool do_multiply = false;
	double val = val_modifier;
	if (activity_modifier & ACTIVITY_MODIFIER_ADDITION) {
		do_add = true;
	}
	if (activity_modifier & ACTIVITY_MODIFIER_MULTIPLY) {
		do_multiply = true;
	}
	if (activity_modifier & ACTIVITY_MODIFIER_INVERT) {
		do_invert = true;
	}

	// forall in activityGrid
	{
		GridContainer::iterator it_activityGrid = activityGrid.begin();
		const GridContainer::const_iterator it_activityGrid_end = activityGrid.end();
		while (it_activityGrid != it_activityGrid_end) {
			double temp_val = it_activityGrid->second;
			if (do_add == true) {
				temp_val = temp_val + val;
			}
			if (do_multiply == true) {
				temp_val = temp_val * val;
			}
			if (do_invert == true) {
				temp_val = -temp_val;
			}
			it_activityGrid->second = temp_val;
			++it_activityGrid;
		}
	}

}

void ActivityGrid::setBoundingBox(const CoordinatesIntegers & new_size, double val) {
#ifdef ACTIVITYGRID_DEBUG
	std::cout << "ActivityGrid::setBoundingBox: " << new_size << std::endl;
#endif
	int new_gridX = new_size.getX();
	int new_gridY = new_size.getY();
	int new_gridZ = new_size.getZ();
	// case 1, no size (new)
	if (activityGrid.size() == 0) {
		for (int i = 0; i < new_gridX; i++) {
			for (int j = 0; j < new_gridY; j++) {
				for (int k = 0; k < new_gridZ; k++) {
					CoordinatesIntegers coords(i, j, k);
					activityGrid[coords] = val;
#ifdef ACTIVITYGRID_DEBUG
					//std::cout << "ActivityGrid::setBoundingBox: " << coords << " = " << val << std::endl;

#endif
				}
			}
		}
	} else {
		int current_gridX = (this->getBoundingUnitCoordinates().getX());
		int current_gridY = (this->getBoundingUnitCoordinates().getY());
		int current_gridZ = (this->getBoundingUnitCoordinates().getZ());
		const int x_diff = new_gridX - current_gridX;
		const int y_diff = new_gridY - current_gridY;
		const int z_diff = new_gridZ - current_gridZ;

		const int x_grow = std::max(0, x_diff);
		const int y_grow = std::max(0, y_diff);
		const int z_grow = std::max(0, z_diff);

		const int x_shrink = std::min(0, x_diff);
		const int y_shrink = std::min(0, y_diff);
		const int z_shrink = std::min(0, z_diff);
		this->growDimensions(CoordinatesIntegers(x_grow, y_grow, z_grow), val);
		this->shrinkDimensions(CoordinatesIntegers(x_shrink, y_shrink, z_shrink));
	}

	boundingUnitCoordinates = CoordinatesIntegers(new_size.getX(), new_size.getY(), new_size.getZ());
	assert(activityGrid.size() == (new_size.getX() *new_size.getY() *new_size.getZ() ));
}

void ActivityGrid::growDimensions(const CoordinatesIntegers & coords, double val) {

	int x_grow = coords.getX();
	int y_grow = coords.getY();
	int z_grow = coords.getZ();
	int current_gridX = (this->getBoundingUnitCoordinates().getX());
	int current_gridY = (this->getBoundingUnitCoordinates().getY());
	int current_gridZ = (this->getBoundingUnitCoordinates().getZ());

#ifdef ACTIVITYGRID_DEBUG
	std::cout << "ActivityGrid::growDimensions: " << "(" << x_grow << ", " << y_grow << ", " << z_grow << ")"
			<< std::endl;
#endif
	// deal with x increase
	for (int i = 0; i < x_grow; i++) {
		for (int j = 0; j < current_gridY; j++) {
			for (int k = 0; k < current_gridZ; k++) {
				CoordinatesIntegers coords(i + current_gridX + 1, j, k);
				activityGrid[coords] = val;
			}
		}
	}

	// deal with y increase
	for (int i = 0; i < current_gridX; i++) {
		for (int j = 0; j < y_grow; j++) {
			for (int k = 0; k < current_gridZ; k++) {
				CoordinatesIntegers coords(i, j + current_gridY + 1, k);
				activityGrid[coords] = val;
			}
		}
	}

	// deal with z increase
	for (int i = 0; i < current_gridX; i++) {
		for (int j = 0; j < current_gridY; j++) {
			for (int k = 0; k < z_grow; k++) {
				CoordinatesIntegers coords(i, j, k + current_gridZ + 1);
				activityGrid[coords] = val;
			}
		}
	}
	boundingUnitCoordinates = CoordinatesIntegers(boundingUnitCoordinates.getX() + x_grow,
			boundingUnitCoordinates.getY() + y_grow, boundingUnitCoordinates.getZ() + z_grow);

}

void ActivityGrid::shrinkDimensions(const CoordinatesIntegers & coords) {

	int max_x = boundingUnitCoordinates.getX() - coords.getX();
	int max_y = boundingUnitCoordinates.getY() - coords.getY();
	int max_z = boundingUnitCoordinates.getZ() - coords.getZ();

#ifdef ACTIVITYGRID_DEBUG
	std::cout << "ActivityGrid::shrinkDimensions: " << "(" << max_x << ", " << max_y << ", " << max_z << ")"
			<< std::endl;
#endif

	// forall in activityGrid
	{
		GridContainer::iterator it_activityGrid = activityGrid.begin();
		while (it_activityGrid != activityGrid.end()) {
			int temp_x = it_activityGrid->first.getX();
			int temp_y = it_activityGrid->first.getY();
			int temp_z = it_activityGrid->first.getZ();
			if (temp_x > max_x) {
				activityGrid.erase(it_activityGrid);
			} else if (temp_y > max_y) {
				activityGrid.erase(it_activityGrid);
			} else if (temp_z > max_z) {
				activityGrid.erase(it_activityGrid);
			}
			++it_activityGrid;
		}
	}

	boundingUnitCoordinates = CoordinatesIntegers(boundingUnitCoordinates.getX() - coords.getX(),
			boundingUnitCoordinates.getY() - coords.getY(), boundingUnitCoordinates.getZ() - coords.getZ());
}

void ActivityGrid::setScale(const int new_scale) {
	this->scale = new_scale;
}

double ActivityGrid::getUnitFromScaledDistance(double d) const {
	return d / static_cast<double>(this->getScale());
}

double ActivityGrid::getScaledFromUnitDistance(int d) const {
	return this->getScale() * d;
}

const ActivityGrid::CoordinatesIntegers & ActivityGrid::getBoundingUnitCoordinates() const {
	return boundingUnitCoordinates;
}
const ActivityGrid::CoordinatesIntegers ActivityGrid::getScaledBoundingUnitCoordinates() const {
	int sc = this->getScale();
	return CoordinatesIntegers(boundingUnitCoordinates.getX() * sc, boundingUnitCoordinates.getY() * sc,
			boundingUnitCoordinates.getZ() * sc);
}
double ActivityGrid::getActivityDecay() const {
	return activityDecay;
}

void ActivityGrid::setActivityDecay(double d) {
	activityDecay = d;
}

ActivityGrid::GridContainer ActivityGrid::getGridPointsByActivity(double act) {
	return this->getGridPointsByActivity(act, act);
}
ActivityGrid::GridContainer ActivityGrid::getGridPointsByActivity(double act_min, double act_max) {
	GridContainer equal_grid;
	const double DELTA = 0.00001;
	// forall in activityGrid
	{
		GridContainer::const_iterator it_activityGrid = activityGrid.begin();
		const GridContainer::const_iterator it_activityGrid_end = activityGrid.end();
		while (it_activityGrid != it_activityGrid_end) {
			int compare_min = common::Maths::compareDoubles(it_activityGrid->second, act_min, DELTA);
			if (compare_min == 0) {
#ifdef ACTIVITYGRID_DEBUG
				//std::cout << "ActivityGrid::getGridPointsByActivity: compare_min == 0" << it_activityGrid->first << "("
				//		<< it_activityGrid->second << "==" << act_min << ")" << std::endl;
#endif
				equal_grid[it_activityGrid->first] = it_activityGrid->second;
			} else if (compare_min == 1) {
				int compare_max = common::Maths::compareDoubles(it_activityGrid->second, act_max, DELTA);
				if (compare_max == 0) {
#ifdef ACTIVITYGRID_DEBUG
					//std::cout << "ActivityGrid::getGridPointsByActivity: compare_min == 1)" << it_activityGrid->first
					//		<< "(" << it_activityGrid->second << "==" << compare_max << ")" << std::endl;
#endif
					equal_grid[it_activityGrid->first] = it_activityGrid->second;
				} else if (compare_max == -1) {
#ifdef ACTIVITYGRID_DEBUG
					//std::cout << "ActivityGrid::getGridPointsByActivity: compare_max == -1" << it_activityGrid->first
					//		<< "(" << it_activityGrid->second << "<" << compare_max << ")" << std::endl;
#endif
					equal_grid[it_activityGrid->first] = it_activityGrid->second;
				}
			}
			++it_activityGrid;
		}
	}
	return equal_grid;
}
void ActivityGrid::compareGridPoints(const ActivityGrid & grid_comp, int & equal_to, int & greater_than,
		int & less_than, int & not_known) const {
	// forall in activityGrid
	{
		GridContainer::const_iterator it_activityGrid = activityGrid.begin();
		const GridContainer::const_iterator it_activityGrid_end = activityGrid.end();
		while (it_activityGrid != it_activityGrid_end) {
			ActivityGrid::GridContainer::const_iterator it_found = grid_comp.findGridCoordinate(it_activityGrid->first);
			if (it_found != grid_comp.getActivityGrid().end()) {
				int comparison = common::Maths::compareDoubles(it_found->second, it_activityGrid->second);
				//std::cout << "ActivityGrid::compareGridPoints: " << it_found->second << " compared to "
				//		<< it_activityGrid->second << " is " << comparison << std::endl;
				if (comparison == 1) {
#ifdef ACTIVITYGRID_DEBUG
					//	std::cout << "ActivityGrid::compareGridPoints: comparison >" << it_found->first << ": "
					//			<< it_found->second << " == " << it_activityGrid->first << ": " << it_activityGrid->second
					//			<< std::endl;
#endif
					++greater_than;
				} else if (comparison == -1) {
#ifdef ACTIVITYGRID_DEBUG
					//	std::cout << "ActivityGrid::compareGridPoints: comparison <" << it_found->first << ": "
					//			<< it_found->second << " == " << it_activityGrid->first << ": " << it_activityGrid->second
					//			<< std::endl;
#endif
					++less_than;
				} else {
#ifdef ACTIVITYGRID_DEBUG
					//	std::cout << "ActivityGrid::compareGridPoints: comparison == " << it_found->first << ": "
					//			<< it_found->second << " == " << it_activityGrid->first << ": " << it_activityGrid->second
					//			<< std::endl;
#endif
					++equal_to;
				}
			} else {
				++not_known;
			}
			++it_activityGrid;
		}
	}
}

void ActivityGrid::compareGridPoints(const double comp_act, int & equal_to, int & greater_than, int & less_than) const {
	// forall in activityGrid
	{
		GridContainer::const_iterator it_activityGrid = activityGrid.begin();
		const GridContainer::const_iterator it_activityGrid_end = activityGrid.end();
		while (it_activityGrid != it_activityGrid_end) {
			int comparison = common::Maths::compareDoubles(comp_act, it_activityGrid->second);
			if (comparison == 1) {
				++greater_than;
			} else if (comparison == -1) {
				++less_than;
			} else {
				++equal_to;
			}
		}
		++it_activityGrid;
	}
}

double ActivityGrid::getActivitySummation() const {
	double total = 0;
	// forall in activityGrid
	{
		GridContainer::const_iterator it_activityGrid = activityGrid.begin();
		const GridContainer::const_iterator it_activityGrid_end = activityGrid.end();
		while (it_activityGrid != it_activityGrid_end) {
			total += it_activityGrid->second;
			++it_activityGrid;
		}
	}
	return total;
}

bool ActivityGrid::checkGridPointCount(const CoordinatesIntegers & dimension_point_count) {
	bool success = true;
	int actual_size = this->activityGrid.size();
	int expected_size = this->boundingUnitCoordinates.getX() * this->boundingUnitCoordinates.getY()
			* this->boundingUnitCoordinates.getZ();
	if (actual_size != expected_size) {
		std::cout << "ActivityGrid::checkGridPointCount: " << "ERROR: Size is " << actual_size << " when expected "
				<< expected_size << std::endl;
		return false;
	}
	int gridX = dimension_point_count.getX();
	int gridY = dimension_point_count.getY();
	int gridZ = dimension_point_count.getZ();
	for (int i = 0; i < gridX - 1 && (success == true); i++) {
		for (int j = 0; j < gridY - 1 && (success == true); j++) {
			for (int k = 0; k < gridZ - 1 && (success == true); k++) {
				CoordinatesIntegers coords(i, j, k);
				GridContainer::const_iterator it_found = this->findGridCoordinate(coords);
				if (it_found == activityGrid.end()) {
					std::cout << "ActivityGrid::checkGridPointCount: " << "Error: Could not find coordinates "
							<< coords << std::endl;
					success = false;

				}
			}
		}
	}
	return success;
}

bool ActivityGrid::checkGridPointValues(const double min, const double max) {
	bool success = true;
	int gridX = (boundingUnitCoordinates.getX());
	int gridY = (boundingUnitCoordinates.getY());
	int gridZ = (boundingUnitCoordinates.getZ());
#ifdef ACTIVITYGRID_DEBUG
	std::cout << "ActivityGrid::checkGridPointValues: " << " (" << gridX << ", " << gridY << ", " << gridZ << ")"
			<< std::endl;
#endif
	for (int i = 0; i < gridX && (success == true); i++) {
		for (int j = 0; j < gridY && (success == true); j++) {
			for (int k = 0; k < gridZ && (success == true); k++) {
				CoordinatesIntegers coords(i, j, k);
				GridContainer::const_iterator it_found = this->findGridCoordinate(coords);
				if (it_found != activityGrid.end()) {
					if ((it_found->second < min) || (it_found->second > max)) {
						std::cout << "ActivityGrid::checkGridPointValues: " << " coordinates (" << i << ", " << j
								<< ", " << k << ")" << " value " << it_found->second << " out of range " << "{" << min
								<< ", " << max << "}" << std::endl;
						success = false;
					}
				} else {
					std::cout << "ActivityGrid::checkGridPointValues: " << "Error: Could not find coordinates (" << i
							<< ", " << j << ", " << k << ")" << std::endl;
					success = false;
				}
			}
		}
	}
	return success;
}

boost::shared_ptr<BoundingBoxElement> ActivityGrid::getBoundingBoxElement(Point element, Point reference) const {
	Point temp_pt(element.getX(), element.getY(), element.getZ());
	double temp_act = this->getGridPointActivity(element.getX(), element.getY(), element.getZ());
	double temp_dist = reference.getDistance(temp_pt);
	boost::shared_ptr<BoundingBoxElement> temp_element(new BoundingBoxElement(temp_pt, temp_dist, temp_act));
	return temp_element;
}

const boost::shared_ptr<BoundingBox> ActivityGrid::getBoundingBox(Point reference, int depth) {
	boost::shared_ptr<BoundingBox> temp_box(new BoundingBox(*this, reference, depth));
	return temp_box;
}

const ActivityGrid::GridContainer::const_iterator ActivityGrid::findGridCoordinate(const CoordinatesIntegers & coords) const {

	return activityGrid.find(coords);

	/**
	 bool found = false;

	 GridContainer::const_iterator it_found = activityGrid.end();
	 // forall in activityGrid
	 {
	 GridContainer::const_iterator it_activityGrid = activityGrid.begin();
	 const GridContainer::const_iterator it_activityGrid_end = activityGrid.end();
	 while (it_activityGrid != it_activityGrid_end && found == false) {
	 if ((it_activityGrid->first.getX() == coords.getX())) {
	 if (it_activityGrid->first.getY() == coords.getY()) {
	 if (it_activityGrid->first.getZ() == coords.getZ()) {
	 it_found = it_activityGrid;
	 found = true;
	 #ifdef ACTIVITYGRID_DEBUG
	 //	std::cout << "ActivityGrid::findGridCoordinate: Searching for " << coords << " in "
	 //		<< it_activityGrid->first << "....." << "true" << std::endl;
	 #endif
	 }
	 }
	 }else{
	 #ifdef ACTIVITYGRID_DEBUG
	 //	std::cout << "ActivityGrid::findGridCoordinate: Searching for " << coords << " in "
	 //			<< it_activityGrid->first << "....." << "false" << std::endl;
	 #endif
	 }
	 ++it_activityGrid;
	 }
	 }
	 return it_found;
	 **/
}
ActivityGrid::GridContainer::iterator ActivityGrid::findMutableGridCoordinate(const CoordinatesIntegers & coords) {
	return activityGrid.find(coords);

	/**
	 bool found = false;
	 GridContainer::iterator it_found = activityGrid.end();
	 // forall in activityGrid
	 {
	 GridContainer::iterator it_activityGrid = activityGrid.begin();
	 const GridContainer::const_iterator it_activityGrid_end = activityGrid.end();
	 while (it_activityGrid != it_activityGrid_end && found == false) {
	 if ((it_activityGrid->first.getX() == coords.getX())) {
	 if (it_activityGrid->first.getY() == coords.getY()) {
	 if (it_activityGrid->first.getZ() == coords.getZ()) {
	 it_found = it_activityGrid;
	 found = true;
	 #ifdef ACTIVITYGRID_DEBUG
	 //	std::cout << "ActivityGrid::findMutableGridCoordinate: Searching for " << coords << " in "
	 //			<< it_activityGrid->first << "....." << "true" << std::endl;
	 #endif
	 }
	 }
	 }
	 ++it_activityGrid;
	 }
	 }
	 return it_found;
	 */
}

ActivityGrid::GridContainer::iterator ActivityGrid::setGridPointActivity(const CoordinatesIntegers & coords, double val) {
	GridContainer::iterator it_found = this->findMutableGridCoordinate(coords);
	if (it_found != activityGrid.end()) {
		it_found->second = val;
	} else {
		std::cout << "ActivityGrid::setGridCoordinate: " << "WARNING: Not setting value, " << coords << " not found."
				<< std::endl;
	}
	return it_found;
}

std::pair<ActivityGrid::CoordinatesIntegers, double> &
ActivityGrid::applyPointActivityToGridPoint(const Point & source, const double source_activity,
		std::pair<CoordinatesIntegers, double> & sink, const ActivityModifier modifier,
		BoundingBox::InterpolationStyle decayStyle) {
	//	std::cout << "ActivityGrid::applyPointActivityToGridPoint: " << sink.second << std::endl;

	if (source_activity != 0) {
		// calculate applied activity
		Point dest(sink.first.getX(), sink.first.getY(), sink.first.getZ());
		double actual_decay_per_grid_unit = activityDecay / static_cast<double>( this->getScale());
		double act_mod;
		if (decayStyle == BoundingBox::InterpolationStyle::INVERSE_R) {
			act_mod = source_activity - (source.getDistance(dest) * actual_decay_per_grid_unit);
			if (source_activity > 0) {
				act_mod = std::max(0.0, act_mod);
#ifdef ACTIVITYGRID_DEBUG
				//	std::cout << "ActivityGrid::applyPointActivityToGridPoint: " << "dest: " << dest << " act_mod: "
				//			<< act_mod << "=" << source.getDistance(dest) << "*" << actual_decay_per_grid_unit << std::endl;
#endif
			} else {
				act_mod = std::min(0.0, act_mod);
			}
		} else if (decayStyle == BoundingBox::InterpolationStyle::INVERSE_R2) {
			act_mod = source_activity - (source.getDistance(dest) * actual_decay_per_grid_unit);
			if (source_activity > 0) {
				act_mod = std::max(0.0, act_mod);
			} else {
				act_mod = std::min(0.0, act_mod);
			}
		} else {
			std::cout << "ActivityGrid::applyPointActivityToGridPoint: "
					<< "ERROR: Unknown Interpolation style, setting to zero..." << std::endl;
			act_mod = 0;
		}
#ifdef ACTIVITYGRID_DEBUG
		//	std::cout << "ActivityGrid::applyPointActivityToGridPoint: " << "dest: " << dest << " act_mod: " << act_mod
		//		<< "=" << source.getDistance(dest) << "*" << actual_decay_per_grid_unit << std::endl;
#endif
		if (modifier == ActivityModifier::ACTIVITY_MODIFIER_ADDITION) {
			sink.second = sink.second + act_mod;
		} else if (modifier == ActivityModifier::ACTIVITY_MODIFIER_MULTIPLY) {
			sink.second = sink.second * act_mod;
		} else if (modifier == ActivityModifier::ACTIVITY_MODIFIER_INVERT) {
			sink.second = -(sink.second);
		} else {
			sink.second = sink.second + act_mod;
		}
	}
	//std::cout << "ActivityGrid::applyPointActivityToGridPoint: END: " << sink.second << std::endl;
	return sink;
}

std::ostream& operator<<(std::ostream & os, const ActivityGrid & obj) {
	std::cout << "ActivityGrid::operator <<: " << "" << std::endl;
	// forall in activityGrid
	{
		ActivityGrid::GridContainer::const_iterator it_activityGrid = obj.getActivityGrid().begin();
		const ActivityGrid::GridContainer::const_iterator it_activityGrid_end = obj.getActivityGrid().end();
		while (it_activityGrid != it_activityGrid_end) {
			os<< it_activityGrid->first <<" = "<< it_activityGrid->second <<std::endl;
			++it_activityGrid;
		}
	}
	return os;
}

}//NAMESPACE
}
//NAMESPACE
