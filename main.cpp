#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>

float sunX = 1800.0f; // Initial X position near the top-right corner.
float sunY = 950.0f;  // Initial Y position at the top-right.
float moonX = 1800.0f; // Initial X position near the top-right corner.
float moonY = 950.0f;  // Initial Y position at the top-right.
bool isMovingDown = false; // Flag to check if the sun is moving down
bool isMovingUp = false; // Flag to check if the sun is moving up
bool isSkyBlack = false; // To check if the sky is black
float sunSpeed = 2.0f; // Speed of the sun's movement

float carPosition1 = -300.0f;  // Initial position of the first car on the x-axis
float carPosition2 = 300.0f;   // Initial position of the second car on the x-axis
float carSpeed = 1.0f;         // Default car speed
int moving = 0;                // To check if the cars are moving

// Function to draw a filled circle (used for the foliage of the tree)
void drawCircle(float x, float y, float radius) {
    int numSegments = 3600; // Smoothness of the circle
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numSegments; i++) {
        float angle = 2.0f * 3.1415926f * i / numSegments;
        float dx = cosf(angle) * radius;
        float dy = sinf(angle) * radius;
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

void drawSun() {
    // Sun drawing at (sunX, sunY)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for the sun
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 300; i++) {
        float angle = 2.0f * 3.14159f * i / 300;
        float dx = 50 * cos(angle); // Radius of 50 for the sun
        float dy = 50 * sin(angle);
        glVertex2f(sunX + dx, sunY + dy);
    }
    glEnd();
}

void drawMoon() {
    glColor3f(0.9f, 0.9f, 0.9f); // Yellow color for the sun
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 300; i++) {
        float angle = 2.0f * 3.14159f * i / 300;
        float dx = 50 * cos(angle); // Radius of 50 for the sun
        float dy = 50 * sin(angle);
        glVertex2f(moonX + dx, moonY + dy);
    }
    glEnd();
}


void updateCars(int value) {
    if (moving) {
        carPosition1 += carSpeed;
        carPosition2 += carSpeed;

        if (carPosition1 > 2000.0f) carPosition1 = 20.0f;
        if (carPosition2 > 2000.0f) carPosition2 = 20.0f;
    }

    glutPostRedisplay();              // Redraw the scene
    glutTimerFunc(16, updateCars, 0); // Call the update function again after 16ms
}


void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'd': // Toggle car movement when 'D' is pressed
        case 'D':
            moving = !moving;  // Toggle between moving and stopping
            break;
    }

    if (key == 'n' || key == 'N') {
        isMovingDown = true; // Start moving the sun down when 'N' is pressed
    }

    else if (key == 'm' || key == 'M') {
        isMovingUp = true; // Start moving the sun up when 'M' is pressed
    }

    glutPostRedisplay();  // Request a redraw
}



// Special keys handling (Up and Down arrows to adjust car speed)
void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            carSpeed += 0.5f; // Increase speed
            break;
        case GLUT_KEY_DOWN:
            if (carSpeed > 0.5f) { // Prevent going to negative or stopping completely
                carSpeed -= 0.5f;
            }
            break;
    }
    glutPostRedisplay();
}


// Variables to control cloud movement
bool cloudsMoving = false;
float cloudOffset = 0.0f;
float cloudSpeed = 1.0f; // Speed of cloud movement


