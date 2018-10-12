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
#include"../../Resources/include/vmath.h"
#include"../../Resources/include/Sphere.h"

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

GLuint gVao_Sphere;
GLuint gVbo_Pos,gVbo_Normal,gVbo_Elements;

GLuint gModelMatrixUniform, gViewMatrixUniform, gProjectionMatrixUniform;
GLuint gLKeyPressedUniform;

GLuint gLaUniform_White, gLdUniform_White, gLsUniform_White;
GLuint glightPosition_Uniform_White;

GLuint gKaUniform, gKdUniform, gKsUniform;
GLuint gMaterialShininessUniform;

GLuint gToggleShaderUniform;

GLfloat gAngle_Sphere;

GLfloat sphere_vertices[1146];
GLfloat sphere_normals[1146];
GLfloat sphere_textures[764];
unsigned short sphere_elements[2280];
unsigned int gNumVertices,gNumElements;

GLfloat lightAmbient_White[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse_White[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightSpecular_White[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat lightPosition_White[] = { 0.0f,0.0f,100.0f,0.0f };

GLfloat material_ambient_1[] = { 0.0215f,0.1745f,0.0215f,1.0f };
GLfloat material_diffuse_1[] = { 0.07568f,0.61424f,0.07568f,1.0f };
GLfloat material_specular_1[] = { 0.633f,0.727811f,0.633f,1.0f };
GLfloat material_shininess_1 = 0.6f * 128.0f;

GLfloat material_ambient_2[] = { 0.135f,0.2225f,0.1575f,1.0f };
GLfloat material_diffuse_2[] = { 0.54f,0.89f,0.63f,1.0f };
GLfloat material_specular_2[] = { 0.316228f,0.316228f,0.316228f,1.0f };
GLfloat material_shininess_2 = 0.1f * 128.0f;

GLfloat material_ambient_3[] = { 0.05375f,0.05f,0.06625f,1.0f };
GLfloat material_diffuse_3[] = { 0.18275f,0.17f,0.22525f,1.0f };
GLfloat material_specular_3[] = { 0.332741f,0.328634f,0.346435f,1.0f };
GLfloat material_shininess_3 = 0.3f * 128.0f;

GLfloat material_ambient_4[] = { 0.25f,0.20725f,0.20725f,1.0f };
GLfloat material_diffuse_4[] = { 1.0f,0.829f,0.829f,1.0f };
GLfloat material_specular_4[] = { 0.296648f,0.296648f,0.296648f,1.0f };
GLfloat material_shininess_4 = 0.088f * 128.0f;

GLfloat material_ambient_5[] = { 0.1745f,0.01175f,0.01175f,1.0f };
GLfloat material_diffuse_5[] = { 0.61424f,0.04136f,0.04136f,1.0f };
GLfloat material_specular_5[] = { 0.727811f,0.626959f,0.626959f,1.0f };
GLfloat material_shininess_5 = 0.6f * 128.0f;

GLfloat material_ambient_6[] = { 0.1f,0.18725f,0.1745f,1.0f };
GLfloat material_diffuse_6[] = { 0.396f,0.74151f,0.69102f,1.0f };
GLfloat material_specular_6[] = { 0.297254f,0.30829f,0.306678f,1.0f };
GLfloat material_shininess_6 = 0.1f * 128.0f;

GLfloat material_ambient_7[] = { 0.329412f,0.223529f,0.027451f,1.0f };
GLfloat material_diffuse_7[] = { 0.780392f,0.568627f,0.113725f,1.0f };
GLfloat material_specular_7[] = { 0.992157f,0.941176f,0.807843f,1.0f };
GLfloat material_shininess_7 = 0.21794872f * 128.0f;

GLfloat material_ambient_8[] = { 0.2125f,0.1275f,0.054f,1.0f };
GLfloat material_diffuse_8[] = { 0.714f,0.4284f,0.18144f,1.0f };
GLfloat material_specular_8[] = { 0.393548f,0.271906f,0.166721f,1.0f };
GLfloat material_shininess_8 = 0.2f * 128.0f;

GLfloat material_ambient_9[] = { 0.25f,0.25f,0.25f,1.0f };
GLfloat material_diffuse_9[] = { 0.4f,0.4f,0.4f,1.0f };
GLfloat material_specular_9[] = { 0.774597f,0.774597f,0.774597f,1.0f };
GLfloat material_shininess_9 = 0.6f * 128.0f;

GLfloat material_ambient_10[] = { 0.19125f,0.0735f,0.0225f,1.0f };
GLfloat material_diffuse_10[] = { 0.7038f,0.27048f,0.0828f,1.0f };
GLfloat material_specular_10[] = { 0.256777f,0.137622f,0.086014f,1.0f };
GLfloat material_shininess_10 = 0.1f * 128.0f;

GLfloat material_ambient_11[] = { 0.24725f,0.1995f,0.0745f,1.0f };
GLfloat material_diffuse_11[] = { 0.75164f,0.60648f,0.22648f,1.0f };
GLfloat material_specular_11[] = { 0.628281f,0.555802f,0.366065f,1.0f };
GLfloat material_shininess_11 = 0.4f * 128.0f;

GLfloat material_ambient_12[] = { 0.19225f,0.19225f,0.19225f,1.0f };
GLfloat material_diffuse_12[] = { 0.50754f,0.50754f,0.50754f,1.0f };
GLfloat material_specular_12[] = { 0.508273f,0.508273f,0.508273f,1.0f };
GLfloat material_shininess_12 = 0.4f * 128.0f;

GLfloat material_ambient_13[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat material_diffuse_13[] = { 0.01f,0.01f,0.01f,1.0f };
GLfloat material_specular_13[] = { 0.5f,0.5f,0.5f,1.0f };
GLfloat material_shininess_13 = 0.25f * 128.0f;

GLfloat material_ambient_14[] = { 0.0f,0.1f,0.06f,1.0f };
GLfloat material_diffuse_14[] = { 0.0f,0.50980392f,0.50980392f,1.0f };
GLfloat material_specular_14[] = { 0.50196078f,0.50196078f,0.50196078f,1.0f };
GLfloat material_shininess_14 = 0.25f * 128.0f;

GLfloat material_ambient_15[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat material_diffuse_15[] = { 0.1f,0.35f,0.1f,1.0f };
GLfloat material_specular_15[] = { 0.45f,0.55f,0.45f,1.0f };
GLfloat material_shininess_15 = 0.25f * 128.0f;

GLfloat material_ambient_16[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat material_diffuse_16[] = { 0.5f,0.0f,0.0f,1.0f };
GLfloat material_specular_16[] = { 0.7f,0.6f,0.6f,1.0f };
GLfloat material_shininess_16 = 0.25f * 128.0f;

GLfloat material_ambient_17[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat material_diffuse_17[] = { 0.55f,0.55f,0.55f,1.0f };
GLfloat material_specular_17[] = { 0.70f,0.70f,0.70f,1.0f };
GLfloat material_shininess_17 = 0.25f * 128.0f;

GLfloat material_ambient_18[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat material_diffuse_18[] = { 0.5f,0.5f,0.0f,1.0f };
GLfloat material_specular_18[] = { 0.6f,0.6f,0.5f,1.0f };
GLfloat material_shininess_18 = 0.25f * 128.0f;

GLfloat material_ambient_19[] = { 0.02f,0.02f,0.02f,1.0f };
GLfloat material_diffuse_19[] = { 0.1f,0.1f,0.1f,1.0f };
GLfloat material_specular_19[] = { 0.4f,0.4f,0.4f,1.0f };
GLfloat material_shininess_19 = 0.078125f * 128.0f;

GLfloat material_ambient_20[] = { 0.0f,0.05f,0.05f,1.0f };
GLfloat material_diffuse_20[] = { 0.4f,0.5f,0.5f,1.0f };
GLfloat material_specular_20[] = { 0.04f,0.7f,0.7f,1.0f };
GLfloat material_shininess_20 = 0.078125f * 128.0f;

GLfloat material_ambient_21[] = { 0.0f,0.05f,0.0f,1.0f };
GLfloat material_diffuse_21[] = { 0.4f,0.5f,0.4f,1.0f };
GLfloat material_specular_21[] = { 0.04f,0.7f,0.04f,1.0f };
GLfloat material_shininess_21 = 0.078125f * 128.0f;

GLfloat material_ambient_22[] = { 0.05f,0.0f,0.0f,1.0f };
GLfloat material_diffuse_22[] = { 0.5f,0.4f,0.4f,1.0f };
GLfloat material_specular_22[] = { 0.7f,0.04f,0.04f,1.0f };
GLfloat material_shininess_22 = 0.078125f * 128.0f;

GLfloat material_ambient_23[] = { 0.05f,0.05f,0.05f,1.0f };
GLfloat material_diffuse_23[] = { 0.5f,0.5f,0.5f,1.0f };
GLfloat material_specular_23[] = { 0.7f,0.7f,0.7f,1.0f };
GLfloat material_shininess_23 = 0.078125f * 128.0f;

GLfloat material_ambient_24[] = { 0.05f,0.05f,0.0f,1.0f };
GLfloat material_diffuse_24[] = { 0.5f,0.5f,0.4f,1.0f };
GLfloat material_specular_24[] = { 0.7f,0.7f,0.04f,1.0f };
GLfloat material_shininess_24 = 0.078125f * 128.0f;

mat4 gPerspectiveProjectionMatrix;

mat4 gmodelMatrix;
mat4 gviewMatrix;
mat4 gscaleMatrix;

bool gbFullscreen=false;
bool gbAnimate = false;
bool gbLight = false;
bool gbIsAKeyPressed = false;
bool gbIsLKeyPressed = false;
bool gbIsXKeyPressed = false;
bool gbIsYKeyPressed = false;
bool gbIsZKeyPressed = false;

int giHeight, giWidth;

int main(int argc, char *argv[])
{
	void CreateWindow(void);
	void ToggleFullscreen(void);
	void initialize(void);
	void resize(int,int);
	void display(void);
	void update(void);
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

						case XK_A:
						case XK_a:
							if(gbIsAKeyPressed == false)
							{
								gbIsAKeyPressed = true;
								gbAnimate = true;
							}
							else
							{
								gbIsAKeyPressed = false;
								gbAnimate = false;
							}
							break;

						case XK_L:
						case XK_l:
							if(gbIsLKeyPressed == false)
							{
								gbIsLKeyPressed = true;
								gbLight = true;
							}
							else
							{
								gbIsLKeyPressed = false;
								gbLight = false;
							}
							break;

						case XK_X:
						case XK_x:
							if (gbIsXKeyPressed == false)
							{
								gbIsXKeyPressed = true;
								gbIsYKeyPressed = false;
								gbIsZKeyPressed = false;
							}
							else
								gbIsXKeyPressed = false;
							break;

						case XK_Y:
						case XK_y:
							if (gbIsYKeyPressed == false)
							{
								gbIsYKeyPressed = true;
								gbIsXKeyPressed = false;
								gbIsZKeyPressed = false;
							}
							else
								gbIsYKeyPressed = false;
							break;

						case XK_Z:
						case XK_z:
							if (gbIsZKeyPressed == false)
							{
								gbIsZKeyPressed = true;
								gbIsXKeyPressed = false;
								gbIsYKeyPressed = false;
							}
							else
								gbIsZKeyPressed = false;
							break;

						default:
							gbIsZKeyPressed = false;
							gbIsXKeyPressed = false;
							gbIsYKeyPressed = false;
							lightPosition_White[0] = 0.0f;
							lightPosition_White[1] = 0.0f;
							lightPosition_White[2] = 100.0f;
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
					giWidth=winWidth=event.xconfigure.width;
					giHeight=winHeight=event.xconfigure.height;
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
		if(gbAnimate==true)
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

	XStoreName(gpDisplay,gWindow,"PP : 24 Sphere With Material");

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

	getSphereVertexData(sphere_vertices,sphere_normals,sphere_textures,sphere_elements);

	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();
	
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
		"#version 450" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform int u_lighting_enabled;" \
		"uniform vec4 u_light_position_White;" \
		"out vec3 transformed_normals;" \
		"out vec3 light_direction_White;" \
		"out vec3 viewer_vector;" \
		"void main(void)" \
		"{" \
		"if(u_lighting_enabled==1)" \
		"{" \
		"vec4 eye_coordinates = u_view_matrix*u_model_matrix*vPosition;" \
		"transformed_normals = mat3(u_view_matrix*u_model_matrix)*vNormal;" \
		"light_direction_White = vec3(u_light_position_White)-eye_coordinates.xyz;" \
		"viewer_vector = -eye_coordinates.xyz;" \
		"}" \
		"gl_Position = u_projection_matrix*u_view_matrix*u_model_matrix*vPosition;" \
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
		"#version 450" \
		"\n" \
		"in vec3 transformed_normals;" \
		"in vec3 light_direction_White;" \
		"in vec3 viewer_vector;" \
		"out vec4 FragColor;" \
		"uniform vec3 u_La_White;" \
		"uniform vec3 u_Ld_White;" \
		"uniform vec3 u_Ls_White;" \
		"uniform vec3 u_Ka;" \
		"uniform vec3 u_Kd;" \
		"uniform vec3 u_Ks;" \
		"uniform float u_material_shininess;" \
		"uniform int u_lighting_enabled;" \
		"void main(void)" \
		"{" \
		"vec3 phong_ads_color;" \
		"if(u_lighting_enabled == 1)" \
		"{" \
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
		"vec3 normalized_light_direction_White = normalize(light_direction_White);" \
		"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
		"float tn_dot_ld_White = max(dot(normalized_transformed_normals,normalized_light_direction_White),0.0);" \
		"vec3 reflection_vector_White = reflect(-normalized_light_direction_White,normalized_transformed_normals);" \
		"vec3 ambient = u_La_White * u_Ka;" \
		"vec3 diffuse = u_Ld_White * u_Kd * tn_dot_ld_White;" \
		"vec3 specular = u_Ls_White * u_Ks * pow(max(dot(reflection_vector_White,normalized_viewer_vector),0.0),u_material_shininess);" \
		"phong_ads_color = ambient + diffuse + specular;" \
		"}" \
		"else" \
		"{" \
		"phong_ads_color = vec3(1.0,1.0,1.0);" \
		"}" \
		"FragColor = vec4(phong_ads_color,1.0);" \
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

	glBindAttribLocation(gShaderProgramObject,HAD_ATTRIBUTE_NORMAL,"vNormal");

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

	gModelMatrixUniform = glGetUniformLocation(gShaderProgramObject, "u_model_matrix");
	gViewMatrixUniform = glGetUniformLocation(gShaderProgramObject, "u_view_matrix");
	gProjectionMatrixUniform = glGetUniformLocation(gShaderProgramObject, "u_projection_matrix");

	gLKeyPressedUniform = glGetUniformLocation(gShaderProgramObject, "u_lighting_enabled");

	gLaUniform_White = glGetUniformLocation(gShaderProgramObject, "u_La_White");
	gLdUniform_White = glGetUniformLocation(gShaderProgramObject, "u_Ld_White");
	gLsUniform_White = glGetUniformLocation(gShaderProgramObject, "u_Ls_White");

	glightPosition_Uniform_White = glGetUniformLocation(gShaderProgramObject, "u_light_position_White");

	gKaUniform = glGetUniformLocation(gShaderProgramObject, "u_Ka");
	gKdUniform = glGetUniformLocation(gShaderProgramObject, "u_Kd");
	gKsUniform = glGetUniformLocation(gShaderProgramObject, "u_Ks");

	gMaterialShininessUniform = glGetUniformLocation(gShaderProgramObject, "u_material_shininess");

	/************Sphere*************/
	glGenVertexArrays(1,&gVao_Sphere);
	glBindVertexArray(gVao_Sphere);

	/***********VBO Position Sphere*********/
	glGenBuffers(1,&gVbo_Pos);
	glBindBuffer(GL_ARRAY_BUFFER,gVbo_Pos);
	glBufferData(GL_ARRAY_BUFFER,sizeof(sphere_vertices),sphere_vertices,GL_STATIC_DRAW);

	glVertexAttribPointer(HAD_ATTRIBUTE_POSITION,3,GL_FLOAT,GL_FALSE,0,NULL);

	glEnableVertexAttribArray(HAD_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	/************VBO Normals Sphere***********/
	glGenBuffers(1,&gVbo_Normal);
	glBindBuffer(GL_ARRAY_BUFFER,gVbo_Normal);
	glBufferData(GL_ARRAY_BUFFER,sizeof(sphere_normals),sphere_normals,GL_STATIC_DRAW);

	glVertexAttribPointer(HAD_ATTRIBUTE_NORMAL,3,GL_FLOAT,GL_FALSE,0,NULL);

	glEnableVertexAttribArray(HAD_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	/**************VBO Elements************/
	glGenBuffers(1,&gVbo_Elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,gVbo_Elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(sphere_elements),sphere_elements,GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glBindVertexArray(0);

	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.25f,0.25f,0.25f,0.0f);

	gPerspectiveProjectionMatrix=mat4::identity();

	resize(WIN_WIDTH,WIN_HEIGHT);
}

void resize(int width,int height)
{
	if(height==0)
		height=1;

	glViewport(0,0,(GLsizei)width,(GLsizei)height);

	gPerspectiveProjectionMatrix=perspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	void Draw_Sphere_1(void);
	void Draw_Sphere_2(void);
	void Draw_Sphere_3(void);
	void Draw_Sphere_4(void);
	void Draw_Sphere_5(void);
	void Draw_Sphere_6(void);
	void Draw_Sphere_7(void);
	void Draw_Sphere_8(void);
	void Draw_Sphere_9(void);
	void Draw_Sphere_10(void);
	void Draw_Sphere_11(void);
	void Draw_Sphere_12(void);
	void Draw_Sphere_13(void);
	void Draw_Sphere_14(void);
	void Draw_Sphere_15(void);
	void Draw_Sphere_16(void);
	void Draw_Sphere_17(void);
	void Draw_Sphere_18(void);
	void Draw_Sphere_19(void);
	void Draw_Sphere_20(void);
	void Draw_Sphere_21(void);
	void Draw_Sphere_22(void);
	void Draw_Sphere_23(void);
	void Draw_Sphere_24(void);

	GLfloat X_Coord_Of_Light;
	GLfloat Y_Coord_Of_Light;

	//Use Shader Program Object
	glUseProgram(gShaderProgramObject);

	if (gbLight == true)
	{
		X_Coord_Of_Light = cos(gAngle_Sphere);
		Y_Coord_Of_Light = sin(gAngle_Sphere);

		if (gbIsXKeyPressed == true)
		{
			lightPosition_White[1] = X_Coord_Of_Light * 100.0f;
			lightPosition_White[2] = Y_Coord_Of_Light * 100.0f;
			lightPosition_White[0] = 0.0f;
		}
		else if (gbIsYKeyPressed == true)
		{
			lightPosition_White[0] = X_Coord_Of_Light * 100.0f;
			lightPosition_White[2] = Y_Coord_Of_Light * 100.0f;
			lightPosition_White[1] = 0.0f;
		}
		else if (gbIsZKeyPressed == true)
		{
			lightPosition_White[0] = X_Coord_Of_Light * 100.0f;
			lightPosition_White[1] = Y_Coord_Of_Light * 100.0f;
			lightPosition_White[2] = -2.0f;
		}
		
		glUniform1i(gLKeyPressedUniform, 1);

		glUniform3fv(gLaUniform_White, 1, lightAmbient_White);
		glUniform3fv(gLdUniform_White, 1, lightDiffuse_White);
		glUniform3fv(gLsUniform_White, 1, lightSpecular_White);
		glUniform4fv(glightPosition_Uniform_White, 1, lightPosition_White);
	}
	else
	{
		glUniform1i(gLKeyPressedUniform, 0);
	}

	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();

	modelMatrix = translate(0.0f, 0.0f, -4.0f);

	scaleMatrix = scale((GLfloat)giHeight/(GLfloat)giWidth, 1.0f, 1.0f);

	//modelMatrix = modelMatrix * scaleMatrix;

	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);

	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);

	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glViewport(0, giHeight * 5 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_1();
	glViewport(0, giHeight * 4 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_2();
	glViewport(0, giHeight * 3 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_3();
	glViewport(0, giHeight * 2 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_4();
	glViewport(0, giHeight * 1 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_5();
	glViewport(0, -30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_6();
	glViewport(giWidth / 4, giHeight * 5 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_7();
	glViewport(giWidth / 4, giHeight * 4 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_8();
	glViewport(giWidth / 4, giHeight * 3 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_9();
	glViewport(giWidth / 4, giHeight * 2 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_10();
	glViewport(giWidth / 4, giHeight * 1 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_11();
	glViewport(giWidth / 4, -30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_12();
	glViewport(giWidth / 2, giHeight * 5 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_13();
	glViewport(giWidth / 2, giHeight * 4 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_14();
	glViewport(giWidth / 2, giHeight * 3 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_15();
	glViewport(giWidth / 2, giHeight * 2 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_16();
	glViewport(giWidth / 2, giHeight * 1 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_17();
	glViewport(giWidth / 2, -30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_18();
	glViewport((giWidth /2) + (giWidth /4), giHeight * 5 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_19();
	glViewport((giWidth / 2) + (giWidth / 4), giHeight * 4 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_20();
	glViewport((giWidth / 2) + (giWidth / 4), giHeight * 3 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_21();
	glViewport((giWidth / 2) + (giWidth / 4), giHeight * 2 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_22();
	glViewport((giWidth / 2) + (giWidth / 4), giHeight * 1 / 6 - 30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_23();
	glViewport((giWidth / 2) + (giWidth / 4), -30, (GLsizei)giWidth / 4, (GLsizei)giHeight / 4);
	Draw_Sphere_24();

	glBindVertexArray(0);

	glUseProgram(0);

	glXSwapBuffers(gpDisplay,gWindow);
}

void update(void)
{
	gAngle_Sphere=gAngle_Sphere+0.01f;
	if(gAngle_Sphere>=360.0f)
		gAngle_Sphere=gAngle_Sphere-360.0f;
}

void Draw_Sphere_1(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_1);
		glUniform3fv(gKdUniform, 1, material_diffuse_1);
		glUniform3fv(gKsUniform, 1, material_specular_1);
		glUniform1f(gMaterialShininessUniform, material_shininess_1);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_2(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_2);
		glUniform3fv(gKdUniform, 1, material_diffuse_2);
		glUniform3fv(gKsUniform, 1, material_specular_2);
		glUniform1f(gMaterialShininessUniform, material_shininess_2);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_3(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_3);
		glUniform3fv(gKdUniform, 1, material_diffuse_3);
		glUniform3fv(gKsUniform, 1, material_specular_3);
		glUniform1f(gMaterialShininessUniform, material_shininess_3);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_4(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_4);
		glUniform3fv(gKdUniform, 1, material_diffuse_4);
		glUniform3fv(gKsUniform, 1, material_specular_4);
		glUniform1f(gMaterialShininessUniform, material_shininess_4);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_5(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_5);
		glUniform3fv(gKdUniform, 1, material_diffuse_5);
		glUniform3fv(gKsUniform, 1, material_specular_5);
		glUniform1f(gMaterialShininessUniform, material_shininess_5);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_6(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_6);
		glUniform3fv(gKdUniform, 1, material_diffuse_6);
		glUniform3fv(gKsUniform, 1, material_specular_6);
		glUniform1f(gMaterialShininessUniform, material_shininess_6);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_7(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_7);
		glUniform3fv(gKdUniform, 1, material_diffuse_7);
		glUniform3fv(gKsUniform, 1, material_specular_7);
		glUniform1f(gMaterialShininessUniform, material_shininess_7);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_8(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_8);
		glUniform3fv(gKdUniform, 1, material_diffuse_8);
		glUniform3fv(gKsUniform, 1, material_specular_8);
		glUniform1f(gMaterialShininessUniform, material_shininess_8);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_9(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_9);
		glUniform3fv(gKdUniform, 1, material_diffuse_9);
		glUniform3fv(gKsUniform, 1, material_specular_9);
		glUniform1f(gMaterialShininessUniform, material_shininess_9);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_10(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_10);
		glUniform3fv(gKdUniform, 1, material_diffuse_10);
		glUniform3fv(gKsUniform, 1, material_specular_10);
		glUniform1f(gMaterialShininessUniform, material_shininess_10);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_11(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_11);
		glUniform3fv(gKdUniform, 1, material_diffuse_11);
		glUniform3fv(gKsUniform, 1, material_specular_11);
		glUniform1f(gMaterialShininessUniform, material_shininess_11);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}


void Draw_Sphere_12(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_12);
		glUniform3fv(gKdUniform, 1, material_diffuse_12);
		glUniform3fv(gKsUniform, 1, material_specular_12);
		glUniform1f(gMaterialShininessUniform, material_shininess_12);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_13(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_13);
		glUniform3fv(gKdUniform, 1, material_diffuse_13);
		glUniform3fv(gKsUniform, 1, material_specular_13);
		glUniform1f(gMaterialShininessUniform, material_shininess_13);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_14(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_14);
		glUniform3fv(gKdUniform, 1, material_diffuse_14);
		glUniform3fv(gKsUniform, 1, material_specular_14);
		glUniform1f(gMaterialShininessUniform, material_shininess_14);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}


void Draw_Sphere_15(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_15);
		glUniform3fv(gKdUniform, 1, material_diffuse_15);
		glUniform3fv(gKsUniform, 1, material_specular_15);
		glUniform1f(gMaterialShininessUniform, material_shininess_15);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_16(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_16);
		glUniform3fv(gKdUniform, 1, material_diffuse_16);
		glUniform3fv(gKsUniform, 1, material_specular_16);
		glUniform1f(gMaterialShininessUniform, material_shininess_16);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_17(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_17);
		glUniform3fv(gKdUniform, 1, material_diffuse_17);
		glUniform3fv(gKsUniform, 1, material_specular_17);
		glUniform1f(gMaterialShininessUniform, material_shininess_17);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_18(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_18);
		glUniform3fv(gKdUniform, 1, material_diffuse_18);
		glUniform3fv(gKsUniform, 1, material_specular_18);
		glUniform1f(gMaterialShininessUniform, material_shininess_18);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_19(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_19);
		glUniform3fv(gKdUniform, 1, material_diffuse_19);
		glUniform3fv(gKsUniform, 1, material_specular_19);
		glUniform1f(gMaterialShininessUniform, material_shininess_19);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_20(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_20);
		glUniform3fv(gKdUniform, 1, material_diffuse_20);
		glUniform3fv(gKsUniform, 1, material_specular_20);
		glUniform1f(gMaterialShininessUniform, material_shininess_20);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_21(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_21);
		glUniform3fv(gKdUniform, 1, material_diffuse_21);
		glUniform3fv(gKsUniform, 1, material_specular_21);
		glUniform1f(gMaterialShininessUniform, material_shininess_21);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_22(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_22);
		glUniform3fv(gKdUniform, 1, material_diffuse_22);
		glUniform3fv(gKsUniform, 1, material_specular_22);
		glUniform1f(gMaterialShininessUniform, material_shininess_22);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_23(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_23);
		glUniform3fv(gKdUniform, 1, material_diffuse_23);
		glUniform3fv(gKsUniform, 1, material_specular_23);
		glUniform1f(gMaterialShininessUniform, material_shininess_23);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}

void Draw_Sphere_24(void)
{
	if (gbLight == true)
	{
		glUniform3fv(gKaUniform, 1, material_ambient_24);
		glUniform3fv(gKdUniform, 1, material_diffuse_24);
		glUniform3fv(gKsUniform, 1, material_specular_24);
		glUniform1f(gMaterialShininessUniform, material_shininess_24);
	}

	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Elements);
}


void uninitialize(void)
{
	if(gVao_Sphere)
	{
		glDeleteVertexArrays(1,&gVao_Sphere);
		gVao_Sphere=0;
	}

	if(gVbo_Pos)
	{
		glDeleteBuffers(1,&gVbo_Pos);
		gVbo_Pos=0;
	}

	if(gVbo_Normal)
	{
		glDeleteBuffers(1,&gVbo_Normal);
		gVbo_Normal=0;
	}

	if(gVbo_Elements)
	{
		glDeleteBuffers(1,&gVbo_Elements);
		gVbo_Elements=0;
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
