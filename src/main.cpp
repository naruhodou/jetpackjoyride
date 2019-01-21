#include "main.h"
#include "timer.h"
#include "ball.h"
#include <iostream>
#include "wall.h"
#include "coin.h"
#include "coinzone.h"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball1;
Wall wall1, wall2;
Coin coinarr[1000];
Coin_Zone czarr[10];
int total_coins = 0;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
int sn = 1, standard_coin_length = 10, standard_coin_width = 2, standard_start_px = 5, standard_start_py = 5;
Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( ball1.position.x, 0, 1 );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (ball1.position.x, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ball1.draw(VP);
    wall1.draw(VP);
    wall2.draw(VP);
    for(int i = 0; i < total_coins; i++)
    {
        if(coinarr[i].isdraw)
            coinarr[i].draw(VP);
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_SPACE);
    if(left || right)
    {
        ball1.horizontal_movement(true);
        if (left) {
            // Do something
            ball1.tick(-1);
        }
        else if(right)
        {
            ball1.tick(1);
        }
    }
    else
        ball1.horizontal_movement(false);
    if(up)
    {
        ball1.vertical_movement(true);
        ball1.tick(2);
    }
    else
        ball1.vertical_movement(false);
}

void tick_elements() {
    //function called regularly
    ball1.tick(0);
    for(int i = 0; i < total_coins; i++)
    {
        if(detect_collision(ball1.player, coinarr[i].coin) && coinarr[i].isdraw)
        {
            coinarr[i].isdraw = false;
            ball1.score += 100;
        }
    }
    cout << ball1.score << endl;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball1       = Ball(0, 0, COLOR_RED);
    wall1       = Wall(0, window_size / 2, COLOR_GREEN, 2.0, 200, -100.0, (float)window_size / 2);
    wall2       = Wall(0, 0, COLOR_BLACK, window_size - 0.5, 200, -100.0, -0.5);
    for(int i = 0; i < 10; i++, total_coins += standard_coin_length * standard_coin_width)
    {
        czarr[i] = Coin_Zone(standard_start_px + 8 * i, standard_start_py, standard_coin_length, standard_coin_width);
        for(int j = 0; j < standard_coin_width; j++)
            for(int k = 0; k < standard_coin_length; k++)
                coinarr[total_coins + j * standard_coin_length + k] = Coin(0.25, czarr[i].start_x + k * 0.6, czarr[i].start_y + j * 0.6, COLOR_YELLOW);
    }
    
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers
        // cout << wall1 << endl;
        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + window_size / screen_zoom;
    float bottom = screen_center_y - window_size / screen_zoom;
    float left   = screen_center_x - window_size / screen_zoom;
    float right  = screen_center_x + window_size / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
