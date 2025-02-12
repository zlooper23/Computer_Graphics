#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "particleSystem.h"
#include "button.h"
#include "entity.h"
#include "camera.h"


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
	this->savedImage.LoadPNG("images/fruits.png");
	this->zBuffer = FloatImage(width, height);
	this->zBuffer.Fill(__FLT_MAX__);
	
}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
	initToolbar();
	particleSystem.Init(&framebuffer, 1);
	cam.LookAt(Vector3(2, 2, 2), Vector3(0, 0, 0), Vector3(0, 1, 0));
	cam.SetPerspective(45, framebuffer.width/framebuffer.height, 0.01, 100);
	cam.type = 0;

	Mesh m1;
	m1.CreateCube(0.5);
	ents[0] = Entity(m1);
	Mesh m2;
	m2.LoadOBJ("./meshes/anna.obj");
	Image texture;
	texture.LoadTGA("./textures/anna_normal.tga", true);
	ents[1] = Entity(m2, texture);
	//framebuffer = texture;
	//ents[1] = Entity(m2);
	
	

}

// Render one frame
void Application::Render(void)
{
	
	framebuffer.Render();
	
	/*SDL_Event sdlEvent;
	if(mode == 6){
		particleSystem.Render(&framebuffer);
		framebuffer.Render();
	}else{
		DrawToolbar();
		if(!creating){
			prevIm = framebuffer;
		}
	}
	while(SDL_PollEvent(&sdlEvent))
	{
	switch(sdlEvent.type)
	{
	case SDL_QUIT: return; break;

	case SDL_MOUSEBUTTONDOWN: 
		if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
		for(int i = 0; i<17; i++){
			if(toolBar[i].IsMouseInside(mouse_position)){
				creating = false;
				switch(toolBar[i].id) {
					case 0: framebuffer.Fill(backgroundColor); prevIm = framebuffer; break;
					case 1: framebuffer.DrawImage(savedImage, 0, 0); prevIm = framebuffer; break;
					case 2: savedImage = framebuffer; break;
					case 3: mode = 0; break;
					case 4: mode = 1; break;
					case 5: mode = 2; break;
					case 6: mode = 3; break;
					case 7: mode = 4; break;
					case 8: mode = 5; break;
					case 9: if((isFilled&&(mode!=5))&&(mode!=1)){primaryColor=Color::BLACK;}else{borderColor=Color::BLACK;}; break;
					case 10: if((isFilled&&(mode!=5))&&(mode!=1)){primaryColor=Color::WHITE;}else{borderColor=Color::WHITE;};break;
					case 11: if((isFilled&&(mode!=5))&&(mode!=1)){primaryColor=Color::BLUE;}else{borderColor=Color::BLUE;};break;
					case 12: if((isFilled&&(mode!=5))&&(mode!=1)){primaryColor=Color::CYAN;}else{borderColor=Color::CYAN;};break;
					case 13: if((isFilled&&(mode!=5))&&(mode!=1)){primaryColor=Color::GREEN;}else{borderColor=Color::GREEN;};break;
					case 14: if((isFilled&&(mode!=5))&&(mode!=1)){primaryColor=Color::PURPLE;}else{borderColor=Color::PURPLE;};break;
					case 15: if((isFilled&&(mode!=5))&&(mode!=1)){primaryColor=Color::RED;}else{borderColor=Color::RED;};break;
					case 16: if((isFilled&&(mode!=5))&&(mode!=1)){primaryColor=Color::YELLOW;}else{borderColor=Color::YELLOW;};break;
				}
			}
		}
			if(!creating){
				switch(mode) {
				case 0: break;
				case 1: pos1 = Vector2(mouse_position.x, mouse_position.y); break;
				case 2: pos1 = Vector2(mouse_position.x, mouse_position.y); break;
				case 3: pos1 = Vector2(mouse_position.x, mouse_position.y); break;
				case 4: pos1 = Vector2(mouse_position.x, mouse_position.y); break;
				case 5: break;
				}
			}
			creating = true;
			
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
			switch(mode) {
				case 0: break;
				case 1: framebuffer.DrawImage(prevIm, 0, 0);framebuffer.DrawLineDDA(pos1.x, pos1.y,  mouse_position.x, mouse_position.y, borderColor); break;
				case 2: framebuffer.DrawImage(prevIm, 0, 0);framebuffer.DrawRect(pos1.x, pos1.y,  mouse_position.x-pos1.x, mouse_position.y-pos1.y, borderColor, commonWidth, isFilled, primaryColor); break;
				case 3: framebuffer.DrawImage(prevIm, 0, 0);framebuffer.DrawCircle(pos1.x, pos1.y,  pos1.Distance(mouse_position), borderColor, commonWidth, isFilled, primaryColor); break;
				case 4: framebuffer.DrawImage(prevIm, 0, 0);framebuffer.DrawTriangle(pos1,  Vector2(2*pos1.x-mouse_position.x, mouse_position.y), mouse_position, borderColor, isFilled, primaryColor); break;
				case 5: break;
			}
			creating = false;
		}
		break;

	case SDL_MOUSEMOTION:
		if (sdlEvent.button.button == SDL_BUTTON_LEFT && mode != 6) {
		if(creating){
			switch(mode){
				case 0: framebuffer.DrawCircle(mouse_position.x, mouse_position.y, 1, backgroundColor, commonWidth, isFilled, backgroundColor); break;
				case 1: framebuffer.DrawImage(prevIm, 0, 0);framebuffer.DrawLineDDA(pos1.x, pos1.y,  mouse_position.x, mouse_position.y, borderColor);break;
				case 2: framebuffer.DrawImage(prevIm, 0, 0);framebuffer.DrawRect(pos1.x, pos1.y,  mouse_position.x-pos1.x, mouse_position.y-pos1.y, borderColor, commonWidth, isFilled, primaryColor); break;
				case 3: framebuffer.DrawImage(prevIm, 0, 0);framebuffer.DrawCircle(pos1.x, pos1.y,  pos1.Distance(mouse_position), borderColor, commonWidth, isFilled, primaryColor); break;
				case 4: framebuffer.DrawImage(prevIm, 0, 0);framebuffer.DrawTriangle(pos1, Vector2(2*pos1.x-mouse_position.x, mouse_position.y), mouse_position, borderColor, isFilled, primaryColor); break;
				case 5: framebuffer.DrawCircle(mouse_position.x, mouse_position.y, 1, borderColor, commonWidth, true, borderColor); break;
			}
		}
		}else{
			creating = false;
		}
		break;

	case SDL_KEYUP:  
		if(mode == 6 && sdlEvent.key.keysym.sym != 6){
			framebuffer.Fill(backgroundColor);
			particleSystem.Init(&framebuffer, !particleSystem.type);

		}
		switch(sdlEvent.key.keysym.sym) {
			case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
			case SDLK_PLUS: commonWidth++;printf("Width: %d\n", commonWidth); break; 
			case SDLK_MINUS: if(commonWidth>1){commonWidth--;printf("Width: %d\n", commonWidth);} break; 
			case SDLK_0: mode = 0; break;
			case SDLK_1: mode = 1; break;
			case SDLK_2: mode = 2; break;
			case SDLK_3: mode = 3; break;
			case SDLK_4: mode = 4; break;
			case SDLK_5: mode = 5; break;
			case SDLK_6: mode = 6; framebuffer.Fill(backgroundColor); break;
			case SDLK_f: isFilled = !isFilled;break;
		}
		break;
	case SDL_WINDOWEVENT:
		switch (sdlEvent.window.event) {
			case SDL_WINDOWEVENT_RESIZED: // Resize OpenGL context
				std::cout << "window resize" << std::endl;
				SetWindowSize( sdlEvent.window.data1, sdlEvent.window.data2 );
				break;
		}
		break;
	}
	DrawToolbar();
	
	
}*/
	zBuffer = FloatImage(framebuffer.width, framebuffer.height);
	zBuffer.Fill(__FLT_MAX__);

	framebuffer.Fill(Color::BLACK);
	//e.modelMatrix.Rotate(PI*time, Vector3(1, 0, 0));
	//e.modelMatrix.Rotate(PI*time, Vector3(0, 1, 0));
	//printf("%lf, %lf\n", sin(time), cos(time));
	
	ents[1].Render(&framebuffer, &cam, Color::RED, &zBuffer);
	//ents[0].Render(&framebuffer, &cam, Color::RED, &zBuffer);
}

