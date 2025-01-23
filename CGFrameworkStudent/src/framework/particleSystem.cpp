#include "particleSystem.h"
#include "utils.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

//Initializes all particles with random values
void ParticleSystem::Init(Image* framebuffer, int t) {
    type = t;
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < MAX_PARTICLES; ++i) { 
        Color fixedColor = Color::WHITE;
        particles[i].position = Vector2(RandomInt(2, framebuffer->width-2), RandomInt(2, framebuffer->height-2)); 
        if(type){
            particles[i].velocity = Vector2(RandomInt(200, 200), RandomInt(-800, -1000)); //Negative because they are going down
        }else{
            particles[i].velocity = Vector2(RandomInt(-5, 5), RandomInt(-10, -50)); //Negative because they are going down
        }
        
       

        particles[i].color = fixedColor;
        particles[i].acceleration = RandomInt(1, 4); 
        particles[i].ttl =  RandomInt(20, 40); 
        particles[i].inactive = false;
    }
}

//Render all particles as a white circle to simulate snow
void ParticleSystem::Render(Image* framebuffer) {
    framebuffer->Fill(Color(20, 20, 20));
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        if (!particles[i].inactive) {
            // Draw the particle to the framebuffer
            if(type){
                framebuffer->DrawLineDDA(particles[i].position.x, particles[i].position.y, particles[i].position.x+3, particles[i].position.y-10, Color::BLUE);
            }else{
                framebuffer->DrawCircle(particles[i].position.x, particles[i].position.y, 2, Color::WHITE, 1, true, Color::WHITE);
            }
        }
    }
}

//Update the position of each particle to simulate an animation
void ParticleSystem::Update(float dt, Image* framebuffer) {
    framebuffer->Fill(Color(20, 20, 20));
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        Particle& p = particles[i];
        if(p.position.y < 2){ 
            p.inactive = true;
        }

        if (!p.inactive) {
            //After each update the previous position is filled with the same shape but with the background color
            //framebuffer->DrawCircle(particles[i].position.x, particles[i].position.y, 4, Color::BLACK, 1, false, Color::BLACK);
            p.ttl -= dt;
            if (p.ttl <= 0) {
                p.inactive = true;
            } else {
                Vector2 accelerationVector = p.velocity * p.acceleration;
                p.position = p.position + accelerationVector * dt;
            }
        }else{
            //framebuffer->DrawCircle(particles[i].position.x, particles[i].position.y, 4, Color::BLACK, 1, false, Color::BLACK);

            if(type){
                p.position = Vector2(RandomInt(2, framebuffer->width-2), RandomInt(framebuffer->height/10, framebuffer->height-2));            
            }else{
                p.position = Vector2(RandomInt(2, framebuffer->width-2), framebuffer->height-2);
            }
            
            p.ttl =  RandomInt(20, 40); 
            p.inactive = false;
        }
    }
    if(type && RandomInt(0, 200)==0){
        framebuffer->Fill(Color::WHITE);
    }
}

//Function to get a random number within a range
int ParticleSystem::RandomInt(int min, int max) {
    return (rand() / (float)RAND_MAX) * (max - min) + min;
}



