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

float angulo = 0.0f;

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
	meshList.push_back(obj1); // [0]
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
meshList.push_back(cubo);  ///[1]
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
	meshColorList.push_back(letras);        //[0]

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		
	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);  // [1]

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
	meshColorList.push_back(cuadradoverde);   //[2]


	// --------------------- Letras nombre QZM
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
	meshColorList.push_back(letraQ);  // [3]

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
	meshColorList.push_back(letraZ);  // [4]

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
	meshColorList.push_back(letraM);  // [5]

}


void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		
													
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		

		/* -------------------------------------- Renderiza en 2D ---------------------------------- */
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshColorList[1]->RenderMeshColor();

		/* ---------------------- Letras Ejercicio 1 --------------------- */

		//Letra Q
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[3]->RenderMeshColor();

		//Letra Z
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[4]->RenderMeshColor();

		//Letra M
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[5]->RenderMeshColor();

		

		/* ---------------------- Casita Ejercicio 2 --------------------- */






		/* -------------------------------------- Renderiza en 3D ---------------------------------- */
		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		shaderList[0].useShader(); 
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//meshList[1]->RenderMesh();

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