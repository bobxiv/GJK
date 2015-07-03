#include "gjk.h"
#include "App.h"
#include <iostream>

namespace GJK
{

	//-------------------------------------------------------------//
	//-------------	Constructor/Destructor	-----------------------//
	//-------------------------------------------------------------//

	App::App(): m_Window(sf::VideoMode(800,600,32), "Prueba GJK: Espacio del mundo"),
		        m_Window_Mink(sf::VideoMode(800, 600, 32), "Prueba GJK: Espacio de Minkowski"),
				m_Camara(sf::Vector2f(0,0), sf::Vector2f(400,300)),
				m_Camara_Mink(sf::Vector2f(0,0), sf::Vector2f(400,300)),
				m_EstadoPasoAPaso(Paso)
	{
		m_Window.Show(true);
		m_Window.SetFramerateLimit(60);

		m_Window_Mink.Show(true);
		m_Window_Mink.SetFramerateLimit(60);

		drag_A = false;
		drag_B = false;
		hay_interseccion = false;
		paso_a_paso = false;
		paso_a_paso_next = false;
		m_NombreEstadoPasoAPaso.SetFont( sf::Font::GetDefaultFont() );
		m_NombreEstadoPasoAPaso.SetPosition(-400,-300);

		m_DistanciaPoli.SetFont( sf::Font::GetDefaultFont() );
		m_DistanciaPoli.SetPosition(-400,-270);

		m_ResultadoGJK.SetFont( sf::Font::GetDefaultFont() );
		m_ResultadoGJK.SetPosition(-400,-300);

		CrearEscena();
	}

	App::~App()
	{
	}

	//-------------------------------------------------------------//
	//-----------------		Metodos		---------------------------//
	//-------------------------------------------------------------//

	void App::CrearEscena()
	{
		float height = m_Window.GetHeight();
		float width  = m_Window.GetWidth();

		m_A.AddVertex( Point(100, 110) );
		m_A.AddVertex( Point(200, 120) );
		m_A.AddVertex( Point(150, 160) );

		m_B.AddVertex( Point(300, 110) );
		m_B.AddVertex( Point(400, 120) );
		m_B.AddVertex( Point(350, 160) );
		m_B.AddVertex( Point(300, 160) );

		std::cout<<"Calculo de Colisiones mediante algoritmo GJK"<<std::endl;
		std::cout<<"--------------------------------------------"<<std::endl;
		std::cout<<std::endl;
		std::cout<<"Resumen:"<<std::endl;
		std::cout<<"En la ventana \"Espacio del mundo\" se puede interactuar con los poligonos con los";
		std::cout<<"que se probara la interseccion. En la ventana \"Espacio de Minkowski\" se puede   ";
		std::cout<<"ver la resta de Minkowski de los poligonos. Ademas en el \"Espacio de Minkowski\" ";
		std::cout<<"se puede ver el simplex encontrado (en magenta), y activar el modo paso a paso  ";
		std::cout<<"que permite ver como el algoritmo GJK encuentra la respuesta (ver controles).";
		std::cout<<std::endl;
		std::cout<<"Controles:"<<std::endl;
		std::cout<<"Click izquierdo ... arrastrar los poligonos"<<std::endl;
		std::cout<<"Click derecho ..... rotar los poligonos"<<std::endl;
		std::cout<<"Return ............ < iniciar modo paso a paso | pasar al siguiente estado >"<<std::endl;
		std::cout<<"Esc ............... Cerrar aplicacion"<<std::endl;
		std::cout<<"t ................. Activar/Desactivar coherencia temporal"<<std::endl;
	}

