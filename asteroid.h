#include "glm/glm.hpp"

class Asteroid {
    public:
        glm::vec3 location;
        glm::vec3 velocity;
        int mass, size, density;

        Asteroid(double x, double y, double z, float velx, float vely, float velz, float size, float density) {
            location = glm::vec3(x, y, z);
            velocity = glm::vec3(velx, vely, velz);
            size = size;
            density = density;
            mass = size * density;
        }
        
};
