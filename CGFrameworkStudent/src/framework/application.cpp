#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "particleSystem.h"
#include "button.h"
#include "entity.h"
#include "camera.h"
#include "material.h"


Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w,h;
	SDL_GetWindowSize(window,&w,&h);
	printf("%d, %d\n", w, h);

	this->mouse_state = 0;
	this->time = 0.f;
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

	this->framebuffer.Resize(w, h);

	this->commonWidth=1;
	this->mode=14;
	this->submode=0;
	this->backgroundColor = Color::BLACK;
	this->primaryColor = Color::WHITE;
	this->borderColor = Color::RED;
	this->isFilled = true;
	this->creating = false;
	this->savedImage.LoadPNG("images/fruits.png");
	this->zBuffer = FloatImage(width, height);
	this->zBuffer.Fill(__FLT_MAX__);
	this->mouseButton = -1;
	this->cam.type = 0;
	this->uData.cam = &this->cam;
	this->light = {Vector3(1.0, 1.0, 1.0), Vector3(1.0, 1.0, 1.0)};
	this->uData.light = this->light;
	this->uData.Ia = Vector3(0.2, 0.2, 0.2);
}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;

	/*initToolbar();
	particleSystem.Init(&framebuffer, 1);

	Mesh m1;
	m1.LoadOBJ("./meshes/cleo.obj");
	Image *texture0 = new Image();
	texture0->LoadTGA("./textures/cleo_color_specular.tga", true);
	ents[0] = Entity(m1, texture0);
	ents[0].modelMatrix.Translate(0.5, 0, 0);

	Mesh m2;
	m2.LoadOBJ("./meshes/anna.obj");
	Image *texture1 = new Image();
	texture1->LoadTGA("./textures/anna_color_specular.tga", true);
	ents[1] = Entity(m2, texture1);
	ents[1].modelMatrix.Translate(0, 0, 0);

	Mesh m3;
	m3.LoadOBJ("./meshes/lee.obj");
	Image *texture2 = new Image();
	texture2->LoadTGA("./textures/lee_color_specular.tga", true);
	ents[2] = Entity(m3, texture2);
	ents[2].modelMatrix.Translate(-0.5, 0, 0);*/


	cam.LookAt(Vector3(0, 0, 3), Vector3(0, 0, 0), Vector3(0, 1, 0));
	cam.SetPerspective(45, framebuffer.width/framebuffer.height, 0.01, 100);

	texture = new Texture();
	texture = Texture::Get("./images/fruits.png");

	LoadShaders();
	
	mesh = new Mesh();
	mesh->CreateQuad();

	Mesh m1;
	m1.LoadOBJ("./meshes/cleo.obj");
	Texture *texture0 = new Texture();
	Shader *sh = new Shader();
	sh = Shader::Get("./shaders/raster.vs","./shaders/raster.fs");
	texture0 = Texture::Get("./textures/cleo_color_specular.tga");

	Material* mat = new Material(texture0, sh);

	ents[0] = Entity(m1, mat);
	//ents[0] = Entity(m1, texture0, sh);
}

// Render one frame
void Application::Render(void)
{
	/*if(mode == 2){
		ents[1].Render(&framebuffer, &cam, Color::RED, &zBuffer);
		
	}

	if(mode == 3){
		ents[0].Render(&framebuffer, &cam, Color::RED, &zBuffer);
		

		ents[1].Render(&framebuffer, &cam, Color::GREEN, &zBuffer);	
		

		ents[2].Render(&framebuffer, &cam, Color::BLUE, &zBuffer);
	}
	
	//framebuffer.Fill(Color::BLACK);
	framebuffer.Render();*/



	if(mode == 14){
		ents[0].Render(uData);
	}else{
		int i = mode+submode;
		shader[i]->Enable();
		shader[i]->SetTexture("u_texture", texture);
		shader[i]->SetFloat("u_time", time);
		shader[i]->SetVector2("u_size", Vector2(window_height, window_width));
		mesh->Render();
		shader[i]->Disable();
	}


	


}

