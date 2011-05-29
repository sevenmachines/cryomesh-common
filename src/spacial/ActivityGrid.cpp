/*
 * ActivityGrid.cpp
 *
 *  Created on: 27 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#define ACTIVITYGRID_DEBUG

#include "ActivityGrid.h"

namespace cryomesh {

namespace spacial {

ActivityGrid::ActivityGrid(const int x, const int y, const int z, const int sc) :
	activityDecay(1), scale(sc), MIN_SCALE(1), MAX_SCALE(1000) {
	common::Maths::clamp(scale, MIN_SCALE, MAX_SCALE);
	this->setBoundingBox(CoordinatesIntegers(x, y, z));
}

ActivityGrid::~ActivityGrid() {

}

double ActivityGrid::getInterpolatedActivity(const Point & point, int depth,
		const BoundingBox::InterpolationStyle style) {
	// get rescaled point
	Point rescaled_point = point.getScaled(1.0/(double)this->getScale());
	// repeatedly get bounding boxes and sum the results
	double total_act = 0;
	for (int i = 0; i < depth; i++) {
		boost::shared_ptr<BoundingBox> temp_box = this->getBoundingBox(rescaled_point, i + 1);
		total_act += temp_box->getInterpolatedActivity();
#ifdef ACTIVITYGRID_DEBUG
		std::cout << "ActivityGrid::getInterpolatedActivity: " << "level: " << i + 1 << " total_act: " << total_act
				<< std::endl;
#endif
	}
	return total_act;
}

void ActivityGrid::setNearestActivity(const Point & point, const double & activity) {
	// get rescaled point
	Point rescaled_point = point.getScaled(1.0/(double)this->getScale());
	std::cout << "setNearestActivity: " << "TODO" << std::endl;
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
		const GridContainer::const_iterator it_activityGrid_end = activityGrid.end();
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

const ActivityGrid::CoordinatesIntegers & ActivityGrid::getBoundingUnitCoordinates() const {
	return boundingUnitCoordinates;
}
const ActivityGrid::CoordinatesIntegers  ActivityGrid::getScaledBoundingUnitCoordinates() const {
	int sc = this->getScale();
	return CoordinatesIntegers(boundingUnitCoordinates.getX()*sc, boundingUnitCoordinates.getY()*sc, boundingUnitCoordinates.getZ()*sc );
}
double ActivityGrid::getActivityDecay() const {
	return activityDecay;
}

void ActivityGrid::setActivityDecay(double d) {
	activityDecay = d;
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

ActivityGrid::GridContainer::iterator ActivityGrid::setGridCoordinate(const CoordinatesIntegers & coords, double val) {
	GridContainer::iterator it_found = this->findMutableGridCoordinate(coords);
	if (it_found != activityGrid.end()) {
		it_found->second = val;
	} else {
		std::cout << "ActivityGrid::setGridCoordinate: " << "WARNING: Not setting value, " << coords << " not found."
				<< std::endl;
	}
}

}//NAMESPACE
}//NAMESPACE
