#include "particleSystem.h"
#include "utils.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

//Initializes all particles with random values
void ParticleSystem::Init() {
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < MAX_PARTICLES; ++i) { 
        Color fixedColor = Color::WHITE;
        particles[i].position = Vector2(RandomInt(0, 1200), RandomInt(80, 800)); //Y position starts at 80 to avoid any contact with the toolbar
        particles[i].velocity = Vector2(RandomInt(2, 5), RandomInt(-50, -100)); //Negative because they are going down
        particles[i].color = fixedColor;
        particles[i].acceleration = RandomInt(1, 4); 
        particles[i].ttl =  RandomInt(20, 40); 
        particles[i].inactive = false;
    }
}

//Render all particles as a white circle to simulate snow
void ParticleSystem::Render(Image* framebuffer) {
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        if (!particles[i].inactive) {
            // Draw the particle to the framebuffer
            framebuffer->DrawCircle(particles[i].position.x, particles[i].position.y, 3, Color::WHITE, 2, true, Color::WHITE);
        }
    }
}

//Update the position of each particle to simulate an animation
void ParticleSystem::Update(float dt, Image* framebuffer) {
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        Particle& p = particles[i];
        if(p.position.y < 65){ //To avoid any contact with the toolbar
            framebuffer->DrawCircle(particles[i].position.x, particles[i].position.y, 3, Color::BLACK, 2, false, Color::BLACK);
            p.position = Vector2(p.position.x, 800);

        }

        if (!p.inactive) {
            //After each update the previous position is filled with the same shape but with the background color
            framebuffer->DrawCircle(particles[i].position.x, particles[i].position.y, 3, Color::BLACK, 2, false, Color::BLACK);
            p.ttl -= dt;
            if (p.ttl <= 0) {
                p.inactive = true;
            } else {
                Vector2 accelerationVector = p.velocity * p.acceleration;
                p.position = p.position + accelerationVector * dt;
            }
        }
    }
}

//Function to get a random number within a range
int ParticleSystem::RandomInt(int min, int max) {
    return rand() % (max - min + 1) + min;
}



