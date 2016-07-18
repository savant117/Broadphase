#pragma once

#include <vector>
#include <string>
using namespace std;

#include "util.h"

/// A structure dependent handle for an added object, which is valid until the object is removed
typedef void* ObjectHandle;

/// Represents an object which has a bounding box and can be asleep
struct Object2D
{
	aabb2 bounds;
	bool asleep;
	
	Object2D()
	{}

	Object2D(const aabb2& bounds_, bool asleep_)
		: bounds(bounds), asleep(asleep_)
	{}
};

/// Single collision between two objects, not ordered in any particular way
struct Collision
{
	ObjectHandle a;
	ObjectHandle b;

	Collision()
	{}

	Collision(ObjectHandle a_, ObjectHandle b_)
		: a(a_), b(b_)
	{}
};

/// A broadphase collision detector. Objects can be added, updated, and removed. Commit must be called after adds, removes, and updates to ensure
/// that the query functions will return correct results
struct IBroadphase2D
{
	/// Initializes a blank data structure
	virtual void init() = 0;

	/// Called after some number of add, update, and removes. Query functions not garunteed to return valid results until this is called.
	virtual void commit() = 0;

	/// Add an object to the structure and return a handle for the new object, valid until the object is removed
	virtual ObjectHandle add(Object2D *obj) = 0;

	/// Signals that the object's state has been updated in some way
	virtual void update(ObjectHandle handle) = 0;

	/// Removes the object and releases its handle
	virtual void remove(ObjectHandle handle) = 0;

	/// Get the actual object associated with the handle
	virtual Object2D* get(ObjectHandle handle) const = 0;

	/// Find all collision pairs, such that their bounding boxes overlap
	virtual vector<Collision> findPairs() const = 0;

	/// Find all objects within or intersecting a bounding box
	virtual vector<ObjectHandle> queryBounds(const aabb2& bounds) const = 0;

	/// Find all objects intersecting with a ray
	virtual vector<ObjectHandle> raycast(const ray2& ray) const = 0;

	/// Calculates the approximate memory used in bytes
	virtual size_t measureMemory() const = 0;

	/// Get a friendly name for display
	virtual string toString() const = 0;
};