// Called after render
void Application::Update(float seconds_elapsed)
{	
	/*framebuffer.Fill(Color::BLACK);
	if(mode == 3){
		ents[0].Update(seconds_elapsed, 2);
		ents[1].Update(seconds_elapsed, 1);
		ents[2].Update(seconds_elapsed, 0);
	}
	zBuffer = FloatImage(framebuffer.width, framebuffer.height);
	zBuffer.Fill(__FLT_MAX__);*/
}

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
		
		//“Z”Toggle between OCCLUSIONS and NO OCCLUSIONS
		/*case SDLK_t:
			for(int i = 0; i<3; i++){
				if(ents[i].GetMode()!=3){
					ents[i].ChangeMode(3);
				}else{
					ents[i].ChangeMode(4);
				}
				
			}
			break;
		case SDLK_c:
			for(int i = 0; i<3; i++){
				if(ents[i].GetMode()!=2){
					ents[i].ChangeMode(2);
				}else{
					ents[i].ChangeMode(4);
				}
				
			}

			break;
		case SDLK_z:
			for(int i = 0; i<3; i++){
				if(ents[i].oclussions){
					ents[i].oclussions= false;
				}else{
					ents[i].oclussions = true;
				}
			}
			break;
		case SDLK_l:
			for(int i = 0; i<3; i++){
				int a = ents[i].GetMode();
				if(a>3){
					ents[i].ChangeMode(0);
				}else{
					ents[i].ChangeMode(a+1);
				}
			}
			break;
		case SDLK_PLUS:
			switch (perspectiveChange)
			{
			case 0:
				cam.SetPerspective(cam.fov+1, cam.aspect, cam.near_plane, cam.far_plane);
				printf("Fov: %lf\n", cam.fov);
				break;
			case 1:
				cam.SetPerspective(cam.fov, cam.aspect, cam.near_plane+0.01, cam.far_plane);
				printf("Near: %lf\n", cam.near_plane);
				break;
			case 2:
				cam.SetPerspective(cam.fov, cam.aspect, cam.near_plane, cam.far_plane+1);
				printf("Far: %lf\n", cam.far_plane);
				break;
			}
			break;
		case SDLK_MINUS:
			switch (perspectiveChange)
			{
			case 0:
				cam.SetPerspective(cam.fov-1, cam.aspect, cam.near_plane, cam.far_plane);
				printf("Fov: %lf\n", cam.fov);
				break;
			case 1:
				cam.SetPerspective(cam.fov, cam.aspect, cam.near_plane-0.01, cam.far_plane);
				printf("Near: %lf\n", cam.near_plane);
				break;
			case 2:
				cam.SetPerspective(cam.fov, cam.aspect, cam.near_plane, cam.far_plane-1);
				printf("Far: %lf\n", cam.far_plane);
				break;
			}
			break;
		case SDLK_v: perspectiveChange = 0; break;
		case SDLK_n: perspectiveChange = 1; break;
		case SDLK_f: perspectiveChange = 2; break;

		case SDLK_1: mode = 2; break;
		case SDLK_2: mode = 3; break;*/

		case SDLK_1: mode = 0; submode = 0;break;
		case SDLK_2: mode = 6; submode = 0;break;
		case SDLK_3: mode = 12; submode = 0;break;
		case SDLK_4: mode = 14; submode = 0;break;

		case SDLK_a: submode = 0;break;
		case SDLK_b: if(mode<13){submode = 1;}break;
		case SDLK_c: if(mode<7){submode = 2;}break;
		case SDLK_d: if(mode<7){submode = 3;}break;
		case SDLK_e: if(mode<7){submode = 4;}break;
		case SDLK_f: if(mode<7){submode = 5;}break;
	}
}


void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		if(!creating){
			mouseButton = 0;
		}
		creating = true;
	}else if (event.button == SDL_BUTTON_RIGHT) {
		if(!creating){
			mouseButton = 1;
		}
		creating = true;
	}

}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		
	}
	creating = false;
	mouseButton = -1;
	
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{	
	if (mouseButton == 0) {
		Matrix44 m;
		Matrix44 r;
		m.SetIdentity();
		r.SetIdentity();
		m.Translate(-cam.center.x, -cam.center.y, -cam.center.z);
		r = m*r;
		m.SetIdentity();
		m.Rotate(-mouse_delta.x*0.001, Vector3(0,1,0));
		r = m*r;
		m.SetIdentity();
		m.Translate(cam.center.x, cam.center.y, cam.center.z);
		r=m*r;
		cam.eye = r*cam.eye;
		cam.eye.y = cam.eye.y+mouse_delta.y*0.001;
	}else if(mouseButton == 1){
		Matrix44 m;
		Matrix44 r;
		m.SetIdentity();
		r.SetIdentity();
		m.Translate(-cam.eye.x, -cam.eye.y, -cam.eye.z);
		r = m*r;
		m.SetIdentity();
		m.Rotate(-mouse_delta.x*0.0003, Vector3(0,1,0));
		r = m*r;
		m.SetIdentity();
		m.Translate(cam.eye.x, cam.eye.y, cam.eye.z);
		r=m*r;
		cam.center = r*cam.center;
		cam.center.y = cam.center.y+mouse_delta.y*0.001;
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

void Application::LoadShaders(){
	shader[0] = Shader::Get("./shaders/Sh1.vs","./shaders/Sh1.fs");
	shader[1] = Shader::Get("./shaders/Sh2.vs","./shaders/Sh2.fs");
	shader[2] = Shader::Get("./shaders/Sh3.vs","./shaders/Sh3.fs");
	shader[3] = Shader::Get("./shaders/Sh4.vs","./shaders/Sh4.fs");
	shader[4] = Shader::Get("./shaders/Sh5.vs","./shaders/Sh5.fs");
	shader[5] = Shader::Get("./shaders/Sh6.vs","./shaders/Sh6.fs");
	shader[6] = Shader::Get("./shaders/Tx1.vs","./shaders/Tx1.fs");
	shader[7] = Shader::Get("./shaders/Tx2.vs","./shaders/Tx2.fs");
	shader[8] = Shader::Get("./shaders/Tx3.vs","./shaders/Tx3.fs");
	shader[9] = Shader::Get("./shaders/Tx4.vs","./shaders/Tx4.fs");
	shader[10] = Shader::Get("./shaders/Tx5.vs","./shaders/Tx5.fs");
	shader[11] = Shader::Get("./shaders/Tx6.vs","./shaders/Tx6.fs");
	shader[12] = Shader::Get("./shaders/Tr1.vs","./shaders/Tr1.fs");
	shader[13] = Shader::Get("./shaders/Tr2.vs","./shaders/Tr2.fs");

}