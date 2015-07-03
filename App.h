#pragma once

#ifndef APP_H
#define APP_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "gjk.h"

namespace GJK
{

	//Clase principal de la aplicacion, contiene toda la estructura
	//Este objeto sera el responsable de:
	//	-Crear la escena
	//	-Dibujar la pantalla
	//	-Actualizar la aplicacion
	class App
	{
	private:
		
		//Ventana principal
		sf::RenderWindow m_Window, m_Window_Mink;

		sf::View m_Camara, m_Camara_Mink;

		sf::Event m_EventoWin, m_EventoMink;

		double m_Distance;

		//Elementos de la escena:
		//-----------------------

		Polygon m_A, m_B, m_Interseccion, m_PolyPasoAPaso;
		Point m_DirPasoAPaso;
		bool drag_A, drag_B, rotate_A, rotate_B, hay_interseccion, paso_a_paso, paso_a_paso_next;
		sf::Vector2f m_rotatePresPoint;//Punto de rotacion
		GJK::EstadoPasoAPaso m_EstadoPasoAPaso;
		sf::String m_NombreEstadoPasoAPaso, m_ResultadoGJK, m_DistanciaPoli;

	public:

		App();

		~App();

		//Inicializa la escena, se llama luego de creada la instacia de Game
		void Iniciar();

		//Crea la escena
		void CrearEscena();

		//Actualiza la escena
		void Actualizar();

		//Dibuja la escena
		void Dibujar();

	};

}

#endif // end APP_H
