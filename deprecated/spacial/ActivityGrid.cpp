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
	boundingUnitCoordinates(x, y, z), activityDecay(1), scale(sc), MIN_SCALE(1), MAX_SCALE(1000) {
	common::Maths::clamp(scale, MIN_SCALE, MAX_SCALE);
	std::cout << "ActivityGrid::ActivityGrid: " << "SCALE: " << scale << std::endl;
	int gridX = (scale * this->getBoundingUnitCoordinates().get<0> ()) ;
	int gridY = (scale * this->getBoundingUnitCoordinates().get<1> ()) ;
	int gridZ = (scale * this->getBoundingUnitCoordinates().get<2> ()) ;
	this->reSize(Coordinates(gridX, gridY, gridZ));
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
		std::cout << "ActivityGrid::getInterpolatedActivity: " << "level: " << i + 1 << " total_act: " << total_act
				<< std::endl;
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

int ActivityGrid::getScale() const {
	return scale;
}

void ActivityGrid::randomise(const double min, const double max) {
	int gridX = (scale * this->getBoundingUnitCoordinates().get<0> ()) - 1;
	int gridY = (scale * this->getBoundingUnitCoordinates().get<1> ()) - 1;
	int gridZ = (scale * this->getBoundingUnitCoordinates().get<2> ()) - 1;
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
#ifdef ACTIVITYGRID_DEBUG
	std::cout << "ActivityGrid::clearGrid: " << "" << std::endl;
#endif
	int gridX = (scale * this->getBoundingUnitCoordinates().get<0> ()) - 1;
	int gridY = (scale * this->getBoundingUnitCoordinates().get<1> ()) - 1;
	int gridZ = (scale * this->getBoundingUnitCoordinates().get<2> ()) - 1;
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

	int gridX = (scale * this->getBoundingUnitCoordinates().get<0> ()) - 1;
	int gridY = (scale * this->getBoundingUnitCoordinates().get<1> ()) - 1;
	int gridZ = (scale * this->getBoundingUnitCoordinates().get<2> ()) - 1;
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
void ActivityGrid::reSize(const Coordinates & new_size, double val) {

	int new_gridX = scale * new_size.get<0> ();
	int new_gridY = scale * new_size.get<1> ();
	int new_gridZ = scale * new_size.get<2> ();

#ifdef ACTIVITYGRID_DEBUG
	std::cout << "ActivityGrid::reSize: " << "(" << new_gridX << ", " << new_gridY << ", " << new_gridZ << ")"
			<< std::endl;
#endif
	// case 1, no size (new)
	if (activityGrid.size() == 0) {
		for (int i = 0; i < new_gridX; i++) {
			for (int j = 0; j < new_gridY; j++) {
				for (int k = 0; k < new_gridZ; k++) {
					Coordinates coords(i, j, k);
					activityGrid[coords] = val;
				}
			}
		}
	} else {
		int current_gridX = (scale * this->getBoundingUnitCoordinates().get<0> ()) - 1;
		int current_gridY = (scale * this->getBoundingUnitCoordinates().get<1> ()) - 1;
		int current_gridZ = (scale * this->getBoundingUnitCoordinates().get<2> ()) - 1;
		const int x_diff = new_gridX - current_gridX;
		const int y_diff = new_gridY - current_gridY;
		const int z_diff = new_gridZ - current_gridZ;

		const int x_grow = std::max(0, x_diff);
		const int y_grow = std::max(0, y_diff);
		const int z_grow = std::max(0, z_diff);

		const int x_shrink = std::min(0, x_diff);
		const int y_shrink = std::min(0, y_diff);
		const int z_shrink = std::min(0, z_diff);
		this->growDimensions(Coordinates(x_grow, y_grow, z_grow), val);
		this->shrinkDimensions(Coordinates(x_shrink, y_shrink, z_shrink));
	}

	boundingUnitCoordinates = Coordinates(new_size.get<0> (), new_size.get<1> (), new_size.get<2> ());
}

void ActivityGrid::growDimensions(const Coordinates & coords, double val) {

	int x_grow = coords.get<0> ();
	int y_grow = coords.get<1> ();
	int z_grow = coords.get<2> ();
	int current_gridX = (scale * this->getBoundingUnitCoordinates().get<0> ()) - 1;
	int current_gridY = (scale * this->getBoundingUnitCoordinates().get<1> ()) - 1;
	int current_gridZ = (scale * this->getBoundingUnitCoordinates().get<2> ()) - 1;

#ifdef ACTIVITYGRID_DEBUG
	std::cout << "ActivityGrid::growDimensions: " << "(" << x_grow << ", " << y_grow << ", " << z_grow << ")"
			<< std::endl;
#endif
	// deal with x increase
	for (int i = 0; i < x_grow; i++) {
		for (int j = 0; j < current_gridY; j++) {
			for (int k = 0; k < current_gridZ; k++) {
				Coordinates coords(i + current_gridX + 1, j, k);
				activityGrid[coords] = val;
			}
		}
	}

	// deal with y increase
	for (int i = 0; i < current_gridX; i++) {
		for (int j = 0; j < y_grow; j++) {
			for (int k = 0; k < current_gridZ; k++) {
				Coordinates coords(i, j + current_gridY + 1, k);
				activityGrid[coords] = val;
			}
		}
	}

	// deal with z increase
	for (int i = 0; i < current_gridX; i++) {
		for (int j = 0; j < current_gridY; j++) {
			for (int k = 0; k < z_grow; k++) {
				Coordinates coords(i, j, k + current_gridZ + 1);
				activityGrid[coords] = val;
			}
		}
	}

}

void ActivityGrid::shrinkDimensions(const Coordinates & coords) {

	double max_x = coords.get<0> ();
	double max_y = coords.get<1> ();
	double max_z = coords.get<2> ();

#ifdef ACTIVITYGRID_DEBUG
	std::cout << "ActivityGrid::shrinkDimensions: " << "(" << max_x << ", " << max_y << ", " << max_z << ")"
			<< std::endl;
#endif

	// forall in activityGrid
	{
		std::map<Coordinates, double>::iterator it_activityGrid = activityGrid.begin();
		const std::map<Coordinates, double>::const_iterator it_activityGrid_end = activityGrid.end();
		while (it_activityGrid != activityGrid.end()) {
			double temp_x = it_activityGrid->first.get<0> ();
			double temp_y = it_activityGrid->first.get<1> ();
			double temp_z = it_activityGrid->first.get<2> ();
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
}

void ActivityGrid::reScale(const int new_scale) {
	std::map<Coordinates, double> new_grid;

	int sc = new_scale;
	common::Maths::clamp(sc, MIN_SCALE, MAX_SCALE);
	int gridX = (scale * this->getBoundingUnitCoordinates().get<0> ()) - 1;
	int gridY = (scale * this->getBoundingUnitCoordinates().get<1> ()) - 1;
	int gridZ = (scale * this->getBoundingUnitCoordinates().get<2> ()) - 1;
	for (int i = 0; i < gridX - 1; i++) {
		for (int j = 0; j < gridY - 1; j++) {
			for (int k = 0; k < gridZ - 1; k++) {
				double scaled_x = (double) i / (double) sc;
				double scaled_y = (double) j / (double) sc;
				double scaled_z = (double) k / (double) sc;
				double interpolated_act = this->getInterpolatedActivity(Point(scaled_x, scaled_y, scaled_z));
				Coordinates coords(i, j, k);
				new_grid[coords] = interpolated_act;
			}
		}
	}
	this->activityGrid = new_grid;
	this->scale = new_scale;
}

const ActivityGrid::Coordinates & ActivityGrid::getBoundingUnitCoordinates() const {
	return boundingUnitCoordinates;
}
double ActivityGrid::getActivityDecay() const {
	return activityDecay;
}

void ActivityGrid::setActivityDecay(double d) {
	activityDecay = d;
}

bool ActivityGrid::checkGridPointCount(const Coordinates & dimension_point_count) {
	bool success = true;
	int gridX = dimension_point_count.get<0> ();
	int gridY = dimension_point_count.get<1> ();
	int gridZ = dimension_point_count.get<2> ();
	for (int i = 0; i < gridX-1 && (success == true); i++) {
		for (int j = 0; j < gridY-1 && (success == true); j++) {
			for (int k = 0; k < gridZ -1&& (success == true); k++) {
				Coordinates coords(i, j, k);
				std::map<Coordinates, double>::iterator it_found = activityGrid.find(coords);
				if (it_found == activityGrid.end()) {
					std::cout << "ActivityGrid::checkGridPointCount: " << "Error: Could not find coordinates (" << i
							<< ", " << j << ", " << k << ")" << std::endl;
					success = false;

				}
			}
		}
	}
	return success;
}

bool ActivityGrid::checkGridPointValues(const double min, const double max) {
	bool success = true;
	int gridX = (scale * boundingUnitCoordinates.get<0> ()) - 1;
	int gridY = (scale * boundingUnitCoordinates.get<1> ()) - 1;
	int gridZ = (scale * boundingUnitCoordinates.get<2> ()) - 1;
#ifdef ACTIVITYGRID_DEBUG
	std::cout << "ActivityGrid::checkGridPointValues: " << " (" << gridX << ", " << gridY << ", " << gridZ << ")"
			<< std::endl;
#endif
	for (int i = 0; i < gridX && (success == true); i++) {
		for (int j = 0; j < gridY && (success == true); j++) {
			for (int k = 0; k < gridZ && (success == true); k++) {
				Coordinates coords(i, j, k);
				std::map<Coordinates, double>::iterator it_found = activityGrid.find(coords);
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

}//NAMESPACE
}//NAMESPACE
