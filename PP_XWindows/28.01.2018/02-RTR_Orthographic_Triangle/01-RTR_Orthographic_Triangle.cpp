#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/XKBlib.h>
#include<X11/keysym.h>

#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include"../../vmath.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

using namespace vmath;

enum
{
	HAD_ATTRIBUTE_POSITION = 0,
	HAD_ATTRIBUTE_COLOR,
	HAD_ATTRIBUTE_NORMAL,
	HAD_ATTRIBUTE_TEXTURE,
};

FILE *gpFile=NULL;

Display *gpDisplay=NULL;
XVisualInfo *gpXVisualInfo=NULL;
Colormap gColormap;
Window gWindow;
typedef GLXContext (*glXCreateContextAttribsARBProc) (Display*,GLXFBConfig,GLXContext,Bool,const int*);//Similar to typedef in Explicit DLL for Function Pointer
glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;//Creating the object of Function Pointer
GLXFBConfig gGLXFBConfig;//Creating Framebuffer Configuration Structures object
GLXContext gGLXContext;//Parallel to HGLRC

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;

GLuint gVao;
GLuint gVbo;
GLuint gMVPUniform;

mat4 gOrthographicProjectionMatrix;

bool gbFullscreen=false;

int main(int argc, char *argv[])
{
	void CreateWindow(void);
	void ToggleFullscreen(void);
	void initialize(void);
	void resize(int,int);
	void display(void);
	void uninitialize(void);

	gpFile=fopen("Log.txt","w");
	if(gpFile==NULL)
	{
		printf("Cannot Create Log File...\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		fprintf(gpFile,"Log File Is Created Successfully...\n");
	}

	CreateWindow();

	initialize();

	XEvent event;
	KeySym keysym;
	int winWidth;
	int winHeight;
	bool bDone=false;

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
							bDone=true;
							break;

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

		display();
	}

	uninitialize();
	return(0);
}

