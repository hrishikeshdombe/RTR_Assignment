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

GLuint gLaUniform_red, gLdUniform_red, gLsUniform_red;
GLuint glightPosition_Uniform_red;

GLuint gLaUniform_blue, gLdUniform_blue, gLsUniform_blue;
GLuint glightPosition_Uniform_blue;

GLuint gLaUniform_green, gLdUniform_green, gLsUniform_green;
GLuint glightPosition_Uniform_green;

GLuint gKaUniform, gKdUniform, gKsUniform;
GLuint gMaterialShininessUniform;

GLuint gToggleShaderUniform;

GLfloat gAngle_Sphere;

GLfloat sphere_vertices[1146];
GLfloat sphere_normals[1146];
GLfloat sphere_textures[764];
unsigned short sphere_elements[2280];
unsigned int gNumVertices,gNumElements;

GLfloat lightAmbient_Red[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse_Red[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat lightSpecular_Red[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat lightPosition_Red[] = { 0.0f,10.0f,10.0f,0.0f };

GLfloat lightAmbient_Blue[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse_Blue[] = { 0.0f,0.0f,1.0f,1.0f };
GLfloat lightSpecular_Blue[] = { 0.0f,0.0f,1.0f,1.0f };
GLfloat lightPosition_Blue[] = { 10.0f,10.0f,-2.0f,0.0f };

GLfloat lightAmbient_Green[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat lightDiffuse_Green[] = { 0.0f,1.0f,0.0f,1.0f };
GLfloat lightSpecular_Green[] = { 0.0f,1.0f,0.0f,1.0f };
GLfloat lightPosition_Green[] = { 10.0f,0.0f,10.0f,0.0f };

GLfloat materialAmbient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat materialDiffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialSpecular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat materialShininess = 50.0f;

mat4 gPerspectiveProjectionMatrix;

bool gbFullscreen=false;
bool gbAnimate = false;
bool gbLight = false;
bool gbIsAKeyPressed = false;
bool gbIsLKeyPressed = false;
bool gbIsTKeyPressed = false;

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

						case XK_T:
						case XK_t:
							if (gbIsTKeyPressed == false)
								gbIsTKeyPressed = true;
							else
								gbIsTKeyPressed = false;
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

	XStoreName(gpDisplay,gWindow,"PP : 3 Rotating Lights On Sphere");

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
		"uniform vec4 u_light_position_red;" \
		"uniform vec4 u_light_position_blue;" \
		"uniform vec4 u_light_position_green;" \
		"uniform int u_toggle_shader;" \
		"uniform vec3 u_La_red;" \
		"uniform vec3 u_Ld_red;" \
		"uniform vec3 u_Ls_red;" \
		"uniform vec3 u_La_blue;" \
		"uniform vec3 u_Ld_blue;" \
		"uniform vec3 u_Ls_blue;" \
		"uniform vec3 u_La_green;" \
		"uniform vec3 u_Ld_green;" \
		"uniform vec3 u_Ls_green;" \
		"uniform vec3 u_Ka;" \
		"uniform vec3 u_Kd;" \
		"uniform vec3 u_Ks;" \
		"uniform float u_material_shininess;" \
		"out vec3 transformed_normals;" \
		"out vec3 light_direction_red;" \
		"out vec3 light_direction_blue;" \
		"out vec3 light_direction_green;" \
		"out vec3 viewer_vector;" \
		"out vec3 phong_ads_color_vertex;" \
		"void main(void)" \
		"{" \
		"if(u_lighting_enabled==1)" \
		"{" \
		"vec4 eye_coordinates = u_view_matrix*u_model_matrix*vPosition;" \
		"transformed_normals = mat3(u_view_matrix*u_model_matrix)*vNormal;" \
		"light_direction_red = vec3(u_light_position_red)-eye_coordinates.xyz;" \
		"light_direction_blue = vec3(u_light_position_blue)-eye_coordinates.xyz;" \
		"light_direction_green = vec3(u_light_position_green)-eye_coordinates.xyz;" \
		"viewer_vector = -eye_coordinates.xyz;" \
		"if(u_toggle_shader == 1)" \
		"{" \
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
		"vec3 normalized_light_direction_red = normalize(light_direction_red);" \
		"vec3 normalized_light_direction_blue = normalize(light_direction_blue);" \
		"vec3 normalized_light_direction_green = normalize(light_direction_green);" \
		"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
		"vec3 ambient = u_La_red * u_Ka + u_La_blue * u_Ka + u_La_green * u_Ka;" \
		"float tn_dot_ld_red = max(dot(normalized_transformed_normals,normalized_light_direction_red),0.0);" \
		"float tn_dot_ld_blue = max(dot(normalized_transformed_normals,normalized_light_direction_blue),0.0);" \
		"float tn_dot_ld_green = max(dot(normalized_transformed_normals,normalized_light_direction_green),0.0);" \
		"vec3 diffuse = u_Ld_red * u_Kd * tn_dot_ld_red + u_Ld_blue * u_Kd * tn_dot_ld_blue + u_Ld_green * u_Kd * tn_dot_ld_green;" \
		"vec3 reflection_vector_red = reflect(-normalized_light_direction_red,normalized_transformed_normals);" \
		"vec3 reflection_vector_blue = reflect(-normalized_light_direction_blue,normalized_transformed_normals);" \
		"vec3 reflection_vector_green = reflect(-normalized_light_direction_green,normalized_transformed_normals);" \
		"vec3 specular = u_Ls_red * u_Ks * pow(max(dot(reflection_vector_red,normalized_viewer_vector),0.0),u_material_shininess) + u_Ls_blue * u_Ks * pow(max(dot(reflection_vector_blue,normalized_viewer_vector),0.0),u_material_shininess) + u_Ls_green * u_Ks * pow(max(dot(reflection_vector_green,normalized_viewer_vector),0.0),u_material_shininess);" \
		"phong_ads_color_vertex = ambient + diffuse + specular;" \
		"}" \
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
		"in vec3 light_direction_red;" \
		"in vec3 light_direction_blue;" \
		"in vec3 light_direction_green;" \
		"in vec3 viewer_vector;" \
		"in vec3 phong_ads_color_vertex;" \
		"out vec4 FragColor;" \
		"uniform vec3 u_La_red;" \
		"uniform vec3 u_Ld_red;" \
		"uniform vec3 u_Ls_red;" \
		"uniform vec3 u_La_blue;" \
		"uniform vec3 u_Ld_blue;" \
		"uniform vec3 u_Ls_blue;" \
		"uniform vec3 u_La_green;" \
		"uniform vec3 u_Ld_green;" \
		"uniform vec3 u_Ls_green;" \
		"uniform vec3 u_Ka;" \
		"uniform vec3 u_Kd;" \
		"uniform vec3 u_Ks;" \
		"uniform float u_material_shininess;" \
		"uniform int u_lighting_enabled;" \
		"uniform int u_toggle_shader;" \
		"void main(void)" \
		"{" \
		"vec3 phong_ads_color;" \
		"if(u_lighting_enabled == 1)" \
		"{" \
		"if(u_toggle_shader == 0)" \
		"{" \
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
		"vec3 normalized_light_direction_red = normalize(light_direction_red);" \
		"vec3 normalized_light_direction_blue = normalize(light_direction_blue);" \
		"vec3 normalized_light_direction_green = normalize(light_direction_green);" \
		"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
		"float tn_dot_ld_red = max(dot(normalized_transformed_normals,normalized_light_direction_red),0.0);" \
		"float tn_dot_ld_blue = max(dot(normalized_transformed_normals,normalized_light_direction_blue),0.0);" \
		"float tn_dot_ld_green = max(dot(normalized_transformed_normals,normalized_light_direction_green),0.0);" \
		"vec3 reflection_vector_red = reflect(-normalized_light_direction_red,normalized_transformed_normals);" \
		"vec3 reflection_vector_blue = reflect(-normalized_light_direction_blue,normalized_transformed_normals);" \
		"vec3 reflection_vector_green = reflect(-normalized_light_direction_green,normalized_transformed_normals);" \
		"vec3 ambient = u_La_red * u_Ka + u_La_blue * u_Ka + u_La_green * u_Ka;" \
		"vec3 diffuse = u_Ld_red * u_Kd * tn_dot_ld_red + u_Ld_blue * u_Kd * tn_dot_ld_blue + u_Ld_green * u_Kd * tn_dot_ld_green;" \
		"vec3 specular = u_Ls_red * u_Ks * pow(max(dot(reflection_vector_red,normalized_viewer_vector),0.0),u_material_shininess) + u_Ls_blue * u_Ks * pow(max(dot(reflection_vector_blue,normalized_viewer_vector),0.0),u_material_shininess) + u_Ls_green * u_Ks * pow(max(dot(reflection_vector_green,normalized_viewer_vector),0.0),u_material_shininess);" \
		"phong_ads_color = ambient + diffuse + specular;" \
		"}" \
		"else" \
		"{" \
		"phong_ads_color = phong_ads_color_vertex;" \
		"}" \
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

	gLaUniform_red = glGetUniformLocation(gShaderProgramObject, "u_La_red");
	gLdUniform_red = glGetUniformLocation(gShaderProgramObject, "u_Ld_red");
	gLsUniform_red = glGetUniformLocation(gShaderProgramObject, "u_Ls_red");

	glightPosition_Uniform_red = glGetUniformLocation(gShaderProgramObject, "u_light_position_red");

	gLaUniform_blue = glGetUniformLocation(gShaderProgramObject, "u_La_blue");
	gLdUniform_blue = glGetUniformLocation(gShaderProgramObject, "u_Ld_blue");
	gLsUniform_blue = glGetUniformLocation(gShaderProgramObject, "u_Ls_blue");

	glightPosition_Uniform_blue = glGetUniformLocation(gShaderProgramObject, "u_light_position_blue");

	gLaUniform_green = glGetUniformLocation(gShaderProgramObject, "u_La_green");
	gLdUniform_green = glGetUniformLocation(gShaderProgramObject, "u_Ld_green");
	gLsUniform_green = glGetUniformLocation(gShaderProgramObject, "u_Ls_green");

	glightPosition_Uniform_green = glGetUniformLocation(gShaderProgramObject, "u_light_position_green");

	gKaUniform = glGetUniformLocation(gShaderProgramObject, "u_Ka");
	gKdUniform = glGetUniformLocation(gShaderProgramObject, "u_Kd");
	gKsUniform = glGetUniformLocation(gShaderProgramObject, "u_Ks");

	gMaterialShininessUniform = glGetUniformLocation(gShaderProgramObject, "u_material_shininess");
	gToggleShaderUniform = glGetUniformLocation(gShaderProgramObject, "u_toggle_shader");

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

	glClearColor(0.0f,0.0f,0.0f,0.0f);

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

	GLfloat X_Coord_Of_Light;
	GLfloat Y_Coord_Of_Light;

	glUseProgram(gShaderProgramObject);

	if (gbLight == true)
	{
		X_Coord_Of_Light = sin(gAngle_Sphere);
		Y_Coord_Of_Light = cos(gAngle_Sphere);

		lightPosition_Blue[0] = X_Coord_Of_Light * 2.0f;
		lightPosition_Blue[1] = Y_Coord_Of_Light * 2.0f;

		lightPosition_Red[1] = X_Coord_Of_Light * 2.0f;
		lightPosition_Red[2] = Y_Coord_Of_Light * 2.0f;

		lightPosition_Green[0] = X_Coord_Of_Light * 2.0f;
		lightPosition_Green[2] = Y_Coord_Of_Light * 2.0f;

		glUniform1i(gLKeyPressedUniform, 1);

		if (gbIsTKeyPressed == true)
		{
			glUniform1i(gToggleShaderUniform, 0);
		}
		else
		{
			glUniform1i(gToggleShaderUniform, 1);
		}

		glUniform3fv(gLaUniform_red, 1, lightAmbient_Red);
		glUniform3fv(gLdUniform_red, 1, lightDiffuse_Red);
		glUniform3fv(gLsUniform_red, 1, lightSpecular_Red);
		glUniform4fv(glightPosition_Uniform_red, 1, lightPosition_Red);

		glUniform3fv(gLaUniform_blue, 1, lightAmbient_Blue);
		glUniform3fv(gLdUniform_blue, 1, lightDiffuse_Blue);
		glUniform3fv(gLsUniform_blue, 1, lightSpecular_Blue);
		glUniform4fv(glightPosition_Uniform_blue, 1, lightPosition_Blue);

		glUniform3fv(gLaUniform_green, 1, lightAmbient_Green);
		glUniform3fv(gLdUniform_green, 1, lightDiffuse_Green);
		glUniform3fv(gLsUniform_green, 1, lightSpecular_Green);
		glUniform4fv(glightPosition_Uniform_green, 1, lightPosition_Green);

		glUniform3fv(gKaUniform, 1, materialAmbient);
		glUniform3fv(gKdUniform, 1, materialDiffuse);
		glUniform3fv(gKsUniform, 1, materialSpecular);
		glUniform1f(gMaterialShininessUniform, materialShininess);
	}
	else
	{
		glUniform1i(gLKeyPressedUniform, 0);
	}

	mat4 modelMatrix=mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 rotationMatrix=mat4::identity();
	mat4 scaleMatrix=mat4::identity();

	modelMatrix=translate(0.0f,0.0f,-2.0f);

	//scaleMatrix=scale(0.80f,0.80f,0.80f);
	//modelMatrix=modelMatrix*scaleMatrix;

	rotationMatrix=rotate(gAngle_Sphere,0.0f,1.0f,0.0f);
	modelMatrix=modelMatrix*rotationMatrix;

	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);

	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);

	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	//Square
	glBindVertexArray(gVao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,gVbo_Elements);
	glDrawElements(GL_TRIANGLES,gNumElements,GL_UNSIGNED_SHORT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glBindVertexArray(0);

	glUseProgram(0);

	glXSwapBuffers(gpDisplay,gWindow);
}

void update(void)
{
	gAngle_Sphere=gAngle_Sphere+0.005f;
	if(gAngle_Sphere>=360.0f)
		gAngle_Sphere=gAngle_Sphere-360.0f;
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
