/*
 * GridContainer.h
 *
 *  Created on: 22 Jun 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#ifndef GRIDCONTAINER_H_
#define GRIDCONTAINER_H_

#include <vector>
#include <map>
#include "Point.h"
#include "Coordinates.h"

namespace cryomesh {

namespace spacial {

typedef std::map<Coordinates<int>, double > GridStorage;
typedef std::map<Coordinates<int>, double >::iterator GridStorageIterator;
typedef std::map<Coordinates<int>, double >::const_iterator GridStorageConstIterator;

class GridContainer {
public:
	GridContainer();
	GridContainer(const Point & point, const double val = 0.0);
	virtual ~GridContainer();
	void clear(double d) ;
	void setBoundingBox(const Point & point) ;

	double getValue(const Coordinates<int> & coords) const ;
	void setValue(const Coordinates<int> & coords, const double val);
	int getIndex(const Coordinates<int> & coords) const;
	const GridStorage & getValues()const;

protected:
	Point boundingBox;
	GridStorage values;
};

}

}

#endif /* GRIDCONTAINER_H_ */
