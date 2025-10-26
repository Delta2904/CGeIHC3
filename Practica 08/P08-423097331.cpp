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

Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

Model coche_cuerpo;
Model coche_rueda_izquierda_1;
Model coche_rueda_izquierda_2;
Model coche_rueda_derecha_1;
Model coche_rueda_derecha_2;
Model Cofre_M;
Model Lampara_M;
Model Baston_M;

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
// para declarar varias luces de tipo spotlight
SpotLight spotLights[MAX_SPOT_LIGHTS];
// 1er arreglo tipo spotlight
SpotLight spotLightsF[MAX_SPOT_LIGHTS];
// 2do arreglo tipo spotlight
SpotLight spotLightsT[MAX_SPOT_LIGHTS];
// 1er arreglo tipo pointlight
PointLight pointLightL[MAX_POINT_LIGHTS];
// 2do arreglo de luces puntuales
PointLight pointLightB[MAX_POINT_LIGHTS];
// 3er arreglo para ambas luces prendidas
PointLight pointLightsCombinadas[MAX_POINT_LIGHTS];

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

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();

	coche_cuerpo = Model();
	coche_cuerpo.LoadModel("Models/coche.dae");
	coche_rueda_izquierda_1 = Model();
	coche_rueda_izquierda_1.LoadModel("Models/coche_rueda_izquierda_1-423097331.dae");
	coche_rueda_izquierda_2 = Model();
	coche_rueda_izquierda_2.LoadModel("Models/coche_rueda_izquierda_2-423097331.dae");
	coche_rueda_derecha_1 = Model();
	coche_rueda_derecha_1.LoadModel("Models/coche_rueda_derecha_1-423097331.dae");
	coche_rueda_derecha_2 = Model();
	coche_rueda_derecha_2.LoadModel("Models/coche_rueda_derecha_2-423097331.dae");
	Cofre_M = Model();
	Cofre_M.LoadModel("Models/cofre.dae");
	Lampara_M = Model();
	Lampara_M.LoadModel("Models/lampara.obj");
	Baston_M = Model();
	Baston_M.LoadModel("Models/baston.dae");

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

	unsigned int spotLightCount = 0;
	// Luz 0: Luz del Cofre (morada)
	spotLightsF[0] = SpotLight(0.7f, 0.1f, 0.9f,
		0.0f, 4.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.5f, 0.1f, 0.0f,
		25.0f);
	spotLightCount++;

	spotLightsT[0] = spotLightsF[0];

	// Luz 1: Luz delantera (verde árbol) 
	spotLightsF[1] = SpotLight(0.1f, 0.6f, 0.1f,
		0.0f, 8.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.5f, 0.1f, 0.0f,
		25.0f);
	spotLightCount++;

	// Luz 1: Luz trasera (ambar)
	spotLightsT[1] = SpotLight(0.9f, 0.7f, 0.1f,
		0.0f, 4.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.5f, 0.1f, 0.0f,
		25.0f);

	float tipoSpotlight = 0;

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	// Lampara
	pointLightL[0] = PointLight(1.0f, 1.0f, 1.0f,
		0.1f, 2.5f,
		5.0f, 10.0f, -20.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// Bastón del vacío (League of Legends)
	pointLightB[0] = PointLight(0.4f, 0.0f, 0.67f,
		0.1f, 5.0f,
		20.0f, 6.5f, -20.0f,
		0.3f, 0.2f, 0.1f);

	// Ambos pointlights prendidos
	pointLightsCombinadas[0] = pointLightL[0];
	pointLightsCombinadas[1] = pointLightB[0];

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);


	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lowerLight = glm::vec3(0.0f, 0.0f, 0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
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

		// Variable base para la posición de todas las luces del coche
		glm::vec3 posCoche = glm::vec3(0.0f + mainWindow.getmuevex(), 2.0f, -4.0f);

		// Ejercicio 1: Actualizar luz del Cofre (Morada)
		glm::vec3 posLuzCofre = posCoche + glm::vec3(0.0f, 0.0f, 0.0f);
		float anguloCofre = mainWindow.getAbrirCofre() - mainWindow.getCerrarCofre();
		glm::mat4 rotMatrixCofre = glm::rotate(glm::mat4(1.0f), glm::radians(anguloCofre), glm::vec3(0.0f, 0.0f, -1.0f));
		glm::vec3 dirLuzCofre = glm::vec3(rotMatrixCofre * glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f));
		spotLightsF[0].SetFlash(posLuzCofre, glm::normalize(dirLuzCofre));
		spotLightsT[0].SetFlash(posLuzCofre, glm::normalize(dirLuzCofre));

		// Ejercicio 2: Actualizar luces Delantera 
		glm::vec3 posFaroDelantero = posCoche + glm::vec3(0.0f, -0.5f, -1.0f);
		glm::vec3 posFaroTrasero = posCoche + glm::vec3(7.0f, -0.5f, 1.0f);
		spotLightsF[1].SetFlash(posFaroDelantero, glm::vec3(-1.0f, 0.0f, 0.0f));
		spotLightsT[1].SetFlash(posFaroTrasero, glm::vec3(1.0f, 0.0f, 0.0f));

		tipoSpotlight = mainWindow.getlucespuntuales();
		if (tipoSpotlight < 0.5f)
		{
			shaderList[0].SetSpotLights(spotLightsF, spotLightCount);
		}
		else if (tipoSpotlight < 1.5f)
		{
			shaderList[0].SetSpotLights(spotLightsT, spotLightCount);
		}

		// Ejercicio 3: Prender y apagar luces
		if (mainWindow.getprendida() && mainWindow.getBastonPrendido())
		{
			// Ambas luces prendidas
			shaderList[0].SetPointLights(pointLightsCombinadas, 2);
		}
		else if (mainWindow.getprendida())
		{
			// Solo la lampara prendida
			shaderList[0].SetPointLights(pointLightL, 1);
		}
		else if (mainWindow.getBastonPrendido())
		{
			// Solo el bastón prendido
			shaderList[0].SetPointLights(pointLightB, 1);
		}
		else
		{
			// Ambas luces apagadas
			shaderList[0].SetPointLights(pointLightL, 0);
		}

		shaderList[0].SetDirectionalLight(&mainLight);

		// --- INICIO DE RENDERIZADO DE OBJETOS ---
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		// Instancia del coche
		// (Usamos la variable 'posCoche' que definimos arriba para la luz, pero la 'Y' es diferente)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posCoche.x, 2.0f, -3.0f)); // Posición del coche
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		coche_cuerpo.RenderModel();

		// Rueda izquierda 1
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.5f, -1.0f, 3.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		coche_rueda_izquierda_1.RenderModel();

		// Rueda izquierda 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.0f, -1.0f, 2.75f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		coche_rueda_izquierda_2.RenderModel();

		// Rueda derecha 1
		model = modelaux;
		model = glm::translate(model, glm::vec3(-4.75f, -1.0f, -4.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		coche_rueda_derecha_1.RenderModel();

		// Rueda derecha 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(11.75f, -1.0f, -4.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(11.0f, 11.0f, 11.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		coche_rueda_derecha_2.RenderModel();

		// Cofre
		model = modelaux;
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getAbrirCofre()), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getCerrarCofre()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre_M.RenderModel();

		// Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, -1.0f, -20.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara_M.RenderModel();

		// Bastón del vacío (League of Legends)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 2.5f, -20.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Baston_M.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
