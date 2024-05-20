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

class dielectric : public material {
public:
  dielectric(double n) : _refraction_index(n) {}
  
  bool scatter(
    const ray& r_in, const hit_record& rec, color& attentuation, ray& scattered
  ) const override {
    attentuation = color(1.0, 1.0, 1.0);
    double ri = rec.front_face ? (1 / _refraction_index) : _refraction_index;

    auto unit_direction = unit_vector(r_in.direction());
    double cos_theta    = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta    = sqrt(1 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    vec3 direction;
    
    // For total internal reflection
    if (cannot_refract || reflactance(cos_theta, ri) > random_double())
      direction = reflect(unit_direction, rec.normal);
    else
      direction = refract(unit_direction, rec.normal, ri);

    scattered = ray(rec.p, direction);
    return true;
  }

private:
  // Refractive index in vacuum or air, or the ratio of the material's refractive index over
  // the refractive index of the enclosing media
  double _refraction_index;

  static reflactance(double cosine, double refraction_index) {
    // Use Schlick's approximation for reflactance.
    auto r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }
};


#endif // !MATERIAL_H

