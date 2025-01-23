#include "application.h"
#include "button.h"



// Button constructor 
Button::Button(int x, int y, Image im, int id){
    this->position = Vector2(x, y);
    this->image = im;
    this->id = id;
}
Button::Button(){
    
}


void Button::Render(Image &c) {
    // Ensure the image is loaded properly before rendering
    if ((image.width > 0) && (image.height > 0)) {
        c.DrawImage(image, position.x, position.y);
    } else {
        std::cerr << "Error: Button has an invalid image and cannot be rendered." << std::endl;
    }

}

// Check if a point is inside the button area
bool Button::IsMouseInside(Vector2 mousePosition) const {
    return (mousePosition.x >= position.x &&
            mousePosition.x <= position.x + image.width &&
            mousePosition.y >= position.y &&
            mousePosition.y <= position.y + image.height);
}

