#include "button.h"
#include "application.h"

std:: vector<Button*> Button::toolbarButtons;

// Button constructor 
Button::Button(Vector2 position, Vector2 size, Color color, 
               Color border_color, int border_width, Image* image)
    : position(position), size(size), color(color), 
      border_color(border_color), border_width(border_width), 
      image(image) {
        if(image){
            image->LoadPNG("res/images/blue.png");
        }
      }

void Button::Render() {
    if (this->image) {
        // Ensure the image is loaded properly before rendering
        if (this->image->width > 0 && this->image->height > 0) {
            // Render the button's image at its position
            this->image->DrawImage(*image, position.x, position.y);
        } else {
            std::cerr << "Error: Button has an invalid image and cannot be rendered." << std::endl;
        }
    } else {
        // Render a rectangle if no image is provided
        this->image->DrawRect(position.x, position.y, size.x, size.y, border_color, border_width, true, color);
    }
}

// Check if a point is inside the button area
bool Button::IsMouseInside(Vector2 mousePosition) const {
    return (mousePosition.x >= position.x &&
            mousePosition.x <= position.x + size.x &&
            mousePosition.y >= position.y &&
            mousePosition.y <= position.y + size.y);
}


void Button::RenderToolbar() {
    int startX = 10;   // Initial X position for the toolbar
    int startY = 10;   // Y position for the toolbar
    int spacing = 5;   // Spacing between buttons

    int currentX = startX;

    for (Button* button : toolbarButtons) {
        if (button) {
            // Set the position dynamically for each button
            button->position = Vector2(currentX, startY);

            // Render the button
            button->Render();

            // Update the X position for the next button
            currentX += button->size.x + spacing;
        }
    }
}

void Button::InitToolbar() {
    
    // Add buttons to the toolbar with corresponding images
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1, new Image()));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1, new Image()));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1, new Image()));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1, new Image()));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1, new Image()));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1, new Image()));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1, new Image()));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1, new Image()));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::BLACK, Color::WHITE, 1, new Image()));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::WHITE, Color::BLACK, 1, new Image()));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::YELLOW, Color::BLACK, 1, new Image()));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::RED, Color::BLACK, 1, new Image()));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::BLUE, Color::BLACK, 1, new Image()));
    toolbarButtons.push_back(new Button(Vector2(0, 0), Vector2(40, 40), Color::CYAN, Color::BLACK, 1, new Image()));
    toolbarButtons[0]->image->LoadPNG("./image/clear.png");
    toolbarButtons[1]->image->LoadPNG("./image/load.png");
    toolbarButtons[2]->image->LoadPNG("./image/save.png");
    toolbarButtons[3]->image->LoadPNG("./image/eraser.png");
    toolbarButtons[4]->image->LoadPNG("./image/line.png");
    toolbarButtons[5]->image->LoadPNG("./image/rectangle.png");
    toolbarButtons[6]->image->LoadPNG("./image/circle.png");
    toolbarButtons[7]->image->LoadPNG("./image/triangle.png");
    toolbarButtons[8]->image->Fill(Color::BLACK);
    toolbarButtons[9]->image->Fill(Color::WHITE);
    toolbarButtons[10]->image->Fill(Color::YELLOW);
    toolbarButtons[11]->image->Fill(Color::RED);
    toolbarButtons[12]->image->Fill(Color::BLUE);
    toolbarButtons[13]->image->Fill(Color::CYAN);
}
