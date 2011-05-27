/*
 * BoundingBox.h
 *
 *  Created on: 27 May 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_

#include "BoundingBoxElement.h"
#include "Point.h"
#include <map>
#include <boost/shared_ptr.hpp>

namespace cryomesh {

namespace spacial {

class ActivityGrid;

class BoundingBox {
public:
	/**
	 * Style to drop of bounding box activities
	 * lose one activity per unit distance
	 */
	enum InterpolationStyle{
		INVERSE_R, INVERSE_R2
	};
	enum BoxOrientation {
		Reference,
		FrontBottomLeft,
		FrontBottomRight,
		BackBottomLeft,
		BackBottomRight,
		FrontTopLeft,
		FrontTopRight,
		BackTopLeft,
		BackTopRight,
	};
	BoundingBox(const ActivityGrid  & ag, Point & pt, int depth = 1);
	virtual ~BoundingBox();
	double getInterpolatedActivity(const InterpolationStyle style = INVERSE_R)const;
	const boost::shared_ptr<BoundingBoxElement> getBoxElement(const BoxOrientation & box_orientation) const;

	const std::map<BoxOrientation, boost::shared_ptr<BoundingBoxElement> > & getBoxElements()const;

	/**
		 * To stream operator
		 *
		 *	@param std::ostream & os
		 *		The output stream
		 *	@param const BoundingBox & obj
		 *		The object to stream
		 *
		 *	@return std::ostream &
		 *		The output stream
		 */
		friend std::ostream& operator<<(std::ostream & os, const BoundingBox & obj);
private:
	const ActivityGrid & activityGrid;
	std::map<BoxOrientation, boost::shared_ptr<BoundingBoxElement> > boxElements;
};

}

}

#endif /* BOUNDINGBOX_H_ */
