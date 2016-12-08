#include "ParticleSystem.h"
#include "GeometryShader.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

const static unsigned int POSITION_LOC = 0;
const static unsigned int VELOCITY_LOC = 1;
const static unsigned int FORCE_LOC = 2;
const static unsigned int COLOR_LOC = 3;
const static unsigned int MASS_LOC = 4;
const static unsigned int LIFETIME_LOC = 5;

const static float DEFUALT_LIFETIME = 10.0f;
Vector3f hiddenPosition(1000.0f, 1000.0f, 1000.0f);

ParticleSystem::ParticleSystem() {
    this->vboId = 0;
    this->shader = nullptr;
    std::srand(0);

    this->bounceEnergy = 0.8f;
    this->gravity.set(0.0f, -9.8f, 0.0f);
    this->initVelocity = 1.0f;
    this->minLifetime = 1.0f;
    this->maxLifetime = 10.0f;
    this->color = Color3f(0.3f, 0.2f, 1.0f);
}

ParticleSystem::~ParticleSystem() {}

bool ParticleSystem::loadShader(const std::string& vertexFilename, const std::string& geometryFilename, const std::string& fragmentFilename) {
    this->shader = std::make_shared<GeometryShader>();

    if ( !shader->load(vertexFilename, geometryFilename, fragmentFilename) ) {
        std::cerr << "[ParticleSystem:loadShader] Error: Could not load shader." << std::endl;
        return false;
    }

    if ( !shader->compile() ) {
        std::cerr << "[ParticleSystem:loadShader] Error: Could not compile shader." << std::endl;
        return false;
    }
    glBindAttribLocation(this->shader->getProgramID(), POSITION_LOC, "position");
    glBindAttribLocation(this->shader->getProgramID(), VELOCITY_LOC, "velocity");
    glBindAttribLocation(this->shader->getProgramID(), FORCE_LOC, "force");
    glBindAttribLocation(this->shader->getProgramID(), COLOR_LOC, "color");
    glBindAttribLocation(this->shader->getProgramID(), MASS_LOC, "mass");

    if ( !shader->link() ) {
        std::cerr << "[ParticleSystem:loadShader] Error: Could not link shader program." << std::endl;
        return false;
    }

    return true;
}

float Random(float lower, float upper) {
    int r = rand(); 
    float fraction = ((float) r / RAND_MAX) * (upper - lower); 
    float x = lower + fraction; 
    return x;
}

/* Generate a random color based on baseColor. */
Color3f RandomColor(const Color3f& baseColor) {
    float r = baseColor.r() + Random(0.5f, 1.0f);
    float g = baseColor.g() + Random(0.5f, 1.0f);
    float b = baseColor.b() + Random(0.5f, 1.0f);
    return Color3f(r, g, b);
}

void ParticleSystem::setMaxParticleCount(std::size_t particleCount) {
    this->particles.resize(particleCount);

    for ( std::size_t i = 0; i < this->particles.size(); i++ ) {
        particles[i].position = hiddenPosition;
        particles[i].velocity = Vector3f::Zero();
        particles[i].force = Vector3f::Zero();
        particles[i].color = RandomColor(this->color);
        particles[i].mass = 1.0f;
        particles[i].lifetime = Random(this->minLifetime, this->maxLifetime);
    }

    this->constructOnGPU();
}

/*
 * Very simple collision based on a cube around the origin. This collision
 * is the simplest form of collilsion that can be implemented for a
 * particle simulation.
 */
void HandleCollision(Particle& p, float extent, float bounceEnergy) {
    if ( p.position.x() < -extent ) {
        p.position.x() = -extent;
        p.velocity.x() *= -1.0f;
        p.velocity.x() *= bounceEnergy;
    }

    if ( p.position.x() > extent ) {
        p.position.x() = extent;
        p.velocity.x() *= -1.0f;
        p.velocity.x() *= bounceEnergy;
    }

    if ( p.position.z() < -extent ) {
        p.position.z() = -extent;
        p.velocity.z() *= -1.0f;
        p.velocity.z() *= bounceEnergy;
    }

    if ( p.position.z() > extent ) {
        p.position.z() = extent;
        p.velocity.z() *= -1.0f;
        p.velocity.z() *= bounceEnergy;
    }

    if ( p.position.y() < 0.0f ) {
        p.position.y() = 0.0f;
        p.velocity.y() *= -1.0f;
        p.velocity.y() *= bounceEnergy;
    }

    if ( p.position.y() > extent ) {
        p.position.y() = extent;
        p.velocity.y() *= -1.0f;
        p.velocity.y() *= bounceEnergy;
    }
}

