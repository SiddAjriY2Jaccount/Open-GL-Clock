#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// define glu objects
int about_int=0;

GLUquadricObj *Cylinder;
GLUquadricObj *Disk;

struct tm *newtime;
time_t ltime;
int M_TWOPI=0;
int mode = -1;
GLfloat rx, ry, rz, angle;

// lighting
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightPosition[]= { 5.0f, 25.0f, 15.0f, 1.0f };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

static int light_state = 0; // light on = 1, light off = 0
static int view_state = 0; // Ortho view = 1, Perspective = 0

float randomNum()
{
return ((float)rand())/RAND_MAX;
}

void Sprint( float x, float y, char *st)
{
	int l,i;

	l=strlen( st );
	glRasterPos3f( x, y, -1);
	for( i=0; i < l; i++)
		{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]);
	}

}

static void TimeEvent(int te)
{

	rx = 30 * cos( angle );
	ry = 30 * sin( angle );
	rz = 30 * cos( angle );
	angle += 0.01;
	if (angle > M_TWOPI) angle = 0;

	glutPostRedisplay();
	glutTimerFunc( 100, TimeEvent, 1);
}

void init(void)
{


   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);
   // Lighting is added to scene
   glLightfv(GL_LIGHT1 ,GL_AMBIENT, LightAmbient);
   glLightfv(GL_LIGHT1 ,GL_DIFFUSE, LightDiffuse);
   glLightfv(GL_LIGHT1 ,GL_POSITION, LightPosition);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT1);


   Cylinder = gluNewQuadric();
   gluQuadricDrawStyle( Cylinder, GLU_FILL);
   gluQuadricNormals( Cylinder, GLU_SMOOTH);
   gluQuadricOrientation( Cylinder, GLU_OUTSIDE);
   gluQuadricTexture( Cylinder, GL_TRUE);

   Disk = gluNewQuadric();
   gluQuadricDrawStyle( Disk, GLU_FILL);
   gluQuadricNormals( Disk, GLU_SMOOTH);
   gluQuadricOrientation( Disk, GLU_OUTSIDE);
   gluQuadricTexture( Disk, GL_TRUE);


}

void Draw_gear( void )
{

	int i;
	glPushMatrix();
	gluCylinder(Cylinder, 2.5, 2.5, 1, 16, 16);
	gluDisk(Disk, 0, 2.5, 32, 16);
	glTranslatef(0,0,1);
    gluDisk(Disk, 0, 2.5, 32, 16);
	glPopMatrix();
    for( i = 0; i < 8; i++)
		{
	    glPushMatrix();
		glTranslatef( 0.0, 0.0, 0.50);
		glRotatef( (360/8) * i, 0.0, 0.0, 1.0);
		glTranslatef( 3.0, 0.0, 0.0);
		glutSolidCube( 1.0 );
		glPopMatrix();
	    }


}

