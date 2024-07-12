#ifndef CAMERA_H
#define CAMERA_H
// Classe que representa a camera do programa


#include "rt_main.h"
#include "hittable.h"
#include "material.h"

class camera {
    public:
        double aspect_ratio = 1.0;  // Proporção da imagem (largura/altura)
        int image_width  = 100;  // Comprimento da imagem em pixels
        int max_depth = 10; // Profundidade maxima da recursão (para renderizar o fundo)
        int samples_per_pixel = 10;   // Quantidade de amostras por pixel
        void render(const hittable& world) {
            initialize();

            ofstream image; // Cria um arquivo para salvar a imagem
            image.open("image.ppm");

            if(image.is_open()){
                image << "P3" << endl;
                image << image_width << ' ' << image_height << endl;
                image << "255" << endl;
                
                for (int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (int i = 0; i < image_width; i++) {
                    color pixel_color(0, 0, 0);
                    for (int s = 0; s < samples_per_pixel; s++) {
                        auto offset = sample_square();
                        auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

                        auto ray_origin = center;
                        auto ray_direction = pixel_sample - ray_origin;

                        ray r(ray_origin, ray_direction);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    write_color(image, pixel_samples_scale * pixel_color);
                }
            }

            std::clog << "\rDone.                 \n";
            }
        }

    private:
        int    image_height;   // Altura da imagem
        double pixel_samples_scale; // Escala para o pixel
        point3 center;         // Centro da camera
        point3 pixel00_loc;    // Localização do primeiro pixel
        vec3   pixel_delta_u;  // Offset para o pixel a direita
        vec3   pixel_delta_v;  // Offset para o pixel abaixo

        void initialize() {
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;
            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = point3(0, 0, 0);

            // Determine viewport dimensions.
            auto focal_length = 1.0;
            auto viewport_height = 2.0;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            // Calculate the vectors across the horizontal and down the vertical viewport edges.
            auto viewport_u = vec3(viewport_width, 0, 0);
            auto viewport_v = vec3(0, -viewport_height, 0);

            // Calculate the horizontal and vertical delta vectors from pixel to pixel.
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel.
            auto viewport_upper_left =
                center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
        }

        vec3 sample_square() const {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }
        color ray_color(const ray& r, int depth, const hittable& world) {
            if (depth <= 0) {
                return color(0, 0, 0);
            }
            
            hit_record rec;

            if (world.hit(r, interval(0.001, infinity), rec)) {
                ray scattered;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                    return attenuation * ray_color(scattered, depth - 1, world);
                }
                return color(0, 0, 0);
            }
            vec3 unit_direction = unit_vector(r.direction());
            auto t = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
        }
};

#endif