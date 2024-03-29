#include "main.h"
#include "timer.h"
#include "ball.h"
#include <iostream>
#include "wall.h"
#include "coin.h"
#include "coinzone.h"
#include "enemy1.h"
#include "ring.h"
#include <string>
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;


/*********************
 * Objects in the game
 ********************/
Ball ball1;

Wall wall1, wall2;

Enemy1 e1arr[100], e2, e3, special_enemy;
float e3x, e3y;

Coin coinarr[10000], special_coin;
Coin jet_propulsion[4];
Coin_Zone czarr[100];
int let_it_come = 0, random1 = 1;
vector <Coin> wballoon;
vector <Enemy1> display_score;
Ring r1;

seven_segment score_map[10];
int total_coins = 0, beg = 0, inc = 0;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
int sn = 1, standard_coin_length = 4, standard_coin_width = 4, standard_start_px = 10, standard_start_py = 5;
Timer t60(1.0 / 60);
int timestamp = 0, etime_stamp = 1, utime_stamp = 0;

/**************************
* Customizable functions *
**************************/

//score display logic
string get_score(int score)
{
    if(score == 0)
        return "0";
    string ans;
    while(score)
    {
        ans = (char)(score % 10 + '0') + ans;
        score /= 10;
    }
    return ans;
}

void print_score(string s, glm::mat4 VP)
{
    float start_x = ball1.position.x - 4, start_y = window_size - 0.2, length = 0.4, delta = 0.1;
    for(int i = 0; i < s.size(); i++)
    {
        int num = s[i] - '0';
        float x = start_x + i;
        if(score_map[num].coding[0])
        {
            display_score.push_back(Enemy1(x + length / 2, start_y, length, delta, false, 0, COLOR_BLACK));
        }
        if(score_map[num].coding[1])
        {
            display_score.push_back(Enemy1(x + length, start_y - length / 2, length, delta, false, 90, COLOR_BLACK));
        }
        if(score_map[num].coding[2])
        {
            display_score.push_back(Enemy1(x + length, start_y - 3 * length / 2, length, delta, false, 90, COLOR_BLACK));
        }
        if(score_map[num].coding[3])
        {
            display_score.push_back(Enemy1(x + length / 2.0, start_y - 2 * length, length, delta, false, 0, COLOR_BLACK));
        }
        if(score_map[num].coding[4])
        {
            display_score.push_back(Enemy1(x, start_y - 3 * length / 2, length, delta, false, 90, COLOR_BLACK));
        }
        if(score_map[num].coding[5])
        {
            display_score.push_back(Enemy1(x, start_y - length / 2, length, delta, false, 90, COLOR_BLACK));
        }
        if(score_map[num].coding[6])
        {
            display_score.push_back(Enemy1(x + length / 2, start_y - length, length, delta, false, 0, COLOR_BLACK));
        }
    }
    for(int i = 0; i < display_score.size(); i++)
        display_score[i].draw(VP);
    display_score.clear();
}


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
    for(int i = 0; i < 100; i++)
    {
        e1arr[i].draw(VP);
    }
    for(int i = 0; i < total_coins; i++)
    {
        if(coinarr[i].isdraw)
            coinarr[i].draw(VP);
    }
    if(special_coin.isdraw)
        special_coin.draw(VP);
    for(int i = 0; i < 4; i++)
        if(jet_propulsion[i].isdraw)
            jet_propulsion[i].draw(VP);
    if(beg)
        e2.draw(VP);
    if(e3.ismove)
        e3.draw(VP);
    if(special_enemy.ismove)
        special_enemy.draw(VP);
    print_score(get_score(ball1.score), VP);
    for(int i = 0; i < wballoon.size(); i++)
        wballoon[i].draw(VP);
    r1.draw(VP);
}

/****************************
 * Random Flying objects 
 * *************************/
