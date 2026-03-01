//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

static const char* vShaderrojo = "shaders/shaderrojo.vert";
static const char* vShaderverde = "shaders/shaderverde.vert";
static const char* vShaderazul = "shaders/shaderazul.vert";
static const char* vShadercafe = "shaders/shadercafe.vert";
static const char* vShaderverdeoscuro = "shaders/shaderverdeoscuro.vert";


//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);   //0
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);   //1
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {	
			//X			Y			Z			R		G		B
			-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			1.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
			/*1.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,  1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
			-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,*/
			
	};
	MeshColor *letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,18);
	meshColorList.push_back(letras);			//0

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		
	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);		//1

	GLfloat vertices_trianguloazul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,

	};

	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);		//2


	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);		//3

	GLfloat vertices_cuadradorojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);   //4

	/* ----------------------------------- Letras del nombre QZM -------------------------- */
	GLfloat vertices_letraQ[] = {
		//X			Y			Z			R		G		B
		//-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		//1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		//0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,

		//1
		-0.24f,-0.60f,0.0f,			1.0f,	0.0f,	0.0f,    //Izquierda
		-0.17f,-0.60f, 0.0f,		1.0f,	0.0f,	0.0f,    //Derecha
		-0.17f,0.0f,0.0f,			1.0f,	0.0f,	0.0f,      //Arriba

		//2
		-0.24f,-0.60f,0.0f,			1.0f,	0.0f,	0.0f,    //Izquierda
		-0.24f,0.0f, 0.0f,			1.0f,	0.0f,	0.0f,     //Derecha
		-0.17f,0.0f,0.0f,			1.0f,	0.0f,	0.0f,      //Arriba

		//--
		//3
		-0.70f,-0.07f,0.0f,			1.0f,	0.0f,	0.0f,    //Izquierda
		-0.17f,-0.07f, 0.0f,		1.0f,	0.0f,	0.0f,     //Derecha
		-0.17f,0.0f,0.0f,			1.0f,	0.0f,	0.0f,      //Arriba

		//4
		-0.70f,-0.07f,0.0f,			1.0f,	0.0f,	0.0f,    //Izquierda
		-0.17f,0.0f, 0.0f,			1.0f,	0.0f,	0.0f,     //Derecha
		-0.70f,0.0f,0.0f,			1.0f,	0.0f,	0.0f,      //Arriba

		//--
		//5
		-0.70f,-0.60f,0.0f,			1.0f,	0.0f,	0.0f,    //Izquierda
		-0.63f,-0.60f, 0.0f,		1.0f,	0.0f,	0.0f,     //Derecha
		-0.63f,0.0f,0.0f,			1.0f,	0.0f,	0.0f,//Arriba

		//6
		-0.70f,-0.60f,0.0f,			1.0f,	0.0f,	0.0f,    //Izquierda
		-0.63f,0.0f, 0.0f,			1.0f,	0.0f,	0.0f,     //Derecha
		-0.70f,0.0f,0.0f,			1.0f,	0.0f,	0.0f,      //Arriba

		//--
		//7
		-0.70f,-0.60f,0.0f,			1.0f,	0.0f,	0.0f,    //Izquierda
		-0.17f,-0.53f, 0.0f,		1.0f,	0.0f,	0.0f,     //Derecha
		-0.70f,-0.53f,0.0f,			1.0f,	0.0f,	0.0f,      //Arriba

		//8
		-0.70f,-0.60f,0.0f,			1.0f,	0.0f,	0.0f,    //Izquierda
		-0.17f,-0.60f, 0.0f,		1.0f,	0.0f,	0.0f,     //Derecha
		-0.17f,-0.53f,0.0f,			1.0f,	0.0f,	0.0f,      //Arriba


		//--
		//9
		-0.40f,-0.45f,0.0f,			1.0f,	0.0f,	0.0f,    //Izquierda
		-0.19f,-0.72f, 0.0f,		1.0f,	0.0f,	0.0f,     //Derecha
		-0.33f,-0.41f,0.0f,			1.0f,	0.0f,	0.0f,      //Arriba

		//10
		-0.19f,-0.72f,0.0f,			1.0f,	0.0f,	0.0f,    //Izquierda
		-0.14f,-0.69f, 0.0f,		1.0f,	0.0f,	0.0f,     //Derecha
		-0.33f,-0.41f,0.0f,			1.0f,	0.0f,	0.0f,      //Arriba

	};

	MeshColor* letraQ = new MeshColor();
	letraQ->CreateMeshColor(vertices_letraQ, 180);
	meshColorList.push_back(letraQ);  // [5]

	GLfloat vertices_letraZ[] = {
		//------ Letra Z
		//1
		-0.15f,0.23f,0.0f,			1.0f,	1.0f,	0.0f,    //Izquierda
		0.38f,0.30f, 0.0f,			1.0f,	1.0f,	0.0f,    //Derecha
		-0.15f,0.30f,0.0f,			1.0f,	1.0f,	0.0f,      //Arriba

		//2
		-0.15f,0.23f,0.0f,			1.0f,	1.0f,	0.0f,    //Izquierda
		0.38f,0.23f, 0.0f,			1.0f,	1.0f,	0.0f,    //Derecha
		0.38f,0.30f,0.0f,			1.0f,	1.0f,	0.0f,      //Arriba


		//3
		-0.15f,-0.30f,0.0f,			1.0f,	1.0f,	0.0f,    //Izquierda
		0.38f,-0.30f, 0.0f,			1.0f,	1.0f,	0.0f,    //Derecha
		0.38f,-0.23f,0.0f,			1.0f,	1.0f,	0.0f,      //Arriba

		//4
		-0.15f,-0.30f,0.0f,			1.0f,	1.0f,	0.0f,    //Izquierda
		0.38f,-0.23f, 0.0f,			1.0f,	1.0f,	0.0f,    //Derecha
		-0.15f,-0.23f,0.0f,			1.0f,	1.0f,	0.0f,      //Arriba


		//5
		-0.15f,-0.23f,0.0f,			1.0f,	1.0f,	0.0f,    //Izquierda
		-0.08f,-0.23f, 0.0f,		1.0f,	1.0f,	0.0f,    //Derecha
		0.31f,0.23f,0.0f,			1.0f,	1.0f,	0.0f,      //Arriba

		//6
		-0.08f,-0.23f,0.0f,			1.0f,	1.0f,	0.0f,    //Izquierda
		0.38f,0.23f, 0.0f,			1.0f,	1.0f,	0.0f,    //Derecha
		0.31f,0.23f,0.0f,			1.0f,	1.0f,	0.0f,      //Arriba

	};

	MeshColor* letraZ = new MeshColor();
	letraZ->CreateMeshColor(vertices_letraZ, 108);
	meshColorList.push_back(letraZ);  // [6]

	GLfloat vertices_letraM[] = {
		//------ Letra M

		//1
		0.40f,0.0f,0.0f,			0.0f,	1.0f,	1.0f,    //Izquierda
		0.47f,0.60f, 0.0f,			0.0f,	1.0f,	1.0f,    //Derecha
		0.40f,0.60f,0.0f,			0.0f,	1.0f,	1.0f,      //Arriba

		//2
		0.40f,0.0f,0.0f,			0.0f,	1.0f,	1.0f,    //Izquierda
		0.47f,0.0f, 0.0f,			0.0f,	1.0f,	1.0f,    //Derecha
		0.47f,0.60f,0.0f,			0.0f,	1.0f,	1.0f,      //Arriba


		//3
		0.86f,0.0f, 0.0f,			0.0f,	1.0f,	1.0f,    //Izquierda
		0.93f,0.0f, 0.0f,			0.0f,	1.0f,	1.0f,    //Derecha
		0.93f,0.60f, 0.0f,			0.0f,	1.0f,	1.0f,      //Arriba

		//4
		0.86f,0.0f, 0.0f,			0.0f,	1.0f,	1.0f,    //Izquierda
		0.93f,0.60f, 0.0f,			0.0f,	1.0f,	1.0f,    //Derecha
		0.86f,0.60f, 0.0f,			0.0f,	1.0f,	1.0f,      //Arriba


		//5
		0.47f, 0.53f, 0.0f,			0.0f,	1.0f,	1.0f,    //Izquierda
		0.665f, 0.335f, 0.0f,		0.0f,	1.0f,	1.0f,    //Derecha
		0.47f, 0.60f, 0.0f,			0.0f,	1.0f,	1.0f,      //Arriba

		//6
		0.47f, 0.53f, 0.0f,			0.0f,	1.0f,	1.0f,    //Izquierda
		0.665f, 0.265f, 0.0f,		0.0f,	1.0f,	1.0f,    //Derecha
		0.665f, 0.335f, 0.0f,		0.0f,	1.0f,	1.0f,      //Arriba

		//7
		0.665f, 0.265f, 0.0f,		0.0f,	1.0f,	1.0f,    //Izquierda
		0.86f, 0.53f, 0.0f,			0.0f,	1.0f,	1.0f,    //Derecha
		0.665f, 0.335f, 0.0f,		0.0f,	1.0f,	1.0f,      //Arriba

		//8
		0.665f, 0.335f, 0.0f,		0.0f,	1.0f,	1.0f,    //Izquierda
		0.86f, 0.53f, 0.0f,			0.0f,	1.0f,	1.0f,    //Derecha
		0.86f, 0.60f, 0.0f,			0.0f,	1.0f,	1.0f,      //Arriba

	};

	MeshColor* letraM = new MeshColor();
	letraM->CreateMeshColor(vertices_letraM, 144);
	meshColorList.push_back(letraM);  // [7]




}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);    //0

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);   //1

	// ----------------- Shaders por colores ------------------------

	Shader* shader3 = new Shader();//shader para usar color como parte del VAO: letras rojo
	shader3->CreateFromFiles(vShaderrojo, fShaderColor);
	shaderList.push_back(*shader3);   //2

	Shader* shader4 = new Shader();//shader para usar color como parte del VAO: letras verde
	shader4->CreateFromFiles(vShaderverde, fShaderColor);
	shaderList.push_back(*shader4);   //3

	Shader* shader5 = new Shader();//shader para usar color como parte del VAO: letras azul
	shader5->CreateFromFiles(vShaderazul, fShaderColor);
	shaderList.push_back(*shader5);   //4

	Shader* shader6 = new Shader();//shader para usar color como parte del VAO: letras cafe
	shader6->CreateFromFiles(vShadercafe, fShaderColor);
	shaderList.push_back(*shader6);   //5

	Shader* shader7 = new Shader();//shader para usar color como parte del VAO: letras verdeoscuro
	shader7->CreateFromFiles(vShaderverdeoscuro, fShaderColor);
	shaderList.push_back(*shader7);   //6

}

