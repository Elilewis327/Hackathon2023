#include "glm/glm.hpp"

class Asteroid {
    public:
        Asteroid(glm::vec3 location, glm::vec3 init_velocity) {
            location = location;
            velocity = init_velocity;
        }
        
        glm::vec3 location, velocity;
};