void drawCloud(float x, float y, float width, float height) {
    //glColor4f(1.0f, 1.0f, 1.0f, 0.8f); // White color with transparency

    if (isSkyBlack) {
        glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
    } else {
       glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
    }

    // Draw the main ellipse (cloud body)
    glBegin(GL_TRIANGLE_FAN);
    float centerX = x;
    float centerY = y;
    int numVertices = 360; // Number of points to form the ellipse
    float radiusX = width / 2.0f; // Half-width of the ellipse
    float radiusY = height / 2.0f; // Half-height of the ellipse

    for (int i = 0; i <= numVertices; ++i) {
        float angle = 2.0f * M_PI * i / numVertices;
        float dx = radiusX * cos(angle);
        float dy = radiusY * sin(angle);
        glVertex2f(centerX + dx, centerY + dy);
    }
    glEnd();

    // Draw 5 rounded lobes
    int numLobes = 5;
    for (int i = 0; i < numLobes; ++i) {
        float lobeCenterX = x + (radiusX * 0.9f) * cos(i * 2 * M_PI / numLobes);
        float lobeCenterY = y + (radiusY * 0.9f) * sin(i * 2 * M_PI / numLobes);
        float lobeWidth = radiusX * 1.2f;
        float lobeHeight = radiusY * 1.2f;

        // Draw each lobe
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(lobeCenterX, lobeCenterY);
        for (int j = 0; j <= numVertices; ++j) {
            float angle = 2.0f * M_PI * j / numVertices;
            float dx = lobeWidth * cos(angle);
            float dy = lobeHeight * sin(angle);
            glVertex2f(lobeCenterX + dx, lobeCenterY + dy);
        }
        glEnd();
    }
}


// Function to draw a tree with rounded foliage
void drawTree(float x, float y) {
    // Tree trunk
    glBegin(GL_QUADS);
    glColor3f(0.545f, 0.271f, 0.075f);  // Brown color for the trunk
    glVertex2f(x - 10, y);              // Bottom left
    glVertex2f(x + 10, y);              // Bottom right
    glVertex2f(x + 10, y + 50);         // Top right
    glVertex2f(x - 10, y + 50);         // Top left
    glEnd();

    // Rounded foliage (using circles to create cloud-like structure)
    glColor3f(0.0f, 0.5f, 0.0f);        // Green color for the foliage
    drawCircle(x, y + 80, 30);          // Central circle
    drawCircle(x - 25, y + 70, 20);     // Left circle
    drawCircle(x + 25, y + 70, 20);     // Right circle
    drawCircle(x - 15, y + 100, 15);    // Upper-left circle
    drawCircle(x + 15, y + 100, 15);    // Upper-right circle
}

// Function to draw a human figure
void drawHuman(float x, float y) {
    // Draw the head (circle)
    glColor3f(0.98f, 0.80f, 0.68f);  // Skin color
    drawCircle(x, y + 60, 15);       // Head positioned above the body

    // Draw the body (rectangle)
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue color for the body (shirt)
    glBegin(GL_QUADS);
    glVertex2f(x - 10, y + 45);      // Bottom left
    glVertex2f(x + 10, y + 45);      // Bottom right
    glVertex2f(x + 10, y);           // Top right
    glVertex2f(x - 10, y);           // Top left
    glEnd();

    // Draw the arms (rectangles)
    glColor3f(0.98f, 0.80f, 0.68f);  // Skin color
    glBegin(GL_QUADS);
    // Left arm
    glVertex2f(x - 20, y + 45);      // Top left
    glVertex2f(x - 10, y + 45);      // Top right
    glVertex2f(x - 10, y + 15);      // Bottom right
    glVertex2f(x - 20, y + 15);      // Bottom left
    glEnd();

    glBegin(GL_QUADS);
    // Right arm
    glVertex2f(x + 10, y + 45);      // Top left
    glVertex2f(x + 20, y + 45);      // Top right
    glVertex2f(x + 20, y + 15);      // Bottom right
    glVertex2f(x + 10, y + 15);      // Bottom left
    glEnd();

    // Draw the legs (rectangles)
    glColor3f(0.0f, 0.0f, 0.0f);     // Black color for the legs (pants)
    glBegin(GL_QUADS);
    // Left leg
    glVertex2f(x - 8, y);            // Top left
    glVertex2f(x, y);                // Top right
    glVertex2f(x, y - 40);           // Bottom right
    glVertex2f(x - 8, y - 40);       // Bottom left
    glEnd();

    glBegin(GL_QUADS);
    // Right leg
    glVertex2f(x, y);                // Top left
    glVertex2f(x + 8, y);            // Top right
    glVertex2f(x + 8, y - 40);       // Bottom right
    glVertex2f(x, y - 40);           // Bottom left
    glEnd();
}

