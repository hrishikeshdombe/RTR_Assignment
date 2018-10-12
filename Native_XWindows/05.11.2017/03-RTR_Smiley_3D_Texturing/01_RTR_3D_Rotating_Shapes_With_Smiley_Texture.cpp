#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/XKBlib.h>
#include<X11/keysym.h>

#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glx.h>
#include<SOIL/SOIL.h>

using namespace std;

bool gbFullscreen=false;
Display *gpDisplay=NULL;
XVisualInfo *gpXVisualInfo=NULL;
Colormap gColormap;
Window gWindow;
int giWindowWidth=800;
int giWindowHeight=600;
GLfloat gfAspect_Ratio,gfAngle;

GLuint Texture_Smiley,Texture_Stone;

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

	XStoreName(gpDisplay,gWindow,"Smiley Texture On 3D Rotating Geometry");

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
	int LoadGLTextures(GLuint*,const char*);

	gGLXContext=glXCreateContext(gpDisplay,gpXVisualInfo,NULL,GL_TRUE);

	glXMakeCurrent(gpDisplay,gWindow,gGLXContext);

	glClearColor(0.1f,0.1f,0.1f,0.0f);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glEnable(GL_TEXTURE_2D);

	LoadGLTextures(&Texture_Smiley,"./Smiley.bmp");
	LoadGLTextures(&Texture_Stone, "./Stone.bmp");

	resize(giWindowWidth,giWindowHeight);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, Texture_Stone);
	glTranslatef(-1.5f, 0.0f, -6.0f);
	glRotatef(gfAngle, 0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	//Front Face
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	//Right Face
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	//Back Face
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	//Left Face
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glEnd();
	//Quad below Pyramid
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glEnd();


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D,Texture_Smiley);
	glTranslatef(1.5f,0.0f,-6.0f);
	glScalef(0.75f,0.75f,0.75f);
	glRotatef(gfAngle,1.0f,0.0f,0.0f);
	glRotatef(gfAngle,0.0f,1.0f,0.0f);
	glRotatef(gfAngle,0.0f,0.0f,1.0f);
	glBegin(GL_QUADS);
		//Front face of Cube
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(1.0f,1.0f,1.0f);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-1.0f,1.0f,1.0f);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,1.0f);
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(1.0f,-1.0f,1.0f);
		//Left face of Cube
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(-1.0f,1.0f,1.0f);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-1.0f,1.0f,-1.0f);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,1.0f);
		//Back face of Cube
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(1.0f,1.0f,-1.0f);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-1.0f,1.0f,-1.0f);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(1.0f,-1.0f,-1.0f);
		//Right face of Cube
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(1.0f,1.0f,-1.0f);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(1.0f,1.0f,1.0f);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(1.0f,-1.0f,1.0f);
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(1.0f,-1.0f,-1.0f);
		//Upper face of Cube
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(1.0f,1.0f,-1.0f);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-1.0f,1.0f,-1.0f);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-1.0f,1.0f,1.0f);
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(1.0f,1.0f,1.0f);
		//Bottom face of Cube
		glTexCoord2f(1.0f,1.0f);
		glVertex3f(1.0f,-1.0f,-1.0f);
		glTexCoord2f(0.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		glTexCoord2f(0.0f,0.0f);
		glVertex3f(-1.0f,-1.0f,1.0f);
		glTexCoord2f(1.0f,0.0f);
		glVertex3f(1.0f,-1.0f,1.0f);
	glEnd();

	glXSwapBuffers(gpDisplay,gWindow);

}

int LoadGLTextures(GLuint *texture, const char *path)
{
	unsigned char *imageData=NULL;
	int iHeight,iWidth,iStatus=false;
	
	glGenTextures(1,texture);
	imageData=SOIL_load_image(path,&iWidth,&iHeight,0,SOIL_LOAD_RGB);
	if(imageData)
	{
		iStatus=True;
		glPixelStorei(GL_UNPACK_ALIGNMENT,4);
		glBindTexture(GL_TEXTURE_2D,*texture);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D,3,iHeight,iWidth,GL_RGB,GL_UNSIGNED_BYTE,(void *)imageData);
		SOIL_free_image_data(imageData);
	}
	return(iStatus);
}

void update(void)
{
	gfAngle=gfAngle+0.07f;
	if(gfAngle>=360.0f)
		gfAngle=0.0f;
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
