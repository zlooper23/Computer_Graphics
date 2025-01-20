#include "application.h"
#include <fstream>
#include <string>

class Button {
    public:
        Vector2 position;      // Top-left position
        Vector2 size;          // Width and height
        Color color;           // Fill color for the color  buttons
        Color border_color;    // Border color
        int border_width;      // Border width
        Image* image;          // Image for the button

        //Vector to store all the buttons of the toolbar
        static std::vector<Button*> toolbarButtons; 

        // Constructor
        Button(Vector2 position, Vector2 size, 
            Color color, Color border_color = Color::BLACK, 
            int border_width = 1, Image* image = nullptr);

        // Renders the button on the framebuffer
        void Render();

        // Checks if a point is inside the button area
        bool IsMouseInside(Vector2 mousePosition) const;
        
        //Renders the whole toolbar
        static void RenderToolbar();

        //Initializes the toolbar
        static void InitToolbar();
    };
