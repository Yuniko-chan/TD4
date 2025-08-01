//struct Particle {

//	float32_t3 translate;
//	float32_t3 scale;
//	float32_t lifeTime;
//	float32_t3 velocity;
//	float32_t currentTime;
//	float32_t4 color;

//};
struct Particle
{

    float32_t3 translate;
    float32_t3 scale;
    float32_t lifeTime;
    float32_t3 velocity;
    float32_t currentTime;
    float32_t4 color;
    float32_t3 rotate;
    float32_t3 rotateVelocity;
    float32_t4x4 rotateMatrix;

};
static const uint32_t kMaxParticles = 1024;