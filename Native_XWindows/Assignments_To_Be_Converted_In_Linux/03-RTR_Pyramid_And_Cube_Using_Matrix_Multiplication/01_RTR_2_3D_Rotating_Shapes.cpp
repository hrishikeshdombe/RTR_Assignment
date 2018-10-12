#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>//for memset()
#include<memory.h>
#include<math.h>

#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/XKBlib.h>
#include<X11/keysym.h>

#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glx.h>

#define PI 3.1415

using namespace std;

bool gbFullscreen=false;
Display *gpDisplay=NULL;
XVisualInfo *gpXVisualInfo=NULL;
Colormap gColormap;
Window gWindow;
int giWindowWidth=800;
int giWindowHeight=600;
GLfloat gfAspect_Ratio,gfAngle_Cube_Degree=0.0f,gfAngle_Cube_Radian;
GLfloat gIdentity_Matrix[16],gTranslation_Matrix_Cube[16],gTranslation_Matrix_Pyramid[16],gScale_Matrix[16],gRotation_Matrix_X[16],gRotation_Matrix_Y[16],gRotation_Matrix_Z[16];

GLXContext gGLXContext;

int main(void)
{
	void CreateWindow(void);
	void ToggleFullscreen(void);
	void initialize(void);
	void display(void);
	void update(void);
	void resize(int,int);
	void uninitialize(void);

	int winWidth=giWindowWidth;
	int winHeight=giWindowHeight;

	bool bDone=false;

	CreateWindow();

	initialize();

	XEvent event;
	KeySym keysym;

	while(bDone==false)
	{
		while(XPending(gpDisplay))
		{
			XNextEvent(gpDisplay,&event);
			switch(event.type)
			{
				case MapNotify:
					break;

				case KeyPress:
					keysym=XkbKeycodeToKeysym(gpDisplay,event.xkey.keycode,0,0);
					switch(keysym)
					{
						case XK_Escape:
							uninitialize();
							exit(0);

						case XK_F:
						case XK_f:
							if(gbFullscreen==false)
							{
								ToggleFullscreen();
								gbFullscreen=true;
							}
							else
							{
								ToggleFullscreen();
								gbFullscreen=false;
							}
							break;

							default:
							break;
					}
					break;

				case ButtonPress:
					switch(event.xbutton.button)
					{
						case 1:
							break;

						case 2:
							break;

						case 3:
							break;

						default:
							break;
					}
					break;

				case MotionNotify:
					break;

				case ConfigureNotify:
					winWidth=event.xconfigure.width;
					winHeight=event.xconfigure.height;
					resize(winWidth,winHeight);
					break;

				case Expose:
					break;

				case DestroyNotify:
					break;

				case 33:
					bDone=true;
					break;

				default:
					break;
			}
		}
		update();
		display();
	}
	uninitialize();
	return(0);
}

void CreateWindow(void)
{
	void uninitialize(void);

	XSetWindowAttributes winAttribs;

	int defaultScreen;
	int defaultDepth;
	int styleMask;

	static int frameBufferAttributes[]=
	{
		GLX_RGBA,
		GLX_RED_SIZE, 8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE, 8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		GLX_DOUBLEBUFFER, True,
		None
	};

	gpDisplay=XOpenDisplay(NULL);
	if(gpDisplay==NULL)
	{
		printf("ERROR : Unable To Open X Display.\nExitting Now...\n");
		uninitialize();
		exit(1);
	}

	defaultScreen=XDefaultScreen(gpDisplay);

	gpXVisualInfo=glXChooseVisual(gpDisplay,defaultScreen,frameBufferAttributes);

	winAttribs.border_pixel=0;
	winAttribs.background_pixmap=0;
	winAttribs.colormap=XCreateColormap(gpDisplay,RootWindow(gpDisplay,gpXVisualInfo->screen),gpXVisualInfo->visual,AllocNone);

	gColormap=winAttribs.colormap;

	winAttribs.background_pixel=BlackPixel(gpDisplay,defaultScreen);

	winAttribs.event_mask=ExposureMask|VisibilityChangeMask|ButtonPressMask|KeyPressMask|PointerMotionMask|StructureNotifyMask;

	styleMask=CWBorderPixel|CWBackPixel|CWEventMask|CWColormap;

	gWindow=XCreateWindow(gpDisplay,RootWindow(gpDisplay,gpXVisualInfo->screen),0,0,giWindowWidth,giWindowHeight,0,gpXVisualInfo->depth,InputOutput,gpXVisualInfo->visual,styleMask,&winAttribs);

	if(!gWindow)
	{
		printf("Error : Failed To Create Main Window.\nExitting Now...\n");
		uninitialize();
		exit(1);
	}

	XStoreName(gpDisplay,gWindow,"First OpenGL Window");

	Atom windowManagerDelete=XInternAtom(gpDisplay,"WM_DELETE_WINDOW",True);
	XSetWMProtocols(gpDisplay,gWindow,&windowManagerDelete,1);

	XMapWindow(gpDisplay,gWindow);
}

