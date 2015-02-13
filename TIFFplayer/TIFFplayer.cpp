#include <GLFW/glfw3.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <iomanip>


using namespace cv;
using namespace std;

float FR = 120;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwTerminate();

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		FR++;

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		FR--;
}

void display(GLFWwindow &window)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();


	/* Draw a rectangle onto which the TIFF image will get textured.  Notice
	that the texture coordinates are set to each corner of the texture
	image. */
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex2i(-1, -1);
	glTexCoord2i(1, 0);
	glVertex2i(1, -1);
	glTexCoord2i(1, 1);
	glVertex2i(1, 1);
	glTexCoord2i(0, 1);
	glVertex2i(-1, 1);
	glEnd();

	glPopMatrix();

	/* Swap front and back buffers */
	glfwSwapBuffers(&window);
}



int main(void)
{
	GLFWwindow* window;
	glClearColor(0.f, 0.f, 0.f, 1.f);

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1920, 1080, "Hello World", glfwGetPrimaryMonitor(), NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/*Load Image*/

	GLuint texture;
	string frDir;
	string frameDir;

	frDir = std::to_string(FR);

	int frameCount = 1;
	int maxFrame = 360;
	string fileDir;
	stringstream ss;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		
		ss << setfill('0') << setw(4) << std::to_string(frameCount);
		fileDir = "D:\\Data\\120fps\\" + ss.str() + ".tif";
		ss.str(std::string());

		cv::Mat image = cv::imread(fileDir);

		if (image.empty()){
			std::cout << "Image Empty" << std::endl;
		}
		else{
			cv::flip(image, image, 0);
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, image.data);
		}

		

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_ACCUM_BUFFER_BIT);

		double start = glfwGetTime();

		glEnable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
			glTexCoord2f(1, 1);
			glVertex2f(1, 1);
			glTexCoord2f(0, 1);
			glVertex2f(-1, 1);
			glTexCoord2f(0, 0);
			glVertex2f(-1, -1);
			glTexCoord2f(1, 0);
			glVertex2f(1, -1);
		glEnd();

		while ((glfwGetTime() - start) < 1 / FR){}

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		glDeleteTextures(1, &texture);

		glfwSetKeyCallback(window, key_callback);

		/* Poll for and process events */
		glfwPollEvents();

		if (frameCount == maxFrame){
			frameCount = 1;
		}
		else{
			frameCount++;
		}
		

	}

	return 0;
}