void Draw_clock( GLfloat cx, GLfloat cy, GLfloat cz )
{

  int hour_ticks , sec_ticks;
  glPushMatrix();
  glTranslatef(cx,cy,cz);
  glRotatef( 180, 1.0, 0.0, 0.0);

  /*glPushMatrix(); // Draw large wire cube (outside of disk clock)
  glColor3f(1.0, 1.0, 1.0);
  glTranslatef( 0.0, 0.0, 6.0);
  glutWireCube(14.0);
  glPopMatrix();*/ 

  glPushMatrix(); // Draw clock face
  glTranslatef( 0, 0, 1.0);
  gluDisk(Disk, 0, 6.75, 32, 16);
  glPopMatrix();

  glPushMatrix();// Draw hour hand
  srand((unsigned int)time(NULL));
  glColor3f(randomNum(), randomNum(), randomNum());
  glTranslatef( 0, 0, 0.0);
  glRotatef( (360/12) * newtime->tm_hour  + (360/60) * (60 / (newtime->tm_min+1)), 0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0.0, 0.0, 2.0);
  Draw_gear();
  glPopMatrix();
  glRotatef( 90, 1.0, 0.0, 0.0);
  gluCylinder(Cylinder, 0.75, 0, 4, 16, 16);
  glPopMatrix();

  glPushMatrix();// Draw minute hand
  glColor3f(randomNum(), randomNum(), randomNum());
  //glColor3f(1.0, 0.5, 1.0);
  glTranslatef( 0, 0, 0.0);
  glRotatef( (360/60) * newtime->tm_min, 0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0.0, 0.0, 3.0);
  glScalef(0.5, 0.5, 1.0);
  Draw_gear();
  glPopMatrix();
  glRotatef( 90, 1.0, 0.0, 0.0);
  gluCylinder(Cylinder, 0.5, 0, 6, 16, 16);
  glPopMatrix();

  glPushMatrix();// Draw second hand
  //glColor3f(randomNum(), randomNum(), randomNum());
  glColor3f(1.0, 0.0, 0.5);
  glTranslatef( 0, 0, -0.0);
  glRotatef( (360/60) * newtime->tm_sec, 0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0.0, 0.0, 4.0);
  glScalef(0.25, 0.25, 1.0);
  Draw_gear();
  glPopMatrix();
  glRotatef( 90, 1.0, 0.0, 0.0);
  gluCylinder(Cylinder, 0.25, 0, 6, 16, 16);
  glPopMatrix();


  for(hour_ticks = 0; hour_ticks < 12; hour_ticks++)
	  {
	  glPushMatrix();// Draw next arm axis.
//	glColor3f(randomNum(), randomNum(), randomNum());      
	glColor3f(0.0, 1.0, 1.0); // give it a color
	  glTranslatef(0.0, 0.0, 0.0);
	  glRotatef( (360/12) * hour_ticks, 0.0, 0.0, 1.0);
      glTranslatef( 6.0, 0.0, 0.0);
	  glutSolidCube(1.0);

      glPopMatrix();
  }
  
  for(sec_ticks = 0; sec_ticks < 60; sec_ticks++)
	 {
   	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef( (360/60) * sec_ticks, 0.0, 0.0, 1.0);
	glTranslatef(6.0, 0.0, 0.0);
	glutSolidCube(0.25);
	glPopMatrix();
	}


  glPopMatrix();

}

void num()
{
    if(view_state == 1)
	{  //srand((unsigned int)time(NULL));
  	glColor3f(randomNum(), randomNum(), randomNum());

//	glColor3f( 0.0, 0.0, 1.0);
	Sprint(-6.2,-0.2,"9"); //counting from center
	Sprint(-0.2,-6.2,"6"); 
	Sprint(-0.4,5.7,"12");
	Sprint(5.8,-0.2,"3");
	}
}

void about()
{
    //glClear(GL_COLOR_BUFFER_BIT);
	glColor3f( 1.0, 1.0, 1.0);
    Sprint(-6,-2.8,"\tOpenGL Clock : ");
	Sprint(-6,-3.6,"\tAnalog and Digital Clock implemented");
	Sprint(-6,-4.4,"\tthat chages its colour randomly");
    Sprint(-6,-5.2,"\teverytime the second hand moves.");
	Sprint(-6,-6.0,"\tClock displays local system time.");
	//glFlush(); 
}

void display_clock()
{
	time(&ltime); // Get time
  newtime = localtime(&ltime); // Convert to local time

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // easy way to put text on the screen.
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-8.0, 8.0, -8.0, 8.0, 1.0, 60.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);

  // Put view state on screen
  glColor3f( 1.0, 1.0, 1.0);
  if (view_state == 0 && about_int==0)
	  {
	  //Sprint(-3, -4, "Perspective view");
		about();
	  }
  else if (view_state != 0 && about_int==0)
  {
	  Sprint(-2, -4, "Ortho view");
  }
  else 
  {if(view_state==0)
	  {about();}
	else {Sprint(-2, -4, "Ortho view");}
  }


      Sprint(-4,-7.7, asctime(newtime));

  // Turn Perspective mode on/off
  if (view_state == 0)
     {
     glMatrixMode (GL_PROJECTION);
     glLoadIdentity();

     gluPerspective(60.0, 1, 1.0, 60.0);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     gluLookAt( rx, 0.0, rz, 0.0, 0.0, -14.0, 0, 1, 0);
	 }

	if (light_state == 1)
	  {
	   glEnable(GL_LIGHTING);
	   glEnable(GL_COLOR_MATERIAL);  // Enable for lighTing
      }else
	  {
	  glDisable(GL_LIGHTING);
	  glDisable(GL_COLOR_MATERIAL);  // Disable for no lighTing
  }