void ParticleSystem::update(bool spawnParticles, const Vector3f& spawnPosition, const Vector3f& spawnDirection, float dt) {
    if ( this->vboId == 0 ) return;

    //--------------------------------------------------------------------------
    // Using explicit Euler integration, update the the velocity and position
    // of every particle.
    //--------------------------------------------------------------------------
    for ( std::size_t i = 0; i < this->particles.size(); i++ ) {
        this->particles[i].velocity = this->particles[i].velocity + (dt * this->particles[i].force * 1.0f / this->particles[i].mass);
        this->particles[i].position = this->particles[i].position + (dt * this->particles[i].velocity);
        this->particles[i].lifetime -= dt;

        float y = this->particles[i].position.y();
        float lifetime = this->particles[i].lifetime;

        //--------------------------------------------------------------------------
        // For the ground collision event, the particle loses kinectic energy
        // than an impulse is applied to make the particle bounce.
        //--------------------------------------------------------------------------
        HandleCollision(this->particles[i], 16.0f, this->bounceEnergy);

        //--------------------------------------------------------------------------
        // If the system should be currently spawning particles, move them to the
        // provided spawn position if the particles lifetime is less than 0 (dead).
        //--------------------------------------------------------------------------
        if ( spawnParticles && lifetime < 0.0f ) {
            this->particles[i].position = spawnPosition;
            this->particles[i].velocity = spawnDirection * this->initVelocity;
            this->particles[i].force = this->gravity;
            this->particles[i].color = RandomColor(this->color);
            this->particles[i].lifetime = Random(this->minLifetime, this->maxLifetime);
        }
        //--------------------------------------------------------------------------
        // If the particle is dead (but resizing the particle array on the GPU is
        // expensive), then move it to a 'hidden position' where it will wait as
        // as reserve particle ready to be spawned when spawnParticles is true.
        //--------------------------------------------------------------------------
        else {
            if ( lifetime < 0.0f ) {
                this->particles[i].position = hiddenPosition;
                this->particles[i].velocity = Vector3f::Zero();
                this->particles[i].force = Vector3f::Zero();
                this->particles[i].color = RandomColor(this->color);
                this->particles[i].lifetime = 0.0f;
            }
        }

        
    }

    //--------------------------------------------------------------------------
    // Upload the new data to the GPU. NOTE: This is not the most efficient way of implementing a
    // particle system, but it greatly reduces the additional code within the shader implementation.
    //--------------------------------------------------------------------------
    glBufferSubData(GL_ARRAY_BUFFER, 0, this->particles.size() * sizeof(Particle), &this->particles[0]);
}

void ParticleSystem::beginRender() const {
    if ( this->shader != nullptr ) this->shader->enable();

    glBindBuffer(GL_ARRAY_BUFFER, this->vboId);

    //--------------------------------------------------------------------------
    // Particle position data is the first component in the vertex structure so
    // it is loaded first (with a byte offset of 0).
    //--------------------------------------------------------------------------
    glEnableVertexAttribArray(POSITION_LOC);
    glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), BUFFER_OFFSET(0));

    //--------------------------------------------------------------------------
    // Particle velocity.
    //--------------------------------------------------------------------------
    glEnableVertexAttribArray(VELOCITY_LOC);
    glVertexAttribPointer(VELOCITY_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), BUFFER_OFFSET(3 * sizeof(float)));
    
    //--------------------------------------------------------------------------
    // Particle external force.
    //--------------------------------------------------------------------------
    glEnableVertexAttribArray(FORCE_LOC);
    glVertexAttribPointer(FORCE_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), BUFFER_OFFSET(6 * sizeof(float)));

    //--------------------------------------------------------------------------
    // Particle color. Normalizing the color (GL_TRUE) is critical for making
    // transparency (alpha) work properly.
    //--------------------------------------------------------------------------
    glEnableVertexAttribArray(COLOR_LOC);
    glVertexAttribPointer(COLOR_LOC, 3, GL_FLOAT, GL_TRUE, sizeof(Particle), BUFFER_OFFSET(9 * sizeof(float)));

    //--------------------------------------------------------------------------
    // Particle mass.
    //--------------------------------------------------------------------------
    glEnableVertexAttribArray(MASS_LOC);
    glVertexAttribPointer(MASS_LOC, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), BUFFER_OFFSET(10 * sizeof(float)));

    //--------------------------------------------------------------------------
    // Particle Lifetime;
    //--------------------------------------------------------------------------
    glEnableVertexAttribArray(LIFETIME_LOC);
    glVertexAttribPointer(LIFETIME_LOC, 1, GL_FLOAT, GL_TRUE, sizeof(Particle), BUFFER_OFFSET(11 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboId);
}

void ParticleSystem::endRender() const {
    glDrawArrays(GL_POINTS, 0, this->particles.size());
    if ( this->shader != nullptr ) this->shader->disable();
}

std::shared_ptr<GeometryShader>& ParticleSystem::getShader() {
    return this->shader;
}

const std::shared_ptr<GeometryShader>& ParticleSystem::getShader() const {
    return this->shader;
}

bool ParticleSystem::constructOnGPU() {
    //--------------------------------------------------------------------------
    // Bind the array buffer with a dynamic draw flag because we will be
    // constantly replacing the particle definitions.
    //--------------------------------------------------------------------------
    glGenBuffers(1, &this->vboId);
    glBindBuffer(GL_ARRAY_BUFFER, this->vboId);
    glBufferData(GL_ARRAY_BUFFER, this->particles.size() * sizeof(Particle), &this->particles[0], GL_DYNAMIC_DRAW);
    return true;
}