// Function to draw a car
void drawCar(float x, float y, float r, float g, float b) {
    // Draw the car body (rectangle)
    glColor3f(r, g, b);  // Car body color
    glBegin(GL_QUADS);
    glVertex2f(x - 100, y);       // Bottom left
    glVertex2f(x + 100, y);       // Bottom right
    glVertex2f(x + 100, y + 60);  // Top right
    glVertex2f(x - 100, y + 60);  // Top left
    glEnd();

    // Draw the car roof (larger rectangle on top)
    glBegin(GL_QUADS);
    glVertex2f(x - 100, y + 60);   // Bottom left
    glVertex2f(x + 60, y + 60);    // Bottom right
    glVertex2f(x + 60, y + 100);   // Top right
    glVertex2f(x - 100, y + 100);  // Top left
    glEnd();

    // Draw the windows (two smaller rectangles)
    glColor3f(1.0f, 1.0f, 1.0f);  // White color for windows

    // Left window
    glBegin(GL_QUADS);
    glVertex2f(x - 80, y + 50);   // Bottom left of window
    glVertex2f(x - 30, y + 50);   // Bottom right of window
    glVertex2f(x - 30, y + 85);   // Top right of window
    glVertex2f(x - 80, y + 85);   // Top left of window
    glEnd();

    // Right window
    glBegin(GL_QUADS);
    glVertex2f(x - 10, y + 50);   // Bottom left of window
    glVertex2f(x + 40, y + 50);   // Bottom right of window
    glVertex2f(x + 40, y + 85);   // Top right of window
    glVertex2f(x - 10, y + 85);   // Top left of window
    glEnd();

    // Draw the wheels (outer black and inner gray circles)
    glColor3f(0.0f, 0.0f, 0.0f);  // Black for outer wheel
    drawCircle(x - 60, y - 10, 30);  // Left wheel (outer)
    drawCircle(x + 60, y - 10, 30);  // Right wheel (outer)

    // Draw inner gray strip of the wheels
    glColor3f(0.5f, 0.5f, 0.5f);  // Gray for inner strip
    drawCircle(x - 60, y - 10, 16);  // Left wheel (inner)
    drawCircle(x + 60, y - 10, 16);  // Right wheel (inner)
}