float angulo = 0.0f;

int main()
{
	mainWindow = Window(800, 800);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{

		angulo += 0.1f;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
		//shaderList[1].useShader();
		//uniformModel = shaderList[1].getModelLocation();
		//uniformProjection = shaderList[1].getProjectLocation();

		/* -------------------------------------------------  LETRAS NOMBRE ------------------------------------ */
		//Letra Q
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[5]->RenderMeshColor();

		//Letra Z
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[6]->RenderMeshColor();

		//Letra M
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[7]->RenderMeshColor();


		/* ----------------------------------------------- Generación Casa --------------------------------- */
		/* -------------------------------------------------  ROJO ----------------------------------------*/
		//Para dibujar cubo de color rojo
		shaderList[2].useShader(); 
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();  // 1 cubo

		/* -------------------------------------------------  VERDE ----------------------------------------*/
		//Para dibujar cubo de color verde izquierda
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.25f, 0.3f, -2.25f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();  // 1 cubo
		
		//Para dibujar cubo de color verde derecha
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);								// más cerca a 0 en z se acerca
		model = glm::translate(model, glm::vec3(0.25f, 0.3f, -2.25f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();  // 1 cubo

		//Para dibujar cubo de color verde centro
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);								// más cerca a 0 en z se acerca
		model = glm::translate(model, glm::vec3(0.0f, -0.4f, -2.25f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();  // 1 cubo

		/* -------------------------------------------------  AZUL ----------------------------------------*/

		//Para dibujar pirámide de color azul
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);								// más cerca a 0 en z se acerca
		model = glm::translate(model, glm::vec3(0.0f, 0.9f, -2.25f));
		model = glm::scale(model, glm::vec3(1.3f, 0.7f, 1.3f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();  // 0 piramide

		/* -------------------------------------------------  VERDE OSCURO ----------------------------------------*/
		//Para dibujar pirámide de color verde oscuro izquierda
		shaderList[6].useShader();
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);								// más cerca a 0 en z se acerca
		model = glm::translate(model, glm::vec3(-1.0f, -0.2f, -2.25f));
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.3f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();  // 0 piramide

		//Para dibujar pirámide de color verde oscuro derecha
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);								// más cerca a 0 en z se acerca
		model = glm::translate(model, glm::vec3(1.0f, -0.2f, -2.25f));
		model = glm::scale(model, glm::vec3(0.3f, 0.4f, 0.3f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();  // 0 piramide

		/* -------------------------------------------------  CAFE ----------------------------------------*/
		
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();

		//Para dibujar cubo de color cafe izquierda
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);								// más cerca a 0 en z se acerca
		model = glm::translate(model, glm::vec3(-1.0f, -0.45f, -2.25f));
		model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.12f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();  // 1 cubo

		//Para dibujar cubo de color cafe derecha
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);								// más cerca a 0 en z se acerca
		model = glm::translate(model, glm::vec3(1.0f, -0.45f, -2.25f));
		model = glm::scale(model, glm::vec3(0.12f, 0.12f, 0.12f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();  // 1 cubo

				

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/