// Called after render
void Application::Update(float seconds_elapsed)
{	 
	if(mode == 6){
		particleSystem.Update(seconds_elapsed, &framebuffer);
	}
}

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
		case SDLK_PLUS:Vector3 a = cam.eye-cam.center;a.Normalize();cam.eye = cam.center+(10*a);cam.UpdateViewMatrix();break;
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		if(!creating){
			pos1 = Vector2(mouse_position.x, mouse_position.y);
		}
		creating = true;
	}

}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		creating = false;
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{	Vector2 v = mouse_position-pos1;
	if (event.button == SDL_BUTTON_LEFT) {
		//Vector3 vcam = cam.eye-cam.center;
		//float d = Vector2(vcam.x, vcam.z).Distance(Vector2(0, 0));
		//cam.eye = Vector3((cam.center.x)+(sin((v.x)*0.01))*d, (cam.center.y)+cos(v.y*0.01)*d, (cam.center.z)+(cos((v.x)*0.01))*d);

		Matrix44 m;
		Matrix44 r;
		m.SetIdentity();
		r.SetIdentity();
		m.Translate(-cam.center.x, -cam.center.y, -cam.center.z);
		r = m*r;
		m.SetIdentity();
		m.Rotate(v.x*0.0001, Vector3(0,1,0));
		r = m*r;
		m.SetIdentity();
		m.Translate(cam.center.x, cam.center.y, cam.center.z);
		r=m*r;
		cam.eye = r*cam.eye;

	}if(event.button == SDL_BUTTON_RIGHT){
		printf("test\n");
		Matrix44 m;
		m.SetIdentity();
		m.Translate(-cam.eye.x, -cam.eye.y, -cam.eye.z);
		cam.center = m*cam.center;
		m.Rotate(v.x*0.0001, Vector3(0,1,0));
		cam.center = m*cam.center;
		m.Translate(cam.eye.x, cam.eye.y, cam.eye.z);
		cam.center = m*cam.center;

	}
	
	cam.UpdateViewMatrix();	
}

