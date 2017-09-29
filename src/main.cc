#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <string.h>

const unsigned int port = 2500;

float camera[2] = {0.0f, 0.0f};
float cameraSpeed = 0.2f;
float camScale = 0.4f;

bool keys[1024] = {false};

struct {
  int socket;
  hostent *host;
  sockaddr_in addr;
} server;

char buffer[256];

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

static float mouseX = 0.0f;
static float mouseY = 0.0f;
static bool mouseDown = false;

// settings
const unsigned int width = 1200;
const unsigned int height = 700;

static void mousemove(GLFWwindow* window, double posx, double posy) {
  mouseX = (float)posx;
  mouseY = (float)posy;
}

static float distance(float x1, float y1, float x2, float y2) {
  return sqrtf( (x2-x1) * (x2-x1) + (y2-y1) * (y2-y1) );
}

static bool circle_circle_collision(float x1, float y1, float r1, float x2, float y2, float r2) {
  return distance(x1, y1, x2, y2) <= r1 + r2;
}

static bool circle_point_collision(float x, float y, float cx, float cy, float cr) {
  return distance(x, y, cx, cy) < cr;
}

// PLAYER STRUCTURE
struct Player {
  float newX;
  float newY;
  float x;
  float y;
  float r;
  float speed;
};

struct Player player1;

unsigned int numPlayers = 0;
struct Player **players;

void register_player(Player *player) {
  numPlayers += 1;
  players = (struct Player**)realloc(players, sizeof(struct Player*) * numPlayers);
  players[numPlayers-1] = player;
}

// MAKE 20 RANDOM CIRCLES
struct Circle {
  int owner;
  float newX;
  float newY;
  float newR;
  float speed;
  float x;
  float y;
  float r;
};

static float offsetX = -1900.0f;
static float offsetY = -1900.0f;

const int num_circles = 200;

struct Circle circles[num_circles];

static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {

  if (action == GLFW_PRESS) {
    keys[key] = true;
  }

  if (action == GLFW_RELEASE) {
    keys[key] = false;
  }

}

static void mousebutton(GLFWwindow* window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    // CHECK CIRCLE COLLISION AND MOVE TO TOP
    /*
       for (int i=0; i<20; i++) {
       struct Circle *circle = &circles[i];
       if (circle_point_collision(mouseX, mouseY, circle->x, circle->y, circle->r)) {
       circle->x = offsetX;
       circle->y = offsetY;
       circle->r = 10.0f;

       offsetX += 30.0f;
       }
       }
       */
    // MOVE PLAYER1 TO CURSOR
    // player1.newX = mouseX;
    // player1.newY = mouseY;
    mouseDown = true;
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    mouseDown = false;
  }
}

static void scroll(GLFWwindow* window, double offsetX, double offsetY) {
  if (offsetY < 0) {
    camScale -= 0.01f;
  }
  if (offsetY > 0) {
    camScale += 0.01f;
  }
}

static void circle(float xPos, float yPos, float r, GLenum type) {
  glBegin(type);
  int sectors = 20;
  for (int i=0; i<=sectors; i++) {
    float angle = 2.0f * M_PI * (float)i / sectors;
    float x = xPos + cosf(angle) * r;
    float y = yPos + sinf(angle) * r;
    glVertex2f(x, y);
  }
  glEnd();
}

int connect_to_server () {
  server.socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server.socket < 0) {
    fprintf(stderr, "FAILED TO CREATE SOCKET\n");
    return -1;
  }
  server.host = gethostbyname("localhost");
  if (server.host == NULL) {
    fprintf(stderr, "ERROR: NO SUCH HOST\n");
    return -1;
  }
  bzero((char*)&server.addr, sizeof(server.addr));
  server.addr.sin_family = AF_INET;
  bcopy((char*)server.host->h_addr, (char*)&server.addr.sin_addr.s_addr, server.host->h_length);
  server.addr.sin_port = htons(port);
  printf("CONNECTING TO SERVER\n");
  if (connect(server.socket, (struct sockaddr *)&server.addr, sizeof(server.addr)) < 0) {
    fprintf(stderr, "FAILED TO CONNECT\n");
    return -1;
  }
  printf("CONNECTED TO SERVER\n");
  bzero(buffer, 256);

  printf("SENDING DATA TO SERVER\n");
  sprintf(buffer, "A MESSAGE");
  if (send(server.socket, buffer, strlen(buffer), 0) < 0) {
    fprintf(stderr, "FAILED TO WRITE TO SOCKET\n");
    return -1;
  }
  bzero(buffer, 256);
  printf("WAITING FOR DATA FROM SERVER\n");
  if (recv(server.socket, buffer, strlen(buffer), 0) < 0) {
    fprintf(stderr, "FAILED TO READ FROM SOCKET\n");
    return -1;
  }
  return 0;
}