void handle_special_coins(int trigger)
{
    if(trigger == 0)
    {
        special_coin.position.x = 10 + ball1.position.x;
        special_coin.position.y = window_size;
        special_coin.isdraw = true;
    }
    if(!special_coin.isdraw)
        return;
    // cout << special_coin.position.x << " " << special_coin.position.y << " " << ball1.position.x << " " << ball1.position.y << endl;
    if((special_coin.position.x >= ball1.position.x - 0.5 && special_coin.position.x <= ball1.position.x + 0.5) && (special_coin.position.y >= ball1.position.y - 0.5 && special_coin.position.y <= ball1.position.y + 0.5))
    {
        special_coin.isdraw = false;
        ball1.score += 1000;
        return;
    }
    special_coin.position.x -= (double)0.05;
    special_coin.position.y -= (double)trigger / (double)360;
    if(special_coin.position.y < 0.05)
        special_coin.isdraw = false;
}

void handle_special_enemy(int trigger)
{
    if(trigger == 150)
    {
        special_enemy.position.x = ball1.position.x - window_size;
        special_enemy.position.y = window_size;
        special_enemy.ismove = true;
    }
    if(!special_enemy.ismove)
        return;
    // cout << special_enemy.position.x << " " << special_enemy.position.y << " " << ball1.position.x << " " << ball1.position.y << endl;
    if(detect_collision(special_enemy.checker, ball1.player))
    {
        special_enemy.ismove = false;
        ball1.speed *= 1.5;
        return;
    }
    special_enemy.position.x += (double)0.05;
    special_enemy.position.y -= (double)(trigger - 150) / (double)360;
    if(special_enemy.position.y < 0.05)
        special_enemy.ismove = false;
}

double dis(double x1, double y1, double x2, double y2)
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

