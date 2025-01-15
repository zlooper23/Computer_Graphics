#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->mouse_state = 0;
	this->time = 0.f;
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

	this->framebuffer.Resize(w, h);
}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
}

// Render one frame
void Application::Render(void)
{
	// ...

	/////////////////////////

	framebuffer.Fill(Color::BLACK);
	//framebuffer.DrawRect(100, 100, mouse_position.x-100, mouse_position.y-100, Color::RED, 10, true, Color::WHITE);
	//framebuffer.DrawLineDDA(mouse_position.x, mouse_position.y, mouse_position.x + 100 * cos(time), mouse_position.y + 100 * sin(time), Color::WHITE);
	//framebuffer.DrawTriangle(Vector2(0, 0), Vector2(500, 250), Vector2(mouse_position.x, mouse_position.y), Color::WHITE, true, Color::BLUE);
	framebuffer.DrawCircle(mouse_position.x, mouse_position.y, abs(100*sin(time)), Color::RED, 10, true,Color::WHITE);


	/////////////////////////
 
	framebuffer.Render();
}

// Called after render
void Application::Update(float seconds_elapsed)
{

}

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{

}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;

	// ...
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}