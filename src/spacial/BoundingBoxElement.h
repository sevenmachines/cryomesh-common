/*
 * BoundingBoxElement.h
 *
 *  Created on: 27 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef BOUNDINGBOXELEMENT_H_
#define BOUNDINGBOXELEMENT_H_

#include "Point.h"
namespace cryomesh {

namespace spacial {

class BoundingBoxElement {
public:
	BoundingBoxElement(const Point & p) :
		position(p), distance(0), activity(0) {

	}
	BoundingBoxElement(const Point & p, double dist, double act) :
		position(p), distance(dist) , activity(act){
	}
	virtual ~BoundingBoxElement() {
	}

	const Point & getPosition() const {
		return position;
	}
	void setPosition(const Point & pt)  {
			 position = Point(pt);
		}
	double getDistance() const {
		return distance;
	}
	void setDistance(double d)  {
		distance = d;
	}
	double getActivity() const {
		return activity;
	}
	void setActivity(double d)  {
		activity = d;
	}

	/**
		 * To stream operator
		 *
		 *	@param std::ostream & os
		 *		The output stream
		 *	@param const BoundingBoxElement & obj
		 *		The object to stream
		 *
		 *	@return std::ostream &
		 *		The output stream
		 */
		friend std::ostream& operator<<(std::ostream & os, const BoundingBoxElement & obj){
			os << "BoundingBoxElement: "<< obj.getPosition()<<" distance: "<<obj.getDistance()<<" activity: "<<obj.getActivity();
			return os;
		}
private:
	Point position;
	double distance;
	double activity;

};

}

}

#endif /* BOUNDINGBOXELEMENT_H_ */