//key input
int leftk, rightk, up, bulletfire;
void tick_input(GLFWwindow *window) {
    leftk  = glfwGetKey(window, GLFW_KEY_LEFT);
    rightk = glfwGetKey(window, GLFW_KEY_RIGHT);
    up = glfwGetKey(window, GLFW_KEY_SPACE);
    bulletfire = glfwGetKey(window, GLFW_KEY_F);
    if(abs(dis(ball1.position.x, ball1.position.y, r1.position.x, r1.position.y) -r1.radius) < EPS && ball1.position.y <= r1.position.y)
    {
        
        if(abs(ball1.ay) <= 0.1)
        {
            if(leftk)
            {
                // cout << "left\n";
                ball1.tick(4);
            }
            else if(rightk)
            {
                // cout << "right\n";                
                ball1.tick(5);
            }
        }
        else
        {
            ball1.circle = true;
            ball1.ctheta = atan((ball1.position.y - r1.position.y) / (ball1.position.x - r1.position.x));
            // cout << ball1.ctheta << endl;
            ball1.cr = r1.radius;
            ball1.ay = 0;
        }
    }
    else
    {
        ball1.ay = -10;
        if(leftk || rightk)
        {
            ball1.horizontal_movement(true);
            if (leftk) {
                // Do something
                ball1.tick(-1);
            }
            else if(rightk)
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
        {
            ball1.tick(0);
            ball1.vertical_movement(false);
        }
    }
    if(bulletfire) 
    {
        let_it_come++;
        let_it_come %= 5;
        if(!let_it_come)
            wballoon.push_back(Coin(0.25, ball1.position.x + 0.7, ball1.position.y, COLOR_BLACK));
    }
}



//reset all
void reset_game()
{
    ball1.score = 0;
    ball1.speed = 0.05;
    ball1.position.x = ball1.position.y = 0;
    for(int i = 0; i < total_coins; i++)
        coinarr[i].isdraw = true;
    beg = 0; inc = 0;
    etime_stamp = 1;
}



//collision with enemy 1
void collision_enemy1_checker()
{
    double x1 = 0.50000 + ball1.position.x, x2 = ball1.position.x - 0.50000;
    double y1 = 0.50000 + ball1.position.y, y2 = ball1.position.y - 0.50000;
    for(int i = 0; i < 100; i++)
    {
        double angle = M_PI * e1arr[i].rotation / 180;
        double cx1 = e1arr[i].position.x - e1arr[i].length / 2 * (double)cos(angle);
        double cx2 = e1arr[i].position.x + e1arr[i].length / 2 * (double)cos(angle);
        double cy1 = e1arr[i].position.y - e1arr[i].length / 2 * (double)sin(angle);
        double cy2 = e1arr[i].position.y + e1arr[i].length / 2 * (double)sin(angle);
        if((x1 >= cx1 && x1 <= cx2 && abs((y2 - cy1) - (x1 - cx1) * (double)tan(angle)) < EPS) || (x2 >= cx1 && x2 <= cx2 && abs((y1 - cy1) - (x2 - cx1) * (double)tan(angle)) < EPS))
        {
            reset_game();
            return;
        }
    }
    if(beg && e2.position.y >= ball1.position.y - 0.5 && e2.position.y <= ball1.position.y + 0.5)
    {
        reset_game();
        return;
    }
}

void handle_wballoon()
{
    vector <Coin> temp;
    for(int i = 0; i < wballoon.size(); i++)
    {
        wballoon[i].position.y -= 0.05;
        if(wballoon[i].position.y < EPS || (abs(e2.position.y - wballoon[i].position.y) < EPS && beg))
        {
            if(abs(e2.position.y - wballoon[i].position.y) < EPS)
            {
                beg = 0;
                etime_stamp = 1;
            }
            continue;
        }
        temp.push_back(wballoon[i]);
    }
    wballoon.clear();
    for(int i = 0; i < temp.size(); i++)
        wballoon.push_back(temp[i]);
}


//enemy3 motion
void enemy3_motion(float angle, float a, float b, float x, float y)
{
    e3.position.x = x + a * cos(angle);
    e3.position.y = y + b * sin(angle);
}


//that function called everytime
void tick_elements() {
    //function called regularly


    //universal time stamp
    utime_stamp++;
    utime_stamp %= 600;
    
    //general motion

    //detection collision with coins
    for(int i = 0; i < total_coins; i++)
    {
        if(detect_collision(ball1.player, coinarr[i].coin) && coinarr[i].isdraw)
        {
            coinarr[i].isdraw = false;
            ball1.score += 100;
        }
    }

    //jet propulsion logic
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            jet_propulsion[i * 2 + j].position.x = (ball1.position.x - 0.25) + j * 0.5;
            jet_propulsion[i * 2 + j].position.y = (ball1.position.y - 1) - 0.5 * i;
            jet_propulsion[i * 2 + j].isdraw = (timestamp % 30 >= 15);
            if(jet_propulsion[i * 2 + j].position.y <= 0.0f)
                jet_propulsion[i * 2 + j].isdraw = false;
        }
    }

    //enemy 1 collision
    timestamp += 7;
    timestamp %= 30;
    collision_enemy1_checker();


    //enemy 2 collision
    if(etime_stamp == 0)
    {
        inc = 1;
        e2.position.x = ball1.position.x;
        e2.position.y = 1 + ((int)ball1.position.y * 9 + ball1.score * 37) % 9;
        beg += inc;
        beg %= 1200;
    }
    if(beg == 0)
    {
        inc = 0;
    }
    else
    {
        e2.position.x = ball1.position.x;
        e2.position.y += (double)0.01;
    }
    etime_stamp++;
    etime_stamp %= 600;


    // balloon
    handle_wballoon();

    //enemy 3
    if(utime_stamp >= 100)
    {
        e3.ismove = true;
        float a = 6, b = 2, angle = 45;
        if(utime_stamp == 100)
        {
            e3x = ball1.position.x;
            e3y = ball1.position.y;
            e3.position = glm :: vec3(ball1.position.x, ball1.position.y, 0);
        }
        angle += utime_stamp - 100;
        if(angle <= 360 && e3.position.y > 0.05)
        {
            enemy3_motion(angle * M_PI / 180, a, b, e3x, e3y);
        }
        else
            e3.ismove = false;
    }
    else
        e3.ismove = false;
    if(detect_collision(e3.checker, ball1.player) && e3.ismove)
        reset_game();
    
    //special coins
    handle_special_coins(random1);
    handle_special_enemy(random1);
    random1++;
    random1 %= 300;


    if(ball1.position.x >= r1.position.x - r1.radius && ball1.position.x <= r1.position.x + r1.radius && ball1.position.y >= r1.position.y - r1.radius && !ball1.circle)
    {
        ball1.ay = -10 - ((double)100) / (ball1.position.y - r1.position.y + r1.radius) / (ball1.position.y - r1.position.y + r1.radius); 
    }
    else if(!ball1.circle)
    {
        ball1.ay = -10;
    }
    // cout << dis(ball1.position.x, ball1.position.y, r1.position.x, r1.position.y) - r1.radius << endl;
    // cout << ball1.score << endl;
}




