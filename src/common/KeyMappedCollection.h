/**
 * KeyMappedCollection.h
 *
 *  Created on: 25 Jan 2011
 *      Author: SevenMachines<SevenMachines@yahoo.co.uk>
 */

#ifndef KEYMAPPEDCOLLECTION_H_
#define KEYMAPPEDCOLLECTION_H_

#include <map>
#include <list>
#include <set>
#include <boost/uuid/uuid.hpp>

#include <boost/shared_ptr.hpp>
namespace cryomesh {

namespace common {

/**
 *  KeyMappedCollection is a template class of U, a key class which supports getKey(), and T, the object class
 */
template<class U, class T>
class KeyMappedCollection {
public:
	/**
	 * Constructor
	 *
	 *		Constructor for Collection<U,T>
	 */
	KeyMappedCollection() :
			objects() {
	}

	/**
	 * Destructor
	 *
	 *		Destructor for Collection<U,T>
	 */
	virtual ~KeyMappedCollection() {

	}

	/**
	 * Clear all objects from collection
	 */
	void clear() {
		objects.clear();
	}

	/**
	 * Add object to collection
	 *
	 * @param boost::shared_ptr<T> obj
	 * 		The object to be added to the collection
	 *
	 * @return boost::shared_ptr<T>
	 * 		The object added to the collection
	 *
	 */
	boost::shared_ptr<T> add(boost::shared_ptr<T> obj) {
		objects[obj->getKey()] = obj;
		return objects.find(obj->getKey())->second;
	}

	/**
	 * Add object to collection by initialiser list
	 *
	 * @param std::initialiser_list<T> obj
	 * 		The values to be added to the collection
	 */
	virtual void add(const std::initializer_list<boost::shared_ptr<T> > & list) {
		// forall in list
		{
			typename std::initializer_list<boost::shared_ptr<T> >::iterator it_list = list.begin();
			const typename std::initializer_list<boost::shared_ptr<T> >::const_iterator it_list_end = list.end();
			while (it_list != it_list_end) {
				objects[(*it_list)->getKey()] = *it_list;
				++it_list;
			}
		}
	}

	/**
	 * Add object to collection by list
	 *
	 * @param std::list<T> list
	 * 		The list values to be added to the collection
	 */
	virtual void add(const std::list<boost::shared_ptr<T> > & list) {
		// forall in list
		{
			typename std::list<boost::shared_ptr<T> >::const_iterator it_list = list.begin();
			const typename std::list<boost::shared_ptr<T> >::const_iterator it_list_end = list.end();
			while (it_list != it_list_end) {
				objects[(*it_list)->getKey()] = *it_list;
				++it_list;
			}
		}
	}
	virtual void add(const std::set<boost::shared_ptr<T> > & list) {
		// forall in list
		{
			typename std::set<boost::shared_ptr<T> >::const_iterator it_list = list.begin();
			const typename std::set<boost::shared_ptr<T> >::const_iterator it_list_end = list.end();
			while (it_list != it_list_end) {
				objects[(*it_list)->getKey()] = *it_list;
				++it_list;
			}
		}
	}

	virtual void add(const std::map<boost::uuids::uuid, boost::shared_ptr<T> > & list) {
		// forall in list
		{
			typename std::map<boost::uuids::uuid, boost::shared_ptr<T> >::const_iterator it_list = list.begin();
			const typename std::map<boost::uuids::uuid, boost::shared_ptr<T> >::const_iterator it_list_end = list.end();
			while (it_list != it_list_end) {
				objects[it_list->first] = it_list->second;
				++it_list;
			}
		}
	}

	/**
	 * Remove objects from collection by initialiser list
	 *
	 * @param std::initialiser_list<T> obj
	 * 		The values to be removed from the collection
	 */
	virtual void remove(const std::initializer_list<boost::shared_ptr<T> > & list) {
		// forall in list
		{
			typename std::initializer_list<boost::shared_ptr<T> >::iterator it_list = list.begin();
			const typename std::initializer_list<boost::shared_ptr<T> >::const_iterator it_list_end = list.end();
			while (it_list != it_list_end) {
				this->remove((*it_list)->getKey());
				++it_list;
			}
		}
	}

	/**
	 * Remove objects from collection by list
	 *
	 * @param std::list<T> list
	 * 		The list values to be removed to the collection
	 */
	virtual void remove(const std::list<boost::shared_ptr<T> > & list) {
		// forall in list
		{
			typename std::list<boost::shared_ptr<T> >::const_iterator it_list = list.begin();
			const typename std::list<boost::shared_ptr<T> >::const_iterator it_list_end = list.end();
			while (it_list != it_list_end) {
				this->remove((*it_list)->getKey());
				++it_list;
			}
		}
	}

