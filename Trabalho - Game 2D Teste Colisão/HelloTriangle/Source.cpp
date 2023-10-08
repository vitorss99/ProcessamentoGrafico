/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Vitor Soares Souza
 * para a disciplina de Processamento Gr�fico - Unisinos
 * Vers�o inicial: 14/08/2023
 * �ltima atualiza��o em 08/10/2023
 */

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

using namespace std;


// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//STB_IMAGE
#include <stb_image.h>

//Classe para manipula��o dos shaders
#include "Shader.h"

//Classe Sprite 
#include "Sprite.h"

//Classe Timer
#include "Timer.h"

// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupGeometry();
int setupTexture(string filePath, int &width, int &height);
int setupSprite();

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 600;

Sprite personagem;
Sprite personagem2;

// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! -- Vitor Soares", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
	int width, height;
	


	// Compilando e buildando o programa de shader
	Shader shader("../shaders/helloTriangle.vs", "../shaders/helloTriangle.fs");

	shader.Use();

	//Fazendo a leitura da textura do personagem
	int sprWidth, sprHeight;
	int texID = setupTexture("../../Textures/characters/walk.png", sprWidth, sprHeight);

	int sprWidth2, sprHeight2;
	int texID2 = setupTexture("../../Textures/backgrounds/postapocalypse3.png", sprWidth2, sprHeight2);

	// Criando a inst�ncia de nosso objeto sprite do Personagem
	personagem.initialize(1,6);
	personagem.setPosition(glm::vec3(400.0, 300.0, 0.0));
	personagem.setDimensions(glm::vec3(sprWidth/6, sprHeight, 1.0));
	personagem.setShader(&shader);
	personagem.setTexID(texID);

	// Criando a inst�ncia de nosso objeto sprite do Personagem
	personagem2.initialize(1, 6);
	personagem2.setPosition(glm::vec3(100.0, 100.0, 0.0));
	personagem2.setDimensions(glm::vec3(sprWidth / 6, sprHeight, 1.0));
	personagem2.setShader(&shader);
	personagem2.setTexID(texID);

	//Criando a inst�ncia de nosso objeto sprite do fundo (background)
	Sprite background;
	background.initialize(1,1);
	background.setPosition(glm::vec3(400.0, 300.0, 0.0));
	background.setDimensions(glm::vec3(sprWidth2, sprHeight2, 1.0));
	background.setShader(&shader);
	background.setTexID(texID2);

	//Cria a matriz de proje��o paralela ortog�fica
	glm::mat4 projection = glm::mat4(1); //matriz identidade
	projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	
	shader.setMat4("projection", glm::value_ptr(projection));
	shader.setInt("texbuffer", 0);

	glActiveTexture(GL_TEXTURE0);

	//Habilitando a transpar�ncia
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Habilitando o teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	Timer timer;

	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		timer.start();
		
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		// Recuperando o tamanho da janela da aplica��o
		glfwGetFramebufferSize(window, &width, &height);
		// Dimensiona a viewport
		glViewport(0, 0, width, height);
		

		//-------------------------------------------------------------
		background.update();
		background.draw();
		//-------------------------------------------------------------


		//-------------------------------------------------------------
		personagem.update();
		personagem.draw();

		personagem2.update();
		personagem2.draw();
		//-------------------------------------------------------------


		timer.finish();
		double waitingTime = timer.calcWaitingTime(24, timer.getElapsedTimeMs());
		if (waitingTime)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds((int)waitingTime));
		}

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}

	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if ( key == GLFW_KEY_A || key == GLFW_KEY_LEFT )
	{
		personagem.setState(MOVING_LEFT);
		personagem.moveLeft();
	}
	else if ( key == GLFW_KEY_D || key == GLFW_KEY_RIGHT )
	{
		personagem.setState(MOVING_RIGHT);	
		personagem.moveRight();
	}
	else if (key == GLFW_KEY_W || key == GLFW_KEY_UP)
	{
		personagem.setState(MOVING_UP);
		personagem.moveUp();
	}
	else if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN)
	{
		personagem.setState(MOVING_DOWN);
		personagem.moveDown();
	}
	if (action == GLFW_RELEASE) //soltou a tecla
	{
		personagem.setState(IDLE);
	}
}

int setupGeometry()
{
	GLfloat vertices[] = {
		//x   y     z    s    t   
		-0.5, -0.5, 0.0, 0.0, 0.0,//v0
		 0.5, -0.5, 0.0, 1.0, 0.0,//v1
 		 0.0,  0.5, 0.0, 0.5, 1.0 //v2 
	};

	GLuint VBO, VAO;
	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Atributo posi��o
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo coordenada de textura 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0); 

	return VAO;
}

int setupTexture(string filePath, int &width, int &height)
{
	GLuint texID;
	// Gera��o do identificador do buffer
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Configurando par�metro de wrapping da textura em s e t
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	//Configurando o par�metro de filtering de magnifica��o e minifica��o da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	
	int nrChannels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels,0);
	if (data)
	{
		if (nrChannels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

	}
	else
	{
		cout << "Erro ao ler a textura!" << endl;
		return -1;
	}
	
	return texID;
}

int setupSprite()
{
	GLfloat vertices[] = {
		//Primeiro Tri�ngulo
		//x   y     z    s    t   
		-0.5, -0.5, 0.0, 0.0, 0.0, //v0
		 0.5,  0.5, 0.0, 1.0, 1.0, //v1
		-0.5,  0.5, 0.0, 0.0, 1.0, //v2

		//Segundo Tri�ngulo
		-0.5, -0.5, 0.0, 0.0, 0.0, //v0
		 0.5, -0.5, 0.0, 1.0, 0.0, //v3 
		 0.5,  0.5, 0.0, 1.0, 1.0, //v1
	};

	GLuint VBO, VAO;
	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo coordenada de textura 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}


