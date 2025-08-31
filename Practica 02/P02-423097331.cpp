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
const float toRadians = 3.14159265f / 180.0; //grados a radianes
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
static const char* fRojo = "./P02-423097331-rojo.frag";
static const char* fVerde = "./P02-423097331-verde.frag";
static const char* fAzul = "./P02-423097331-azul.frag";
static const char* fCafe = "./P02-423097331-cafe.frag";
static const char* fVerdeOsc = "./P02-423097331-verde-oscuro.frag";

float angulo = 0.0f;

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
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
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
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	// Letra E
	GLfloat vertices_letraE[] = {
		-0.9f,	0.5f,	0.0f,		0.424f,	0.275f,	0.459f,
		-0.8f,	0.5f,	0.0f,		0.424f,	0.275f,	0.459f,
		-0.9f,	-0.5f,	0.0f,		0.424f,	0.275f,	0.459f,

		-0.9f,	-0.5f,	0.0f,		0.424f,	0.275f,	0.459f,
		-0.8f,	0.5f,	0.0f,		0.424f,	0.275f,	0.459f,
		-0.8f,	-0.5f,	0.0f,		0.424f,	0.275f,	0.459f,

		-0.8f,	0.5f,	0.0f,		0.424f,	0.275f,	0.459f,
		-0.4f,	0.5f,	0.0f,		0.424f,	0.275f,	0.459f,
		-0.4f,	0.4f,	0.0f,		0.424f,	0.275f,	0.459f,

		-0.8f,	0.5f,	0.0f,		0.424f,	0.275f,	0.459f,
		-0.4f,	0.4f,	0.0f,		0.424f,	0.275f,	0.459f,
		-0.8f,	0.4f,	0.0f,		0.424f,	0.275f,	0.459f,

		-0.8f,	-0.5f,	0.0f,		0.424f,	0.275f,	0.459f,
		-0.4f,	-0.4f,	0.0f,		0.424f,	0.275f,	0.459f,
		-0.8f,	-0.4f,	0.0f,		0.424f,	0.275f,	0.459f,

		-0.8f,	-0.5f,	0.0f,		0.424f,	0.275f,	0.459f,
		-0.4f,	-0.5f,	0.0f,		0.424f,	0.275f,	0.459f,
		-0.4f,	-0.4f,	0.0f,		0.424f,	0.275f,	0.459f,

		-0.8f,	0.05f,	0.0f,		0.424f,	0.275f,	0.459f,
		-0.4f,	0.05f,	0.0f,		0.424f,	0.275f,	0.459f,
		-0.8f,	-0.05f, 0.0f,		0.424f,	0.275f,	0.459f,

		-0.8f,	-0.05f, 0.0f,		0.424f,	0.275f,	0.459f,
		-0.4f,	0.05f,	0.0f,		0.424f,	0.275f,	0.459f,
		-0.4f,	-0.05f, 0.0f,		0.424f,	0.275f,	0.459f,
	};

	// Letra C 
	GLfloat vertices_letraC[] = {
		-0.3f,	0.5f,	0.0f,		0.259f,	0.478f,	0.255f,
		-0.2f,	0.5f,	0.0f,		0.259f,	0.478f,	0.255f,
		-0.3f,	-0.5f,	0.0f,		0.259f,	0.478f,	0.255f,

		-0.3f,	-0.5f,	0.0f,		0.259f,	0.478f,	0.255f,
		-0.2f,	0.5f,	0.0f,		0.259f,	0.478f,	0.255f,
		-0.2f,	-0.5f,	0.0f,		0.259f,	0.478f,	0.255f,

		-0.2f,	0.5f,	0.0f,		0.259f,	0.478f,	0.255f,
		0.3f,	0.5f,	0.0f,		0.259f,	0.478f,	0.255f,
		0.3f,	0.4f,	0.0f,		0.259f,	0.478f,	0.255f,

		-0.2f,	0.5f,	0.0f,		0.259f,	0.478f,	0.255f,
		0.3f,	0.4f,	0.0f,		0.259f,	0.478f,	0.255f,
		-0.2f,	0.4f,	0.0f,		0.259f,	0.478f,	0.255f,

		-0.2f,	-0.5f,	0.0f,		0.259f,	0.478f,	0.255f,
		0.3f,	-0.4f,	0.0f,		0.259f,	0.478f,	0.255f,
		-0.2f,	-0.4f,	0.0f,		0.259f,	0.478f,	0.255f,

		-0.2f,	-0.5f,	0.0f,		0.259f,	0.478f,	0.255f,
		0.3f,	-0.5f,	0.0f,		0.259f,	0.478f,	0.255f,
		0.3f,	-0.4f,	0.0f,		0.259f,	0.478f,	0.255f,
	};

	// Letra G
	GLfloat vertices_letraG[] = {
		0.5f,	0.5f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.4f,	0.5f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.5f,	-0.5f,	0.0f,		0.961f,	0.251f,	0.129f,

		0.5f,	-0.5f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.4f,	0.5f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.4f,	-0.5f,	0.0f,		0.961f,	0.251f,	0.129f,

		0.5f,	0.5f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.9f,	0.5f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.9f,	0.4f,	0.0f,		0.961f,	0.251f,	0.129f,

		0.5f,	0.5f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.9f,	0.4f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.5f,	0.4f,	0.0f,		0.961f,	0.251f,	0.129f,

		0.5f,	-0.5f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.9f,	-0.5f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.9f,	-0.4f,	0.0f,		0.961f,	0.251f,	0.129f,

		0.5f,	-0.5f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.9f,	-0.4f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.5f,	-0.4f,	0.0f,		0.961f,	0.251f,	0.129f,

		0.7f,	0.05f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.9f,	0.05f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.7f,	-0.05f, 0.0f,		0.961f,	0.251f,	0.129f,

		0.7f,	-0.05f, 0.0f,		0.961f,	0.251f,	0.129f,
		0.9f,	0.05f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.9f,	-0.05f, 0.0f,		0.961f,	0.251f,	0.129f,

		0.9f,	0.0f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.8f,	0.0f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.9f,	-0.5f,	0.0f,		0.961f,	0.251f,	0.129f,

		0.9f,	-0.5f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.8f,	0.0f,	0.0f,		0.961f,	0.251f,	0.129f,
		0.8f,	-0.5f,	0.0f,		0.961f,	0.251f,	0.129f,
	};

	// Mesh para la letra E
	MeshColor* mE = new MeshColor();
	mE->CreateMeshColor(vertices_letraE, 144);
	meshColorList.push_back(mE);

	// Mesh para la letra C
	MeshColor* mC = new MeshColor();
	mC->CreateMeshColor(vertices_letraC, 108);
	meshColorList.push_back(mC);

	// Mesh para la letra G
	MeshColor* mG = new MeshColor();
	mG->CreateMeshColor(vertices_letraG, 180);
	meshColorList.push_back(mG);
}

