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
  metal(const color& albedo) : _albedo(albedo) {}

  bool scatter(
    const ray& r_in, const hit_record& rec, color& attentuation, ray& scattered
  ) const override {
    auto reflected = reflect(r_in.direction(), rec.normal);
    scattered = ray(rec.p, reflected);
    attentuation = _albedo;
    return true;
  }


private:
  color _albedo;
};


#endif // !MATERIAL_H

