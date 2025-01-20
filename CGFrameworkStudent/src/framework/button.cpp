#include "application.h"
#include "button.h"


std:: vector<Button*> Button::toolbarButtons;

// Button constructor 
Button::Button(Vector2 position, Vector2 size, Color color, Color border_color, int border_width){
    this->position = position;
    this->size = size;
    this->color = color;
    this->border_color = border_color; 
    this->border_width = border_width; 
}
    

void Button::Render(const Image& c) {
    // Ensure the image is loaded properly before rendering
    if ((image.width > 0) && (image.height > 0)) {
        // Render the button's image at its position
        //c.DrawImage(image, position.x, position.y);
        printf("Rendered corfectly\n");
    } else {
        std::cerr << "Error: Button has an invalid image and cannot be rendered." << std::endl;
    }

}

// Check if a point is inside the button area
bool Button::IsMouseInside(Vector2 mousePosition) const {
    return (mousePosition.x >= position.x &&
            mousePosition.x <= position.x + size.x &&
            mousePosition.y >= position.y &&
            mousePosition.y <= position.y + size.y);
}


void Button::RenderToolbar(const Image& c) {
    int startX = 10;   // Initial X position for the toolbar
    int startY = 10;   // Y position for the toolbar
    int spacing = 5;   // Spacing between buttons

    int currentX = startX;

    for (Button* button : toolbarButtons) {
        if (button) {
            // Set the position dynamically for each button
            button->position = Vector2(currentX, startY);

            // Render the button
            button->image.LoadPNG("image/clear.png");
            button->Render(c);

            // Update the X position for the next button
            currentX += button->size.x + spacing;
        }
    }
}

void Button::InitToolbar() {
    
    // Add buttons to the toolbar with corresponding images
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::BLACK, Color::WHITE, 1));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::YELLOW, Color::BLACK, 1));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::RED, Color::BLACK, 1));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::BLUE, Color::BLACK, 1));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::CYAN, Color::BLACK, 1));
    (toolbarButtons[0])->image.LoadPNG("image/clear.png");
    (toolbarButtons[1])->image.LoadPNG("image/load.png");
    (toolbarButtons[2])->image.LoadPNG("image/save.png");
    (toolbarButtons[3])->image.LoadPNG("image/eraser.png");
    (toolbarButtons[4])->image.LoadPNG("image/line.png");
    (toolbarButtons[5])->image.LoadPNG("image/rectangle.png");
    (toolbarButtons[6])->image.LoadPNG("image/circle.png");
    (toolbarButtons[7])->image.LoadPNG("image/triangle.png");
    (toolbarButtons[8])->image.LoadPNG("image/triangle.png");
    (toolbarButtons[9])->image.LoadPNG("image/triangle.png");
    (toolbarButtons[10])->image.LoadPNG("image/triangle.png");
    (toolbarButtons[11])->image.LoadPNG("image/triangle.png");
    (toolbarButtons[12])->image.LoadPNG("image/triangle.png");
    (toolbarButtons[13])->image.LoadPNG("image/triangle.png");
}
