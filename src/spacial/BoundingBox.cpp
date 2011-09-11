/*
 * BoundingBox.cpp
 *
 *  Created on: 27 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

//#define BOUNDINGBOX_DEBUG

#include "BoundingBox.h"
#include "ActivityGrid.h"
#include "Coordinates.h"

namespace cryomesh {

namespace spacial {

BoundingBox::BoundingBox(const ActivityGrid & ag, Point & pt, int depth) :
	activityGrid(ag), boxElements (){
	const Coordinates<int> & coords = activityGrid.getBoundingUnitCoordinates();
	int gridX = coords.getX();
	int gridY = coords.getY();
	int gridZ = coords.getZ();

	if ((pt.getX() > gridX) || (pt.getY() > gridY) || (pt.getZ() > gridZ)) {
		std::cout << "BoundingBox::BoundingBox: " << "Error: Not constructing bounding box, reference point " << "("
				<< pt.getX() << ", " << pt.getY() << ", " << pt.getZ() << ") is outside bounding box " << "(" << gridX
				<< ", " << gridY << ", " << gridZ << ")" << std::endl;
	} else {

		int round_x = static_cast<int>((pt.getX()) - (depth - 1));
		int round_y =  static_cast<int>((pt.getY()) - (depth - 1));
		int round_z =  static_cast<int>( (pt.getZ()) - (depth - 1));

#ifdef BOUNDINGBOX_DEBUG
		std::cout << "BoundingBox::BoundingBox: " << "building from point (" << pt.getX() << ", " << pt.getY() << ", "
		<< pt.getZ() << ")" << " with depth "<<depth<<" using corner (" << round_x << ", " << round_y << ", " << round_z << ")"
		<< std::endl;
#endif
		int corner_plus = (2 * depth) - 1;
		boxElements[Reference] = boost::shared_ptr<BoundingBoxElement>(new BoundingBoxElement(pt));
		boxElements[FrontBottomLeft] = activityGrid.getBoundingBoxElement(Point(round_x, round_y, round_z), pt);
		boxElements[FrontBottomRight] = activityGrid.getBoundingBoxElement(
				Point(round_x + corner_plus, round_y, round_z), pt);
		boxElements[BackBottomLeft] = activityGrid.getBoundingBoxElement(
				Point(round_x, round_y + corner_plus, round_z), pt);
		boxElements[BackBottomRight] = activityGrid.getBoundingBoxElement(
				Point(round_x + corner_plus, round_y + corner_plus, round_z), pt);
		boxElements[FrontTopLeft] = activityGrid.getBoundingBoxElement(Point(round_x, round_y, round_z + corner_plus),
				pt);
		boxElements[FrontTopRight] = activityGrid.getBoundingBoxElement(
				Point(round_x + corner_plus, round_y, round_z + corner_plus), pt);
		boxElements[BackTopLeft] = activityGrid.getBoundingBoxElement(
				Point(round_x, round_y + corner_plus, round_z + corner_plus), pt);
		boxElements[BackTopRight] = activityGrid.getBoundingBoxElement(
				Point(round_x + corner_plus, round_y + corner_plus, round_z + corner_plus), pt);
	}

#ifdef BOUNDINGBOX_DEBUG
	std::cout << "BoundingBox::BoundingBox: " << "Created: " << *this<<std::endl;
#endif

}

BoundingBox::~BoundingBox() {
}

const std::map<BoundingBox::BoxOrientation, boost::shared_ptr<BoundingBoxElement> > & BoundingBox::getBoxElements() const {
	return boxElements;
}

const boost::shared_ptr<BoundingBoxElement> BoundingBox::getBoxElement(const BoxOrientation & box_orientation) const {
	boost::shared_ptr<BoundingBoxElement> temp_element;
	const std::map<BoxOrientation, boost::shared_ptr<BoundingBoxElement> >::const_iterator it_found = boxElements.find(
			box_orientation);
	if (it_found != boxElements.end()) {
		temp_element = it_found->second;
	}
	return temp_element;
}

double BoundingBox::getInterpolatedActivity(const InterpolationStyle style) const {
#ifdef BOUNDINGBOX_DEBUG
	std::cout << "BoundingBox::getInterpolatedActivity: START" << std::endl;
#endif
	const double ZERO_DELTA = 0.00001;
	double activity_sum = 0;
	// forall in boxElements
	{
		std::map<BoxOrientation, boost::shared_ptr<BoundingBoxElement> >::const_iterator it_boxElements =
				boxElements.begin();
		const std::map<BoxOrientation, boost::shared_ptr<BoundingBoxElement> >::const_iterator it_boxElements_end =
				boxElements.end();
		while (it_boxElements != it_boxElements_end) {
			double box_point_act = it_boxElements->second->getActivity();
			double box_point_dist = it_boxElements->second->getDistance();
			double interpolated_act;
			if ((box_point_act >  ZERO_DELTA) || (box_point_act <  -ZERO_DELTA)) {
				if (style == InterpolationStyle::INVERSE_R) {
					double remove_activity = (activityGrid.getActivityDecay() * box_point_dist);
					if (box_point_act >= 0) {
						interpolated_act = std::max(0.0, (box_point_act - remove_activity));
#ifdef BOUNDINGBOX_DEBUG
						std::cout << "BoundingBox::getInterpolatedActivity: "
						<< "interpolated_act = box_point_act - remove_activity -> " << interpolated_act << " = "
						<< box_point_act << " - " << remove_activity << std::endl;
#endif
					} else {
						interpolated_act = std::min(0.0, (box_point_act + remove_activity));
#ifdef BOUNDINGBOX_DEBUG
						std::cout << "BoundingBox::getInterpolatedActivity: "
						<< "interpolated_act = box_point_act + remove_activity -> " << interpolated_act << " = "
						<< box_point_act << " + " << remove_activity << std::endl;
#endif
					}
				} else if (style == InterpolationStyle::INVERSE_R2) {
					double remove_activity = (activityGrid.getActivityDecay() * box_point_dist);
					if (box_point_act >= 0) {
						interpolated_act = box_point_act - (remove_activity * remove_activity);
					} else {
						interpolated_act = box_point_act + (remove_activity * remove_activity);

					}
				} else {
					std::cout << "BoundingBox::getInterpolatedActivity: " << "ERROR: Unknown Interpolation style"
							<< std::endl;
					assert(false);
				}
				if ((box_point_act > 0 && interpolated_act < 0) || (box_point_act < 0 && interpolated_act > 0)) {
					interpolated_act = 0;
				}
				activity_sum += interpolated_act;
			}
			++it_boxElements;
		}
	}
#ifdef BOUNDINGBOX_DEBUG
	std::cout << "BoundingBox::getInterpolatedActivity: END " << "final activity_sum: " << activity_sum << std::endl;
#endif
	return activity_sum;
}

std::ostream& operator<<(std::ostream & os, const BoundingBox & obj) {
	os << "BoundingBox: "<<&obj<< std::endl;
	// forall in boxElements
	{
		std::map<BoundingBox::BoxOrientation, boost::shared_ptr<BoundingBoxElement> >::const_iterator it_boxElements =
				obj.getBoxElements().begin();
		const std::map<BoundingBox::BoxOrientation, boost::shared_ptr<BoundingBoxElement> >::const_iterator
				it_boxElements_end = obj.getBoxElements().end();
		while (it_boxElements != it_boxElements_end) {
			os << "\t" << *(it_boxElements->second) << std::endl;
			++it_boxElements;
		}
	}
	return os;
}

}//NAMESPACE

}//NAMESPACE
