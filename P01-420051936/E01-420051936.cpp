#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>

// Ejercicio 1 ------------------
//Random
#include <cstdlib>
#include <ctime>
// Ejercicio 1 -----------------
//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader; // identidicadores VAO-> para asignar datos de VertexArrayObjet (donde declaramos los vértices para dibujar las líneas, puntos o triángulos)
// Vértice no es igual a x,y,z, podemos tener igual s, etc, otras coordenadas espaciales que no es solo para triángulo
//BVO = BuuferVertexObject, espacio de memoria de video que se asigna para los datos del VAO
//shader es el identificador, vertexShader de VAO a vxys, solo geometrías. fragmentShader calcula coordenadas de color,
// colores planos, colores pode variables uniform, color de materiales y modelo(novisto en clase), iluminación y texturizado.
//usaremos en la prácticas al menos 1 vertexShader y 1 fragmentShader, siempre hay un int de tamaño v3, 
// gl_Position recibe 3 datos flotantes (xys), al pasarselo al main, convierte
//los 3 flotantes en coordenadas (xyz) para ese vértice

//LENGUAJE DE SHADER (SOMBRAS) GLSL
//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";

//Fragment Shader
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(0.0f,1.0f,1.0f,1.0f);	 			\n\
}";



void CrearTriangulo()
{
	GLfloat vertices[] = {
		//Rombo
		-1.0f, 0.0f,0.0f,   //Izquierda
		0.0f,0.0f, 0.0f,    //Derecha
		-0.5f,0.5f,0.0f,       //Arriba

		//2
		-1.0f, 0.0f,0.0f,   //Izquierda
		0.0f,0.0f, 0.0f,    //Derecha
		-0.5f,-0.5f,0.0f,       //Arriba

		//Trapecio
		//5
		0.0f, 0.0f,0.0f,   //Izquierda
		0.5f,0.0f, 0.0f,    //Derecha
		0.25f,0.5f,0.0f,       //Arriba

		//6
		0.5f, 0.0f,0.0f,   //Izquierda
		1.0f,0.0f, 0.0f,    //Derecha
		0.75f,0.5f,0.0f,       //Arriba

		//7
		0.25f, 0.5f,0.0f,   //Izquierda
		0.75f,0.5f, 0.0f,    //Derecha
		0.5f,0.0f,0.0f,       //Arriba

		
	};
	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
		glEnableVertexAttribArray(0);
		//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

}
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica

//the Program recibe los datos de theShader

{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader,1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n",shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

void CompileShaders() {
	shader= glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	 //verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	//****  LAS SIGUIENTES 4 LÍNEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSIÓN DE OPENGL QUE TIENE ****/

	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "E01-420051936", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

 //Llamada a las funciones creadas antes del main
	CrearTriangulo();
	CompileShaders();

		//Ejercicio clase
	float r = 0.0f, g = 0.0f, b = 0.0f; 
	float lastTime = 0.0f;
	int colorIndex = 0;


		//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();

		//ejercicio 1 ------------------------
		//Obtener el tiempo actual
		float currentTime = glfwGetTime();

		if (currentTime - lastTime >= 3.0f)
		{
		

			colorIndex = (colorIndex + 1) % 3; // Cycle 0 → 1 → 2 → 0

			if (colorIndex == 0)
			{
				r = 1.0f; g = 0.0f; b = 0.0f; // Red
			}
			else if (colorIndex == 1)
			{
				r = 0.0f; g = 1.0f; b = 0.0f; // Green
			}
			else if (colorIndex == 2)
			{
				r = 0.0f; g = 0.0f; b = 1.0f; // Blue
			}

			lastTime = currentTime;
		}
		// Ejercicio1 ------------------------
		//Limpiar la ventana
		            //Red//Green//Blue
		glClearColor(r,g,b,1.0f);
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // negro
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES,0,3);  // 3 * cantidad de triangulos
		glDrawArrays(GL_TRIANGLES,0,15);  
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
		 
		//NO ESCRIBIR NINGUNA LÍNEA DESPUÉS DE glfwSwapBuffers(mainWindow); 
	}


	return 0;
}