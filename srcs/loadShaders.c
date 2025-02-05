#include "../include/scop.h"

GLuint loadShaders(const char *vertex_path, const char *fragment_path){
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	
	char 	*vertexShaderPtr = "";
	int vertexShaderFD = open(vertex_path, O_RDONLY);
	if (vertexShaderFD >= 0){
		char	*tmp = get_next_line((vertexShaderFD));
		while (tmp){
			add_to_garbage(tmp);
			vertexShaderPtr = ft_strjoin(vertexShaderPtr, tmp);
			add_to_garbage(vertexShaderPtr);
			tmp = get_next_line(vertexShaderFD);
		}
		close(vertexShaderFD);
	}
	else{
		dprintf(2, "Can't open %s\n", vertex_path);
	}
	char 	*fragmentShaderPtr = "";
	int fragmentShaderFD = open(fragment_path, O_RDONLY);
	if (fragmentShaderFD >= 0){
		char	*tmp = get_next_line((fragmentShaderFD));
		while (tmp){
			add_to_garbage(tmp);
			fragmentShaderPtr = ft_strjoin(fragmentShaderPtr, tmp);
			add_to_garbage(fragmentShaderPtr);
			tmp = get_next_line(fragmentShaderFD);
		}
		close(fragmentShaderFD);
	}
	else{
		dprintf(2, "Can't open %s\n", fragment_path);
	}
	
	GLint result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	const char *vertexShaderCode = vertexShaderPtr;
	glShaderSource(vertexShaderID, 1, &vertexShaderCode , 0);
	glCompileShader(vertexShaderID);

	// Check vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		char vertexShaderErrorMessage[InfoLogLength+1];
		glGetShaderInfoLog(vertexShaderID, InfoLogLength, NULL, &vertexShaderErrorMessage[0]);
		dprintf(2, "%s\n", &vertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	const char *fragmentShaderCode = fragmentShaderPtr;
	glShaderSource(fragmentShaderID, 1, &fragmentShaderCode , 0);
	glCompileShader(fragmentShaderID);

	// Check fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		char fragmentShaderErrorMessage[InfoLogLength+1];
		glGetShaderInfoLog(fragmentShaderID, InfoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}
	
	// Link the program
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertexShaderID);
	glAttachShader(ProgramID, fragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		char ProgramErrorMessage[InfoLogLength+1];
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(ProgramID, vertexShaderID);
	glDetachShader(ProgramID, fragmentShaderID);
	
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return ProgramID;
}