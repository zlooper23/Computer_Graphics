/*
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "particleSystem.h"
#include "button.h"
#include "camera.h"
#include "entity.h"
#include "shader.h"




class Application
{
public:
	// Particle System
	ParticleSystem particleSystem;
	Button toolBar[17];
	// Window

	SDL_Window *window = nullptr;
	int window_width;
	int window_height;

	int commonWidth;
	int mode;
	int submode;
	Color backgroundColor;
	Color primaryColor;
	Color borderColor;
	bool isFilled;
	Vector2 pos1;
	Image prevIm;
	bool creating;
	Image savedImage;
	Camera cam;
	FloatImage zBuffer;
	Entity ents[3];
	int mouseButton;
	int perspectiveChange;
	Shader* shader[14];
	Mesh* mesh;
	Texture* texture;
	sUniformData uData;
	sLight light;


	float time;

	// Input
	const Uint8 *keystate;
	int mouse_state;		// Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta;	// Mouse movement in the last frame

	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnWheel(SDL_MouseWheelEvent event);
	void OnFileChanged(const char *filename);

	// CPU Global framebuffer
	Image framebuffer;

	// Constructor and main methods
	Application(const char *caption, int width, int height);
	~Application();

	void Init(void);
	void Render(void);
	void Update(float dt);

	void initToolbar();
	void DrawToolbar();
	void LoadShaders();

	// Other methods to control the app
	void SetWindowSize(int width, int height)
	{
		glViewport(0, 0, width, height);
		this->window_width = width;
		this->window_height = height;
		this->framebuffer.Resize(width, height);
		this->cam.SetPerspective(cam.fov, width/height, cam.near_plane, cam.far_plane);
	}

	Vector2 GetWindowSize()
	{
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		return Vector2(float(w), float(h));
	}
};
