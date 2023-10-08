#include "Sprite.h"

Sprite::Sprite()
{
	//Inicializar outros atributos
	vel = 5;
}

Sprite::~Sprite()
{
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
}

void Sprite::initialize(int nAnimations, int nFrames)
{
	this->nAnimations = nAnimations;
	this->nFrames = nFrames;
	ds = 1.0 / (float) nFrames;
	dt = 1.0 / (float)nAnimations;
	iFrame = 0;
	iAnimation = 0;

	GLfloat vertices[] = {
		//Primeiro Tri�ngulo
		//x   y     z    s    t   
		-0.5, -0.5, 0.0, 0.0, 0.0, //v0
		 0.5,  0.5, 0.0, ds, dt, //v1
		-0.5,  0.5, 0.0, 0.0, dt, //v2

		//Segundo Tri�ngulo
		-0.5, -0.5, 0.0, 0.0, 0.0, //v0
		 0.5, -0.5, 0.0, ds, 0.0, //v3 
		 0.5,  0.5, 0.0, ds, dt, //v1
	};

	GLuint VBO;
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

	//Inicializa outros atributos
	state = IDLE;
	
}

void Sprite::update()
{
	//Conecta com o VAO
	glBindVertexArray(VAO);

	//Altera a matriz a transforma��o
	glm::mat4 model = glm::mat4(1); //matriz identidade
	//Aplicando as transforma��es
	model = glm::translate(model, position);
	//rotate..
	 
	if (state == MOVING_LEFT) scaleFactor = glm::vec3(-dimensions.x, dimensions.y, dimensions.z);
	else if (state == MOVING_RIGHT) scaleFactor = dimensions;

	model = glm::scale(model, scaleFactor);
	//Enviando a matriz de modelo para o shader
	shader->setMat4("model", glm::value_ptr(model));

	shader->setVec2("offsets", iFrame * ds, iAnimation * dt);

	if (state != IDLE) //neste caso IDLE � ficar parado mesmo, n�o a anima��o de IDLE
	{
		//para anima��es c�clicas, sempre incrementa o �ndice do Frame
		iFrame = (iFrame + 1) % nFrames;
	}


	//Desconecta com o VAO
	glBindVertexArray(0);
}

void Sprite::draw()
{
	//Conecta com o VAO
	glBindVertexArray(VAO);

	//Conecta a textura do sprite
	glBindTexture(GL_TEXTURE_2D, texID);
	
	//Faz a chamada de desenho
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Desconecta com o VAO
	glBindVertexArray(0);

	//Desconecta a textura
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Sprite::moveLeft()
{
	position.x -= vel;
}

void Sprite::moveRight()
{
	position.x += vel;
}

void Sprite::moveUp()
{
	position.y += vel;
}

void Sprite::moveDown()
{
	position.y -= vel;
}