Draw_clock( 0.0, 0.0, -14.0);
num();
glutSwapBuffers();
}
///////////////////////////////////////////////////////*


void drawDetails(char ch[] , float x,float y)
{
    int i;
    glRasterPos2f(x,y);

    for(i = 0; ch[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,ch[i]);
    }
}

void intro()
{
     glClearColor(1.0,1.0,1.0,0.0);
     glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glColor3f(1.0,0.0,0.0);
	//drawDetails("LAB",-0.8,1.0);
        drawDetails("PESIT, BSC - COMPUTER GRAPHICS MINI PROJECT",-0.8,0.8);
        drawDetails("TITLE : OPENGL RANDOM COLOUR CLOCK",-0.8,0.6);
	drawDetails("BY : ",-0.8,0.4);
        drawDetails("NAME : VENKATESH B N",-0.8,-0.2);
        drawDetails("USN : 1PE16CS174",-0.8,-0.4);
        drawDetails("NAME : SIDDHANTH J AJRI",-0.8,0.2);
        drawDetails("USN : 1PE16CS154",-0.8,0.0);
	drawDetails("PROJECT GUIDE : PROF. EVLIN",-0.8,-0.6);
	drawDetails("PRESS LEFT MOUSE BUTTON TO CONTINUE",-0.8,-0.8);
	
glutSwapBuffers();
glFlush();
}



void display(void)
{
  // glClear(GL_COLOR_BUFFER_BIT);
//
glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	if(mode==-1)
    {
        intro();
	 //glFlush();
    }
//
else{	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	 display_clock();
	 glFlush();
    }
}



void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
}

void options(int id)
{
 switch(id)
 {
  /*
  case 1:
	  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	about_int= abs(about_int - 1);
	  break;
*/
  case 2:
	  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	  view_state = abs(view_state - 1);
	  break;
  case 3:
	  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	   light_state = abs(light_state - 1);
      break;
  case 4 :
	  exit(0);
 }
 }

void mouse(int btn, int state, int x, int y){
    if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        mode = 0;
    }
    //if(btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
    //    mode = 1;
    //}
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (50, 50);

   glutCreateWindow (argv[0]);
   glutSetWindowTitle("CLOCK");
   init ();
   glutMouseFunc(mouse);
      glutDisplayFunc(display);
      glutReshapeFunc(reshape);
   glutCreateMenu(options);
   //glutAddMenuEntry("About the Project",1);
   glutAddMenuEntry("Zoom/Enlarge",2);
   glutAddMenuEntry("Light on/off",3);
   glutAddMenuEntry("Quit",4);
   glutAttachMenu(GLUT_RIGHT_BUTTON);
   //glutDisplayFunc(display);
   //glutReshapeFunc(reshape);
   glutTimerFunc(10, TimeEvent, 1);
   glutMainLoop();
   return 0;

}




















