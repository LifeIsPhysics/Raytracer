#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

class hit_record;

class material
{
public:
  virtual ~material() = default;

  virtual bool scatter(
    const ray& r_in, const hit_record& rec, color& attentuation, ray& scattered
  ) const {
    return false;
  }

};

class lambertian : public material {
public:
  lambertian(const color& albedo) : _albedo(albedo) {}

  bool scatter(
    const ray& r_in, const hit_record& rec, color& attentuation, ray& scattered
  ) const override {
    auto scatter_direction = rec.normal + random_unit_vector();

    // Catch degenerate scatter direction
    if (scatter_direction.near_zero())
      scatter_direction = rec.normal;

    scattered = ray(rec.p, scatter_direction);
    attentuation = _albedo;
    return true;
  }

private:
  color _albedo;
};

class metal : public material {
public:
  metal(const color& albedo, double fuzz) : _albedo(albedo), _fuzz(fuzz < 1 ? fuzz: 1){}

  bool scatter(
    const ray& r_in, const hit_record& rec, color& attentuation, ray& scattered
  ) const override {
    auto reflected = reflect(r_in.direction(), rec.normal);
    reflected = unit_vector(reflected) + (_fuzz * random_unit_vector());
    scattered = ray(rec.p, reflected);
    attentuation = _albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
  }


private:
  color  _albedo;
  double _fuzz;
};


#endif // !MATERIAL_H

