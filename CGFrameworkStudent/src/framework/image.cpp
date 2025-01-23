#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "GL/glew.h"
#include "../extra/picopng.h"
#include "image.h"
#include "utils.h"
#include "camera.h"
#include "mesh.h"


Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

// Copy constructor
Image::Image(const Image& c)
{
	pixels = NULL;
	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
}

// Assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	bytes_per_pixel = c.bytes_per_pixel;

	if(c.pixels)
	{
		pixels = new Color[width*height*bytes_per_pixel];
		memcpy(pixels, c.pixels, width*height*bytes_per_pixel);
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete pixels;
}

void Image::Render()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(width, height, bytes_per_pixel == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

// Change image size (the old one will remain in the top-left corner)
void Image::Resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = GetPixel(x,y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

// Change image size and scale the content
void Image::Scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = GetPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::GetArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.SetPixelUnsafe( x, y, GetPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::FlipY()
{
	int row_size = bytes_per_pixel * width;
	Uint8* temp_row = new Uint8[row_size];
#pragma omp simd
	for (int y = 0; y < height * 0.5; y += 1)
	{
		Uint8* pos = (Uint8*)pixels + y * row_size;
		memcpy(temp_row, pos, row_size);
		Uint8* pos2 = (Uint8*)pixels + (height - y - 1) * row_size;
		memcpy(pos, pos2, row_size);
		memcpy(pos2, temp_row, row_size);
	}
	delete[] temp_row;
}

bool Image::LoadPNG(const char* filename, bool flip_y)
{	
	std::string sfullPath = absResPath(filename);
	std::ifstream file(sfullPath, std::ios::in | std::ios::binary | std::ios::ate);

	if (!file.is_open()) {
		printf("Failed to open file: %s\n", sfullPath.c_str());
		return false;
	}

	// Get filesize
	std::streamsize size = 0;
	if (file.seekg(0, std::ios::end).good()) size = file.tellg();
	if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	if (!size){
		printf("Fail size\n");
		return false;
	}
		

	std::vector<unsigned char> buffer;

	// Read contents of the file into the vector
	if (size > 0)
	{
		buffer.resize((size_t)size);
		file.read((char*)(&buffer[0]), size);
	}
	else
		buffer.clear();

	std::vector<unsigned char> out_image;

	if (decodePNG(out_image, width, height, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size(), true) != 0){
		printf("Fail decoding\n");
		return false;
	}
		

	size_t bufferSize = out_image.size();
	unsigned int originalBytesPerPixel = (unsigned int)bufferSize / (width * height);
	
	// Force 3 channels
	bytes_per_pixel = 3;

	if (originalBytesPerPixel == 3) {
		pixels = new Color[bufferSize];
		memcpy(pixels, &out_image[0], bufferSize);
	}
	else if (originalBytesPerPixel == 4) {

		unsigned int newBufferSize = width * height * bytes_per_pixel;
		pixels = new Color[newBufferSize];

		unsigned int k = 0;
		for (unsigned int i = 0; i < bufferSize; i += originalBytesPerPixel) {
			pixels[k] = Color(out_image[i], out_image[i + 1], out_image[i + 2]);
			k++;
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	return true;
}

// Loads an image from a TGA file
bool Image::LoadTGA(const char* filename, bool flip_y)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int imageSize;
	unsigned int bytesPerPixel;

    std::string sfullPath = absResPath( filename );
	

	FILE * file = fopen( sfullPath.c_str(), "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << sfullPath.c_str() << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;
    
	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];
    
	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		fclose(file);
		delete tgainfo;
		return NULL;
	}
    
	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
	tgainfo->data = new unsigned char[imageSize];
    
	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;
            
		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	// Save info in image
	if(pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	// Convert to float all pixels
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			// Make sure we don't access out of memory
			if( (pos < imageSize) && (pos + 1 < imageSize) && (pos + 2 < imageSize))
				SetPixelUnsafe(x, height - y - 1, Color(tgainfo->data[pos + 2], tgainfo->data[pos + 1], tgainfo->data[pos]));
		}
	}

	// Flip pixels in Y
	if (flip_y)
		FlipY();

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::SaveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	std::string fullPath = absResPath(filename);
	FILE *file = fopen(fullPath.c_str(), "wb");
	if ( file == NULL )
	{
		perror("Failed to open file: ");
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0;

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	// Convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[y*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);

	return true;
}

void Image::DrawRect(int x, int y, int w, int h, const Color& c)
{

	for (int i = 0; i < w; ++i) {
		SetPixelUnsafe(x + i, y, c);
		SetPixelUnsafe(x + i, y + h - 1, c);
	}

	for (int j = 0; j < h; ++j) {
		SetPixelUnsafe(x, y + j, c);
		SetPixelUnsafe(x + w - 1, y + j, c);
	}
}

#ifndef IGNORE_LAMBDAS

// You can apply and algorithm for two images and store the result in the first one
// ForEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void ForEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}

#endif

FloatImage::FloatImage(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new float[width * height];
	memset(pixels, 0, width * height * sizeof(float));
}

// Copy constructor
FloatImage::FloatImage(const FloatImage& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
}

// Assign operator
FloatImage& FloatImage::operator = (const FloatImage& c)
{
	if (pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new float[width * height * sizeof(float)];
		memcpy(pixels, c.pixels, width * height * sizeof(float));
	}
	return *this;
}

FloatImage::~FloatImage()
{
	if (pixels)
		delete pixels;
}

// Change image size (the old one will remain in the top-left corner)
void FloatImage::Resize(unsigned int width, unsigned int height)
{
	float* new_pixels = new float[width * height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for (unsigned int x = 0; x < min_width; ++x)
		for (unsigned int y = 0; y < min_height; ++y)
			new_pixels[y * width + x] = GetPixel(x, y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

void Image::DrawLineDDA(int x0, int y0, int x1, int y1, const Color& c){
	float d = std::max(abs(x1-x0), abs(y1-y0));

	Vector2 v(((x1-x0)/d), ((y1-y0)/d));
	Vector2 p((float)x0, (float)y0);

	for(int i = 0; i<d; i++){
		SetPixel((int)floor(p.x), (int)floor(p.y), c);
		p+=v;
	}
}

void Image::DrawRect(int x, int y, int w, int h, const Color& borderColor,
int borderWidth, bool isFilled, const Color& fillColor){
	if(isFilled){
		for( int xi = 0; xi < abs(w); ++xi){
			for( int yi = 0; yi < abs(h); ++yi)
			{
				SetPixel(x+xi*(w/abs(w)), y+yi*(h/abs(h)), fillColor);
			}
		}
	}
	
	// Draw only the border
	for(int i = 0; i<borderWidth; i++){
		if(w<0){
			for( int xi = 0; xi < abs(w)+1; ++xi)
			{
				SetPixel(x+xi*(w/abs(w)), y-i, borderColor);
				SetPixel(x+xi*(w/abs(w)), y + h+i, borderColor);
			}
		}else{
			for( int xi = 1-borderWidth; xi < abs(w)+borderWidth; ++xi)
			{
				SetPixel(x+xi*(w/abs(w)), y-i, borderColor);
				SetPixel(x+xi*(w/abs(w)), y + h+i, borderColor);
			}
		}
		for( int yi = 0; yi < abs(h)+1; ++yi)
		{
			SetPixel(x-i, y+yi*(h/abs(h)), borderColor);
			SetPixel(x + w+i, y+yi*(h/abs(h)), borderColor);
		}
	}
}







void Image::ScanLineDDA(int x0, int y0, int x1, int y1, std::vector<int>& minX, std::vector<int>& maxX){
	float d = std::max(abs(x1-x0), abs(y1-y0));
	Vector2 v(((x1-x0)/d), ((y1-y0)/d));
	Vector2 p((float)x0, (float)y0);
	for(int i = 0; i<d; i++){
		SetPixel((int)floor(p.x), (int)floor(p.y), Color::RED);
		int a = p.y;
		minX[a] = std::min(minX[a], (int)p.x);
		maxX[a] = std::max(maxX[a], (int)p.x);
		p+=v;
	}
}

void Image::DrawTriangle(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Color& borderColor, bool isFilled, const Color& fillColor){
	int mxy = std::max(p2.y, std::max(p1.y, p0.y));
	int mny = std::min(p2.y, std::min(p1.y, p0.y));
	int mxx = std::max(p2.x, std::max(p1.x, p0.x));
	int mnx = std::min(p2.x, std::min(p1.x, p0.x));
	
	std::vector<int> minX;
	std::vector<int> maxX;

	minX.assign(mxy+1, mxx);
	maxX.assign(mxy+1, mnx);

	ScanLineDDA(p0.x, p0.y, p1.x, p1.y, minX, maxX);
	ScanLineDDA(p1.x, p1.y, p2.x, p2.y, minX, maxX);
	ScanLineDDA(p2.x, p2.y, p0.x, p0.y, minX, maxX);

	if(isFilled){
		int a = 0;
		for(int i = 1; i<mxy; i++){
			for(int j = minX[a]+1;j<maxX[a]; j++){
				SetPixel(j, i, Color::WHITE);
			}
			a++;
		}
	}
	
}
void Image::DrawQuarter(int x, int y, int a, int b, const Color& borderColor){
		SetPixel(x+a, y+b, borderColor);		
		SetPixel(x-a, y-b, borderColor);		
		SetPixel(x-a, y+b, borderColor);		
		SetPixel(x+a, y-b, borderColor);
}

void Image::DrawCircle(int x, int y, int r, const Color& borderColor, int borderWidth, bool isFilled, const Color& fillColor){
	std::vector<int> min;
	std::vector<int> max;

	min.assign(r+borderWidth+1, 0);
	max.assign(r+borderWidth+1, 0);
	
	float t1 = (r / 16.0);
	float x0 = (float)r;
	float y0 = 0.0;
	int r1=r;


	for(int z = 0; z<2; z++){

		int a = (int)x0;
		int b = (int)y0;
		if(z == 0){
			min[b] = a;
			min[a] = b;	
			
		}else{
			max[b] = a;
			max[a] = b;	
		}

		while(x0>y0){
			if(isFilled&&z==0){
				for(int i = 0; i<a; i++){
					DrawQuarter(x, y, i, b, fillColor);
					DrawQuarter(x, y, b, i, fillColor);
				}
			}
			DrawQuarter(x, y, a, b, borderColor);
			DrawQuarter(x, y, b, a, borderColor);
			y0++;
			t1 = t1+y0;
			float t2 = t1-x0;
			if(t2>=0){
				t1 = t2;
				x0--;
			}

			a = (int)x0;
			b = (int)y0;
			if(z == 0){
				min[b] = a;
				min[a] = b;	
				
			}else{
				max[b] = a;
				max[a] = b;	
			}
		}
		r1 = r+borderWidth;
		t1 = (r1 / 16.0);
		x0 = (float)r1;
		y0 = 0.0; 
	}
	for(int i = 0; i<r1; i++){
		for(int j = min[i]; j<max[i]; j++){
			DrawQuarter(x, y, j, i, borderColor);
		}
	}
		
}

void Image::DrawImage(const Image& image, int x, int y){
	for(int i = 0; i<image.width; i++){
		for(int j = 0; j<image.height; j++){
			SetPixel(i+x, j+y, image.pixels[ j * image.width + i]);
			/*if(i < 0 || i > width-1){}
			else if(j < 0 || j > height-1){}
			else{
				pixels[ (j+y) * width + i+x] = image.pixels[ j * image.width + i];
			}*/
		}
	}
}




