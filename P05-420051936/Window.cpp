#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;

	//Movimiento coche
	traslacion_coche_value = 0.0f;
	
	giro_cofre_value = 0.0f;

	giro_llantas_value = 0.0f;

	//Flag
	giro_cofre_flag = true;


	// ---------------- Movimiento Goddard
	//mandibula = 0.0f;
	//pie_atras_izq = 0.0f; 
	//pie_atras_der = 0.0f;
	//pie_delante_izq = 0.0f;
	//pie_delante_der = 0.0f;

	////Flags
	//mandibula_flag = true;
	//pie_atras_izq_flag = true;
	//pie_atras_der_flag = true;
	//pie_delante_izq_flag = true; 
	//pie_delante_der_flag = true;

	// -------------
	
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica05: OptimizacionYCargaModelos", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// Movimiento coche
	// Traslación
	if (key == GLFW_KEY_I)
	{
		theWindow->traslacion_coche_value -= 0.5;
	}
	if (key == GLFW_KEY_K)
	{
		theWindow->traslacion_coche_value += 0.5;
	}
	//Cofre
	if (key == GLFW_KEY_C)
	{
		//theWindow->giro_cofre_value += 0.5f;
		//printf("Cofre: %f\n", theWindow->giro_cofre_value);
		if (theWindow->giro_cofre_flag)
		{
			theWindow->giro_cofre_value += 1.0f;
			if (theWindow->giro_cofre_value >= 80.0f)
			{
				theWindow->giro_cofre_value = 80.0f;
				theWindow->giro_cofre_flag = false;
			}
		}
		else
		{
			theWindow->giro_cofre_value -= 1.0f;
			if (theWindow->giro_cofre_value <= 0.0f)
			{
				theWindow->giro_cofre_value = 0.0f;
				theWindow->giro_cofre_flag = true;
			}
		}
	}

	// Llantas
	if (key == GLFW_KEY_O)
	{
		theWindow->giro_llantas_value += 3.0;
	}
	if (key == GLFW_KEY_L)
	{
		theWindow->giro_llantas_value -= 3.0;
	}

	// ---------------- Movimiento Goddard
	//Mandibula
	//if (key == GLFW_KEY_M)
	//{
	//	if (theWindow->mandibula_flag)
	//	{
	//		theWindow->mandibula += 1.0f;
	//		if (theWindow->mandibula >= 30.0f)
	//		{
	//			theWindow->mandibula = 30.0f;
	//			theWindow->mandibula_flag = false;
	//		}
	//	}
	//	else
	//	{
	//		theWindow->mandibula -= 1.0f;
	//		if (theWindow->mandibula <= -3.0f)
	//		{
	//			theWindow->mandibula = -3.0f;
	//			theWindow->mandibula_flag = true;
	//		}
	//	}
	//}

	////Pie atras izq
	//if (key == GLFW_KEY_J)
	//{
	//	if (theWindow->pie_atras_izq_flag)
	//	{
	//		theWindow->pie_atras_izq += 1.0f;
	//		if (theWindow->pie_atras_izq >= 25.0f)
	//		{
	//			theWindow->pie_atras_izq = 25.0f;
	//			theWindow->pie_atras_izq_flag = false;
	//		}
	//	}
	//	else
	//	{
	//		theWindow->pie_atras_izq -= 1.0f;
	//		if (theWindow->pie_atras_izq <= -20.0f)
	//		{
	//			theWindow->pie_atras_izq = -20.0f;
	//			theWindow->pie_atras_izq_flag = true;
	//		}
	//	}
	//}

	////Pie atras der
	//if (key == GLFW_KEY_K)
	//{
	//	if (theWindow->pie_atras_der_flag)
	//	{
	//		theWindow->pie_atras_der += 1.0f;
	//		if (theWindow->pie_atras_der >= 25.0f)
	//		{
	//			theWindow->pie_atras_der = 25.0f;
	//			theWindow->pie_atras_der_flag = false;
	//		}
	//	}
	//	else
	//	{
	//		theWindow->pie_atras_der -= 1.0f;
	//		if (theWindow->pie_atras_der <= -20.0f)
	//		{
	//			theWindow->pie_atras_der = -20.0f;
	//			theWindow->pie_atras_der_flag = true;
	//		}
	//	}
	//}

	////Pie delante izq
	//if (key == GLFW_KEY_U)
	//{
	//	if (theWindow->pie_delante_izq_flag)
	//	{
	//		theWindow->pie_delante_izq += 1.0f;
	//		if (theWindow->pie_delante_izq >= 25.0f)
	//		{
	//			theWindow->pie_delante_izq = 25.0f;
	//			theWindow->pie_delante_izq_flag = false;
	//		}
	//	}
	//	else
	//	{
	//		theWindow->pie_delante_izq -= 1.0f;
	//		if (theWindow->pie_delante_izq <= -20.0f)
	//		{
	//			theWindow->pie_delante_izq = -20.0f;
	//			theWindow->pie_delante_izq_flag = true;
	//		}
	//	}
	//}
	//
	////Pie delante der
	//if (key == GLFW_KEY_I)
	//{
	//	if (theWindow->pie_delante_der_flag)
	//	{
	//		theWindow->pie_delante_der += 1.0f;
	//		if (theWindow->pie_delante_der >= 25.0f)
	//		{
	//			theWindow->pie_delante_der = 25.0f;
	//			theWindow->pie_delante_der_flag = false;
	//		}
	//	}
	//	else
	//	{
	//		theWindow->pie_delante_der -= 1.0f;
	//		if (theWindow->pie_delante_der <= -20.0f)
	//		{
	//			theWindow->pie_delante_der = -20.0f;
	//			theWindow->pie_delante_der_flag = true;
	//		}
	//	}
	//}


	//// ----------------



	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}

	
} //Acaba funcion

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
