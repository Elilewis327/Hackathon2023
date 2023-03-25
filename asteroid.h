#include "glm/glm.hpp"

class Asteroid {
    public:
        glm::vec3 location;
        glm::vec3 velocity;
        float mass;
        float size;
        float density;

        Asteroid(double x, double y, double z, float velx, float vely, float velz, float s, float d) {
            location = glm::vec3(x, y, z);
            velocity = glm::vec3(velx, vely, velz);
            size = s;
            density = d;
            mass = s * density;
        }
        
};
