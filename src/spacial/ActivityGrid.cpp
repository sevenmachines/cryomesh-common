/*
 * ActivityGrid.cpp
 *
 *  Created on: 27 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

//#define ACTIVITYGRID_DEBUG

#include "ActivityGrid.h"

namespace cryomesh {

namespace spacial {

ActivityGrid::ActivityGrid(const int x, const int y, const int z, const double sc) :
	boundingCoordinates(x, y, z), activityDecay(1), scale(sc), MIN_SCALE(1.0 / 1000.0), MAX_SCALE(1000) {
#ifdef ACTIVITYGRID_DEBUG
	std::cout << "ActivityGrid::ActivityGrid: " << "(" << boundingCoordinates.get<0> () << ", "
	<< boundingCoordinates.get<1> () << ", " << boundingCoordinates.get<2> () << ")" << std::endl;
#endif
	int gridX = boundingCoordinates.get<0> ();
	int gridY = boundingCoordinates.get<1> ();
	int gridZ = boundingCoordinates.get<2> ();

	for (int i = 0; i < gridX; i++) {
		for (int j = 0; j < gridY; j++) {
			for (int k = 0; k < gridZ; k++) {
				Coordinates coords(i, j, k);
				activityGrid[coords] = 0.0;
			}
		}
	}
}

ActivityGrid::~ActivityGrid() {

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

double ActivityGrid::getInterpolatedActivity(const Point & point, int depth,
		const BoundingBox::InterpolationStyle style) {
	// repeatedly get bounding boxes and sum the results
	double total_act = 0;
	for (int i = 0; i < depth; i++) {
		boost::shared_ptr<BoundingBox> temp_box = this->getBoundingBox(point, i + 1);
		total_act += temp_box->getInterpolatedActivity();
#ifdef ACTIVITYGRID_DEBUG
		std::cout<<"ActivityGrid::getInterpolatedActivity: "<<"level: "<<i+1<<" total_act: "<< total_act<<std::endl;
#endif
	}
	return total_act;
}
void ActivityGrid::setNearestActivity(const Point & point, const double & activity) {
	std::cout << "setNearestActivity: " << "TODO" << std::endl;
}

const double ActivityGrid::getGridPointActivity(const int x, const int y, const int z) const {
	double activity = 0.0;
	Coordinates coords(x, y, z);
	std::map<Coordinates, double>::const_iterator it_found = activityGrid.find(coords);
	if (it_found != activityGrid.end()) {
		activity = it_found->second;
	} else {
		std::cout << "ActivityGrid::getGridPointActivity: " << "Error: Could not find coordinates (" << x << ", " << y
				<< ", " << z << ")" << std::endl;
	}
	return activity;
}
const std::map<ActivityGrid::Coordinates, double> & ActivityGrid::getActivityGrid() const {
	return activityGrid;
}

double ActivityGrid::getScale() const {
	return scale;
}
void ActivityGrid::setScale(double d) {
	double clamped_scale = std::max(MIN_SCALE, d);
	clamped_scale = std::min(MAX_SCALE, d);
	int gridX = boundingCoordinates.get<0> ();
	int gridY = boundingCoordinates.get<1> ();
	int gridZ = boundingCoordinates.get<2> ();
	this->reScale(gridX, gridY, gridZ, clamped_scale);
}

void ActivityGrid::randomise(const double min, const double max) {
	int gridX = boundingCoordinates.get<0> ();
	int gridY = boundingCoordinates.get<1> ();
	int gridZ = boundingCoordinates.get<2> ();
	for (int i = 0; i < gridX; i++) {
		for (int j = 0; j < gridY; j++) {
			for (int k = 0; k < gridZ; k++) {
				Coordinates coords(i, j, k);
				std::map<Coordinates, double>::iterator it_found = activityGrid.find(coords);
				if (it_found != activityGrid.end()) {
					it_found->second = common::Maths::getRandomDouble(min, max);
				} else {
					std::cout << "ActivityGrid::randomise: " << "Error: Could not find coordinates (" << i << ", " << j
							<< ", " << k << ")" << std::endl;
				}
			}
		}
	}
}

void ActivityGrid::clearGrid(const double val) {
	int gridX = boundingCoordinates.get<0> ();
	int gridY = boundingCoordinates.get<1> ();
	int gridZ = boundingCoordinates.get<2> ();
	for (int i = 0; i < gridX; i++) {
		for (int j = 0; j < gridY; j++) {
			for (int k = 0; k < gridZ; k++) {
				Coordinates coords(i, j, k);
				std::map<Coordinates, double>::iterator it_found = activityGrid.find(coords);
				if (it_found != activityGrid.end()) {
					it_found->second = val;
				} else {
					std::cout << "ActivityGrid::clearGrid: " << "Error: Could not find coordinates (" << i << ", " << j
							<< ", " << k << ")" << std::endl;
				}
			}
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

	int gridX = boundingCoordinates.get<0> ();
	int gridY = boundingCoordinates.get<1> ();
	int gridZ = boundingCoordinates.get<2> ();
	for (int i = 0; i < gridX; i++) {
		for (int j = 0; j < gridY; j++) {
			for (int k = 0; k < gridZ; k++) {
				Coordinates coords(i, j, k);
				std::map<Coordinates, double>::iterator it_found = activityGrid.find(coords);
				if (it_found != activityGrid.end()) {
					if (do_add == true) {
						it_found->second += val;
					}
					if (do_multiply == true) {
						it_found->second *= val;
					}
					if (do_invert == true) {
						it_found->second *= -1;
					}
				} else {
					std::cout << "ActivityGrid::clearGrid: " << "Error: Could not find coordinates (" << i << ", " << j
							<< ", " << k << ")" << std::endl;
				}
			}
		}
	}
}

void ActivityGrid::reScale(const int x, const int y, const int z, const double sc) {
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			for (int k = 0; k < z; k++) {
				double scaled_x = i * sc;
				double scaled_y = j * sc;
				double scaled_z = k * sc;
				double interpolated_act = this->getInterpolatedActivity(Point(scaled_x, scaled_y, scaled_z));
				Coordinates coords(i, j, k);
				std::map<Coordinates, double>::iterator it_found = activityGrid.find(coords);
				if (it_found != activityGrid.end()) {
					it_found->second = interpolated_act;
				} else {
					std::cout << "ActivityGrid::reScale: " << "Error: Could not find coordinates (" << i << ", " << j
							<< ", " << k << ")" << std::endl;
				}
			}
		}
	}

	boundingCoordinates = Coordinates(x, y, z);
}

const ActivityGrid::Coordinates & ActivityGrid::getBoundingCoordinates() const {
	return boundingCoordinates;
}
double ActivityGrid::getActivityDecay() const {
	return activityDecay;
}

void ActivityGrid::setActivityDecay(double d) {
	activityDecay = d;
}

}//NAMESPACE
}//NAMESPACE
