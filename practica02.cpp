/**
VERTEX ARRAY OBJECT = VAO
**/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#define BUFFER_OFFSET(a) ((void*)(a))


#include <GL/glew.h>
#include <GL/freeglut.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

GLuint VertexArrayID;
GLuint vertexbuffer;
GLuint colorbuffer;

GLuint programID;

GLuint MatrixID;
glm::mat4 MVP;
//---------------------------------------------------------------------
//
// LoadShaders Function
//

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path){
    //Creamos los Shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    //Leer el Vertex Shader code from file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path,std::ios::in);

    if(VertexShaderStream.is_open()){
        std::string Line = "";
        while(getline(VertexShaderStream,Line)){
            VertexShaderCode += "\n" + Line;
        }
        VertexShaderStream.close();
    }else{
        printf("Impossible to open %s. Are you in the right directory?",vertex_file_path);
        getchar();
        return 0;
    }

    //Leer el fragment shader
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path,std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream,Line)){
            FragmentShaderCode += "\n" + Line;
        }
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    //Compile Vertex Shader
    printf("Compiling vertex shader: %s\n",vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID,1,&VertexSourcePointer,NULL);
    glCompileShader(VertexShaderID);

    //chek vertex Shader
    glGetShaderiv(VertexShaderID,GL_COMPILE_STATUS,&Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID,InfoLogLength,NULL,&VertexShaderErrorMessage[0]);
        printf("%s\n",&VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
	printf("Compiling fragment shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	//Link the Program

	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID,VertexShaderID);
	glAttachShader(ProgramID,FragmentShaderID);
	glLinkProgram(ProgramID);

	//Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	//Limpiamos la memoria de los shaders
	glDetachShader(ProgramID,VertexShaderID);
	glDetachShader(ProgramID,FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

//---------------------------------------------------------------------
//
// init

void init(){
    glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Enable depth Test
	glEnable(GL_DEPTH_TEST);

	programID = LoadShaders( "SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader" );
    glUseProgram(programID);

    // Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(programID, "MVP");

	//Matriz de proyección
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f),4.0f/4.0f,0.1f,100.0f);

    //Posicion de la camara
    glm::mat4 View = glm::lookAt(
                               glm::vec3(10,5,7),
                               glm::vec3(0,0,0),
                               glm::vec3(0,0,1)
                               );
    // Model Matrix
    glm::mat4 Model = glm::mat4(1.0f);

    MVP = Projection * View * Model;

    GLfloat diff = 1.5;

    static const GLfloat g_vertex_buffer_data[] = {
        -10.0f,0.0f,0.0f,
        10.0f,0.0f,0.0f,
        0.0f,10.0f,0.0f,
        0.0f,-10.0f,0.0f,
        0.0f,0.0f,10.0f,
        0.0f,0.0f,-10.0f,//empieza la letra
		 1.0f,-2.0f,3.0f,
		 -1.0f,-2.0f,3.0f,
		 1.0f,-1.0f,-1.0f,
		 -1.0f,-1.0f,-1.0f,
		 1.0f,0.0f,0.0f,
		 -1.0f,0.0f,0.0f,
		 1.0f,1.0f,-1.0f,
		 -1.0f,1.0f,-1.0f,
		 1.0f,2.0f,3.0f,
		 -1.0f,2.0f,3.0f,
		 1.0f,2.0f,3.0f-diff,
		 -1.0f,2.0f,3.0f-diff,
		 1.0f,1.0f,-1.0f-diff,
		 -1.0f,1.0f,-1.0f-diff,
		 1.0f,0.0f,0.0f-diff,
		 -1.0f,0.0f,0.0f-diff,
		 1.0f,-1.0f,-1.0f-diff,
		 -1.0f,-1.0f,-1.0f-diff,
		 1.0f,-2.0f,3.0f-diff,
		 -1.0f,-2.0f,3.0f-diff,
		 1.0f,-2.0f,3.0f,
		 -1.0f,-2.0f,3.0f,
	};

	static const GLfloat g_color_buffer_data[] = {
	    1.0f,0.0f,0.0f,
	    1.0f,0.0f,0.0f,
	    0.0f,1.0f,0.0f,
	    0.0f,1.0f,0.0f,
	    0.0f,0.0f,1.0f,
	    0.0f,0.0f,1.0f,
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


}

//---------------------------------------------------------------------
//
// display
//

void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(programID);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	//draw the axis
	glDrawArrays(GL_LINES, 0, 2*3);

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLE_STRIP, 6, 22); // 3 indices starting at 0 -> 1 triangle

    glDisableVertexAttribArray(0);

    glutSwapBuffers();

    glFlush();
}



//---------------------------------------------------------------------
//
// main
//


int main(int argc, char** argv){
    glutInit(&argc, argv);

    glewExperimental = GL_TRUE;

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    glutInitWindowSize(512,512);
    glutInitContextVersion(4,3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutCreateWindow(argv[0]);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      exit(0);
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    init();

    glutDisplayFunc(display);


    glutMainLoop();
}
