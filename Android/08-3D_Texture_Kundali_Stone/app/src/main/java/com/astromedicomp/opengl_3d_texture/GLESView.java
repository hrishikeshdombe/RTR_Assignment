package com.astromedicomp.opengl_3d_texture;

import android.content.Context; //For drawing context related
import android.opengl.GLSurfaceView; //For OpenGL Surface View and all related
import javax.microedition.khronos.opengles.GL10; //For OpenGLES 1.0 needed as param type GL10
import javax.microedition.khronos.egl.EGLConfig; //For EGLConfig needed as param type EGLConfig
import android.opengl.GLES32; // For OpenGLES 3.2
//import android.view.Gravity;
import android.view.MotionEvent; // For "MotionEvent"
import android.view.GestureDetector; // For GestureDetector
import android.view.GestureDetector.OnGestureListener; // OnGestureListener
import android.view.GestureDetector.OnDoubleTapListener; // OnDoubleTapListener

//For vbo
import java.nio.ByteBuffer;//nio For non-blocking I/O
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

//For Matrix math
import android.opengl.Matrix;

import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.opengl.GLUtils;

//A View for OpenGLES3 graphics which also receives touch events
public class GLESView extends GLSurfaceView implements GLSurfaceView.Renderer, OnGestureListener, OnDoubleTapListener
{
	//Class variables or class fields
	private final Context context;

	private GestureDetector gestureDetector;

	//Shader Object
	private int vertexShaderObject;
	private int fragmentShaderObject;

	//Program Object
	private int shaderProgramObject;

	//As there are no pointers in java, when we want to use any variable as out parameter
	//we use array with only one member.
	//Vertex Array Object
	private int[] vao_pyramid = new int[1];
	private int[] vao_cube = new int[1];
	private int[] vbo_position = new int[1];
	private int[] vbo_texture = new int[1];

	private float anglePyramid,angleCube;

	private int mvpUniform;
	private int texture0_sampler_uniform;

	private int[] texture_kundali = new int[1];
	private int[] texture_stone = new int[1];

	//4 x 4 matrix
	private float perspectiveProjectionMatrix[] = new float[16];

	public GLESView(Context drawingContext)
	{
		super(drawingContext);

		context = drawingContext;

		//Accordingly set EGLContext to current supported version of OpenGL-ES
		setEGLContextClientVersion(3);

		//Set Renderer for drawing on GLSurfaceView
		setRenderer(this);

		//Render the view only when there is a change in drawing data
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

		//this means 'handle' i.e who is going to handle 
		gestureDetector = new GestureDetector(context,this,null,false);
	
		//this means 'handle' i.e who is going to handle
		gestureDetector.setOnDoubleTapListener(this);
	}

	//overriden method of GLSurfaceView.Renderer(Init Code)
	@Override
	public void onSurfaceCreated(GL10 gl,EGLConfig config)
	{
		//OpenGL-ES version check
		String version = gl.glGetString(GL10.GL_VERSION);
		System.out.println("HAD: OpenGL-ES Version"+version);//"+" for concatination

		String glslVersion=gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION);
		System.out.println("HAD: GLSL Version"+glslVersion);

