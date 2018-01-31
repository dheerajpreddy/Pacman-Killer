#include "main.h"
#include "timer.h"
#include "ball.h"
#include "rectangle.h"
#include "semic.h"
#include "stdlib.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

Ball player, balls[150];
Rectangle ground, grass, slopes[100];
Semic pool;
unsigned long long score;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

Timer t60(1.0 / 60);

float RandomNumber(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
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
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    int i;
    for(i=0; i<15; i++) {
      balls[i].draw(VP);
    }
    ground.draw(VP);
    grass.draw(VP);
    pool.draw(VP);
    player.draw(VP);
}

void tick_input(GLFWwindow *window) {
  int up = glfwGetKey(window, GLFW_KEY_UP);
  int left  = glfwGetKey(window, GLFW_KEY_LEFT);
  int right = glfwGetKey(window, GLFW_KEY_RIGHT);
  int down = glfwGetKey(window, GLFW_KEY_DOWN);
  if (up && player.position.y==-1) {
    player.speed.y-=0.13;
  }
  if (left && player.position.x>=-3.65) {
    player.position.x-=0.1;
  }
  if (right && player.position.x<=3.65) {
    player.position.x+=0.05;
  }
}

void tick_elements() {
    player.tick();
    char title[100];
    int i;
    for(i=0; i<15; i++) {
      balls[i].tick();
      if(balls[i].position.x>4.3) {
        balls[i].set_position(-4.5, RandomNumber(-0.3, 2.5));
      }
      if(player.speed.y>0 && detect_collision(player.bounding_box(), balls[i].bounding_box())) {
        balls[i].set_position(-4.5, RandomNumber(-0.3, 2.5));
        player.speed.y -= 0.13;
        score++;
        sprintf(title, "SCORE: %llu", score);
        glfwSetWindowTitle(window, title);
      }
    }
    if(player.position.y<-1) {
      player.position.y = -1;
      player.speed.y = 0;
    }
    if(player.speed.y!=0) {
      player.speed.y += 0.005;
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    int i;
    for(i=0; i<15; i++) {
      balls[i] = Ball(-10, -10, 0.2, COLOR_YELLOW);
      balls[i].set_position(RandomNumber(-15.0, -4.0), RandomNumber(-0.7, 2.5));
      balls[i].speed.x = -0.01;
      balls[i].speed.y = 0;
    }
    player = Ball(0, -1, 0.25, COLOR_PURPLE);
    player.speed.x = player.speed.y = 0;
    grass = Rectangle(0, -1.5, 10, 0.5, COLOR_GREEN);
    ground = Rectangle(0, -2.5, 10, 2.5, COLOR_BROWN);
    pool = Semic(2, -1.85, 0.75, COLOR_BLUE);
    score = 0;
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BLACK.r / 256.0, COLOR_BLACK.g / 256.0, COLOR_BLACK.b / 256.0, 0.0f); // R, G, B, A
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
    int width  = 900;
    int height = 900;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

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
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
