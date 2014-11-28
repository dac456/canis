#ifndef __CANISVIEWER_H
#define __CANiSVIEWER_H

#include <Canis.h>

void initialize(int argc, char* argv[]);
void initWindow(int argc, char* argv[]);
void resize(int width, int height);
void render(void);
void idle(void);

void mouse(int x, int y);
void keyboard(unsigned char key, int x, int y);

#endif