		initialize(gl);
	}

	//overriden method of GLSurfaceView.Renderer(Change Size Code)
	@Override
	public void onSurfaceChanged(GL10 unused,int width,int height)
	{
		resize(width,height);
	}

	//overriden method of GLSurfaceView.Renderer(Rendering Code)
	@Override
	public void onDrawFrame(GL10 unused)
	{
		draw();
	}

	//Handling 'onTouchEvent' Is The Most IMPORTANT, Because It Triggers All Gesture And Events
	@Override
	public boolean onTouchEvent(MotionEvent e)
	{
		//code
		int eventaction=e.getAction();
		if(!gestureDetector.onTouchEvent(e))
			super.onTouchEvent(e);
		return(true);
	}

	//abstract method from OnDoubleTapListener so must be implemented
	@Override
	public boolean onDoubleTap(MotionEvent e)
	{
		return(true);
	}

	//abstract method from OnDoubleTapListener so must be implemented
	@Override
	public boolean onDoubleTapEvent(MotionEvent e)
	{
		//Do not write any code here because already written 'onDOubleTap'
		return(true);
	}

	//abstract method from OnDoubleTapListener so must be implemented
	@Override
	public boolean onSingleTapConfirmed(MotionEvent e)
	{
		return(true);
	}

	//abstract method from OnGestureListener so must be implemented
	@Override
	public boolean onDown(MotionEvent e)
	{
		//Do not write any code here because already written 'onSingleTapConfirmed'
		return(true);
	}

	//abstract method from OnGestureListener so must be implemented
	@Override
	public boolean onFling(MotionEvent e1,MotionEvent e2,float velocityX,float velocityY)
	{
		return(true);
	}

	//abstract method from OnGestureListener so must be implemented
	@Override
	public void onLongPress(MotionEvent e)
	{
	}

	//abstract method from OnGestureListener so must be implemented
	@Override
	public boolean onScroll(MotionEvent e1,MotionEvent e2,float distanceX,float distanceY)
	{
		uninitialize();
		System.exit(0);
		return(true);
	}

	//abstract method from OnGestureListener so must be implemented
	@Override
	public void onShowPress(MotionEvent e)
	{

	}

	//abstract method from OnGestureListener so must be implemented
	@Override
	public boolean onSingleTapUp(MotionEvent e)
	{
		return(true);
	}

	private void initialize(GL10 gl)
	{
		//Vertex Shader
		//Create Vertex Shader

		vertexShaderObject=GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

		//vertex shader source code
		final String vertexShaderSourceCode = String.format
		(
			"#version 320 es"+
			"\n"+
			"in vec4 vPosition;"+
			"in vec2 vTexture0_Coord;"+
			"uniform mat4 u_mvp_matrix;"+
			"out vec2 out_texture0_coord;"+
			"void main(void)"+
			"{"+
			"gl_Position = u_mvp_matrix * vPosition;"+
			"out_texture0_coord=vTexture0_Coord;"+
			"}"
		);

		//Provide Source Code to Shader
		GLES32.glShaderSource(vertexShaderObject,vertexShaderSourceCode);

		//Compile Shader & Check errors
		GLES32.glCompileShader(vertexShaderObject);
		int[] iShaderCompiledStatus = new int[1];
		int[] iInfoLogLength = new int[1];
		String szInfoLog = null; 
		GLES32.glGetShaderiv(vertexShaderObject,GLES32.GL_COMPILE_STATUS,iShaderCompiledStatus,0);
		if(iShaderCompiledStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(vertexShaderObject,GLES32.GL_INFO_LOG_LENGTH,iInfoLogLength,0);
			if(iInfoLogLength[0] > 0)
			{
				szInfoLog = GLES32.glGetShaderInfoLog(vertexShaderObject);
				System.out.println("HAD: Vertex Shader Compilation Log = "+szInfoLog);
				uninitialize();
				System.exit(0);
			}
		}

		//Fragment Shader
		//Create Shader
		fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

		//Fragment Shader Source Code
		final String fragmentShaderSourceCode = String.format
		(
			"#version 320 es"+
			"\n"+
			"precision highp float;"+
			"in vec2 out_texture0_coord;"+
			"out vec4 FragColor;"+
			"uniform highp sampler2D u_texture0_sampler;"+
			"void main(void)"+
			"{"+
			"FragColor = texture(u_texture0_sampler , out_texture0_coord);"+
			"}"
		);

		GLES32.glShaderSource(fragmentShaderObject,fragmentShaderSourceCode);

		//Compile Shader and check for errors
		GLES32.glCompileShader(fragmentShaderObject);
		iShaderCompiledStatus[0] = 0;//reinitialize
		iInfoLogLength[0] = 0;//reinitialize
		szInfoLog=null;//reinitialize

		GLES32.glGetShaderiv(fragmentShaderObject,GLES32.GL_COMPILE_STATUS,iShaderCompiledStatus,0);
		if(iShaderCompiledStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetShaderiv(fragmentShaderObject,GLES32.GL_INFO_LOG_LENGTH,iInfoLogLength,0);
			if(iInfoLogLength[0] > 0)
			{
				szInfoLog = GLES32.glGetShaderInfoLog(fragmentShaderObject);
				System.out.println("HAD: Fragment Shader Compilation Log = "+szInfoLog);
				uninitialize();
				System.exit(0);
			}
		}

		//Create Shader Program
		shaderProgramObject = GLES32.glCreateProgram();

		//Attach Vertex Shader to Program Object
		GLES32.glAttachShader(shaderProgramObject,vertexShaderObject);

		//Attach Fragment Shader to Program Object
		GLES32.glAttachShader(shaderProgramObject,fragmentShaderObject);

		//Pre-link binding of shader program object with vertex shader attributes
		GLES32.glBindAttribLocation(shaderProgramObject,GLESMacros.HAD_ATTRIBUTE_VERTEX,"vPosition");

		GLES32.glBindAttribLocation(shaderProgramObject,GLESMacros.HAD_ATTRIBUTE_TEXTURE0,"vTexture0_Coord");

		//Link 2 shaders together to shader program object
		GLES32.glLinkProgram(shaderProgramObject);
		int[] iShaderProgramLinkStatus = new int[1];
		iInfoLogLength[0] = 0;//reinitialize
		szInfoLog = null;//reinitialize

		GLES32.glGetProgramiv(shaderProgramObject,GLES32.GL_LINK_STATUS,iShaderProgramLinkStatus,0);
		if(iShaderProgramLinkStatus[0] == GLES32.GL_FALSE)
		{
			GLES32.glGetProgramiv(shaderProgramObject,GLES32.GL_INFO_LOG_LENGTH,iInfoLogLength,0);
			if(iInfoLogLength[0] > 0)
			{
				szInfoLog = GLES32.glGetProgramInfoLog(shaderProgramObject);
				System.out.println("HAD: Shader Program Link Log = "+szInfoLog);
				uninitialize();
				System.exit(0);
			}
		}

		//Get MVP uniform location
		mvpUniform = GLES32.glGetUniformLocation(shaderProgramObject,"u_mvp_matrix");
		texture0_sampler_uniform=GLES32.glGetUniformLocation(shaderProgramObject,"u_texture0_sampler");

		texture_stone[0]=loadGLTexture(R.raw.stone);
		texture_kundali[0]=loadGLTexture(R.raw.vijay_kundali_horz_inverted);

		//Vertices, Color,Shader Attribs, Vbo, Vao initializations
		final float pyramidVertices[] = new float[]
		{
			0.0f,1.0f,0.0f,
			-1.0f,-1.0f,1.0f,
			1.0f,-1.0f,1.0f,

			0.0f,1.0f,0.0f,
			1.0f,-1.0f,1.0f,
			1.0f,-1.0f,-1.0f,

			0.0f,1.0f,0.0f,
			1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,

			0.0f,1.0f,0.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,1.0f
		};

		final float pyramidTexcoords[] = new float[]
		{
			0.5f,1.0f,
			0.0f,0.0f,
			1.0f,0.0f,

			0.5f,1.0f,
			0.0f,0.0f,
			1.0f,0.0f,

			0.5f,1.0f,
			0.0f,0.0f,
			1.0f,0.0f,

			0.5f,1.0f,
			0.0f,0.0f,
			1.0f,0.0f
		};

		final float cubeVertices[] = new float[]
		{
			1.0f,1.0f,1.0f,
			-1.0f,1.0f,1.0f,
			-1.0f,-1.0f,1.0f,
			1.0f,-1.0f,1.0f,

			1.0f,1.0f,-1.0f,
			1.0f,1.0f,1.0f,
			1.0f,-1.0f,1.0f,
			1.0f,-1.0f,-1.0f,

			-1.0f,1.0f,-1.0f,
			1.0f,1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			
			-1.0f,1.0f,1.0f,
			-1.0f,1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,1.0f,

			1.0f,1.0f,-1.0f,
			-1.0f,1.0f,-1.0f,
			-1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,

			1.0f,-1.0f,1.0f,
			-1.0f,-1.0f,1.0f,
			-1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f
		}; 

		final float cubeTexcoords[] =new float[]
		{
			0.0f,0.0f,
			1.0f,0.0f,
			1.0f,1.0f,
			0.0f,1.0f,
			
			0.0f,0.0f,
			1.0f,0.0f,
			1.0f,1.0f,
			0.0f,1.0f,

			0.0f,0.0f,
			1.0f,0.0f,
			1.0f,1.0f,
			0.0f,1.0f,

			0.0f,0.0f,
			1.0f,0.0f,
			1.0f,1.0f,
			0.0f,1.0f,

			0.0f,0.0f,
			1.0f,0.0f,
			1.0f,1.0f,
			0.0f,1.0f,

			0.0f,0.0f,
			1.0f,0.0f,
			1.0f,1.0f,
			0.0f,1.0f
		};

		/*******************Triangle*****************/
		GLES32.glGenVertexArrays(1,vao_pyramid,0);
		GLES32.glBindVertexArray(vao_pyramid[0]);

		/****************Triangle Position**************/
		GLES32.glGenBuffers(1,vbo_position,0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_position[0]);

		ByteBuffer byteBuffer=ByteBuffer.allocateDirect(pyramidVertices.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer verticesBuffer=byteBuffer.asFloatBuffer();
		verticesBuffer.put(pyramidVertices);
		verticesBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,pyramidVertices.length*4,verticesBuffer,GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.HAD_ATTRIBUTE_VERTEX,3,GLES32.GL_FLOAT,false,0,0);

		GLES32.glEnableVertexAttribArray(GLESMacros.HAD_ATTRIBUTE_VERTEX);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

		/****************Triangle Texture**************/
		GLES32.glGenBuffers(1,vbo_texture,0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_texture[0]);

		byteBuffer = ByteBuffer.allocateDirect(pyramidTexcoords.length * 4);
		byteBuffer.order(ByteOrder.nativeOrder());
		FloatBuffer textureBuffer=byteBuffer.asFloatBuffer();
		textureBuffer.put(pyramidTexcoords);
		textureBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,pyramidTexcoords.length*4,textureBuffer,GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.HAD_ATTRIBUTE_TEXTURE0,2,GLES32.GL_FLOAT,false,0,0);

		GLES32.glEnableVertexAttribArray(GLESMacros.HAD_ATTRIBUTE_TEXTURE0);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

		GLES32.glBindVertexArray(0);

		/*****************Square*****************/
		GLES32.glGenVertexArrays(1,vao_cube,0);
		GLES32.glBindVertexArray(vao_cube[0]);

		/****************Square Position**************/
		GLES32.glGenBuffers(1,vbo_position,0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_position[0]);

		byteBuffer=ByteBuffer.allocateDirect(cubeVertices.length*4);
		byteBuffer.order(ByteOrder.nativeOrder());
		verticesBuffer=byteBuffer.asFloatBuffer();
		verticesBuffer.put(cubeVertices);
		verticesBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,cubeVertices.length*4,verticesBuffer,GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.HAD_ATTRIBUTE_VERTEX,3,GLES32.GL_FLOAT,false,0,0);

		GLES32.glEnableVertexAttribArray(GLESMacros.HAD_ATTRIBUTE_VERTEX);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);

		/****************Square Color**************/
		GLES32.glGenBuffers(1,vbo_texture,0);
		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,vbo_texture[0]);

		byteBuffer=ByteBuffer.allocateDirect(cubeTexcoords.length*4);
		byteBuffer.order(ByteOrder.nativeOrder());
		textureBuffer=byteBuffer.asFloatBuffer();
		textureBuffer.put(cubeTexcoords);
		textureBuffer.position(0);

		GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,cubeTexcoords.length*4,textureBuffer,GLES32.GL_STATIC_DRAW);

		GLES32.glVertexAttribPointer(GLESMacros.HAD_ATTRIBUTE_TEXTURE0,2,GLES32.GL_FLOAT,false,0,0);

		GLES32.glEnableVertexAttribArray(GLESMacros.HAD_ATTRIBUTE_TEXTURE0);

		GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER,0);
		GLES32.glBindVertexArray(0);

		//Enable DepthTest
		GLES32.glEnable(GLES32.GL_DEPTH_TEST);

		//Specify Depth test to be done
		GLES32.glDepthFunc(GLES32.GL_LEQUAL);

		//We will always cull the back faces for better performance
		GLES32.glEnable(GLES32.GL_CULL_FACE);

		//Set the background frame color
		GLES32.glClearColor(0.0f,0.0f,0.0f,1.0f);

		//Set ProjectionMatrix to identity matrix
		Matrix.setIdentityM(perspectiveProjectionMatrix,0);
	}

	private int loadGLTexture(int imageFileResourceID)
	{
		BitmapFactory.Options options = new BitmapFactory.Options();

		options.inScaled=false;

		Bitmap bitmap = BitmapFactory.decodeResource(context.getResources(),imageFileResourceID,options);

		int[] texture = new int[1];

		GLES32.glGenTextures(1,texture,0);

		GLES32.glPixelStorei(GLES32.GL_UNPACK_ALIGNMENT,1);

		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D,texture[0]);

		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D,GLES32.GL_TEXTURE_MAG_FILTER,GLES32.GL_LINEAR);
		GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D,GLES32.GL_TEXTURE_MIN_FILTER,GLES32.GL_LINEAR_MIPMAP_LINEAR);

		GLUtils.texImage2D(GLES32.GL_TEXTURE_2D,0,bitmap,0);

		GLES32.glGenerateMipmap(GLES32.GL_TEXTURE_2D);

		return(texture[0]);
	}

	private void resize(int width,int height)
	{
		//Adjust the viewport based on geometry changes such as screen rotation
		GLES32.glViewport(0,0,width,height);

		//Perspective Projection
		Matrix.perspectiveM(perspectiveProjectionMatrix,0,45.0f,(float)width/(float)height,0.1f,100.0f);
	}

	public void draw()
	{
		//Draw background color
		GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT|GLES32.GL_DEPTH_BUFFER_BIT);

		//Use Shader Program
		GLES32.glUseProgram(shaderProgramObject);

		float modelViewMatrix[] = new float[16];
		float modelViewProjectionMatrix[] = new float[16];
		float rotationMatrix[] = new float[16];
		float scaleMatrix[] = new float[16];

		//set ModelView and ModelViewProjection matrices to identity
		Matrix.setIdentityM(modelViewMatrix,0);
		Matrix.setIdentityM(modelViewProjectionMatrix,0);
		Matrix.setIdentityM(rotationMatrix,0);

		Matrix.translateM(modelViewMatrix,0,-1.5f,0.0f,-6.0f);

		Matrix.rotateM(rotationMatrix,0,anglePyramid,0.0f,1.0f,0.0f);

		Matrix.multiplyMM(modelViewMatrix,0,modelViewMatrix,0,rotationMatrix,0);

		//multiply the modelview and projection matrix to get modelviewprojection matrix
		Matrix.multiplyMM(modelViewProjectionMatrix,0,perspectiveProjectionMatrix,0,modelViewMatrix,0);

		//pass above modelviewprojection matrix to the vertex shader in 'u_mvp_matrix' shader variable
		//whose position value we already calculated in initWithFrame() by using glGetUniformLocation()
		GLES32.glUniformMatrix4fv(mvpUniform,1,false,modelViewProjectionMatrix,0);

		//Bind Vao
		GLES32.glBindVertexArray(vao_pyramid[0]);

		GLES32.glActiveTexture(GLES32.GL_TEXTURE0);
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D,texture_stone[0]);
		GLES32.glUniform1i(texture0_sampler_uniform,0);


		//Draw either by glDrawTriangles() or glDrawArrays() or glDrawElements()
		GLES32.glDrawArrays(GLES32.GL_TRIANGLES,0,12); // 3 each with its x,y,z

		//unbind vao
		GLES32.glBindVertexArray(0);

		Matrix.setIdentityM(modelViewMatrix,0);
		Matrix.setIdentityM(modelViewProjectionMatrix,0);
		Matrix.setIdentityM(rotationMatrix,0);
		Matrix.setIdentityM(scaleMatrix,0);


		Matrix.translateM(modelViewMatrix,0,1.5f,0.0f,-6.0f);

		Matrix.rotateM(rotationMatrix,0,angleCube,1.0f,0.0f,0.0f);
		Matrix.rotateM(rotationMatrix,0,angleCube,0.0f,1.0f,0.0f);
		Matrix.rotateM(rotationMatrix,0,angleCube,0.0f,1.0f,0.0f);

		Matrix.multiplyMM(modelViewMatrix,0,modelViewMatrix,0,rotationMatrix,0);

		Matrix.scaleM(scaleMatrix,0,0.75f,0.75f,0.75f);

		Matrix.multiplyMM(modelViewMatrix,0,modelViewMatrix,0,scaleMatrix,0);

		Matrix.multiplyMM(modelViewProjectionMatrix,0,perspectiveProjectionMatrix,0,modelViewMatrix,0);

		GLES32.glUniformMatrix4fv(mvpUniform,1,false,modelViewProjectionMatrix,0);

		GLES32.glBindVertexArray(vao_cube[0]);

		GLES32.glActiveTexture(GLES32.GL_TEXTURE0);
		GLES32.glBindTexture(GLES32.GL_TEXTURE_2D,texture_kundali[0]);
		GLES32.glUniform1i(texture0_sampler_uniform,0);

		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,0,4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,4,4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,8,4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,12,4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,16,4);
		GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN,20,4);

		GLES32.glBindVertexArray(0);

		//un-use shader program
		GLES32.glUseProgram(0);

		update();
		//Like SwapBuffers() in Windows
		requestRender();
	}

	private void update()
	{
		anglePyramid=anglePyramid+1.0f;
		if(anglePyramid>=360.0f)
			anglePyramid=anglePyramid-360.0f;

		angleCube=angleCube+1.0f;
		if(angleCube>=360.0f)
			angleCube=angleCube-360.0f;
	}

	public void uninitialize()
	{
		if(vao_pyramid[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1,vao_pyramid,0);
			vao_pyramid[0]=0;
		}

		if(vao_cube[0] != 0)
		{
			GLES32.glDeleteVertexArrays(1,vao_cube,0);
			vao_cube[0]=0;
		}

		if(vbo_position[0] != 0)
		{
			GLES32.glDeleteBuffers(1,vbo_position,0);
			vbo_position[0]=0;
		}

		if(vbo_texture[0] != 0)
		{
			GLES32.glDeleteBuffers(1,vbo_texture,0);
			vbo_texture[0]=0;
		}

		if(texture_stone[0]!=0)
		{
			GLES32.glDeleteTextures(1,texture_stone,0);
			texture_stone[0]=0;
		}

		if(texture_kundali[0]!=0)
		{
			GLES32.glDeleteTextures(1,texture_kundali,0);
			texture_kundali[0]=0;
		}

		if(shaderProgramObject != 0)
		{
			if(vertexShaderObject != 0)
			{
				GLES32.glDetachShader(shaderProgramObject,vertexShaderObject);
				GLES32.glDeleteShader(vertexShaderObject);
				vertexShaderObject=0;
			}

			if(fragmentShaderObject != 0)
			{
				GLES32.glDetachShader(shaderProgramObject,fragmentShaderObject);
				GLES32.glDeleteShader(fragmentShaderObject);
				fragmentShaderObject=0;
			}
		}

		if(shaderProgramObject != 0)
		{
			GLES32.glDeleteProgram(shaderProgramObject);
			shaderProgramObject=0;
		}
	}
}