	void App::Iniciar()
	{
		while( m_Window.IsOpened() )
		{
			//Atrapamos los eventos para cerra la ventana y los clicks del mouse para
			//hacer peek sobre las pelotas
			while( m_Window.GetEvent(m_EventoWin) )
			{
				switch( m_EventoWin.Type )
				{
				case sf::Event::Closed:
                    m_Window.Close();
					break;

				case sf::Event::KeyPressed:
					{
						if( m_EventoWin.Key.Code == sf::Key::Escape )
							m_Window.Close();

						if( m_EventoWin.Key.Code == sf::Key::Left )
						{
							for(int k=0; k < m_A.GetVertexCount() ; ++k)
							{
								m_A(k).x -= 1;
							}
						}
						if( m_EventoWin.Key.Code == sf::Key::Right )
						{
							for(int k=0; k < m_A.GetVertexCount() ; ++k)
							{
								m_A(k).x += 1;
							}
						}

						if( m_EventoWin.Key.Code == sf::Key::Up )
						{
							for(int k=0; k < m_A.GetVertexCount() ; ++k)
							{
								m_A(k).y -= 1;
							}
						}
						if( m_EventoWin.Key.Code == sf::Key::Down )
						{
							for(int k=0; k < m_A.GetVertexCount() ; ++k)
							{
								m_A(k).y += 1;
							}
						}
						if( m_EventoWin.Key.Code == sf::Key::Return )
						{
							switch(m_EstadoPasoAPaso)
							{
								case GJK::Paso:
									{
										if( !paso_a_paso )
											paso_a_paso = true;
										paso_a_paso_next = true;
									}
									break;
								case GJK::NoInterseccion:
								case GJK::Interseccion:
									{
										paso_a_paso = false;
										paso_a_paso_next = false;
										m_EstadoPasoAPaso = GJK::Paso;
										m_PolyPasoAPaso = Polygon();
									}
									break;
							}
						}
					}
					break;
                case sf::Event::MouseButtonPressed:
					{
						// TODO: problema si se clickea en un punto que cubren los dos
						int x = m_EventoWin.MouseButton.X;
						int y = m_EventoWin.MouseButton.Y;

						sf::Vector2f temp = m_Window.ConvertCoords(x, y, &m_Camara);
						x = temp.x;
						y = temp.y;
						switch( m_EventoWin.MouseButton.Button )
						{
							case sf::Mouse::Left:
							{
								if (m_B.Contains(Point(x, y)))
									drag_B = true;
								else if (m_A.Contains(Point(x, y)))
									drag_A = true;
							}
							break;
							case sf::Mouse::Right:
							{
								if (m_B.Contains(Point(x, y)))
								{
									rotate_B = true;
									m_rotatePresPoint = sf::Vector2f(x,y);
								}else if (m_A.Contains(Point(x, y)))
								{
									rotate_A = true;
									m_rotatePresPoint = sf::Vector2f(x,y);
								}
							}
							break;
						}
					}
                    break;

				case sf::Event::MouseButtonReleased:
					{
						switch( m_EventoWin.MouseButton.Button )
						{
							case sf::Mouse::Left:
							{
								drag_A = false;
								drag_B = false;
							}
							break;
							case sf::Mouse::Right:
							{
								rotate_A = false;
								rotate_B = false;
							}
							break;
						}
					}
                    break;

				case sf::Event::MouseMoved:
					static int prev_x = m_EventoWin.MouseMove.X,
						       prev_y = m_EventoWin.MouseMove.Y;

					int x = m_EventoWin.MouseMove.X;
					int y = m_EventoWin.MouseMove.Y;
					sf::Vector2f temp = m_Window.ConvertCoords(x, y, &m_Camara);
					x = temp.x;
					y = temp.y;

					if(drag_A)
					{
						int dx, dy;
						dx = x - prev_x;
						dy = y - prev_y;
						m_A.Move(dx, dy);
					}

					if(drag_B)
					{
						int dx, dy;
						dx = x - prev_x;
						dy = y - prev_y;
						m_B.Move(dx, dy);
					}

					if(rotate_A)
					{
						float angRad = atan2((y-m_rotatePresPoint.y),(x-m_rotatePresPoint.x));//angulo respecto al punto de rotacion
																							  //rotacion counter clock wise
						m_A.Rotate(angRad, GJK::Point(m_rotatePresPoint.x, m_rotatePresPoint.y));// NO ESTA IMPLEMENTADA TODAVIA ESTA FUNCION
					}

					if(rotate_B)
					{
						float angRad = atan2((y-m_rotatePresPoint.y),(x-m_rotatePresPoint.x));//angulo respecto al punto de rotacion
																							  //rotacion counter clock wise
						m_B.Rotate(angRad, GJK::Point(m_rotatePresPoint.x, m_rotatePresPoint.y));// NO ESTA IMPLEMENTADA TODAVIA ESTA FUNCION
					}

					prev_x = m_EventoWin.MouseMove.X;
					prev_y = m_EventoWin.MouseMove.Y;
					temp = m_Window.ConvertCoords(prev_x, prev_y, &m_Camara);
					prev_x = temp.x;
					prev_y = temp.y;

					break;

				}
				// Tomar el enter y correr una vuelta del gjk en modo debug
				// (Con otra tecla que haga lo mismo pero sin coherencia)
				// Que una funcion de App haga todo eso. Que sean funciones de
				// debug para no ensuciar el resto que esta perfecto PERFECTO
				// (en gjk)
			}

			Actualizar();    //Actualizamos la escena

			Dibujar();       //Dibujamos todo el juego
		}
	}