void ToggleFullscreen(void)
{
	Atom wm_state;

	Atom fullscreen;

	XEvent xev={0};

	wm_state=XInternAtom(gpDisplay,"_NET_WM_STATE",False);

	memset(&xev,0,sizeof(xev));

	xev.type=ClientMessage;

	xev.xclient.window=gWindow;

	xev.xclient.message_type=wm_state;

	xev.xclient.format=32;

	xev.xclient.data.l[0]=gbFullscreen?0:1;

	fullscreen=XInternAtom(gpDisplay,"_NET_WM_STATE_FULLSCREEN",False);

	xev.xclient.data.l[1]=fullscreen;

	XSendEvent(gpDisplay,RootWindow(gpDisplay,gpXVisualInfo->screen),False,StructureNotifyMask,&xev);
}

void initialize(void)
{
	void resize(int,int);

	gGLXContext=glXCreateContext(gpDisplay,gpXVisualInfo,NULL,GL_TRUE);

	glXMakeCurrent(gpDisplay,gWindow,gGLXContext);

	memset(&gIdentity_Matrix,0.0f,sizeof(gIdentity_Matrix));
	memset(&gTranslation_Matrix_Pyramid,0.0f,sizeof(gTranslation_Matrix_Pyramid));
	memset(&gTranslation_Matrix_Cube,0.0f,sizeof(gTranslation_Matrix_Cube));
	memset(&gScale_Matrix,0.0f,sizeof(gScale_Matrix));
	memset(&gRotation_Matrix_X,0.0f,sizeof(gRotation_Matrix_X));
	memset(&gRotation_Matrix_Y,0.0f,sizeof(gRotation_Matrix_Y));
	memset(&gRotation_Matrix_Z,0.0f,sizeof(gRotation_Matrix_Z));

	gIdentity_Matrix[0]=1.0f;
	gIdentity_Matrix[5]=1.0f;
	gIdentity_Matrix[10]=1.0f;
	gIdentity_Matrix[15]=1.0f;

	gTranslation_Matrix_Cube[0]=1.0f;
	gTranslation_Matrix_Cube[5]=1.0f;
	gTranslation_Matrix_Cube[10]=1.0f;
	gTranslation_Matrix_Cube[15]=1.0f;
	gTranslation_Matrix_Cube[12]=1.5f;
	gTranslation_Matrix_Cube[13]=0.0f;
	gTranslation_Matrix_Cube[14]=-6.0f;

	gTranslation_Matrix_Pyramid[0]=1.0f;
	gTranslation_Matrix_Pyramid[5]=1.0f;
	gTranslation_Matrix_Pyramid[10]=1.0f;
	gTranslation_Matrix_Pyramid[15]=1.0f;
	gTranslation_Matrix_Pyramid[12]=-1.5f;
	gTranslation_Matrix_Pyramid[13]=0.0f;
	gTranslation_Matrix_Pyramid[14]=-6.0f;

	gScale_Matrix[0]=0.75f;
	gScale_Matrix[5]=0.75f;
	gScale_Matrix[10]=0.75f;
	gScale_Matrix[15]=1.0f;

	gRotation_Matrix_X[0]=1.0f;
	gRotation_Matrix_X[15]=1.0f;

	gRotation_Matrix_Y[5]=1.0f;
	gRotation_Matrix_Y[15]=1.0f;

	gRotation_Matrix_Z[10]=1.0f;
	gRotation_Matrix_Z[15]=1.0f;

	glClearColor(0.1f,0.1f,0.1f,0.0f);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

	resize(giWindowWidth,giWindowHeight);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	gRotation_Matrix_X[5]=cos(gfAngle_Cube_Radian);
	gRotation_Matrix_X[6]=sin(gfAngle_Cube_Radian);
	gRotation_Matrix_X[9]=-sin(gfAngle_Cube_Radian);
	gRotation_Matrix_X[10]=cos(gfAngle_Cube_Radian);

	gRotation_Matrix_Y[0]=cos(gfAngle_Cube_Radian);
	gRotation_Matrix_Y[2]=-sin(gfAngle_Cube_Radian);
	gRotation_Matrix_Y[8]=sin(gfAngle_Cube_Radian);
	gRotation_Matrix_Y[10]=cos(gfAngle_Cube_Radian);

	gRotation_Matrix_Z[0]=cos(gfAngle_Cube_Radian);
	gRotation_Matrix_Z[1]=sin(gfAngle_Cube_Radian);
	gRotation_Matrix_Z[4]=-sin(gfAngle_Cube_Radian);
	gRotation_Matrix_Z[5]=cos(gfAngle_Cube_Radian);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(gIdentity_Matrix);

	/*glTranslatef(-1.8f,0.0f,-6.0f);
	glRotatef(gfAngle,0.0f,1.0f,0.0f);*/
	glMultMatrixf(gTranslation_Matrix_Pyramid);
	gfAngle_Cube_Radian=gfAngle_Cube_Degree*(PI / 180.0f);
	glMultMatrixf(gRotation_Matrix_Y);
	glBegin(GL_TRIANGLES);
		//Front face of Pyramid
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(-1.0f,-1.0f,1.0f);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(1.0f,-1.0f,1.0f);
		//Left side face of Pyramid
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(-1.0f,-1.0f,1.0f);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		//Back face of Pyramid
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(1.0f,-1.0f,-1.0f);
		//Right side face of Pyramid
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(0.0f,1.0f,0.0f);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(1.0f,-1.0f,-1.0f);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(1.0f,-1.0f,1.0f);
	glEnd();

	/*glMultMatrixf(gIdentity_Matrix);
	//Pyramid Bottom face
	glMultMatrixf(gTranslation_Matrix_Pyramid);
	glMultMatrixf(gRotation_Matrix_Y);*/
	glBegin(GL_QUADS);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(1.0f,-1.0f,1.0f);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(-1.0f,-1.0f,1.0f);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(1.0f,-1.0f,-1.0f);
	glEnd();

	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(gIdentity_Matrix);

	/*glTranslatef(1.8f,0.0f,-6.0f);
	glRotatef(gfAngle,1.0f,0.0f,0.0f);
	glRotatef(gfAngle,0.0f,1.0f,0.0f);
	glRotatef(gfAngle,0.0f,0.0f,1.0f);
	glScalef(0.8f,0.8f,0.8f);*/
	glMultMatrixf(gTranslation_Matrix_Cube);
	glMultMatrixf(gScale_Matrix);
	glMultMatrixf(gRotation_Matrix_X);
	glMultMatrixf(gRotation_Matrix_Y);
	glMultMatrixf(gRotation_Matrix_Z);
	glBegin(GL_QUADS);
		//Front face of Cube
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(1.0f,1.0f,1.0f);
		glColor3f(0.0f,0.0f,0.0f);
		glVertex3f(-1.0f,1.0f,1.0f);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,1.0f);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(1.0f,-1.0f,1.0f);
		//Left face of Cube
		glColor3f(0.0f,0.0f,0.0f);
		glVertex3f(-1.0f,1.0f,1.0f);
		glColor3f(0.0f,1.0f,1.0f);
		glVertex3f(-1.0f,1.0f,-1.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,1.0f);
		//Back face of Cube
		glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(1.0f,1.0f,-1.0f);
		glColor3f(0.0f,1.0f,1.0f);
		glVertex3f(-1.0f,1.0f,-1.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glColor3f(1.0f,1.0f,0.0f);
		glVertex3f(1.0f,-1.0f,-1.0f);
		//Right face of Cube
		glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(1.0f,1.0f,-1.0f);
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(1.0f,1.0f,1.0f);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(1.0f,-1.0f,1.0f);
		glColor3f(1.0f,1.0f,0.0f);
		glVertex3f(1.0f,-1.0f,-1.0f);
		//Upper face of Cube
		glColor3f(1.0f,0.0f,1.0f);
		glVertex3f(1.0f,1.0f,-1.0f);
		glColor3f(0.0f,1.0f,1.0f);
		glVertex3f(-1.0f,1.0f,-1.0f);
		glColor3f(0.0f,0.0f,0.0f);
		glVertex3f(-1.0f,1.0f,1.0f);
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(1.0f,1.0f,1.0f);
		//Bottom face of Cube
		glColor3f(1.0f,1.0f,0.0f);
		glVertex3f(1.0f,-1.0f,-1.0f);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,1.0f);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(1.0f,-1.0f,1.0f);
	glEnd();

	glXSwapBuffers(gpDisplay,gWindow);

}