void Application::OnWheel(SDL_MouseWheelEvent event)
{
	float dy = event.preciseY;// 1 or -1
	dy*=0.03;
	Vector3 v = cam.eye - cam.center;
	v = v * dy;
	cam.eye = cam.eye-v;
	cam.UpdateViewMatrix();	
	// ...
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}

void Application::initToolbar(){
	const char *s[17] = {"./images/clear.png","./images/load.png", "./images/save.png", "./images/eraser.png", 
	"./images/line.png", "./images/rectangle.png", "./images/circle.png", "./images/triangle.png", "./images/pencil.png", "./images/black.png", "./images/white.png", "./images/blue.png"
	, "./images/cyan.png", "./images/green.png", "./images/pink.png", "./images/red.png", "./images/yellow.png"};

    // Printing Strings stored in 2D array
	for(int i = 0; i<17; i++){
		Image im;
		im.LoadPNG(s[i]);
		Button b = Button(10+(i*(42)), 10, im, i);
		toolBar[i] = b;


	}
}


void Application::DrawToolbar(){
	framebuffer.DrawRect(0, 0, framebuffer.width, 52, Color::GRAY, 1, true, Color::GRAY);
    // Printing Strings stored in 2D array
	for(int i = 0; i<17; i++){
		toolBar[i].Render(framebuffer);
	}
}