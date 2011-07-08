/*
 * GridContainer.cpp
 *
 *  Created on: 22 Jun 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "GridContainer.h"
#include <iostream>

namespace cryomesh {

namespace spacial {

GridContainer::GridContainer() {
}
GridContainer::GridContainer(const Point & point, const double val) {
	this->setBoundingBox(point, val);
}
GridContainer::~GridContainer() {
	// TODO Auto-generated destructor stub
}

void GridContainer::clear(double d) {
	values.clear();
	int bx = (int)(boundingBox.getX() + 0.5);
	int by = (int)(boundingBox.getY() + 0.5);
	int bz = (int)(boundingBox.getZ() + 0.5);

	for (int i = 0; i< bx; i++) {
		for (int j = 0; i< by; j++) {
			for (int k = 0; i< bz; k++) {
				values[Coordinates<int>(i,j,k)] =d;
			}
		}
	}
}
void GridContainer::setBoundingBox(const Point & point, const double val) {
	boundingBox = point.getMagnitudes();
}

double GridContainer::getValue(const Coordinates<int> & coords) const {
	int cx = coords.getX();
	int cy = coords.getY();
	int cz = coords.getZ();

	double val;

	if ((cx > boundingBox.getX()) || (cy > boundingBox.getY()) || (cz > boundingBox.getZ()) ) {
		std::cout << "GridContainer::getValue: " << "Coords outside bounding box" << std::endl;
		val = 0;
	} else {
		GridStorageConstIterator it_found = values.find(coords);
		if (it_found !=values.end()){
		val = it_found->second;
		}else{
			std::cout<<"GridContainer::getValue: "<<"Coords not found!"<<std::endl;
			val =0;
		}
	}
	return  val;
}

void GridContainer::setValue(const Coordinates<int> & coords, const double val) {
	int cx = coords.getX();
	int cy = coords.getY();
	int cz = coords.getZ();

	if ((cx > boundingBox.getX()) || (cy > boundingBox.getY()) || (cz > boundingBox.getZ()) ) {
		std::cout << "GridContainer::getValue: " << "Coords outside bounding box" << std::endl;
	} else {
		values[Coordinates<int>(cx,cy,cz)] = val;
	}
}

const GridStorage & GridContainer::getValues() const {
	return values;
}
}//NAMESPACE

}//NAMESPACE