void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to white

    // Set the sky color based on the condition
    glBegin(GL_QUADS);
    if (isSkyBlack) {
        glColor3f(0.1f, 0.1f, 0.1f); // Black sky
    } else {
        glColor3f(0.588f, 0.937f, 1.0f); // Sky blue color
    }
    glVertex2i(0, 650);          // x, y
    glVertex2i(1920, 650);       // x, y
    glVertex2i(1920, 1080);      // x, y
    glVertex2i(0, 1080);
    glEnd();

    if (isSkyBlack) {
        drawMoon();
    }

    // sun
    drawSun();

    // tree
    drawTree(200, 570);
    drawTree(420, 585);
    drawTree(670, 585);
    drawTree(1800, 580);



    // Grass
    glBegin(GL_QUADS);
    if (isSkyBlack) {
        glColor3f(0.0039f, 0.1960f, 0.1254f);
    }
    else {
        glColor3f(0.3f, 0.8f, 0.5f);
    }

    glVertex2i(0, 400);          // x, y
    glVertex2i(1920, 400);       // x, y
    glVertex2i(1920, 650);       // x, y
    glVertex2i(0, 650);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2i(0, 380);
    glVertex2i(1920, 380);
    glVertex2i(1920, 400);
    glVertex2i(0, 400);

    // Street
    glBegin(GL_QUADS);
    glColor3f(0.211f, 0.270f, 0.309f); // Dark gray color for street
    glVertex2i(0, 0);          // x, y
    glVertex2i(1920, 0);       // x, y
    glVertex2i(1920, 400);     // x, y
    glVertex2i(0, 400);
    glEnd();

    // street lines
    glLineWidth(5);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Define the starting position and the increment
    int startX = 10;
    int increment = 140; // Distance between the start points of each line
    int endX = 1950; // Last endpoint

    // Loop to draw the street lines
    for (int x = startX; x <= endX; x += increment) {
        glBegin(GL_LINES);
            glVertex2i(x, 200);
            glVertex2i(x + 120, 200); // Each line is 120 units long
        glEnd();
    }


    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.3f, 0.4f);
    glVertex2i(0, 330);
    glVertex2i(1920, 330);
    glVertex2i(1920, 400);
    glVertex2i(0, 400);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.6f, 0.7f);
    glVertex2i(0, 330);
    glVertex2i(1920, 330);
    glVertex2i(1920, 350);
    glVertex2i(0, 350);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.3f, 0.4f);
    glVertex2i(0, 0);
    glVertex2i(1920, 0);
    glVertex2i(1920, 70);
    glVertex2i(0, 70);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.6f, 0.7f);
    glVertex2i(0, 50);
    glVertex2i(1920, 50);
    glVertex2i(1920, 70);
    glVertex2i(0, 70);
    glEnd();

    // Drawing the sun in the sky
    //drawSun(1600.0f, 900.0f, 80.0f); // Sun at position (1600, 900) with radius 80


    // Drawing clouds with updated appearance

    // Drawing clouds with updated appearance (apply cloudOffset)


    drawCloud(10.0f + cloudOffset, 970.0f, 140.0f, 70.0f);  // Cloud 1
    drawCloud(850.0f + cloudOffset, 780.0f, 130.0f, 65.0f);  // Cloud 2
    drawCloud(1200.0f + cloudOffset, 980.0f, 125.0f, 60.0f); // Cloud 3
    drawCloud(1700.0f + cloudOffset, 930.0f, 150.0f, 65.0f); // Cloud 4

    // Building 1 (Left)
    glBegin(GL_QUADS);
    if (!isSkyBlack) {
        glColor3f(0.996f, 0.364f, 0.149f);
    }
    else {
        glColor3f(0.696f, 0.064f, 0.049f);
    }

    glVertex2i(400, 400);    // x, y
    glVertex2i(600, 400);    // x, y
    glVertex2i(600, 750);    // x, y
    glVertex2i(400, 750);
    glEnd();


    // ROOF
    glBegin(GL_QUADS);
    glColor3f(0.949f, 0.752f, 0.470f);
    glVertex2i(390, 740);    // x, y
    glVertex2i(610, 740);    // x, y
    glVertex2i(610, 760);    // x, y
    glVertex2i(390, 760);
    glEnd();

    // Windows for Building 1
    // Window 1 (Top-left)
    glBegin(GL_QUADS);
    glColor3f(0.980f, 0.929f, 0.792f);          // White color for windows
    glVertex2i(425, 725);    // x, y
    glVertex2i(475, 725);    // x, y
    glVertex2i(475, 675);    // x, y
    glVertex2i(425, 675);
    glEnd();

    // Window 2 (Top-right)
    glBegin(GL_QUADS);
    glColor3f(0.980f, 0.929f, 0.792f);
    glVertex2i(525, 725);
    glVertex2i(575, 725);
    glVertex2i(575, 675);
    glVertex2i(525, 675);
    glEnd();

    // Window 3 (Middle-left)
    glBegin(GL_QUADS);
    glColor3f(0.980f, 0.929f, 0.792f);
    glVertex2i(425, 650);
    glVertex2i(475, 650);
    glVertex2i(475, 600);
    glVertex2i(425, 600);
    glEnd();

    // Window 4 (Middle-right)
    glBegin(GL_QUADS);
    glColor3f(0.980f, 0.929f, 0.792f);
    glVertex2i(525, 650);
    glVertex2i(575, 650);
    glVertex2i(575, 600);
    glVertex2i(525, 600);
    glEnd();

    // Window 5 (Bottom-left)
    glBegin(GL_QUADS);
    glColor3f(0.980f, 0.929f, 0.792f);
    glVertex2i(425, 575);
    glVertex2i(475, 575);
    glVertex2i(475, 525);
    glVertex2i(425, 525);
    glEnd();

    // Window 6 (Bottom-right)
    glBegin(GL_QUADS);
    glColor3f(0.980f, 0.929f, 0.792f);
    glVertex2i(525, 575);
    glVertex2i(575, 575);
    glVertex2i(575, 525);
    glVertex2i(525, 525);
    glEnd();



    // Building 2 (Right) - Smaller
    glBegin(GL_QUADS);
    if (!isSkyBlack) {
        glColor3f(0.313f, 0.490f, 0.737f);
    }
    else {
        glColor3f(0.013f, 0.190f, 0.437f);
    }

    glVertex2i(800, 400);    // x, y
    glVertex2i(1000, 400);   // x, y
    glVertex2i(1000, 750);   // x, y
    glVertex2i(800, 750);
    glEnd();

    // ROOF
    glBegin(GL_QUADS);
    glColor3f(0.015f, 0.031f, 0.058f);
    glVertex2i(795, 740);    // x, y
    glVertex2i(1005, 740);    // x, y
    glVertex2i(1005, 760);    // x, y
    glVertex2i(795, 760);
    glEnd();

    // Windows for Building 2
    // Window 1 (Top-left)
    glBegin(GL_QUADS);
    glColor3f(0.631f, 0.776f, 0.917f);
    glVertex2i(825, 725);    // x, y
    glVertex2i(875, 725);    // x, y
    glVertex2i(875, 675);    // x, y
    glVertex2i(825, 675);
    glEnd();

    // Window 2 (Top-right)
    glBegin(GL_QUADS);
    glColor3f(0.631f, 0.776f, 0.917f);
    glVertex2i(925, 725);
    glVertex2i(975, 725);
    glVertex2i(975, 675);
    glVertex2i(925, 675);
    glEnd();

    // Window 3 (Middle-left)
    glBegin(GL_QUADS);
    glColor3f(0.631f, 0.776f, 0.917f);
    glVertex2i(825, 650);
    glVertex2i(875, 650);
    glVertex2i(875, 600);
    glVertex2i(825, 600);
    glEnd();

    // Window 4 (Middle-right)
    glBegin(GL_QUADS);
    glColor3f(0.631f, 0.776f, 0.917f);
    glVertex2i(925, 650);
    glVertex2i(975, 650);
    glVertex2i(975, 600);
    glVertex2i(925, 600);
    glEnd();

    // Window 5 (Bottom-left)
    glBegin(GL_QUADS);
    glColor3f(0.631f, 0.776f, 0.917f);
    glVertex2i(825, 575);
    glVertex2i(875, 575);
    glVertex2i(875, 525);
    glVertex2i(825, 525);
    glEnd();

    // Window 6 (Bottom-right)
    glBegin(GL_QUADS);
    glColor3f(0.631f, 0.776f, 0.917f);
    glVertex2i(925, 575);
    glVertex2i(975, 575);
    glVertex2i(975, 525);
    glVertex2i(925, 525);
    glEnd();

    // Drawing 5 smaller buildings in the background

    glBegin(GL_QUADS);
    glColor3f(0.7f, 0.8f, 0.9f);
    glVertex2i(240, 650);    // x, y (smaller, far away)
    glVertex2i(290, 650);    // x, y
    glVertex2i(290, 700);    // x, y
    glVertex2i(240, 700);    // x, y
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.5f, 0.9f);
    glVertex2i(210, 650);    // x, y (smaller, far away)
    glVertex2i(260, 650);    // x, y
    glVertex2i(260, 720);    // x, y
    glVertex2i(210, 720);    // x, y
    glEnd();


    // Building 3 (Small Left)
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.3f, 0.8f);    // Purple color for the third building
    glVertex2i(1400, 650);    // x, y (smaller, far away)
    glVertex2i(1450, 650);    // x, y
    glVertex2i(1450, 680);    // x, y
    glVertex2i(1400, 680);    // x, y
    glEnd();

    // Building 4 (Small Middle)
    glBegin(GL_QUADS);
    glColor3f(0.7f, 0.2f, 0.2f);    // Dark red for the fourth building
    glVertex2i(1500, 650);    // x, y (smaller, far away)
    glVertex2i(1550, 650);    // x, y
    glVertex2i(1550, 690);    // x, y
    glVertex2i(1500, 690);
    glEnd();

    // Building 5 (Small Right)
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.6f, 0.3f);    // Green color for the fifth building
    glVertex2i(1600, 650);    // x, y (smaller, far away)
    glVertex2i(1650, 650);    // x, y
    glVertex2i(1650, 670);    // x, y
    glVertex2i(1600, 670);
    glEnd();

    // Drawing the human on the pavement
    drawHuman(150, 390); // Position on the left side of the road

   // Draw the first car (red)
    drawCar(carPosition1, 170.0f, 1.0f, 0.0f, 0.0f); // Red car

    // Draw the second car (blue)
    drawCar(carPosition2, 270.0f, 0.0f, 0.0f, 1.0f); // Blue car




    // Drawing trees on the grass
    drawTree(50, 580);  // Tree 1 on the left
    drawTree(1500, 400);  // Tree 2 in the middle
    drawTree(1800, 450);  // Tree 3 on the right

    glutSwapBuffers();  // Swap buffers for double buffering

    glFlush();  // Render now
}