void CreateShaders() 
{
	shaderList.clear();

	auto mk = [&](const char* vs, const char* fs) {
		Shader* s = new Shader();
		s->CreateFromFiles(vs, fs);
		shaderList.push_back(*s);
		};
	mk(vShader, fRojo);      // 0
	mk(vShader, fVerde);     // 1
	mk(vShader, fAzul);      // 2
	mk(vShader, fCafe);      // 3
	mk(vShader, fVerdeOsc);  // 4
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	
	//CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	
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
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad

		/*
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		// Bucle para dibujar cada letra
		for (int i = 0; i < meshColorList.size(); i++)
		{
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			meshColorList[i]->RenderMeshColor();
		}
		*/

		// Techo
		shaderList[2].useShader();
		GLuint uM = shaderList[2].getModelLocation();
		GLuint uP = shaderList[2].getProjectLocation();
		glUniformMatrix4fv(uP, 1, GL_FALSE, glm::value_ptr(projection));
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.6f, -2.0f));
		model = glm::scale(model, glm::vec3(1.30f, 0.70f, 1.0f));
		glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Cubo rojo
		shaderList[0].useShader();
		uM = shaderList[0].getModelLocation(); uP = shaderList[0].getProjectLocation();
		glUniformMatrix4fv(uP, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.25f, -2.05f));
		model = glm::scale(model, glm::vec3(1.12f, 1.02f, 1.0f));
		glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Ventana izquierda
		shaderList[1].useShader();
		uM = shaderList[1].getModelLocation(); uP = shaderList[1].getProjectLocation();
		glUniformMatrix4fv(uP, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.38f, 0.06f, 0.0f));
		model = glm::scale(model, glm::vec3(0.30f, 0.28f, 1.0f));
		glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Ventana derecha
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.38f, 0.06f, -1.95f));
		model = glm::scale(model, glm::vec3(0.30f, 0.28f, 1.0f));
		glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Puerta
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.45f, -1.95f));
		model = glm::scale(model, glm::vec3(0.30f, 0.42f, 1.0f));
		glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Pino izquierdo
		shaderList[4].useShader();
		uM = shaderList[4].getModelLocation(); uP = shaderList[4].getProjectLocation();
		glUniformMatrix4fv(uP, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.78f, -0.32f, -2.0f));
		model = glm::scale(model, glm::vec3(0.44f, 0.6f, 1.0f));
		glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Tronco izquierdo
		shaderList[3].useShader();
		uM = shaderList[3].getModelLocation(); uP = shaderList[3].getProjectLocation();
		glUniformMatrix4fv(uP, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.78f, -0.82f, -1.9f));
		model = glm::scale(model, glm::vec3(0.18f, 0.4f, 1.0f));
		glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		// Pino derecho
		shaderList[4].useShader();
		uM = shaderList[4].getModelLocation(); uP = shaderList[4].getProjectLocation();
		glUniformMatrix4fv(uP, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.78f, -0.32f, -2.0f));
		model = glm::scale(model, glm::vec3(0.44f, 0.6f, 1.0f));
		glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(model));
		meshList[0]->RenderMesh();

		// Tronco derecho
		shaderList[3].useShader();
		uM = shaderList[3].getModelLocation(); uP = shaderList[3].getProjectLocation();
		glUniformMatrix4fv(uP, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.78f, -0.82f, -1.9f));
		model = glm::scale(model, glm::vec3(0.18f, 0.4f, 1.0f));
		glUniformMatrix4fv(uM, 1, GL_FALSE, glm::value_ptr(model));
		meshList[1]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}
