/* Belov Danil, 10-6, 05.11.2021 */

#include <stdio.h>
#include <stdlib.h>

#include <glut.h>

#pragma comment(lib, "winmm")

#include "lab3d.h"

#define MAX_ENEMIES 5
#define SPOT_H 20
#define SPOT_W 19
#define INF 999
/* RGBA colors */
#define LAB_WALL 0x7FFF00FF
#define LAB_FLOOR 0x7F7F7FFF
#define LAB_AVA 0xFFFFFFFF
#define LAB_ENEMY 0xFF0000FF
/* Lanirinth size */
#define MAX_W 19
#define MAX_H 20

typedef struct tagVEC2
{
  INT X, Y;
} VEC2;

typedef struct
{
  VEC2 Pos; /* enemy position */
  DOUBLE DeltaTime; /* time from last step */
} ENEMY;

static BYTE Lab[MAX_H][MAX_W];
static INT Solution[MAX_H][MAX_W];
static VEC2 Ava;
static ENEMY Enemies[MAX_ENEMIES];
static INT NumOfEnemies;

/* Set VEC2 struct by components function */
static VEC2 Vec2Set( INT X, INT Y )
{
  VEC2 s;
  s.X = X;
  s.Y = Y;
  return s;
} /* End of 'Vec2Set' function */


/* Load labirinth map from file function */
static BOOL LabLoad( CHAR *FileName )
{
  INT X = 0, Y = 0, c;
  BOOL run = TRUE;
  FILE *F;

  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;
  while (run)
  {
    switch (c = fgetc(F))
    {
    case EOF:
      run = FALSE;
      break;
    case 'a':
      if (Y < MAX_H && X < MAX_W)
        Lab[Y][X] = 2;
      Ava.X = X;
      Ava.Y = Y;
      X++;
      break;
    case 'e':
      if (Y < MAX_H && X < MAX_W)
        if (NumOfEnemies < MAX_ENEMIES)
        {
          Lab[Y][X] = 3;
          Enemies[NumOfEnemies].Pos = Vec2Set(X, Y);
          Enemies[NumOfEnemies].DeltaTime = 0;
          NumOfEnemies++;
        }
      X++;
      break;
    case '\n':
      Y++;
      X = 0;
      break;
    case ' ':
    case '*':
    default:
      if (Y < MAX_H && X < MAX_W)
        Lab[Y][X] = c != '*' ? 0 : 1;
      X++;
      break;
    }
  }
  fclose(F);
  return TRUE;
} /* End of 'LabLoad' function */


/* Initialization of Labirith game function */
VOID LabInit()
{
  int i;

  if (!LabLoad("map.lab"))
    exit(0);

  for (i = 0; i < NumOfEnemies; i++)
    Enemies[i].DeltaTime = 0;
  //mciSendString("open a.mp3 type mpegvideo alias music", NULL, 0, NULL);
  //mciSendString("play music", NULL, 0, NULL);
  /* sndPlaySound("", SND_LOOP | SND_ASYNC); */

} /* End of 'LabInit' function */


/* Control avatar function */
VOID LabKeyboard( INT Key )
{
  switch (Key)
  {
  /* left arrow */
  case 100:
    if (Ava.X > 0 && Lab[Ava.Y][Ava.X - 1] == 0)
    {
      Lab[Ava.Y][Ava.X--] = 0;
      Lab[Ava.Y][Ava.X] = 2;
    }
    break;
  /* down arrow */
  case 101:
    if (Ava.Y < MAX_H && Lab[Ava.Y - 1][Ava.X] == 0)
    {
      Lab[Ava.Y--][Ava.X] = 0;
      Lab[Ava.Y][Ava.X] = 2;
    }
    break;
  /* right arrow */
  case 102:
    if (Ava.X < MAX_W && Lab[Ava.Y][Ava.X + 1] == 0)
    {
      Lab[Ava.Y][Ava.X++] = 0;
      Lab[Ava.Y][Ava.X] = 2;
    }
    break;
  /* up arrow */
  case 103:
    if (Ava.Y > 0 && Lab[Ava.Y + 1][Ava.X] == 0)
    {
      Lab[Ava.Y++][Ava.X] = 0;
      Lab[Ava.Y][Ava.X] = 2;
    }
    break;
  }
} /* End of 'LabKeyboard' function */