	void App::Actualizar()
	{
		if( paso_a_paso )
		{
			if( m_EstadoPasoAPaso == GJK::Paso && paso_a_paso_next )
			{
				gjk_2dIntersection_TestPasoAPaso(m_A, m_B, m_PolyPasoAPaso, m_DirPasoAPaso, m_EstadoPasoAPaso);
				switch(m_EstadoPasoAPaso)
				{
					case GJK::Paso:
						{

						}
						break;
					case GJK::NoInterseccion:
						{
							m_ResultadoGJK.SetText("No Intersectan");
						}
						break;
					case GJK::Interseccion:
						{
							m_ResultadoGJK.SetText("Intersectan");
						}
						break;
				}
				paso_a_paso_next = false;
			}

		}else
		{
			if( gjk_2dIntersection_Test(m_A, m_B, m_Interseccion) ) 
			{
				hay_interseccion = true;
				m_ResultadoGJK.SetText("Intersectan");

				m_Distance = 0.0;
			}
			else
			{
				hay_interseccion = false;
				m_ResultadoGJK.SetText("No Intersectan");

				m_Distance = gjk_2dDistance(m_A, m_B);
				//std::cout << m_Distance << std::endl;
			}
			
		}
	}

	void App::Dibujar()
	{
		// Ventana principal
		m_Window.Clear();

		m_Window.SetView(m_Camara);

		sf::Shape eje_x = sf::Shape::Line(sf::Vector2f(-400, 0), sf::Vector2f(400, 0), 1.0, sf::Color::White);
		sf::Shape eje_y = sf::Shape::Line(sf::Vector2f(0, -300), sf::Vector2f(0, 300), 1.0, sf::Color::White);

		m_Window.Draw(eje_x);
		m_Window.Draw(eje_y);

		sf::Shape polyA;
		for(int k=0; k < m_A.GetVertexCount() ; ++k)
			polyA.AddPoint(sf::Vector2f(m_A(k).x, m_A(k).y), sf::Color::Blue);
		m_Window.Draw(polyA);
		
		sf::Shape polyB;
		for(int k=0; k < m_B.GetVertexCount() ; ++k)
		{
			polyB.AddPoint(sf::Vector2f(m_B(k).x, m_B(k).y), sf::Color::Red);
		}
		m_Window.Draw(polyB);

		m_Window.Draw(m_ResultadoGJK);

		static char texto[30];
		sprintf(texto, "%.3f", m_Distance);
		m_DistanciaPoli.SetText(texto);
		m_Window.Draw(m_DistanciaPoli);

		m_Window.Display();

		// Ventana de Minkowski
		m_Window_Mink.Clear();

		m_Window_Mink.SetView(m_Camara_Mink);

		sf::Shape eje_x_mink = sf::Shape::Line(sf::Vector2f(-400, 0), sf::Vector2f(400, 0), 1.0, sf::Color::White);
		sf::Shape eje_y_mink = sf::Shape::Line(sf::Vector2f(0, -300), sf::Vector2f(0, 300), 1.0, sf::Color::White);
		m_Window_Mink.Draw(eje_x_mink);
		m_Window_Mink.Draw(eje_y_mink);

		Polygon a_minus_b;
		a_minus_b = minkowski_difference(m_A, m_B);
		sf::Shape polyMink;
		for(int k=0; k < a_minus_b.GetVertexCount() ; ++k)
		{
			polyMink.AddPoint(sf::Vector2f(a_minus_b(k).x, a_minus_b(k).y), sf::Color::Red);
		}
		m_Window_Mink.Draw(polyMink);

#ifdef _DEBUG
		
		if (hay_interseccion && !paso_a_paso)// si esta intersectando y no esta en modo paso a paso
		{
			sf::Shape polyInt;
			polyInt.EnableOutline(true);
			polyInt.SetOutlineWidth(2);
			if (m_Interseccion.GetVertexCount() == 2)
			{
				m_Window_Mink.Draw(sf::Shape::Line(sf::Vector2f(m_Interseccion(0).x,m_Interseccion(0).y), sf::Vector2f(m_Interseccion(1).x,m_Interseccion(1).y), 2, sf::Color::Yellow));
			}else
			{
				for(int k=0; k < m_Interseccion.GetVertexCount() ; ++k)
				{
					polyInt.AddPoint(sf::Vector2f(m_Interseccion(k).x, m_Interseccion(k).y), sf::Color::Magenta, sf::Color::Yellow);
				}
				m_Window_Mink.Draw(polyInt);
			}
		}
		
#endif
		if( paso_a_paso )// si esta en modo paso a paso
		{
			sf::Shape polyPaso;
			polyPaso.EnableOutline(true);
			polyPaso.SetOutlineWidth(2);
			if (m_PolyPasoAPaso.GetVertexCount() == 2)
			{
				m_Window_Mink.Draw(sf::Shape::Line(sf::Vector2f(m_PolyPasoAPaso(0).x,m_PolyPasoAPaso(0).y), sf::Vector2f(m_PolyPasoAPaso(1).x,m_PolyPasoAPaso(1).y), 2, sf::Color::Yellow));
			}else
			{
				for(int k=0; k < m_PolyPasoAPaso.GetVertexCount() ; ++k)
				{
					polyPaso.AddPoint(sf::Vector2f(m_PolyPasoAPaso(k).x, m_PolyPasoAPaso(k).y), sf::Color::Magenta, sf::Color::Yellow);
				}
				m_Window_Mink.Draw(polyPaso);
			}

			switch(m_EstadoPasoAPaso)
			{
				case GJK::Paso:
					{
						m_NombreEstadoPasoAPaso.SetText("Modo Paso a Paso: Paso");
						GJK::Point dir = m_DirPasoAPaso;
						Normalize(dir);
						m_Window_Mink.Draw(sf::Shape::Line(sf::Vector2f(0,0), sf::Vector2f(dir.x*100,dir.y*100), 2, sf::Color::Blue));
					}
					break;
				case GJK::NoInterseccion:
					{
						m_NombreEstadoPasoAPaso.SetText("Modo Paso a Paso: NoInterseccion");
						m_Window_Mink.Draw(sf::Shape::Line(sf::Vector2f(-10,10), sf::Vector2f(10,-10), 2, sf::Color::Blue));
						m_Window_Mink.Draw(sf::Shape::Line(sf::Vector2f(10,10), sf::Vector2f(-10,-10), 2, sf::Color::Blue));
					}
					break;
				case GJK::Interseccion:
					{
						m_NombreEstadoPasoAPaso.SetText("Modo Paso a Paso: Interseccion");
						m_Window_Mink.Draw(sf::Shape::Circle(sf::Vector2f(0,0), 5, sf::Color::Green));
					}
					break;
			}
			m_Window_Mink.Draw(m_NombreEstadoPasoAPaso);
		}

		m_Window_Mink.Display();

	}

}