void updateSun(int value) {
    if (isMovingDown) {
        sunY -= sunSpeed;

        // Check if the sun goes behind the grass
        if (sunY < 600.0f) {
            sunY = 600.0f; // Stop at grass level
            isSkyBlack = true; // Change sky color to black
        }
    } else if(isMovingUp) {
        // Move the sun back up
        if (sunY < 900.0f) {
            sunY += sunSpeed;
            isSkyBlack = false; // Change sky color back to blue
        } else {
            sunY = 900.0f; // Stop at initial position
        }
    }

    glutPostRedisplay();
    glutTimerFunc(1000 / 60, updateSun, 0);
}


// Timer function to update the cloud position and re-render the scene
void updateClouds(int value) {
    if (cloudsMoving) {
        cloudOffset += cloudSpeed; // Move the clouds to the right
        if (cloudOffset > 1920) {
            cloudOffset = -200; // Reset the clouds once they move off-screen
        }
        glutPostRedisplay();    // Request to re-render the screen
    }
    glutTimerFunc(16, updateClouds, 0); // 60 FPS (~16ms per frame)
}

// Mouse callback function to toggle cloud movement on left-click
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        cloudsMoving = !cloudsMoving; // Toggle cloud movement
    }
}

void myInit (void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);     // Set background color to white
    glColor3f(0.0f, 0.0f, 0.0f);          // Set drawing color to black
    glPointSize(4.0);                     // Set point size
    glMatrixMode(GL_PROJECTION);          // Set projection mode
    glLoadIdentity();                     // Reset projection matrix
    gluOrtho2D(0.0, 1920.0, 0.0, 1080.0); // Define orthographic projection for 1920x1080 screen
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                 // Initialize GLUT
    glutInitWindowSize(1920, 1080);        // Set the window's initial width & height to 1920x1080
    glutInitWindowPosition(0, 0);          // Set the window's initial position according to the monitor
    glutCreateWindow("City 2D"); // Create a window with the given title
    glutDisplayFunc(display);              // Register display callback handler for window re-paint
    myInit();

    // Set up the initial state

    glutKeyboardFunc(keyboard);  // Register keyboard callback for car movement

    glutTimerFunc(100, updateSun, 0);  // Start the timer to move the sun

    glutSpecialFunc(specialKeys); // Register special keys callback for speed adjustments


    glutTimerFunc(0, updateCars, 0); // Start car updates


    glutMouseFunc(mouseClick);    // Set the mouse callback function
    glutTimerFunc(16, updateClouds, 0);

    glutMainLoop();                        // Enter the event-processing loop
    return 0;
}
