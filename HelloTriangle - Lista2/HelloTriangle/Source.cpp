/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 14/08/2023
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;


// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Classe para manipula��o dos shaders
#include "Shader.h"

// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupGeometry();

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 600;

// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! -- Vitor", nullptr, nullptr);
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

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	GLuint VAO = setupGeometry();
		
	glm::mat4 projection = glm::mat4(1); //matriz identidade

	float angle = (float)glfwGetTime();

	//Aplicando transformacoes
	//model = glm::translate(model, glm::vec3(400.0, 300.0, 0.0));
	//model = glm::rotate(model, /*glm::radians(45.0f)*/ angle , glm::vec3 * (0.0, 0.0, 1.0));
	//model = glm::scale(model, glm::vec3(100.0, 100.0, 0.0));

	//Enviando a matriz modelo para o shader
	//shader.setMat4(model, glm::value_ptr(model));

	//Exerc�cio 1
	projection = glm::ortho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);

	//Exerc�cio 2
	//projection = glm::ortho(0.0, 800.0, 600.0, 00.0, -1.0, 1.0);

	//Exerc�cio 3
	//projection = glm::ortho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	shader.Use();

	shader.setMat4("projection", glm::value_ptr(projection));

	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		// Recuperando o tamanho da janela da aplica��o
		glfwGetFramebufferSize(window, &width, &height);

		// Dimensiona a viewport
		glViewport(width / 2, height / 2, width / 2, height / 2);
		glBindVertexArray(VAO); //Conectando ao buffer de geometria

		shader.setVec4("inputColor", 0.0, 0.0, 1.0, 1.0); //enviando cor para vari�vel uniform inputColor

		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// Chamada de desenho - drawcall
		// Contorno - Loop de linhas - GL_LINE_LOOP
		shader.setVec4("inputColor", 1.0, 1.0, 0.0, 1.0);
		glDrawArrays(GL_LINE_LOOP, 0, 3);
		shader.setVec4("inputColor", 0.0, 1.0, 1.0, 1.0);
		glDrawArrays(GL_LINE_LOOP, 3, 3);

		// Chamada de desenho - drawcall
		// PONTOS - GL_POINTS
		shader.setVec4("inputColor", 1.0, 0.0, 1.0, 1.0);
		glDrawArrays(GL_POINTS, 0, 6);

		// Dimensiona a segunda viewport
		glViewport(0, 0, width / 2, height / 2);
		shader.setVec4("inputColor", 0.0, 0.0, 1.0, 1.0); //enviando cor para vari�vel uniform inputColor

		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Chamada de desenho - drawcall
		// Contorno - Loop de linhas - GL_LINE_LOOP
		shader.setVec4("inputColor", 1.0, 1.0, 0.0, 1.0);
		glDrawArrays(GL_LINE_LOOP, 0, 3);
		shader.setVec4("inputColor", 0.0, 1.0, 1.0, 1.0);
		glDrawArrays(GL_LINE_LOOP, 3, 3);

		// Chamada de desenho - drawcall
		// PONTOS - GL_POINTS
		shader.setVec4("inputColor", 1.0, 0.0, 1.0, 1.0);
		glDrawArrays(GL_POINTS, 0, 6);

		// Dimensiona a terceira viewport
		glViewport(width, height, width, height);
		shader.setVec4("inputColor", 0.0, 0.0, 1.0, 1.0); //enviando cor para vari�vel uniform inputColor

		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Chamada de desenho - drawcall
		// Contorno - Loop de linhas - GL_LINE_LOOP
		shader.setVec4("inputColor", 1.0, 1.0, 0.0, 1.0);
		glDrawArrays(GL_LINE_LOOP, 0, 3);
		shader.setVec4("inputColor", 0.0, 1.0, 1.0, 1.0);
		glDrawArrays(GL_LINE_LOOP, 3, 3);

		// Chamada de desenho - drawcall
		// PONTOS - GL_POINTS
		shader.setVec4("inputColor", 1.0, 0.0, 1.0, 1.0);
		glDrawArrays(GL_POINTS, 0, 6);

		glBindVertexArray(0); //Desconectando o buffer de geometria

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupGeometry()
{
	GLfloat vertices[] = {
		//x   y     z
		-0.5,  0.5, 0.0, //v0
		 0.0,  0.0, 0.0, //v1
 		 0.5,  0.5, 0.0, //v2 

		 0.0,  0.0, 0.0, //v3
		-0.5, -0.5, 0.0, //v4
		 0.5, -0.5, 0.0, //v5 
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
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0); 

	return VAO;
}