/* Get color components */
static VOID GetColorComp( UCHAR *Red, UCHAR *Green, UCHAR *Blue, DWORD Color )
{
  *Red = (Color & 0xFF000000) >> 24;
  *Green = (Color & 0x00FF0000) >> 16;
  *Blue = (Color & 0x0000FF00) >> 8;
} /* End of 'GetColorComp' function */


/* Convert color from RGBA to ARGB format function */
static DWORD ConvertColor( DWORD RGBA )
{
  DWORD ARGB = ((RGBA & 0x000000FF) << 24) + ((RGBA & 0xFF000000) >> 8) +((RGBA & 0x00FF0000) >> 8) + ((RGBA & 0x0000FF00) >> 8);
  return ARGB;
}/* End of 'ConvertColor' function */


/* Draw labirinth cell in 2D and 3D function */
static VOID DrawLabCell( INT X, INT Y, DWORD Color )
{
  INT i, j; 
  DOUBLE s = 0.30;
  UCHAR R, G, B;

  /* Draw 2D labirinth */
  for (i = 1; i < SPOT_H; i++)
    for (j = 1; j < SPOT_W; j++)
    {
      PutPixel(X * SPOT_W + j, Y * SPOT_H + i, ConvertColor(Color));
    }

  /* Draw 3D labirinth */
  glPushMatrix();
  glTranslated(-MAX_W * 0.5 * s, 0, -MAX_H * 0.5 * s);
  glTranslated(X * s, 0, Y * s);

  if (Color == LAB_AVA)
  {
    GetColorComp(&R, &G, &B, LAB_AVA);
    glColor3d(R / 255.0, G / 255.0, B / 255.0);
    glutSolidSphere(s / 2.2, 30, 30);
    GetColorComp(&R, &G, &B, LAB_FLOOR);
    glColor3d(R / 255.0, G / 255.0, B / 255.0);
    glPushMatrix();
    glTranslated(0, -0.13, 0);
    glScaled(1, 0.10, 1);
    glutSolidCube(s / 1.1);
    glPopMatrix();
  }
  else if (Color == LAB_ENEMY)
  {
    GetColorComp(&R, &G, &B, LAB_ENEMY);
    glColor3d(R / 255.0, G / 255.0, B / 255.0);
    glutSolidSphere(s / 2.2, 30, 30);
    GetColorComp(&R, &G, &B, LAB_FLOOR);
    glColor3d(R / 255.0, G / 255.0, B / 255.0);
    glPushMatrix();
    glTranslated(0, -0.13, 0);
    glScaled(1, 0.10, 1);
    glutSolidCube(s / 1.1);
    glPopMatrix();
  }
  else if (Color == LAB_WALL)
  {
    GetColorComp(&R, &G, &B, LAB_WALL);
    glColor3d(R / 255.0, G / 255.0, B / 255.0);;
    glutSolidCube(s / 1.1);
  }
  else if (Color == LAB_FLOOR)
  {
    GetColorComp(&R, &G, &B, LAB_FLOOR);
    glColor3d(R / 255.0, G / 255.0, B / 255.0);
    glPushMatrix();
    glTranslated(0, -0.13, 0);
    glScaled(1, 0.30, 1);
    glutSolidCube(s / 1.1);
    glPopMatrix();
  }
  glPopMatrix();
} /* End of 'DrawLabCell' function */