int main()
{

  // connect_to_server();

  srand(time(NULL));
  std::cout << "siim ei ole jumal" << std::endl;
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mousemove);
  glfwSetMouseButtonCallback(window, mousebutton);
  glfwSetKeyCallback(window, keyboard);
  glfwSetScrollCallback(window, scroll);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }    

  printf("OpenGL Version: %s.\n", glGetString(GL_VERSION));
  printf("OpenGL GLSL Version: %s.\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  printf("OpenGL Vendor: %s.\n", glGetString(GL_VENDOR));

  // INITIALIZATION OF CIRCLES
  for (int i=0; i<num_circles; i++) {
    struct Circle *circle = &circles[i];
    circle->owner = -1;

    float minX = -2000;
    float maxX = +2000;

    float minY = -2000;
    float maxY = +2000;

    circle->speed = 0.1f;
    circle->r = 30;
    circle->newR = circle->r;

    circle->x = (rand() % (int)((maxX*2)-60)) + (minX+60);
    circle->y = (rand() % (int)((maxY*2)-60)) + (minY+60);

    circle->newX = circle->x;
    circle->newY = circle->y;

    for (int j=0; j<i; j++) {
      if (circle_circle_collision(circle->x, circle->y, circle->r, circles[j].x, circles[j].y, circles[j].r)) {
        i -= 1;
        continue;
      }
    }
  }

  player1.x = 50.0f;
  player1.y = 50.0f;

  player1.speed = 0.6f;

  player1.newX = player1.x;
  player1.newY = player1.y;

  player1.r = 45.0f;
  register_player(&player1);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {

    if (mouseX < 10.0f) {
      camera[0] += cameraSpeed;
    }

    if (mouseX > width - 10.0f) {
      camera[0] -= cameraSpeed;
    }

    if (mouseY < 10.0f) {
      camera[1] += cameraSpeed;
    }

    if (mouseY > height - 10.0f) {
      camera[1] -= cameraSpeed;
    }

    if (keys[GLFW_KEY_W]) {
      camera[1] += cameraSpeed;
    }
    if (keys[GLFW_KEY_A]) {
      camera[0] += cameraSpeed;
    }
    if (keys[GLFW_KEY_S]) {
      camera[1] -= cameraSpeed;
    }
    if (keys[GLFW_KEY_D]) {
      camera[0] -= cameraSpeed;
    }

    // MOVE PLAYER1 TO CURSOR
    if (mouseDown) {
      player1.newX = (mouseX - camera[0]) / camScale;
      player1.newY = (mouseY - camera[1]) / camScale;
    }
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0f, 0.3f, 0.3f);

    static float point=10;
    static float angle=0;


    glPointSize(point);

    //point+=1;

    //glBegin(GL_POINTS);
    //glVertex2f(0.0f, 0.0f);
    //glEnd();

    //glBegin(GL_TRIANGLES);
    //glVertex2f(-0.5f, -0.5f);
    //glVertex2f(+0.5f, -0.5f);
    //glVertex2f(+0.0f, +0.5f);
    //glEnd();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, (float)width, float(height), 0.0f, 0.0f, 1.0f);
    glTranslatef(camera[0], camera[1], 0.0f);
    glScalef(camScale, camScale, camScale);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
    glBegin(GL_LINES);
    for (int i=-2000; i<2000; i += 100) {
      glVertex2f(-2000.0f, i);
      glVertex2f(+2000.0f, i);

      glVertex2f(i, -2000.0f);
      glVertex2f(i, +2000.0f);
    }
    glEnd();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);

    //glBegin(GL_TRIANGLES);
    //glVertex2f(0.0f, 0.0f);
    //glVertex2f(200.0f, 0.0f);
    //glVertex2f(100.0f, 200.0f);
    //glEnd();

    glBegin(GL_POINTS);
    static float hyp=100;
    static float x,y;
    static float cx=300,cy=300;
    static float pos[2] = {300, 300};
    // pos[0]
    // pos[1]


    x=cx+hyp*sin(angle);
    y=cy+hyp*cos(angle);

    glVertex2f(x, y);
    angle+=.001;

    glEnd();

    glBegin(GL_LINE_LOOP);
    float radius = 100.0f;
    int sectors = 20;
    for (int i=0; i<=sectors; i++) {
      float angle = 2.0f * M_PI * (float)i / sectors;
      float x = pos[0] + cosf(angle) * radius;
      float y = pos[1] + sinf(angle) * radius;
      glVertex2f(x, y);
    }
    glEnd();

    circle((mouseX - camera[0]) / camScale, (mouseY - camera[1]) / camScale, 20.0f, GL_LINE_LOOP);

    glColor3f(1.0f, 1.0f, 0.3f);
    glBegin(GL_LINES);
    // NORTH
    glVertex2f(-2000.0f, +2000.0f);
    glVertex2f(+2000.0f, +2000.0f);

    // WEST
    glVertex2f(-2000.0f, +2000.0f);
    glVertex2f(-2000.0f, -2000.0f);

    // SOUTH
    glVertex2f(-2000.0f, -2000.0f);
    glVertex2f(+2000.0f, -2000.0f);

    // EAST
    glVertex2f(+2000.0f, -2000.0f);
    glVertex2f(+2000.0f, +2000.0f);

    //glVertex2f(-2000.0f, -2000.0f);
    //glVertex2f(+2000.0f, -2000.0f);

    glEnd();

    for (int i=0; i<num_circles; i++) {
      if (circle_point_collision((mouseX-camera[0]) / camScale, (mouseY - camera[1]) / camScale, circles[i].x, circles[i].y, circles[i].r)) {
        glColor3f(0.3f, 1.0f, 0.3f);
      } else if (circles[i].owner == -1) {
        glColor3f(1.0f, 1.0f, 0.3f);
      } else {
        glColor3f(0.3f, 1.0f, 1.0f);
      }
      circle(circles[i].x, circles[i].y, circles[i].r, GL_LINE_LOOP);
      if (circles[i].x < circles[i].newX) {
        circles[i].x += circles[i].speed;
      }

      if (circles[i].y < circles[i].newY) {
        circles[i].y += circles[i].speed;
      }

      if (circles[i].x > circles[i].newX) {
        circles[i].x -= circles[i].speed;
      }

      if (circles[i].y > circles[i].newY) {
        circles[i].y -= circles[i].speed;
      }

      if (circles[i].r < circles[i].newR) {
        circles[i].r += circles[i].speed;
      }

      if (circles[i].r > circles[i].newR) {
        circles[i].r -= circles[i].speed;
      }
    }

    for (int i=0; i<numPlayers; i++) {
      Player *player = players[i];
      glColor3f(1.0f, 0.3f, 0.3f);
      circle(player->x, player->y, player->r, GL_TRIANGLE_FAN);
      glBegin(GL_LINES);
      glVertex2f(player->x, player->y);
      glVertex2f(player->newX, player->newY);

      if (player->x < player->newX) {
        player->x += player->speed;
      }

      if (player->y < player->newY) {
        player->y += player->speed;
      }

      if (player->x > player->newX) {
        player->x -= player->speed;
      }

      if (player->y > player->newY) {
        player->y -= player->speed;
      }

      for (int j=0; j<num_circles; j++) {
        Circle *circle = &circles[j];
        if (circle->owner == -1 && circle_circle_collision(circle->x, circle->y, circle->r, player->x, player->y, player->r)) {
          circle->owner = i;

          circle->newX = offsetX;
          circle->newY = offsetY;

          offsetX += 30.0f;
          circle->newR = 5.0f;
        }
      }

      glEnd();
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}
