#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <vector>


class hittable_list : public hittable
{
public:
	std::vector<shared_ptr<hittable>> _objects;
	
	hittable_list() {};
	hittable_list(shared_ptr<hittable> object) { add(object); }
	
	void clear() { _objects.clear(); }

	void add(shared_ptr<hittable> object) { _objects.push_back(object); }

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		hit_record tmp_rec;
		bool hit_anything = false;
		auto closest_so_far = ray_t.max;

		for (auto& object : _objects) {
			if (object->hit(r, interval(ray_t.min, closest_so_far), tmp_rec)) {
				hit_anything = true;
				closest_so_far = tmp_rec.t;
				rec = tmp_rec;
			}
		}

		return hit_anything;
	}
};


#endif /* HITTABLE_LIST_H*/