/* Draw whole labirinth function */
static VOID LabDraw( VOID )
{
  INT X, Y;

  gluLookAt(-3, 6, 5, 0, 0, 0, -3, 10, 4);

  for (Y = 0; Y < MAX_H; Y++)
    for (X = 0; X < MAX_W; X++)
    {
      switch (Lab[Y][X])
      {
      case 0:
        DrawLabCell(X, Y, LAB_FLOOR);
        break;
      case 1:
        DrawLabCell(X, Y, LAB_WALL);
        break;
      case 2:
        DrawLabCell(X, Y, LAB_AVA);
        break;
      case 3:
        DrawLabCell(X, Y, LAB_ENEMY);
        break;
      }
    }
} /* End of 'LabDraw' function */


/* Build solution matrix (BFS algoritm) function */
static VOID BuildSolution( VOID )
{
  INT i, j, dist = 1;
  BOOL changed = TRUE;

  for (i = 0; i < MAX_H; i++)
    for (j = 0; j < MAX_W; j++)
      Solution[i][j] = INF;

  Solution[Ava.Y][Ava.X] = 0;

  while (changed)
  {
    changed = FALSE;
    for (i = 1; i < MAX_H; i++)
      for (j = 1; j < MAX_W; j++)
      {
        if (Solution[i][j] != INF)
        {
          dist = Solution[i][j] + 1;

          if (Lab[i][j - 1] != 1 && Solution[i][j - 1] > dist)
            Solution[i][j - 1] = dist, changed = TRUE;
          
          if (i + 1 < MAX_H && Lab[i + 1][j] != 1 && Solution[i + 1][j] > dist)
            Solution[i + 1][j] = dist, changed = TRUE;
          
          if (j + 1 < MAX_W && Lab[i][j + 1] != 1 && Solution[i][j + 1] > dist)
            Solution[i][j + 1] = dist, changed = TRUE;
          
          if (Lab[i - 1][j] != 1 && Solution[i - 1][j] > dist)
            Solution[i - 1][j] = dist, changed = TRUE;
        }
      }
  }
} /* End of 'BuildSolution' function */

extern BOOL IsPause;
/* Do one step of game function */
VOID LabStep( DOUBLE SyncTime, DOUBLE DeltaTime )
{
  INT i, j, min_dist = -1;
  DOUBLE LatentTime = 0.85;
  VEC2 min_npos = Vec2Set(INF, INF);
  static INT Dx[] = {0, 1, 0, -1}, Dy[] = {-1, 0, 1, 0};

  BuildSolution();
  for (i = 0; i < NumOfEnemies; i++)
  {
    min_dist = INF;
    min_npos = Enemies[i].Pos;
    Enemies[i].DeltaTime += DeltaTime;

    if (Enemies[i].DeltaTime > LatentTime)
    {
      for (j = 0; j < 4; j++)
      {
        VEC2 npos = Vec2Set(Enemies[i].Pos.X + Dx[j], Enemies[i].Pos.Y + Dy[j]);

        if (npos.X > 0 && npos.X < MAX_W && npos.Y > 0 && npos.Y < MAX_H &&
          Solution[npos.Y][npos.X] != INF && (min_dist == INF || Solution[npos.Y][npos.X] < min_dist))
            min_dist = Solution[npos.Y][npos.X], min_npos = npos;
      }

      if (min_dist != INF && Lab[min_npos.Y][min_npos.X] != 3)
      {
        Lab[Enemies[i].Pos.Y][Enemies[i].Pos.X] = 0;
        Enemies[i].Pos = min_npos;
        if (Lab[Enemies[i].Pos.Y][Enemies[i].Pos.X] == 2)
        {
          exit(30);
        }
        Lab[Enemies[i].Pos.Y][Enemies[i].Pos.X] = 3;
        Enemies[i].DeltaTime -= LatentTime;
      }
    }
  }
  LabDraw();
} /* End of 'LabStep' function */
/* END OF 'LAB3D.C' FILE */