#pragma once

#include <queue>

#include "broadphase.h"

struct NSq2D : public IBroadphase2D
{
	vector<Object2D*> objects;
	queue<size_t> free;

	void init()
	{
		objects.clear();
		objects.reserve(100);
		while (!free.empty())
			free.pop();
	}

	void commit()
	{
	}

	ObjectHandle add(Object2D *obj)
	{
		size_t index = objects.size() - 1;
		if (free.empty())
			objects.push_back(obj);
		else
		{
			index = free.front();
			free.pop();
			objects[index] = obj;
		}
		return (ObjectHandle)index;
	}

	void update(ObjectHandle handle)
	{
	}

	void remove(ObjectHandle handle)
	{
		free.push((size_t)handle);
		objects[(size_t)handle] = 0;
	}

	Object2D* get(ObjectHandle handle) const
	{
		return objects[(size_t)handle];
	}

	vector<Collision> findPairs() const
	{
		vector<Collision> collisions;
		for (size_t i = 0; i < objects.size(); i++)
		{
			for (size_t j = i + 1; j < objects.size(); j++)
			{
				if (!(objects[j]->asleep && objects[j]->asleep) && objects[i]->bounds.intersect(objects[j]->bounds))
				{
					collisions.push_back(Collision((ObjectHandle)i, (ObjectHandle)j));
				}
			}
		}
		return collisions;
	}

	vector<ObjectHandle> queryBounds(const aabb2& bounds) const
	{
		vector<ObjectHandle> hit;
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (bounds.intersect(objects[i]->bounds))
			{
				hit.push_back((ObjectHandle)i);
			}
		}
		return hit;
	}

	vector<ObjectHandle> raycast(const ray2& ray) const
	{
		vector<ObjectHandle> hit;
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (ray.intersect(objects[i]->bounds))
			{
				hit.push_back((ObjectHandle)i);
			}
		}
		return hit;
	}

	size_t measureMemory() const
	{
		return objects.size() * sizeof(Object2D*) + free.size() * sizeof(size_t);
	}

	string toString() const
	{
		return "NSq";
	}
};