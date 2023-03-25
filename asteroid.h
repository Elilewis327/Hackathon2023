#include "glm/glm.hpp"

class Asteroid {
    public:
        glm::vec3 location;
        glm::vec3 velocity;

        Asteroid(float x, float y, float z, float velx, float vely, float velz) {
            location = glm::vec3(x, y, z);
            velocity = glm::vec3(velx, vely, velz);
        }
        
};