void CreateWindow(void)
{
	void uninitialize(void);

	XSetWindowAttributes winAttribs;
	GLXFBConfig *pGLXFBConfigs=NULL;
	GLXFBConfig bestGLXFBConfig;
	XVisualInfo *pTempXVisualInfo=NULL;
	int iNumFBConfigs=0;
	int styleMask;
	int i;

	static int frameBufferAttributes[]=
	{
		GLX_X_RENDERABLE, True,
		GLX_DRAWABLE_TYPE,GLX_WINDOW_BIT,
		GLX_RENDER_TYPE,GLX_RGBA_BIT,
		GLX_X_VISUAL_TYPE,GLX_TRUE_COLOR,
		GLX_RED_SIZE,8,
		GLX_GREEN_SIZE,8,
		GLX_BLUE_SIZE,8,
		GLX_ALPHA_SIZE,8,
		GLX_DEPTH_SIZE,24,
		GLX_STENCIL_SIZE,8,
		GLX_DOUBLEBUFFER,True,
		//GLX_SAMPLE_BUFFERS,1,
		//GLX_SAMPLES,4,
		None//Array must be terminated by 0
	};

	gpDisplay=XOpenDisplay(NULL);
	if(gpDisplay==NULL)
	{
		printf("ERROR : Unable To Obtain X Display.\n");
		fprintf(gpFile,"ERROR : Unable To Obtain X Display.\n");
		uninitialize();
		exit(EXIT_FAILURE);
	} 

	//Get a new FrameBuffer Configuration that meets out attribs requirements
	pGLXFBConfigs=glXChooseFBConfig(gpDisplay,DefaultScreen(gpDisplay),frameBufferAttributes,&iNumFBConfigs);
	if(pGLXFBConfigs==NULL)
	{
		printf("Failed To Get Valid FrameBuffer Configuration. Exitting Now...\n");
		fprintf(gpFile,"Failed To Get Valid FrameBuffer Configuration. Exitting Now...\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}

	//printf("%d Matching FrameBuffer Configurations Found.\n",iNumFBConfigs);

	int bestFramebufferConfig=-1,worstFramebufferConfig=-1,bestNumberOfSamples=-1,worstNumberOfSamples=999;
	for(i=0;i<iNumFBConfigs;i++)
	{
		pTempXVisualInfo=glXGetVisualFromFBConfig(gpDisplay,pGLXFBConfigs[i]);
		if(pTempXVisualInfo)
		{
			int sampleBuffer,samples;

			glXGetFBConfigAttrib(gpDisplay,pGLXFBConfigs[i],GLX_SAMPLE_BUFFERS,&sampleBuffer);
			glXGetFBConfigAttrib(gpDisplay,pGLXFBConfigs[i],GLX_SAMPLES,&samples);

			//fprintf(gpFile,"Matching FrameBuffer Config = %d : Visual ID = 0x%lu : SAMPLE_BUFFERS = %d : SAMPLES = %d\n",i,pTempXVisualInfo->visualid,sampleBuffer,samples);
 
			if(bestFramebufferConfig < 0 || sampleBuffer && samples > bestNumberOfSamples)
			{
				bestFramebufferConfig=i;
				bestNumberOfSamples=samples;
			}
			if(worstFramebufferConfig < 0 || !sampleBuffer || samples < worstNumberOfSamples)
			{
				worstFramebufferConfig=i;
				worstNumberOfSamples=samples;
			}
		}
		XFree(pTempXVisualInfo);
	}
	bestGLXFBConfig=pGLXFBConfigs[bestFramebufferConfig];
	//set global GLXFBConfiguration
	gGLXFBConfig=bestGLXFBConfig;

	//Free FBConfigurations list allocated by glXChooseFBConfig()
	XFree(pGLXFBConfigs);

	gpXVisualInfo=glXGetVisualFromFBConfig(gpDisplay,bestGLXFBConfig);
	//fprintf(gpFile,"Choosen Visual ID=0x%lu\n",gpXVisualInfo->visualid);
	
	

	//Setting Window's Attributes
	winAttribs.border_pixel=0;
	winAttribs.background_pixmap=0;
	//AllocNone for 'Movable' memory allocation
	winAttribs.colormap=XCreateColormap(gpDisplay,RootWindow(gpDisplay,gpXVisualInfo->screen),gpXVisualInfo->visual,AllocNone);
	winAttribs.event_mask=StructureNotifyMask|KeyPressMask|ButtonPressMask|ExposureMask|VisibilityChangeMask|PointerMotionMask;
	styleMask=CWBorderPixel|CWEventMask|CWColormap;
	gColormap=winAttribs.colormap;

	gWindow=XCreateWindow(gpDisplay,RootWindow(gpDisplay,gpXVisualInfo->screen),0,0,WIN_WIDTH,WIN_HEIGHT,0,gpXVisualInfo->depth,InputOutput,gpXVisualInfo->visual,styleMask,&winAttribs);
	if(!gWindow)
	{
		printf("Failure In Window Creation.\n");
		fprintf(gpFile,"Failure In Window Creation.\n");
		uninitialize();
		exit(EXIT_FAILURE);
	}

	XStoreName(gpDisplay,gWindow,"PP : Orthographic Triangle");

	Atom windowManagerDelete=XInternAtom(gpDisplay,"WM_WINDOW_DELETE",True);
	XSetWMProtocols(gpDisplay,gWindow,&windowManagerDelete,1);

	XMapWindow(gpDisplay,gWindow);
}

void ToggleFullscreen(void)
{
	Atom wm_state=XInternAtom(gpDisplay,"_NET_WM_STATE",False);

	XEvent event;
	memset(&event,0,sizeof(XEvent));

	event.type=ClientMessage;
	event.xclient.window=gWindow;
	event.xclient.message_type=wm_state;
	event.xclient.format=32;
	event.xclient.data.l[0]=gbFullscreen ? 0:1;

	Atom fullscreen=XInternAtom(gpDisplay,"_NET_WM_STATE_FULLSCREEN",False);

	event.xclient.data.l[1]=fullscreen;

	XSendEvent(gpDisplay,RootWindow(gpDisplay,gpXVisualInfo->screen),False,StructureNotifyMask,&event);
}

void initialize(void)
{
	void uninitialize(void);
	void resize(int,int);

	glXCreateContextAttribsARB=(glXCreateContextAttribsARBProc)glXGetProcAddressARB((GLubyte *)"glXCreateContextAttribsARB");

	GLint attribs[]={
		GLX_CONTEXT_MAJOR_VERSION_ARB,4,
		GLX_CONTEXT_MINOR_VERSION_ARB,5,
		GLX_CONTEXT_PROFILE_MASK_ARB,
		GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		0
	};

	gGLXContext=glXCreateContextAttribsARB(gpDisplay,gGLXFBConfig,0,True,attribs);

	if(!gGLXContext)
	{
		GLint attribs[]={
		GLX_CONTEXT_MAJOR_VERSION_ARB,1,
		GLX_CONTEXT_MINOR_VERSION_ARB,0,
		0
		};

		printf("Failed to Create GLX 4.5 context. Hence Using Old Style GLX Context.\n");

		gGLXContext=glXCreateContextAttribsARB(gpDisplay,gGLXFBConfig,0,True,attribs);
	}
	else
	{
		fprintf(gpFile,"OpenGL Context 4.5 Is Created.\n");
	}

	if(!glXIsDirect(gpDisplay,gGLXContext))
	{
		printf("Indirect GLX Rendering Context Obtained.\n");
	}

	glXMakeCurrent(gpDisplay,gWindow,gGLXContext);
	
	
	GLenum glew_error=glewInit();
	if(glew_error!=GLEW_OK)
	{
		glXDestroyContext(gpDisplay,gGLXContext);
		gGLXContext=NULL;
	}
	
	fprintf(gpFile,"OpenGL Version : %s\n",(const char *)glGetString(GL_VERSION));
	fprintf(gpFile,"GLSL Version : %s\n",(const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

	/*Vertex Shader*/
	gVertexShaderObject=glCreateShader(GL_VERTEX_SHADER);

	const GLchar *vertexShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"uniform mat4 u_mvp_matrix;" \
 		"void main(void)" \
		"{" \
		"gl_Position = u_mvp_matrix * vPosition;" \
		"}";

	glShaderSource(gVertexShaderObject,1,(const GLchar**)&vertexShaderSourceCode,NULL);

	glCompileShader(gVertexShaderObject);

	GLint iInfoLogLength = 0;
	GLint iShaderCompiledStatus = 0;
	char *szInfoLog = NULL;
	glGetShaderiv(gVertexShaderObject,GL_COMPILE_STATUS,&iShaderCompiledStatus);
	if(iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObject,GL_INFO_LOG_LENGTH,&iInfoLogLength);
		if(iInfoLogLength>0)
		{
			szInfoLog=(char *)malloc(iInfoLogLength);
			if(szInfoLog!=NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObject,iInfoLogLength,&written,szInfoLog);
				fprintf(gpFile,"Vertex Shader Compilation Log : %s\n",szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	} 

	//Fragment Shader
	gFragmentShaderObject=glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar *fragmentShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"out vec4 FragColor;" \
		"void main(void)" \
		"{" \
		"FragColor = vec4(1.0,1.0,1.0,1.0);" \
		"}";

	glShaderSource(gFragmentShaderObject,1,(const GLchar**)&fragmentShaderSourceCode,NULL);

	glCompileShader(gFragmentShaderObject);

	iInfoLogLength=0;
	iShaderCompiledStatus=0;
	szInfoLog=NULL;
	glGetShaderiv(gFragmentShaderObject,GL_COMPILE_STATUS,&iShaderCompiledStatus);
	if(iShaderCompiledStatus==GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObject,GL_INFO_LOG_LENGTH,&iInfoLogLength);
		if(iInfoLogLength>0)
		{
			szInfoLog=(char *)malloc(iInfoLogLength);
			if(szInfoLog!=NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gShaderProgramObject,iInfoLogLength,&written,szInfoLog);
				fprintf(gpFile,"Fragment Shader Compilation Log : %s\n",szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	//Shader Program
	gShaderProgramObject=glCreateProgram();

	glAttachShader(gShaderProgramObject,gVertexShaderObject);

	glAttachShader(gShaderProgramObject,gFragmentShaderObject);

	glBindAttribLocation(gShaderProgramObject,HAD_ATTRIBUTE_POSITION,"vPosition");

	glLinkProgram(gShaderProgramObject);

	GLint iShaderProgramLinkStatus = 0;
	glGetProgramiv(gShaderProgramObject,GL_LINK_STATUS,&iShaderProgramLinkStatus);
	if(iShaderProgramLinkStatus==GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObject,GL_INFO_LOG_LENGTH,&iInfoLogLength);
		if(iInfoLogLength>0)
		{
			szInfoLog=(char *)malloc(iInfoLogLength);
			if(szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject,iInfoLogLength,&written,szInfoLog);
				fprintf(gpFile,"Shader Program Link Log : %s\n",szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	gMVPUniform=glGetUniformLocation(gShaderProgramObject,"u_mvp_matrix");

	const GLfloat triangleVertices[]=
	{
		0.0f,50.0f,0.0f,
		-50.0f,-50.0f,0.0f,
		50.0f,-50.0f,0.0f
	};

	glGenVertexArrays(1,&gVao);
	glBindVertexArray(gVao);

	glGenBuffers(1,&gVbo);
	glBindBuffer(GL_ARRAY_BUFFER,gVbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(triangleVertices),triangleVertices,GL_STATIC_DRAW);

	glVertexAttribPointer(HAD_ATTRIBUTE_POSITION,3,GL_FLOAT,GL_FALSE,0,NULL);

	glEnableVertexAttribArray(HAD_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	//glEnable(GL_CULL_FACE);

	glClearColor(0.0f,0.0f,1.0f,0.0f);

	gOrthographicProjectionMatrix=mat4::identity();

	resize(WIN_WIDTH,WIN_HEIGHT);
}

void resize(int width,int height)
{
	if(height==0)
		height=1;

	glViewport(0,0,(GLsizei)width,(GLsizei)height);

	if(width<=height)
		gOrthographicProjectionMatrix=ortho(-100.0f,100.0f,(-100.0f*(height/width)),(100.0f*(height/width)),-100.0f,100.0f);
	else
		gOrthographicProjectionMatrix=ortho((-100.0f*(width/height)),(100.0f*(width/height)),-100.0f,100.0f,-100.0f,100.0f);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glUseProgram(gShaderProgramObject);

	mat4 modelViewMatrix=mat4::identity();
	mat4 modelViewProjectionMatrix=mat4::identity();

	modelViewProjectionMatrix=gOrthographicProjectionMatrix*modelViewMatrix;

	glUniformMatrix4fv(gMVPUniform,1,GL_FALSE,modelViewProjectionMatrix);

	glBindVertexArray(gVao);

	glDrawArrays(GL_TRIANGLES,0,3);

	glBindVertexArray(0);

	glUseProgram(0);

	glXSwapBuffers(gpDisplay,gWindow);
}

void uninitialize(void)
{
	if(gVao)
	{
		glDeleteVertexArrays(1,&gVao);
		gVao=0;
	}

	if(gVbo)
	{
		glDeleteBuffers(1,&gVbo);
		gVbo=0;
	}

	if(gShaderProgramObject)
	{
		if(gVertexShaderObject)
		{
			glDetachShader(gVertexShaderObject,gShaderProgramObject);
			glDeleteShader(gVertexShaderObject);
			gVertexShaderObject=0;
		}
		if(gFragmentShaderObject)
		{
			glDetachShader(gFragmentShaderObject,gShaderProgramObject);
			glDeleteShader(gFragmentShaderObject);
			gFragmentShaderObject=0;
		}
		glDeleteProgram(gShaderProgramObject);
		gShaderProgramObject=0;

		glUseProgram(0);
	}
	
	GLXContext currentContext=glXGetCurrentContext();
	if(currentContext!=NULL && currentContext==gGLXContext)
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
	if(gpFile)
	{
		fprintf(gpFile,"Log File Is Successfully Closed.\n");
		fclose(gpFile);
		gpFile=NULL;
	}
}
