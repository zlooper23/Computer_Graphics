#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "utils.h"
#include <fstream>
#include <string>

class Button {
    public:
        Vector2 position;      // Top-left position
        Image image;          // Image for the button
        int id;

        //Vector to store all the buttons of the toolbar

        // Constructor
        Button(int x, int y, Image im, int id);
        Button();

        // Renders the button on the framebuffer
        void Render(Image &c);

        // Checks if a point is inside the button area
        bool IsMouseInside(Vector2 mousePosition) const;

    };
