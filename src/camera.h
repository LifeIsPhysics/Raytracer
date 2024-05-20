#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "hittable.h"
#include "material.h"


class camera
{
public:
	int		 image_width			 = 100;								// Rendered image width in pixel count
	double aspect_ratio			 = 1.0;								// Ratio of image width/height
	int		 samples_per_pixel = 10;								// Count of random samples for each pixel
	int		 max_depth				 = 10;
	double vfov							 = 90;								// Vertical view angle (field of view)
	point3 lookfrom					 = point3(0,0,0);			// Point camera is look from
	point3 lookat						 = point3(0, 0, -1);  // Point camera is looking at
	vec3 vup								 = vec3(0, 1, 0);			// Camera-relative "up" direction

	void render(const hittable& world) {
		initialize();

		std::cout << "P3\n" << image_width << ' ' << _image_height << "\n255\n";

		for (int j = 0; j < _image_height; j++) {
			std::clog << "\rScanlines remaining: " << (_image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; i++) {
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; sample++) {
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}
				write_color(std::cout, _pixel_samples_scale * pixel_color);
			}
		}

		std::clog << "\rDone.                 \n";
	}

private:
	int		 _image_height;		// Rendered image height in pixel count
	double _pixel_samples_scale;  // Color scale factor for a sum of pixel samples
	point3 _center;					// Center position of the camera
	point3 _pixel00_loc;		// Location of pixel (0, 0) in Viewport
	vec3	 _pixel_delta_u;  // Offset to pixel to the right
	vec3	 _pixel_delta_v;  // Offset to pixel below
	vec3   _u, _v, _w;			// Camera frame basis vectors

	void initialize() {
		_image_height = int(image_width / aspect_ratio);
		_image_height = (_image_height < 1) ? 1 : _image_height;
		_pixel_samples_scale = 1.0 / samples_per_pixel;
		
		_center = lookfrom;

		// Determine viewport dimensions
		auto focal_length = (lookfrom - lookat).length();
		auto theta = degree_to_radians(vfov);
		auto h = tan(theta / 2.0);
		auto viewport_height = 2 * h * focal_length;
		auto viewport_width = viewport_height * (double(image_width) / _image_height);

		// Calculate the u,v,w unit vectors for the camera coordinate system
		_w = unit_vector(lookfrom - lookat);
		_u = unit_vector(cross(vup, _w));
		_v = cross(_w, _u);

		// Calculate the vectors across the horizontal and vertical viewport edges
		auto viewport_u = viewport_width * _u;		// Vector across viewport horizontal edge
		auto viewport_v = viewport_height * -_v;  // Vector down viewport vertical edge

		// Calculate the horizontal and vertical pixel delta from pixel to pixel
		_pixel_delta_u = viewport_u / image_width;
		_pixel_delta_v = viewport_v / _image_height;

		// Calculate the location of the upper left pixel of the viewport
		auto viewport_upper_left = _center - (focal_length * _w) - viewport_u / 2 - viewport_v / 2;
		_pixel00_loc = viewport_upper_left + 0.5 * (_pixel_delta_u + _pixel_delta_v);
	}

	ray get_ray(int i, int j) {
		// Constructed a camera ray originating from the origin and directed at randomly sampled
		// point around the pixel location (i,j)
		auto offset = sample_square();
		auto pixel_sample = _pixel00_loc + ((i + offset.x()) * _pixel_delta_u)
																		 + ((j + offset.y()) * _pixel_delta_v);

		auto ray_origin = _center;
		auto ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}

	vec3 sample_square() const {
		// Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	color ray_color(const ray& r, int depth, const hittable& world) const {
		// If we've exceeded the ray bounce limit, no more light is gathered.
		if (depth <= 0) return color(0, 0, 0);

		hit_record rec;

		if (world.hit(r, interval(0.001, inf), rec)) {
			ray scattered;
			color attenuation;
			if (rec.mat->scatter(r, rec, attenuation, scattered))
				return attenuation * ray_color(scattered, depth - 1, world);
			return color(0, 0, 0);
		}

		auto unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return  (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}
};

#endif //! CAMERA_H