	virtual void remove(const std::set<boost::shared_ptr<T> > & list) {
		// forall in list
		{
			typename std::set<boost::shared_ptr<T> >::const_iterator it_list = list.begin();
			const typename std::set<boost::shared_ptr<T> >::const_iterator it_list_end = list.end();
			while (it_list != it_list_end) {
				this->remove((*it_list)->getKey());
				++it_list;
			}
		}
	}

	virtual void remove(const std::map<boost::uuids::uuid, boost::shared_ptr<T> >  & list) {
		// forall in list
		{
			typename std::map<boost::uuids::uuid, boost::shared_ptr<T> >::const_iterator it_list = list.begin();
			const typename std::map<boost::uuids::uuid, boost::shared_ptr<T> >::const_iterator it_list_end = list.end();
			while (it_list != it_list_end) {
				this->remove(it_list->first);
				++it_list;
			}
		}
	}



	// TODO Creating shared_ptr from reference causes deletion issues

	/**
	 * Add object to collection
	 *
	 * @param T & refobj
	 * 		The object to be added to the collection
	 *
	 * @return boost::shared_ptr<T>
	 * 		The object added to the collection
	 *
	 boost::shared_ptr<T> add(T & refobj) {
	 boost::shared_ptr<T> obj =  boost::shared_ptr<T>(&refobj);
	 objects[obj->getKey()] = obj;
	 return objects.find(obj->getKey())->second;
	 }
	 */

	/**
	 * Remove object from collection
	 *
	 * @param boost::shared_ptr<T> obj
	 * 		The object to be removed from the collection
	 *
	 * @return boost::shared_ptr<T>
	 * 		The object removed from the collection
	 *
	 */
	boost::shared_ptr<T> remove(boost::shared_ptr<T> obj) {
		return this->remove(obj->getKey());
	}

	/**
	 * Remove object from collection
	 *
	 * @param T & obj
	 * 		The object to be removed from the collection
	 *
	 * @return boost::shared_ptr<T>
	 * 		The object removed from the collection
	 *
	 */
	boost::shared_ptr<T> remove(T & obj) {
		return this->remove(obj.getKey());
	}

