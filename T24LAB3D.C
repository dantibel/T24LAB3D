/* Belov Danil, 10-6, 05.11.21 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <glut.h>

#include "lab3d.h"

/* Time variables */
DOUBLE DeltaTime, SyncTime, FPS;
BOOL IsPause;

/* Measure time function */
VOID Timer( VOID )
{
  LONG t;
  static LONG StartTime = -1, OldTime, PauseTime, FrameCount, FPSTime;

  t = clock();
  if (StartTime == -1)
    StartTime = FPSTime = OldTime = t;

  if (IsPause)
  {
    DeltaTime = 0;
    PauseTime += t - OldTime;
  }
  else
  {
    DeltaTime = (t - OldTime) / (DOUBLE)CLOCKS_PER_SEC;
    OldTime = t;
    SyncTime = (t - PauseTime - StartTime) / (DOUBLE)CLOCKS_PER_SEC;
  }

  FrameCount++;

  if (t - FPSTime > 1 * CLOCKS_PER_SEC)
  {
    FPS = FrameCount / ((t - FPSTime) / (double)CLOCKS_PER_SEC);
    FPSTime = t;
    FrameCount = 0;
  }
} /* End of 'Timer' function */


/* Frame variables */
HWND hWnd;
static INT WinW, WinH, FrameW = 1920, FrameH = 1080;;
static CHAR Buf[100];
static DWORD *Frame;
static DOUBLE FrameZoom = 1.0;

/* Set size of frame function */
static VOID FrameSetSize( INT W, INT H )
{
  if (Frame != NULL)
    free(Frame);
  FrameW = W, FrameH = H;
  Frame = malloc(FrameW * FrameH * 4);
  if (Frame == NULL)
    exit(0);
} /* End of 'FrameSetSize' function */


/* Draw pixel function */
VOID PutPixel( INT X, INT Y, DWORD Color )
{
  if (X < 0 || Y < 0 || X >= FrameW || Y >= FrameH)
    return;

  Frame[Y * FrameW + X] = Color;
} /* End of 'PutPixel' function */


/* GLUT Reshape function */
VOID Reshape( INT NewW, INT NewH )
{
  DOUBLE size = 1, wp = size, hp = size;

  if (NewW > NewH)
    wp *= (DOUBLE)NewW / NewH;
  else
    hp *= (DOUBLE)NewH / NewW;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-wp / 2, wp / 2, -hp / 2, hp / 2, size, 1000); /* Set perspection */
  glMatrixMode(GL_MODELVIEW);
  WinW = NewW;
  WinH = NewH;
  FrameSetSize(WinW / FrameZoom, WinH / FrameZoom);
  memset(Frame, 0, (FrameW * FrameH * 4));
  glViewport(0, 0, WinW, WinH);
} /* End of 'Reshape' function */


/* GLUT Display function */
VOID Display( VOID )
{
  if (!IsPause)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Timer();
    glPushMatrix();
    LabStep(SyncTime, DeltaTime);
    glPopMatrix();

    /* Draw raster */
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
  }

  sprintf(Buf, "FPS: %.3lf", FPS);
  SetWindowText(hWnd, Buf);

  glPixelZoom(FrameZoom, -FrameZoom);
  glRasterPos2d(-1, 1);
  glDrawPixels(FrameW, FrameH, GL_BGRA_EXT, GL_UNSIGNED_BYTE, Frame);

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  glFinish();
  glutSwapBuffers();
  glutPostRedisplay();
} /* End of 'Display' function */


/* GLUT Keyboard function */
VOID Keyboard( UCHAR Key, INT X, INT Y )
{
  switch (Key)
  {
  case 27:
    exit(30);
    break;

  case 32:
    IsPause = !IsPause;
    break;

  case '+':
    if (FrameZoom < 4)
    {
      FrameZoom += 0.25;
      FrameSetSize(WinW / FrameZoom, WinH / FrameZoom);
    }
    break;

  case '-':
    if (FrameZoom > 1)
    {
      FrameZoom -= 0.25;
      FrameSetSize(WinW / FrameZoom, WinH / FrameZoom);
    }
    break;
  }
} /* End of 'Keyboard' function */


/* GLUT Special function */
VOID Special( INT Key, INT X, INT Y )
{
  LabKeyboard(Key);
} /* End of 'Special' function */


/* Program main function */
VOID main( INT argc, CHAR *argv[] )
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(FrameW, FrameH);
  glutCreateWindow("T24LAB3D");
  
  FrameSetSize(FrameW, FrameH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glutReshapeFunc(Reshape);
  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);
  glutSpecialFunc(Special);
  glClearColor(0.1, 0.1, 0.1, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);

  LabInit();
  hWnd = FindWindow(NULL, "T24LAB3D");

  glutMainLoop();
} /* End of 'main' function */
/* END OF "T24LAB3D" FILE */