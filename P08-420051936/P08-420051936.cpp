/*
Práctica 8: Iluminación 2
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

// =========== Ejercicio Práctica 8 ======== //
Texture AguaTexture;
Texture CristalTexture;
// ========================================= //

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

// ========== Práctica (7) =========== //
//Model Carro_M;
//Model Carro_cofre;
//Model llanta_texturizada_izquierda;
//Model llanta_texturizada_derecha;

// ========================================= //

// ========== Práctica (8) =========== //
Model lampara;
Model Nave;

Model Pecera;
Model Pez_abismal_cuerpo;
Model Pez_abismal_antena;
Model Pez_abismal_foco;

SpotLight spotLights2[MAX_SPOT_LIGHTS];
PointLight pointLights_pez[MAX_POINT_LIGHTS];
PointLight pointLights_lampara[MAX_POINT_LIGHTS];
PointLight pointLights_lampara_pez[MAX_POINT_LIGHTS];

// ========================================= //


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};

	// ========== Práctica (8) =========== //
	unsigned int aguaIndices[] = {
	   0, 2, 1,
	   1, 2, 3
	};

	GLfloat aguaVertices[] = {
		-1.0f, 0.0f, -1.0f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, -1.0f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 1.0f,	0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f
	};

	unsigned int cristalIndices[] = {
	   0, 2, 1,
	   1, 2, 3
	};

	GLfloat cristalVertices[] = {
		-1.0f, 0.0f, -1.0f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, -1.0f,	1.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 1.0f,	0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f,	1.0f, 1.0f,		0.0f, 1.0f, 0.0f
	};


	// ========================================= //
		
	// 0
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	// 1
	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	// 2 Piso
	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	// 3
	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	// 4 Textura agua
	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(aguaVertices, aguaIndices, 32, 6);
	meshList.push_back(obj5);

	// 6 Textura cristal
	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(cristalVertices, cristalIndices, 32, 6);
	meshList.push_back(obj6);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	//calcAverageNormals(aguaIndices, 6, aguaVertices, 64, 8, 5);
}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,

		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.0f,  0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	1.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,	0.0f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		1.0f,	1.0f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,  0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f,	0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,	1.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	1.0f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(dado);

}

void CrearOctaedro() {  //Dado de 8 lados   
	unsigned int octaedro_indices[] = {
		//{up}----------------------------------------------------
		//front
		0, 1, 2,
		//right
		3, 4, 5,
		//back
		6, 7, 8,
		//left
		9, 10, 11,
		//{bottom}-------------------------------------------------
		//font
		12, 13, 14,
		//right
		15, 16, 17,
		//back
		18, 19, 20,
		//left
		21, 22, 23,

	};

	GLfloat octaedro_vertices[] = {
		//{UP}-----------------------------------------------------------
		// front																		    /\ [2]
		//x		y		z		S		T			NX		NY		NZ         +           /  \					/
		-0.5f, -0.5f,  0.5f,	0.25f,  0.5f,		0.0f,	-1.0f,	0.0f,   //0      [0]  /____\  [1]			
		0.5f,  -0.5f,  0.5f,	0.75f,	0.5f,		0.0f,	-1.0f,	0.0f,	//1			
		 0.0f,  0.5f,  0.0f,	0.5f,	0.75f,		0.0f,	-1.0f,	0.0f,	//2

		 // right
		 //x		y		z		S		T
		 0.5f, -0.5f,  0.5f,	0.75f,  0.5f,		-1.0f,	0.0f,	0.0f,
		 0.5f, -0.5f,  -0.5f,	1.0f,	0.75f,		-1.0f,	0.0f,	0.0f,
		 0.0f,  0.5f,   0.0f,	0.5f,	0.75f,		-1.0f,	0.0f,	0.0f,

		 // back
		 //x		y		z		S		T
		 -0.5f, -0.5f, -0.5f,	0.75f,   1.0f,		0.0f,	0.0f,	1.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,	0.75f,		0.0f,	0.0f,	1.0f,
		 0.0f,  0.5f, 0.0f,		0.5f,	0.75f,		0.0f,	0.0f,	1.0f,

		 // left
		 //x		y		z		S		T
		 -0.5f, -0.5f,  -0.5f,	0.0f,  0.75f,		1.0f,	0.0f,	0.0f,
		 -0.5f, -0.5f,  0.5f,	0.25f,	0.5f,		1.0f,	0.0f,	0.0f,
		  0.0f,  0.5f,  0.0f,	0.5f,	0.75f,		1.0f,	0.0f,	0.0f,

		  //{Bottom}----------------------------------------------------------
		  // front																	[0]	  \-----/  [1]
		  //x		y		z		S		T			NX		NY		NZ         +           \   /					/
		  -0.5f, -0.5f,  0.5f,	0.25f,  0.5f,		0.0f,	-1.0f,	0.0f,   //0             \ / [12]			
		  0.5f,  -0.5f,  0.5f,	0.75f,	0.50f,		0.0f,	-1.0f,	0.0f,	//1			
		   0.0f,  -1.5f,  0.0f,	0.5f,	0.25f,		0.0f,	-1.0f,	0.0f,	//2

		   // right
		   //x		y		z		S		T
		   0.5f, -0.5f,  0.5f,	    0.75f,  0.5f,		-1.0f,	0.0f,	0.0f,
		   0.5f, -0.5f,  -0.5f,	    1.0f,	0.25f,		-1.0f,	0.0f,	0.0f,
		   0.0f,  -1.5f,   0.0f,	0.5f,	0.25f,		-1.0f,	0.0f,	0.0f,

		   // back
		   //x		y		z		S		T
		   -0.5f, -0.5f, -0.5f,	    0.75f,  0.0f,		0.0f,	0.0f,	1.0f,
		   0.5f, -0.5f, -0.5f,		1.00f,	0.25f,		0.0f,	0.0f,	1.0f,
		   0.0f,  -1.5f, 0.0f,		0.5f,	0.25f,		0.0f,	0.0f,	1.0f,

		   // left
		   //x		y		z		S		T
		   -0.5f, -0.5f,  -0.5f,	0.0f,  0.25f,		1.0f,	0.0f,	0.0f,
		   -0.5f, -0.5f,  0.5f,		0.25f,	0.5f,		1.0f,	0.0f,	0.0f,
			0.0f, -1.5f,  0.0f,		0.5f,	0.25f,		1.0f,	0.0f,	0.0f,

	};

	Mesh* octaedro = new Mesh();
	octaedro->CreateMesh(octaedro_vertices, octaedro_indices, 192, 24);
	meshList.push_back(octaedro);  //Quedaría como el indice 5 en el meshList

}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearDado();
	CrearOctaedro();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	// ========== Práctica (8) =========== //
	AguaTexture = Texture("Textures/agua.tga");
	AguaTexture.LoadTextureA();

	CristalTexture = Texture("Textures/cristal_v2.tga");
	CristalTexture.LoadTextureA();

	// ========================================= //


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");


	// ========== Práctica (7) =========== //
	//Carro_M = Model();
	//Carro_M.LoadModel("Models/coche_texturizado_sin_cofre.obj");

	//Carro_cofre = Model();
	//Carro_cofre.LoadModel("Models/cofre_coche_texturizado.obj");

	//llanta_texturizada_izquierda = Model();
	//llanta_texturizada_izquierda.LoadModel("Models/llanta_texturizada_izquierda.obj");

	//llanta_texturizada_derecha = Model();
	//llanta_texturizada_derecha.LoadModel("Models/llanta_texturizada_derecha.obj");
	//	
	lampara = Model();
	lampara.LoadModel("Models/lampara.obj");

	//// ======================================== //

	//// =========== Ejercicio Práctica 8 ======== //
	//octaedroTexture = Texture("Textures/octaedro_optimizado.tga");
	//octaedroTexture.LoadTextureA();

	// ======================================== //

	// ========== Práctica(8) ========== = //
	Nave = Model();
	Nave.LoadModel("Models/nave.obj");

	Pecera = Model();
	Pecera.LoadModel("Models/pecera.obj");

	Pez_abismal_cuerpo = Model();
	Pez_abismal_cuerpo.LoadModel("Models/cuerpo_pez.obj");

	Pez_abismal_antena = Model();
	Pez_abismal_antena.LoadModel("Models/antena_pez.obj");

	Pez_abismal_foco = Model();
	Pez_abismal_foco.LoadModel("Models/foco_pez.obj");


	// ========================================= //


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	

	// =========================================================================================================================== //
	//contador de luces puntuales
	unsigned int pointLightCount_lampara = 0;
	//Declaración de primer luz puntual
	// ---------------------------------------- Lampara blanca
	pointLights_lampara[0] = PointLight(1.0f, 1.0f, 1.0f,  //Lampara color blanco
		0.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount_lampara++;

	
	unsigned int pointLightCount_pez = 0;
	// ---------------------------------------- Pez abisal
	pointLights_pez[0] = PointLight(0.0f, 1.0f, 1.0f,  //pez color cyan
		0.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount_pez++;

	unsigned int pointLightCount_lampara_pez = 0;

	// ---------------------------------------- ambas luces
	pointLights_lampara_pez[0] = PointLight(1.0f, 1.0f, 1.0f,  //Lampara color blanco
		0.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount_lampara_pez++;

	pointLights_lampara_pez[1] = PointLight(0.0f, 1.0f, 1.0f,  //pez color cyan
		0.0f, 5.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount_lampara_pez++;


	// =========================================================================================================================== //
	
	// ---------------------------- Primer arreglo de spotlights ---------------------
	// Contador de spotlights
	unsigned int spotLightCount = 0;
	//linterna pegada a la cámara siempre VA
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		7.0f);
	spotLightCount++;
	//luz nave
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,     //luz nave X positivo
		1.0f, 2.0f,
		0.0f, -1.0f, 0.0f,
		-3.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//Luz bulbo pezcado
	spotLights[2] = SpotLight(0.0f, 0.0f, 0.0f,
		1.0f, 4.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.1f, 0.05f,
		7.0f);
	spotLightCount++;

	// ---------------------------- Segundo arreglo de spotlights ---------------------
	unsigned int spotLightCount2 = 0;
	//linterna pegada a la cámara siempre VA
	spotLights2[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		7.0f);
	spotLightCount2++;

	//luz nave
	spotLights2[1] = SpotLight(0.0f, 1.0f, 1.0f,     //luz nave X negativo
		1.0f, 2.0f,
		0.0f, -1.0f, 0.0f,
		3.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount2++;

	//Luz bulbo pezcado
	spotLights2[2] = SpotLight(0.0f, 0.0f, 0.0f,
		1.0f, 4.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.1f, 0.05f,
		7.0f);
	spotLightCount2++;

	// =========================================================================================================================== //	


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
	
	
	
	//Configuration
	GLfloat now = 0.0f;
	glm::vec3 lowerLight;
	
	
	// Variables 
	glm::mat4 model(1.0);
	glm::mat4 identidad(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	identidad = glm::mat4(1.0);

	glm::vec3 movimiento_coche;
	glm::vec3 movimiento_nave;
	glm::vec3 pos_pez;
	glm::vec3 pos_pointLight;
	

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		
		// ================================================================= LUZ LAMPARA ================================================================
		//lowerLight = camera.getCameraPosition();
		//lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		////spotLights[1].SetPos(poscoche + glm::vec(x, y, cofre));
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);

		// ================================================================= LUCES INICIO ================================================================
		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);


		if (mainWindow.prendeLuzNave() == 1)
		{
			lowerLight = camera.getCameraPosition();
			lowerLight.y -= 0.3f;
			spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}
		else if (mainWindow.prendeLuzNave() == -1)
		{
			lowerLight = camera.getCameraPosition();
			lowerLight.y -= 0.3f;
			spotLights2[0].SetFlash(lowerLight, camera.getCameraDirection());
			shaderList[0].SetSpotLights(spotLights2, spotLightCount2);
		}

		// ================================================== Ejercicio Práctica (8) ================================================================= //
		if (mainWindow.switch_lampara() && mainWindow.switch_pez()) {
			shaderList[0].SetPointLights(pointLights_lampara_pez, pointLightCount_lampara_pez);
		} else if (mainWindow.switch_lampara()) {
			shaderList[0].SetPointLights(pointLights_lampara, pointLightCount_lampara);
		} else if (mainWindow.switch_pez()) {
			shaderList[0].SetPointLights(pointLights_pez, pointLightCount_pez);
		} else {
			shaderList[0].SetPointLights(pointLights_lampara, 0);
			shaderList[0].SetPointLights(pointLights_pez, 0);
			shaderList[0].SetPointLights(pointLights_lampara_pez, 0);
		}
		// ================================================================= LUCES FIN ================================================================
		
		// ----------------------------------------- PISO -----------------------------------------
		model = identidad;
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
				
		// ================================================== Reporte Práctica (8) ================================================================= //
		
		// 1.-gregar luz de tipo spotlight para la nave de tal forma que al avanzar (mover con teclado hacia 
		// dirección de X negativa ) ilumine con un spotlight en el suelo hacia adelante y al retroceder(mover 
		// con teclado hacia dirección de X positiva) ilumine con un spotlight el suelo hacia  atrás.Son dos 
		// spotlights diferentes que se prenderán y apagarán de acuerdo a la dirección en la que esté moviéndose el helicóptero.
		model = identidad;
		model = glm::translate(model, glm::vec3(mainWindow.getPosNave(), 8.0f, 8.0));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		if (mainWindow.prendeLuzNave() == 1) {
			movimiento_nave = glm::vec3(model[3]);  //Obtengo la posición de la nave
			movimiento_nave.y += 0.5f;
			spotLights[1].SetPos(movimiento_nave);
		}
		else if (mainWindow.prendeLuzNave() == -1) {
			movimiento_nave = glm::vec3(model[3]);  //Obtengo la posición de la nave
			movimiento_nave.y += 0.5f;
			spotLights2[1].SetPos(movimiento_nave);
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Nave.RenderModel();

		// 2. - Crear o instanciar una pecera(traslucida) con las normales hacia adentro y en la parte superior imagen de 
		// agua e instanciar dentro de la pecera al pez abisal con movimiento de teclado para subir y bajar en diagonal.
		model = identidad;
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -20.0));
		modelaux = model;
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pecera.RenderModel();

		//Agua textura
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 9.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		AguaTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();

		// Pez cuerpo
		model = modelaux;                         // X : -7 <-> 7			 Y : -2  <->  2
		model = glm::translate(model, glm::vec3(mainWindow.MovingPezX(), mainWindow.MovingPezY() + 4.5f, 0.0));
		if (mainWindow.getDireccionPez() == 1)
		{
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		else
		{
			model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pez_abismal_cuerpo.RenderModel();

		// Pez antena
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.25f, 1.0f, 0.35));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pez_abismal_antena.RenderModel();

		// Pez foco
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.33f, -1.0f));
		//modelaux = model;
		model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
		// ----------------------------- pointlights luces -----------------------------
		if (mainWindow.switch_lampara() && mainWindow.switch_pez()) {
			pos_pointLight = glm::vec3(model[3]);
			pos_pointLight.y -= 0.5f;
			pos_pointLight.z -= 0.5f;
			pointLights_lampara_pez[1].SetPos(pos_pointLight);
		}
		else if (mainWindow.switch_lampara()) {
		}
		else if (mainWindow.switch_pez()) {
			pos_pointLight = glm::vec3(model[3]);
			pos_pointLight.y -= 0.5f;
			pos_pointLight.z -= 0.5f;
			pointLights_pez[0].SetPos(pos_pointLight);
		}

		// ----------------------------- spotlights luces -----------------------------
		if (mainWindow.spotLightDirPez() == 1) {      // X
			pos_pez = glm::vec3(model[3]);  //Obtengo la posición de la nave
			//pos_pez.y += 2.0f;
			if (mainWindow.prendeLuzNave() == 1) {
				spotLights[2].SetColor(glm::vec3(1.0f, 0.0f, 1.0f));  // Morada
				spotLights[2].SetPos(pos_pez);
				spotLights[2].SetDir(glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
			}
			else if (mainWindow.prendeLuzNave() == -1) {
				spotLights2[2].SetColor(glm::vec3(1.0f, 0.0f, 1.0f));  // Morada
				spotLights2[2].SetPos(pos_pez);
				spotLights2[2].SetDir(glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
			}
		}
		else if (mainWindow.spotLightDirPez() == 2) {  // Y
			pos_pez = glm::vec3(model[3]);  //Obtengo la posición de la nave
			//pos_pez.y += 2.0f;
			if (mainWindow.prendeLuzNave() == 1) {
				spotLights[2].SetColor(glm::vec3(1.0f, 1.0f, 0.0f));  // Amarillo
				spotLights[2].SetPos(pos_pez);
				spotLights[2].SetDir(glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
			}
			else if (mainWindow.prendeLuzNave() == -1) {
				spotLights2[2].SetColor(glm::vec3(1.0f, 1.0f, 0.0f));  // Amarillo
				spotLights2[2].SetPos(pos_pez);
				spotLights2[2].SetDir(glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
			}
		}
		else if (mainWindow.spotLightDirPez() == 3) {   // Z
			pos_pez = glm::vec3(model[3]);  //Obtengo la posición de la nave
			//pos_pez.y += 2.0f;
			if (mainWindow.prendeLuzNave() == 1) {
				spotLights[2].SetColor(glm::vec3(0.0f, 1.0f, 0.0f));  // verde
				spotLights[2].SetPos(pos_pez);
				spotLights[2].SetDir(glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)));
			}
			else if (mainWindow.prendeLuzNave() == -1) {
				spotLights2[2].SetColor(glm::vec3(0.0f, 1.0f, 0.0f));  // Verde
				spotLights2[2].SetPos(pos_pez);
				spotLights2[2].SetDir(glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f)));
			}
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pez_abismal_foco.RenderModel();


		// --------------------------------------- Detalles pecera ---------------------------
		//Cristal pecera fondo
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 5.5f, -4.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		CristalTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		glDisable(GL_BLEND);

		//Cristal pecera derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(9.5f, 5.5f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		CristalTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		glDisable(GL_BLEND);

		//Cristal pecera izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-9.5f, 5.5f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		CristalTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		glDisable(GL_BLEND);


		// 3. - Agregar una luz de tipo puntual  de color azul ligada al bulbo del pez que puedan prender y apagar de forma independiente con 
		// teclado tanto la luz de la lámpara de su práctica 7 como esta luz ( la luz de la lámpara debe de ser puntual, si la crearon 
		// spotlight en su reporte 7 tienen que cambiarla a luz puntual), de tal forma que se pueda ver: las 2 luces apagadas, las 2 luces 
		// prendidas, una luz prendida y una luz apagada y viceversa. 
		model = identidad;
		model = glm::translate(model, glm::vec3(18.0, -1.0f, 20.0f));
		if (mainWindow.switch_lampara() && mainWindow.switch_pez()) {
			pos_pointLight = glm::vec3(model[3]);
			pos_pointLight.y += 8.0f;
			pointLights_lampara_pez[0].SetPos(pos_pointLight);
		}
		else if (mainWindow.switch_lampara()) {
			pos_pointLight = glm::vec3(model[3]);
			pos_pointLight.y += 8.0f;
			pointLights_lampara[0].SetPos(pos_pointLight);
		}
		else if (mainWindow.switch_pez()) {			
		}
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lampara.RenderModel();
						
		// ================================================== Reporte (8) ================================================================= //

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = identidad;
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		
		
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
