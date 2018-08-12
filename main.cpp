#include <GL/glut.h>
#include <ctime>
#include <unistd.h>
#include <cmath>
#include <iostream>
/* ----------------------------------------------------------------------------------------------------------------- */
// resize function
/* ----------------------------------------------------------------------------------------------------------------- */
void onResize(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, 0, h, -1, 1);
}
/* ----------------------------------------------------------------------------------------------------------------- */
int cnt;
/* ----------------------------------------------------------------------------------------------------------------- */
// colors, which are used to draw
/* ----------------------------------------------------------------------------------------------------------------- */
GLfloat r;      // red
GLfloat g;      // green
GLfloat b;      // blue
/* ----------------------------------------------------------------------------------------------------------------- */
// initialize global variables
/* ----------------------------------------------------------------------------------------------------------------- */
void initValues()
{
  r = double(rand() % 100) / 100;
  g = double(rand() % 100) / 100;
  b = double(rand() % 100) / 100;
}
/* ----------------------------------------------------------------------------------------------------------------- */
// continuous transition of colors
/* ----------------------------------------------------------------------------------------------------------------- */
void transition()
{
  double offset = double(rand() % 5) / 100;
  short sign = rand() % 2;

  if(cnt != 0)
    cnt = rand() % 50;      // 49 different shades max

  if(r >= 1.0 || r < 0)     // tady by chtelo asi uvazovat nejakou konstantu kvuli neukoncenemu desetinnemu rozvoji
    r = double(rand() % 100) / 100;

  if(g >= 1.0 || g < 0)     // tady by chtelo asi uvazovat nejakou konstantu kvuli neukoncenemu desetinnemu rozvoji
    g = double(rand() % 100) / 100;

  if(b >= 1.0 || b < 0)     // tady by chtelo asi uvazovat nejakou konstantu kvuli neukoncenemu desetinnemu rozvoji
    b = double(rand() % 100) / 100;

  cnt--;
}
/* ----------------------------------------------------------------------------------------------------------------- */
// resets global variables
/* ----------------------------------------------------------------------------------------------------------------- */
void updateValues()
{
  short r_sign = rand() % 2; // 0 nebo 1
  short g_sign = rand() % 2; // 0 nebo 1
  short b_sign = rand() % 2; // 0 nebo 1

  if(r_sign == 0)
    r += 0.01;
  else
    r -= 0.01;

  if(g_sign == 0)
    g += 0.01;
  else
    g -= 0.01;

  if(b_sign == 0)
    b += 0.01;
  else
    b -= 0.01;

  if(r >= 1.0 || r < 0)     // tady by chtelo asi uvazovat nejakou konstantu kvuli neukoncenemu desetinnemu rozvoji
    r = double(rand() % 100) / 100;

  if(g >= 1.0 || g < 0)     // tady by chtelo asi uvazovat nejakou konstantu kvuli neukoncenemu desetinnemu rozvoji
    g = double(rand() % 100) / 100;

  if(b >= 1.0 || b < 0)     // tady by chtelo asi uvazovat nejakou konstantu kvuli neukoncenemu desetinnemu rozvoji
    b = double(rand() % 100) / 100;
}
/* ----------------------------------------------------------------------------------------------------------------- */
// display a square
// from point [x,y] with length size
/* ----------------------------------------------------------------------------------------------------------------- */
void square(int x, int y, int size)
{
  glVertex2i(x, y);
  glVertex2i(x + size, y);
  glVertex2i(x + size, y + size);
  glVertex2i(x, y + size);
}
/* ----------------------------------------------------------------------------------------------------------------- */
// display a column of squares
// from point [x,y]
// number of squares - count
// spaces between squares - delim_size
// size of squares - block_size
// 8 bit int used (containing 0 or 1) for masking to conditionally display - mask
/* ----------------------------------------------------------------------------------------------------------------- */
void column(int x, int y, int count, int delim_size, 
            int block_size, uint8_t mask)
{
  for(int i = 0; i < count; i++) {
    if(mask & (1 << i))
      square(x, y + i * delim_size + i * block_size, block_size);
  }
}
/* -------------------------------------------------------------------------------------------------------- */
// convert units
/* -------------------------------------------------------------------------------------------------------- */
void preved(uint8_t & tens, uint8_t & units,
            int time_units)
{
  tens = (time_units / 10);
  units = (time_units % 10);
}
/* -------------------------------------------------------------------------------------------------------- */
// display function
/* -------------------------------------------------------------------------------------------------------- */
void disp_clock()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  int size = std::min(glutGet(GLUT_WINDOW_HEIGHT), glutGet(GLUT_WINDOW_WIDTH));
  int delim_size = size / 70;           // magicka konstanta
  //int block_size = size / 4.5;          // magicka konstanta
  int block_size = size / 7;          // magicka konstanta    - je treba 6 sloupcu + oddelovace
  //int block_size = size / 6.0;          // magicka konstanta    - je treba 6 sloupcu + oddelovace

  glColor3f(r, g, b);                // rand, postupne zmeny barev
  updateValues();
  //transition();

  glBegin(GL_QUADS);                      // ted se zacnou vykreslovat trojuhelniky
  
  time_t rawtime;
  struct tm * timeinfo;  
  
  uint8_t desitky_sec, sec, desitky_min, min, desitky_hod, hod;

  /* begin loop */

  time (&rawtime);
  timeinfo = localtime (&rawtime);
  desitky_sec = sec = desitky_min = min = desitky_hod = hod = 0;    // vycisteni hodnot
    
  preved(desitky_sec, sec, timeinfo->tm_sec);
  preved(desitky_min, min, timeinfo->tm_min);
  preved(desitky_hod, hod, timeinfo->tm_hour);

  column(delim_size, delim_size, 2, delim_size, block_size, desitky_hod);     // x, y, pocet bloku, vzdalenost bloku, velikost bloku, pole, delka pole
  column(block_size + delim_size * 2, delim_size, 4, delim_size, block_size, hod);
  column(2 * block_size + delim_size * 4 , delim_size, 3, delim_size, block_size, desitky_min);
  column(3 * block_size + delim_size * 5, delim_size, 4, delim_size, block_size, min);
  column(4 * block_size + delim_size * 7, delim_size, 3, delim_size, block_size, desitky_sec);
  column(5 * block_size + delim_size * 8,delim_size, 4, delim_size, block_size, sec);
  
  usleep(120000);

  glEnd();
  glutSwapBuffers();
  glutPostRedisplay();
}
/* ----------------------------------------------------------------------------------------------------------------- */
// close the program on escape key
/* ----------------------------------------------------------------------------------------------------------------- */
void onKeyboard(unsigned char key, int x, int y)
{
  if (key == 27) // escape key
      exit(0);
}
/* ----------------------------------------------------------------------------------------------------------------- */
// main
/* ----------------------------------------------------------------------------------------------------------------- */
int main(int argc, char * argv[])
{
  srand(unsigned(time(NULL)));

  initValues();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
  glutCreateWindow("bcd clock");
  glutReshapeWindow(800, 600);
  glutPositionWindow(0, 0);
  glutDisplayFunc(disp_clock);
  glutReshapeFunc(onResize);
  glutKeyboardFunc(onKeyboard);
  glutMainLoop();
  return 0;
}
/* ----------------------------------------------------------------------------------------------------------------- */