void update(void)
{
	gfAngle_Cube_Degree=gfAngle_Cube_Degree+0.07f;
	if(gfAngle_Cube_Degree>=360.0f)
		gfAngle_Cube_Degree=0.0f;
}

void resize(int width,int height)
{
	if(height==0)
		height=1;

	glViewport(0,0,(GLsizei)width,(GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gfAspect_Ratio=(GLfloat)width/(GLfloat)height;

	gluPerspective(45.0f,gfAspect_Ratio,0.1f,100.0f);
}

void uninitialize(void)
{
	GLXContext currentGLXContext;

	currentGLXContext=glXGetCurrentContext();

	if(currentGLXContext!=NULL && currentGLXContext==gGLXContext)
	{
		glXMakeCurrent(gpDisplay,0,0);
	}

	if(gGLXContext)
	{
		glXDestroyContext(gpDisplay,gGLXContext);
	}

	if(gWindow)
	{
		XDestroyWindow(gpDisplay,gWindow);
	}

	if(gColormap)
	{
		XFreeColormap(gpDisplay,gColormap);
	}

	if(gpXVisualInfo)
	{
		free(gpXVisualInfo);
		gpXVisualInfo=NULL;
	}

	if(gpDisplay)
	{
		XCloseDisplay(gpDisplay);
		gpDisplay=NULL;
	}
}