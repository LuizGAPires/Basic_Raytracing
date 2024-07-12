#ifndef MATERIAL_H
#define MATERIAL_H
// Classe representando um material 

#include "rt_main.h"

class hit_record;

class material {
    public:
        virtual ~material() = default;
        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const color& albedo) : albedo(albedo) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
            auto scatter_direction = rec.normal + random_unit_vector();
            if (scatter_direction.near_zero()){
                scatter_direction = rec.normal;
            }
            
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        color albedo;
};

#endif