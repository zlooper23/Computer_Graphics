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

	this->commonWidth=1;
	this->mode=1;
	this->backgroundColor = Color::BLACK;
	this->primaryColor = Color::WHITE;
	this->borderColor = Color::RED;
	this->isFilled = true;
	this->creating = false;





	
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

	//framebuffer.Fill(Color::BLACK);
	//framebuffer.DrawRect(100, 100, mouse_position.x-100, mouse_position.y-100, Color::RED, 10, true, Color::WHITE);
	//framebuffer.DrawRect(100, 100, mouse_position.x-100, mouse_position.y-100, Color::RED);

	//framebuffer.DrawLineDDA(mouse_position.x, mouse_position.y, mouse_position.x + 100 * cos(time), mouse_position.y + 100 * sin(time), Color::WHITE);
	//framebuffer.DrawTriangle(Vector2(0, 0), Vector2(500, 250), Vector2(mouse_position.x, mouse_position.y), Color::WHITE, true, Color::BLUE);
	//framebuffer.DrawCircle(mouse_position.x, mouse_position.y, abs(100*sin(time)), Color::RED, 10, false,Color::WHITE);
	//framebuffer.DrawCircle(mouse_position.x, mouse_position.y, 10, Color::RED, 1, true,Color::RED);



	/////////////////////////
	if(!creating){
		prevIm = framebuffer;
	}
	
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
		case SDLK_e: framebuffer.Fill(backgroundColor); break; 
		case SDLK_PLUS: commonWidth++;printf("Width: %d\n", commonWidth); break; 
		case SDLK_MINUS: if(commonWidth>1){commonWidth--;printf("Width: %d\n", commonWidth);} break; 
		case SDLK_0: mode = 0; break;
		case SDLK_1: mode = 1; break;
		case SDLK_2: mode = 2; break;
		case SDLK_3: mode = 3; break;
		case SDLK_4: mode = 4; break;
		case SDLK_5: mode = 5; break;
		case SDLK_f: isFilled = !isFilled;break;
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		/*Image im;
		if(!im.LoadPNG("images/line.png")){
			printf("Fail opening file\n");
			exit(1);
		}
		framebuffer.DrawImage(im, mouse_position.x, mouse_position.y);*/
		creating = true;
		switch(mode) {
			case 0: prevIm = framebuffer;
			case 1: pos1 = Vector2(mouse_position.x, mouse_position.y); break;
			case 2: pos1 = Vector2(mouse_position.x, mouse_position.y); break;
			case 3: pos1 = Vector2(mouse_position.x, mouse_position.y); break;
			case 4: break;
			case 5: break;
		}
	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		creating = false;
		switch(mode) {
			case 0: break;
			case 1: framebuffer.DrawImage(prevIm, 0, 0);framebuffer.DrawLineDDA(pos1.x, pos1.y,  mouse_position.x, mouse_position.y, primaryColor); break;
			case 2: framebuffer.DrawImage(prevIm, 0, 0);framebuffer.DrawRect(pos1.x, pos1.y,  mouse_position.x-pos1.x, mouse_position.y-pos1.y, primaryColor, commonWidth, isFilled, borderColor); break;
			case 3: framebuffer.DrawImage(prevIm, 0, 0);framebuffer.DrawCircle(pos1.x, pos1.y,  pos1.Distance(mouse_position), primaryColor, commonWidth, isFilled, borderColor); break;
			case 4: break;
			case 5: break;
		}
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) {
		switch(mode){
			case 0: break;
			case 1: framebuffer.DrawImage(prevIm, 0, 0);framebuffer.DrawLineDDA(pos1.x, pos1.y,  mouse_position.x, mouse_position.y, primaryColor);break;
			case 2: framebuffer.DrawImage(prevIm, 0, 0);framebuffer.DrawRect(pos1.x, pos1.y,  mouse_position.x-pos1.x, mouse_position.y-pos1.y, primaryColor, commonWidth, isFilled, borderColor); break;
			case 3: framebuffer.DrawImage(prevIm, 0, 0);framebuffer.DrawCircle(pos1.x, pos1.y,  pos1.Distance(mouse_position), primaryColor, commonWidth, isFilled, borderColor); break;
			case 4: break;
			case 5: framebuffer.DrawCircle(mouse_position.x, mouse_position.y, 1, primaryColor, commonWidth, isFilled, primaryColor); break;
		}
		
	}
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