/*






#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// define glu objects
int about_int=0;

GLUquadricObj *Cylinder;
GLUquadricObj *Disk;

struct tm *newtime;
time_t ltime;
int M_TWOPI=0;
GLfloat rx, ry, rz, angle;

// lighting
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightPosition[]= { 5.0f, 25.0f, 15.0f, 1.0f };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };

static int light_state = 0; // light on = 1, light off = 0
static int view_state = 0; // Ortho view = 1, Perspective = 0

float randomNum()
{
return ((float)rand())/RAND_MAX;
}

void Sprint( float x, float y, char *st)
{
	int l,i;

	l=strlen( st );
	glRasterPos3f( x, y, -1);
	for( i=0; i < l; i++)
		{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]);
	}

}

static void TimeEvent(int te)
{

	rx = 30 * cos( angle );
	ry = 30 * sin( angle );
	rz = 30 * cos( angle );
	angle += 0.01;
	if (angle > M_TWOPI) angle = 0;

	glutPostRedisplay();
	glutTimerFunc( 100, TimeEvent, 1);
}

void init(void)
{


   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);
   // Lighting is added to scene
   glLightfv(GL_LIGHT1 ,GL_AMBIENT, LightAmbient);
   glLightfv(GL_LIGHT1 ,GL_DIFFUSE, LightDiffuse);
   glLightfv(GL_LIGHT1 ,GL_POSITION, LightPosition);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT1);


   Cylinder = gluNewQuadric();
   gluQuadricDrawStyle( Cylinder, GLU_FILL);
   gluQuadricNormals( Cylinder, GLU_SMOOTH);
   gluQuadricOrientation( Cylinder, GLU_OUTSIDE);
   gluQuadricTexture( Cylinder, GL_TRUE);

   Disk = gluNewQuadric();
   gluQuadricDrawStyle( Disk, GLU_FILL);
   gluQuadricNormals( Disk, GLU_SMOOTH);
   gluQuadricOrientation( Disk, GLU_OUTSIDE);
   gluQuadricTexture( Disk, GL_TRUE);


}

void Draw_gear( void )
{

	int i;
	glPushMatrix();
	gluCylinder(Cylinder, 2.5, 2.5, 1, 16, 16);
	gluDisk(Disk, 0, 2.5, 32, 16);
	glTranslatef(0,0,1);
    gluDisk(Disk, 0, 2.5, 32, 16);
	glPopMatrix();
    for( i = 0; i < 8; i++)
		{
	    glPushMatrix();
		glTranslatef( 0.0, 0.0, 0.50);
		glRotatef( (360/8) * i, 0.0, 0.0, 1.0);
		glTranslatef( 3.0, 0.0, 0.0);
		glutSolidCube( 1.0 );
		glPopMatrix();
	    }


}

void Draw_clock( GLfloat cx, GLfloat cy, GLfloat cz )
{

  int hour_ticks , sec_ticks;
  glPushMatrix();
  glTranslatef(cx,cy,cz);
  glRotatef( 180, 1.0, 0.0, 0.0);

  /*glPushMatrix(); // Draw large wire cube (outside of disk clock)
  glColor3f(1.0, 1.0, 1.0);
  glTranslatef( 0.0, 0.0, 6.0);
  glutWireCube(14.0);
  glPopMatrix();
*/ 
/*
  glPushMatrix(); // Draw clock face
  glTranslatef( 0, 0, 1.0);
  gluDisk(Disk, 0, 6.75, 32, 16);
  glPopMatrix();

  glPushMatrix();// Draw hour hand
  srand((unsigned int)time(NULL));
  glColor3f(randomNum(), randomNum(), randomNum());
  glTranslatef( 0, 0, 0.0);
  glRotatef( (360/12) * newtime->tm_hour  + (360/60) * (60 / (newtime->tm_min+1)), 0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0.0, 0.0, 2.0);
  Draw_gear();
  glPopMatrix();
  glRotatef( 90, 1.0, 0.0, 0.0);
  gluCylinder(Cylinder, 0.75, 0, 4, 16, 16);
  glPopMatrix();

  glPushMatrix();// Draw minute hand
  glColor3f(randomNum(), randomNum(), randomNum());
  //glColor3f(1.0, 0.5, 1.0);
  glTranslatef( 0, 0, 0.0);
  glRotatef( (360/60) * newtime->tm_min, 0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0.0, 0.0, 3.0);
  glScalef(0.5, 0.5, 1.0);
  Draw_gear();
  glPopMatrix();
  glRotatef( 90, 1.0, 0.0, 0.0);
  gluCylinder(Cylinder, 0.5, 0, 6, 16, 16);
  glPopMatrix();

  glPushMatrix();// Draw second hand
  //glColor3f(randomNum(), randomNum(), randomNum());
  glColor3f(1.0, 0.0, 0.5);
  glTranslatef( 0, 0, -0.0);
  glRotatef( (360/60) * newtime->tm_sec, 0.0, 0.0, 1.0);
  glPushMatrix();
  glTranslatef(0.0, 0.0, 4.0);
  glScalef(0.25, 0.25, 1.0);
  Draw_gear();
  glPopMatrix();
  glRotatef( 90, 1.0, 0.0, 0.0);
  gluCylinder(Cylinder, 0.25, 0, 6, 16, 16);
  glPopMatrix();


  for(hour_ticks = 0; hour_ticks < 12; hour_ticks++)
	  {
	  glPushMatrix();// Draw next arm axis.
//	glColor3f(randomNum(), randomNum(), randomNum());      
	glColor3f(0.0, 1.0, 1.0); // give it a color
	  glTranslatef(0.0, 0.0, 0.0);
	  glRotatef( (360/12) * hour_ticks, 0.0, 0.0, 1.0);
      glTranslatef( 6.0, 0.0, 0.0);
	  glutSolidCube(1.0);

      glPopMatrix();
  }
  
  for(sec_ticks = 0; sec_ticks < 60; sec_ticks++)
	 {
   	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef( (360/60) * sec_ticks, 0.0, 0.0, 1.0);
	glTranslatef(6.0, 0.0, 0.0);
	glutSolidCube(0.25);
	glPopMatrix();
	}


  glPopMatrix();

}

void num()
{
    if(view_state == 1)
	{  //srand((unsigned int)time(NULL));
  	glColor3f(randomNum(), randomNum(), randomNum());

//	glColor3f( 0.0, 0.0, 1.0);
	Sprint(-6.2,-0.2,"9"); //counting from center
	Sprint(-0.2,-6.2,"6"); 
	Sprint(-0.4,5.7,"12");
	Sprint(5.8,-0.2,"3");
	}
}

void about()
{
    //glClear(GL_COLOR_BUFFER_BIT);
	glColor3f( 1.0, 1.0, 1.0);
    Sprint(-5,-2.8,"\tOpenGL Clock");
	Sprint(-5,-3.6,"\tAnalog and Digital Clock");
	Sprint(-5,-4.4,"\tis implemented here");
    Sprint(-5,-5.2,"\tClock shows the system time");
	Sprint(-5,-6.0,"\tas fetched from the computer");
	//glFlush(); 
}

void display_clock()
{
	time(&ltime); // Get time
  newtime = localtime(&ltime); // Convert to local time

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // easy way to put text on the screen.
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-8.0, 8.0, -8.0, 8.0, 1.0, 60.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_LIGHTING);
  glDisable(GL_COLOR_MATERIAL);

  // Put view state on screen
  glColor3f( 1.0, 1.0, 1.0);
  if (view_state == 0 && about_int==0)
	  {
	  Sprint(-3, -4, "Perspective view");
	  }
  else if (view_state != 0 && about_int==0)
  {
	  Sprint(-2, -4, "Ortho view");
  }
  else 
  {
	  about();
  }


      Sprint(-4,-7.7, asctime(newtime));

  // Turn Perspective mode on/off
  if (view_state == 0)
     {
     glMatrixMode (GL_PROJECTION);
     glLoadIdentity();

     gluPerspective(60.0, 1, 1.0, 60.0);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     gluLookAt( rx, 0.0, rz, 0.0, 0.0, -14.0, 0, 1, 0);
	 }

	if (light_state == 1)
	  {
	   glEnable(GL_LIGHTING);
	   glEnable(GL_COLOR_MATERIAL);  // Enable for lighing
      }else
	  {
	  glDisable(GL_LIGHTING);
	  glDisable(GL_COLOR_MATERIAL);  // Disable for no lighing
  }

Draw_clock( 0.0, 0.0, -14.0);
num();
glutSwapBuffers();
}


void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
	 display_clock();
	 glFlush();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
}

void options(int id)
{
 switch(id)
 {
  
  case 1:
	  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	about_int= abs(about_int - 1);
	  break;
  case 2:
	  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	  view_state = abs(view_state - 1);
	  break;
  case 3:
	  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	   light_state = abs(light_state - 1);
      break;
  case 4 :
	  exit(0);
 }
 }

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (50, 50);
   glutCreateWindow (argv[0]);
   glutSetWindowTitle("CLOCK");
   init ();
   glutCreateMenu(options);
   glutAddMenuEntry("About the Project",1);
   glutAddMenuEntry("Zoom/Enlarge",2);
   glutAddMenuEntry("Light on/off",3);
   glutAddMenuEntry("Quit",4);
   glutAttachMenu(GLUT_RIGHT_BUTTON);
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutTimerFunc(10, TimeEvent, 1);
   glutMainLoop();
   return 0;
}
*/
