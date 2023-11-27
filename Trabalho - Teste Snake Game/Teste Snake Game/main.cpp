#include "main.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "shader.h"
#include "square.h"
#include <unistd.h>
using namespace std;

// object id variables
unsigned int vertexBuffer;
unsigned int vertexArray;

// vertex of points, two triangles for square
float length = 0.08;
float vertices[] = {
    -length / 2, length / 2, 0.0f,
    -length / 2, -length / 2, 0.0f,
    length / 2, -length / 2, 0.0f,

    -length / 2, length / 2, 0.0f,
    length / 2, -length / 2, 0.0f,
    length / 2, length / 2, 0.0f };

std::vector<Square*> snakeBlockList;

void moveSnake()
{
    // for each block in the snake, it moves in the same direction
    for (auto next : snakeBlockList)
    {
        next->move();
    }

    // each block should follow the previous one
    for (int i = snakeBlockList.size() - 1; i > 0; i--)
    {
        snakeBlockList[i]->setDirection(snakeBlockList[i - 1]->getDirection());
    }
}
void drawSnake(ShaderProg& program)
{
    // for each block in the snake, we update the position and color in our program and draw
    for (auto next : snakeBlockList)
    {
        program.setVec3("uMove", next->getPosition());
        program.setVec4("uColor", next->getColor());
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void addTail()
{
    int elementCount = snakeBlockList.size();
    if (elementCount == 0) // there is no block in the snake, then add the first one automatically
    {
        snakeBlockList.push_back(new Square(0.0f, 0.0f, length));
    }
    else
    {
        // get the position of the last block in the snake
        Square* lastSquare = snakeBlockList[elementCount - 1];
        glm::vec3 pos = lastSquare->getPosition();
        // according to directions, go one block behind
        Square::DIRECTION dir = lastSquare->getDirection();
        switch (dir)
        {
        case Square::DIR_RIGHT:
            pos -= glm::vec3(length, 0.0f, 0.0f);
            break;
        case Square::DIR_LEFT:
            pos += glm::vec3(length, 0.0f, 0.0f);
            break;
        case Square::DIR_UP:
            pos -= glm::vec3(0.0f, length, 0.0f);
            break;
        case Square::DIR_DOWN:
            pos += glm::vec3(0.0f, length, 0.0f);
            break;
        }
        // create a new block on the pos
        Square* newBlock = new Square(pos.x, pos.y, length);
        snakeBlockList.push_back(newBlock);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // catch ESC, then terminate
    if (key == GLFW_KEY_ESCAPE)
    {
        glfwTerminate();
    }
    if (action == GLFW_PRESS) // if the key is pressed
    {
        if (snakeBlockList.size() != 0)
        {
            Square* first = snakeBlockList[0];
            // catch <-
            if (key == GLFW_KEY_LEFT)
            {
                first->setDirection(Square::DIR_LEFT);
            }
            // catch ->
            if (key == GLFW_KEY_RIGHT)
            {
                first->setDirection(Square::DIR_RIGHT);
            }
            // catch ^
            if (key == GLFW_KEY_UP)
            {
                first->setDirection(Square::DIR_UP);
            }
            // catch V
            if (key == GLFW_KEY_DOWN)
            {
                first->setDirection(Square::DIR_DOWN);
            }
        }
        if (key == GLFW_KEY_SPACE) // add a new block
        {
            addTail();
        }
    }
}

int main(int argc, char** argv)
{
    // initialize GLFW
    if (!glfwInit())
    {
        return -1;
    }

    // create a window pointer
    GLFWwindow* window = glfwCreateWindow(800, 800, "Snake", NULL, NULL);
    // error check
    if (window == NULL)
    {
        cout << "Error. I could not create a window at all!" << endl;
        glfwTerminate();
        return -1;
    }

    // introduce keyboard callback to GLFW
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    // start with 5 blocks in the beginning
    addTail();
    addTail();
    addTail();
    addTail();
    addTail();

    ShaderProg myProgram;
    // attachment of shaders to program object
    myProgram.attach("./shaders/vs.glsl", GL_VERTEX_SHADER);
    myProgram.attach("./shaders/fs.glsl", GL_FRAGMENT_SHADER);
    myProgram.link();

    // bring the id of uniform variables
    myProgram.addUniform("uMove");
    myProgram.addUniform("uColor");

    // buffer
    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexBuffer);

    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    // float movingConstant = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        myProgram.use();

        glBindVertexArray(vertexArray);

        drawSnake(myProgram);

        moveSnake();
        // sleep for a while before the next iteration
        sleep(1);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}