//Random Angle generator
float random_angle(int i)
{
    return (2 * i * i * i + 7 * i * i + 31 * i + 79) % 83;
}




/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball1       = Ball(0, 0, COLOR_RED);
    wall1       = Wall(0, window_size / 2, COLOR_GREEN, 2.0, 2000, -1000.0, (float)window_size / 2);
    wall2       = Wall(0, 0, COLOR_BLACK, window_size - 0.5, 2000, -1000.0, -0.5);
    
    //enemy 1
    for(int i = 0; i < 100; i++)
    {
        e1arr[i] = Enemy1(4 + 16 * i, 5.0, 4.0, 0.3, false, random_angle(i), COLOR_GREEN);
    }
    //enemy 2
    e2          = Enemy1(ball1.position.x, ball1.position.y, 2 * window_size - 2, 0.3, true, 0, COLOR_ORANGE);
    
    //enemy 3
    e3          = Enemy1(-1, 3, 0.8, 0.8,false, 0, COLOR_BLACK);
    special_enemy = Enemy1(-20, 0, 0.8, 0.8, false, 0, COLOR_PURPLE);
    //coins initialization
    for(int i = 0; i < 20; i++, total_coins += standard_coin_length * standard_coin_width)
    {
        czarr[i] = Coin_Zone(standard_start_px + 16 * i, standard_start_py, standard_coin_length, standard_coin_width);
        for(int j = 0; j < standard_coin_width; j++)
            for(int k = 0; k < standard_coin_length; k++)
                coinarr[total_coins + j * standard_coin_length + k] = Coin(0.25, czarr[i].start_x + k * 0.6, czarr[i].start_y + j * 0.6, COLOR_YELLOW);
    }
    special_coin = Coin(0.5, -2 * window_size, 0, COLOR_PURPLE);
    special_coin.isdraw = false;

    //jet propulsion
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
            jet_propulsion[i * 2 + j] = Coin(0.1, (ball1.position.x - 0.3) + i * 0.2, (ball1.position.y - 1) + 0.2 * j, COLOR_ORANGE);
    }

    r1 = Ring(2, 94, 7, COLOR_BLACK);

    //seven segment logic
    score_map[0].coding = {1, 1, 1, 1, 1, 1, 0};
    score_map[1].coding = {0, 1, 1, 0, 0, 0, 0};
    score_map[2].coding = {1, 1, 0, 1, 1, 0, 1};
    score_map[3].coding = {1, 1, 1, 1, 0, 0, 1};
    score_map[4].coding = {0, 1, 1, 0, 0, 1, 1};
    score_map[5].coding = {1, 0, 1, 1, 0, 1, 1};
    score_map[6].coding = {1, 0, 1, 1, 1, 1, 1};
    score_map[7].coding = {1, 1, 1, 0, 0, 0, 0};
    score_map[8].coding = {1, 1, 1, 1, 1, 1, 1};    
    score_map[9].coding = {1, 1, 1, 1, 0, 1, 1};

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