	std::list<boost::shared_ptr<T> > remove(const std::list<U> & keys) {
		std::list<boost::shared_ptr<T> > removed_entries;
		// forall in keys
		{
			typename std::list<U>::const_iterator it_keys = keys.begin();
			const typename std::list<U>::const_iterator it_keys_end = keys.end();
			while (it_keys != it_keys_end) {
				removed_entries.push_back(this->remove(*it_keys));
				++it_keys;
			}
		}
		return removed_entries;
	}
/**
 * Remove object from collection bt key
 *
 * @param U key
 * 		The ey of the object to be removed from the collection
 *
 * @return boost::shared_ptr<T>
 * 		The object removed from the collection
 *
 */
boost::shared_ptr<T> remove(U key) {
	boost::shared_ptr<T> found;
	typename std::map<U, boost::shared_ptr<T> >::iterator it_found = objects.find(key);

	if (it_found != objects.end()) {
		found = it_found->second;
		objects.erase(it_found);
	}
	return found;
}

/**
 * Get keys for underlying connection
 *
 * @return std::list<U>
 * 		The list of keys
 */
std::list<U> getKeyList() const {
	std::list<U> templist;
	// forall in objects
	{
		typename std::map<U, boost::shared_ptr<T> >::const_iterator it_objects = objects.begin();
		const typename std::map<U, boost::shared_ptr<T> >::const_iterator it_objects_end = objects.end();
		while (it_objects != it_objects_end) {
			templist.push_back(it_objects->first);
			++it_objects;
		}
	}
	return templist;
}

/**
 * Get object list for underlying connection
 *
 * @return std::list<U>
 * 		The list of object values
 */
std::vector<boost::shared_ptr<T> > getObjectList() const {
	std::vector<boost::shared_ptr<T> > templist;
	// forall in objects
	{
		typename std::map<U, boost::shared_ptr<T> >::const_iterator it_objects = objects.begin();
		const typename std::map<U, boost::shared_ptr<T> >::const_iterator it_objects_end = objects.end();
		while (it_objects != it_objects_end) {
			templist.push_back(it_objects->second);
			++it_objects;
		}
	}
	return templist;
}

/**
 * Get object using key
 *
 *	@param U key
 *		Key to search for
 * @return boost::shared_ptr<T>
 * 		The found object
 */
const boost::shared_ptr<T> getObjectByKey(const U & key) const {
	boost::shared_ptr<T> tempptr;
	// forall in objects
	typename std::map<U, boost::shared_ptr<T> >::const_iterator it_found = objects.find(key);
	const typename std::map<U, boost::shared_ptr<T> >::const_iterator it_objects_end = objects.end();
	if (it_found != it_objects_end) {
		tempptr = it_found->second;
	}
	return tempptr;
}

/**
 * Get mutable object using key
 *
 *	@param U key
 *		Key to search for
 * @return boost::shared_ptr<T>
 * 		The found object
 */
boost::shared_ptr<T> getMutableObjectByKey(const U & key) {
	boost::shared_ptr<T> tempptr;
	// forall in objects
	typename std::map<U, boost::shared_ptr<T> >::const_iterator it_found = objects.find(key);
	const typename std::map<U, boost::shared_ptr<T> >::const_iterator it_objects_end = objects.end();
	if (it_found != it_objects_end) {
		tempptr = it_found->second;
	}
	return tempptr;
}

boost::shared_ptr<T> getMutableRandomObject() {
	std::vector<boost::shared_ptr<T> > objlist = this->getObjectList();
	random_shuffle(objlist.begin(), objlist.end());
	return *(objlist.begin());
}

std::vector<boost::shared_ptr<T> > getRandomRange(int number) const {
	std::vector<boost::shared_ptr<T> > objlist = this->getObjectList();
	random_shuffle(objlist.begin(), objlist.end());

	// forall in objlist
	typename std::vector<boost::shared_ptr<T> >::iterator it_objlist = objlist.begin();
	const typename std::vector<boost::shared_ptr<T> >::const_iterator it_objlist_end = objlist.end();
	for (int i = 0; i < number && it_objlist != it_objlist_end; i++) {
		++it_objlist;
	}

	std::vector<boost::shared_ptr<T> > rangelist(objlist.begin(), it_objlist);
	return rangelist;

}

const boost::shared_ptr<T> getObjectByIndex(int index) const {
	boost::shared_ptr<T> temp_obj;

	// forall in objects
	{
		typename std::map<U, boost::shared_ptr<T> >::const_iterator it_objects = objects.begin();
		const typename std::map<U, boost::shared_ptr<T> >::const_iterator it_objects_end = objects.end();
		int count = 0;
		while (it_objects != it_objects_end && count < index) {
			++count;
			++it_objects;
		}
		if (it_objects != it_objects_end) {
			temp_obj = it_objects->second;
		}
	}

	return temp_obj;
}
/**
 * Get maximum value
 *
 * @return T
 * 		The maximum value
 */
/*
 boost::shared_ptr< T > getMax()const{
 std::vector<boost::shared_ptr<T> > all_objs = this->getObjectList();
 boost::shared_ptr< T > max(new T);
 // forall in all_objs
 {
 typename std::vector<boost::shared_ptr<T> >::const_iterator it_all_objs = all_objs.begin();
 const  typename std::vector<boost::shared_ptr<T> >::const_iterator it_all_objs_end = all_objs.end();
 while ( it_all_objs != it_all_objs_end){
 if ( **it_all_objs>*max){
 max = *it_all_objs;
 }
 ++it_all_objs;
 }
 }
 return max;
 }
 */
// GETTERS/SETTERS
/**
 * Get underlying collection
 *
 * @return const std::map<U, boost::shared_ptr<T> > &
 * 		The collection
 */
const std::map<U, boost::shared_ptr<T> > & getCollection() const {
	return objects;
}

/**
 * Get mutable underlying collection
 *
 * @return const std::map<U, boost::shared_ptr<T> > &
 * 		The mutable collection
 */
std::map<U, boost::shared_ptr<T> > & getMutableCollection() {
	return objects;
}

/**
 * Get underlying collections size
 *
 * @return int
 * 		The collections size
 */
int getSize() const {
	return objects.size();
}

// Implement iterator

typename std::map<U, boost::shared_ptr<T> >::iterator begin() {
	return objects.begin();
}

typename std::map<U, boost::shared_ptr<T> >::const_iterator begin() const {
	return objects.begin();
}

typename std::map<U, boost::shared_ptr<T> >::iterator end() {
	return objects.end();
}

typename std::map<U, boost::shared_ptr<T> >::const_iterator end() const {
	return objects.end();
}

virtual void enableDebug(bool b) {
#ifdef KEYMAPPEDCOLLECTION_DEBUG
	int count =0;
#endif
	std::map<U, boost::shared_ptr<T> > & all_objects = this->objects;
	// forall in all_objects
	{
		typename std::map<U, boost::shared_ptr<T> >::iterator it_all_objects = all_objects.begin();
		const typename std::map<U, boost::shared_ptr<T> >::const_iterator it_all_objects_end = all_objects.end();
		while (it_all_objects != it_all_objects_end) {
			it_all_objects->second->enableDebug(b);
			++it_all_objects;
#ifdef KEYMAPPEDCOLLECTION_DEBUG
			++ count;
#endif
		}
	}
#ifdef KEYMAPPEDCOLLECTION_DEBUG
	std::cout<<"KeyMappedCollection::enableDebug: "<<count<<std::endl;
#endif
}

protected :
/**
 * The collection of all objects
 *
 * @var std::map<U, boost::shared_ptr<T> >
 */
std::map<U, boost::shared_ptr<T> > objects;
};

}

}

#endif /* KEYMAPPEDCOLLECTION_H_ */
