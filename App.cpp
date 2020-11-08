#include "gjk.h"
#include "App.h"
#include <iostream>
#include <math.h>

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
        m_Window.setFramerateLimit(60);

        m_Window_Mink.setFramerateLimit(60);

        if (!m_fuenteGeneral.loadFromFile("../data/arial.ttf"))
        {
            std::cerr << "Error al cargar la fuente!" << std::endl;
            abort();
        }

        const float height = m_Camara.getSize().y;
        const float width = m_Camara.getSize().x;
        const float halfWidth = width / 2.0f;
        const float halfHeight = height / 2.0f;

        drag_A = false;
        drag_B = false;
        hay_interseccion = false;
        paso_a_paso = false;
        paso_a_paso_next = false;
        m_texNombreEstadoPasoAPaso.setString( m_NombreEstadoPasoAPaso );
        m_texNombreEstadoPasoAPaso.setCharacterSize( 12 );
        m_texNombreEstadoPasoAPaso.setFillColor( sf::Color::White );
        m_texNombreEstadoPasoAPaso.setStyle( sf::Text::Regular );
        m_texNombreEstadoPasoAPaso.setFont( m_fuenteGeneral );
        m_texNombreEstadoPasoAPaso.setPosition( -halfWidth, -halfHeight );

        m_texDistanciaPoli.setString( m_DistanciaPoli );
        m_texDistanciaPoli.setCharacterSize( 12 );
        m_texDistanciaPoli.setFillColor( sf::Color::White );
        m_texDistanciaPoli.setStyle( sf::Text::Regular );
        m_texDistanciaPoli.setFont( m_fuenteGeneral );
        m_texDistanciaPoli.setPosition( -halfWidth, -halfHeight + 15.0f );

        m_texResultadoGJK.setString( m_ResultadoGJK );
        m_texResultadoGJK.setCharacterSize( 12 );
        m_texResultadoGJK.setFillColor( sf::Color::White );
        m_texResultadoGJK.setStyle( sf::Text::Regular );
        m_texResultadoGJK.setPosition( -halfWidth , -halfHeight );
        m_texResultadoGJK.setFont( m_fuenteGeneral );

        CrearEscena();
    }

    //-------------------------------------------------------------//
    //-----------------		Metodos		---------------------------//
    //-------------------------------------------------------------//

    void App::CrearEscena()
    {
        const float height = m_Camara.getSize().y;
        const float width  = m_Camara.getSize().x;
        const GJK::Real halfWidth = width / 2.0;
        const GJK::Real halfHeight = height / 2.0;

        m_A.AddVertex( Point(100 - halfWidth, 110 - halfHeight) );
        m_A.AddVertex( Point(200 - halfWidth, 120 - halfHeight) );
        m_A.AddVertex( Point(150 - halfWidth, 160 - halfHeight) );

        m_B.AddVertex( Point(300 - halfWidth, 110 - halfHeight) );
        m_B.AddVertex( Point(400 - halfWidth, 120 - halfHeight) );
        m_B.AddVertex( Point(350 - halfWidth, 160 - halfHeight) );
        m_B.AddVertex( Point(300 - halfWidth, 160 - halfHeight) );

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
        while( m_Window.isOpen() )
        {
            //Atrapamos los eventos para cerra la ventana y los clicks del mouse para
            //hacer peek sobre las pelotas
            while( m_Window.pollEvent(m_EventoWin) )
            {
                switch( m_EventoWin.type )
                {
                case sf::Event::Closed:
                    m_Window.close();
                    break;

                case sf::Event::KeyPressed:
                    {
                        if( m_EventoWin.key.code == sf::Keyboard::Escape )
                            m_Window.close();

                        if( m_EventoWin.key.code == sf::Keyboard::Left )
                        {
                            for(int k=0; k < m_A.GetVertexCount() ; ++k)
                            {
                                m_A(k).x -= 1;
                            }
                        }
                        if( m_EventoWin.key.code == sf::Keyboard::Right )
                        {
                            for(int k=0; k < m_A.GetVertexCount() ; ++k)
                            {
                                m_A(k).x += 1;
                            }
                        }

                        if( m_EventoWin.key.code == sf::Keyboard::Up )
                        {
                            for(int k=0; k < m_A.GetVertexCount() ; ++k)
                            {
                                m_A(k).y -= 1;
                            }
                        }
                        if( m_EventoWin.key.code == sf::Keyboard::Down )
                        {
                            for(int k=0; k < m_A.GetVertexCount() ; ++k)
                            {
                                m_A(k).y += 1;
                            }
                        }
                        if( m_EventoWin.key.code == sf::Keyboard::Return )
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
                        int x = m_EventoWin.mouseButton.x;
                        int y = m_EventoWin.mouseButton.y;

                        sf::Vector2i pixels(x, y);
                        sf::Vector2f temp = m_Window.mapPixelToCoords(pixels, m_Camara);
                        x = temp.x;
                        y = temp.y;
                        switch( m_EventoWin.mouseButton.button )
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
                        switch( m_EventoWin.mouseButton.button )
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
                    static int prev_x = m_EventoWin.mouseMove.x,
                               prev_y = m_EventoWin.mouseMove.y;

                    int x = m_EventoWin.mouseMove.x;
                    int y = m_EventoWin.mouseMove.y;
                    sf::Vector2i pixels(x, y);
                    sf::Vector2f temp = m_Window.mapPixelToCoords(pixels, m_Camara);
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

                    prev_x = m_EventoWin.mouseMove.x;
                    prev_y = m_EventoWin.mouseMove.y;
                    sf::Vector2i prevPixels(prev_x, prev_y);
                    temp = m_Window.mapPixelToCoords(prevPixels, m_Camara);
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
                            m_ResultadoGJK = "No Intersectan";
                        }
                        break;
                    case GJK::Interseccion:
                        {
                            m_ResultadoGJK = "Intersectan";
                        }
                        break;
                }
                m_texResultadoGJK.setString(m_ResultadoGJK);
                paso_a_paso_next = false;
            }

        }else
        {
#ifdef _DEBUG
            if (gjk_2dIntersection_Test(m_A, m_B, m_Interseccion))
#else
            if( gjk_2dIntersection_Test(m_A, m_B) )
#endif
            {
                hay_interseccion = true;
                m_ResultadoGJK = "Intersectan";

                m_Distance = 0.0;
            }
            else
            {
                hay_interseccion = false;
                m_ResultadoGJK = "No Intersectan";

                m_Distance = gjk_2dDistance(m_A, m_B);
                //std::cout << m_Distance << std::endl;
            }
            m_texResultadoGJK.setString(m_ResultadoGJK);
        }
    }

    void App::Dibujar()
    {
        // Ventana principal
        m_Window.clear( sf::Color::Black );

        m_Window.setView( m_Camara );

        float height = m_Window.getSize().y;
        float width = m_Window.getSize().x;

        auto eje_x = sf::RectangleShape(sf::Vector2f(width, 1.0f));
        eje_x.setPosition(-width / 2.0f, 0.0f);
        eje_x.setFillColor(sf::Color::White);

        auto eje_y = sf::RectangleShape(sf::Vector2f(1.0f, height));
        eje_y.setPosition(0.0f, -height / 2.0f);
        eje_y.setFillColor(sf::Color::White);

        m_Window.draw(eje_x);
        m_Window.draw(eje_y);

        sf::ConvexShape polyA;
        polyA.setPointCount(m_A.GetVertexCount());
        for(int k=0; k < m_A.GetVertexCount() ; ++k)
            polyA.setPoint(k, sf::Vector2f(m_A(k).x, m_A(k).y));
        polyA.setFillColor(sf::Color::Blue);
        m_Window.draw(polyA);

        sf::ConvexShape polyB;
        polyB.setPointCount(m_B.GetVertexCount());
        for(int k=0; k < m_B.GetVertexCount() ; ++k)
        {
            polyB.setPoint(k, sf::Vector2f(m_B(k).x, m_B(k).y));
        }
        polyB.setFillColor(sf::Color::Red);
        m_Window.draw(polyB);

        m_Window.draw(m_texResultadoGJK);

        static char texto[30];
        sprintf(texto, "%.3f", m_Distance);
        m_DistanciaPoli = texto;
        m_texDistanciaPoli.setString(m_DistanciaPoli);
        m_Window.draw(m_texDistanciaPoli);

        m_Window.display();

        // Ventana de Minkowski
        m_Window_Mink.clear( sf::Color::Black );

        m_Window_Mink.setView( m_Camara_Mink );

        float height_Mink = m_Window.getSize().y;
        float width_Mink = m_Window.getSize().x;

        auto eje_x_mink = sf::RectangleShape(sf::Vector2f(width_Mink, 1.0f));
        eje_x_mink.setPosition(-height_Mink / 2.0f, 0.0f);
        eje_x_mink.setFillColor(sf::Color::White);

        auto eje_y_mink = sf::RectangleShape(sf::Vector2f(1.0f, height_Mink));
        eje_y_mink.setPosition(0.0f, -width_Mink / 2.0f);
        eje_y_mink.setFillColor(sf::Color::White);

        m_Window_Mink.draw(eje_x_mink);
        m_Window_Mink.draw(eje_y_mink);

        Polygon a_minus_b;
        a_minus_b = minkowski_difference(m_A, m_B);

        sf::ConvexShape polyMink;
        polyMink.setPointCount(a_minus_b.GetVertexCount());
        for(int k=0; k < a_minus_b.GetVertexCount() ; ++k)
        {
            polyMink.setPoint(k, sf::Vector2f(a_minus_b(k).x, a_minus_b(k).y));
        }
        polyMink.setFillColor(sf::Color::Red);
        m_Window_Mink.draw(polyMink);

#ifdef _DEBUG

        if (hay_interseccion && !paso_a_paso)// si esta intersectando y no esta en modo paso a paso
        {
            if (m_Interseccion.GetVertexCount() == 2)
            {
                sf::Vertex line[] =
                {
                    sf::Vertex(sf::Vector2f(m_Interseccion(0).x,m_Interseccion(0).y), sf::Color::Yellow),
                    sf::Vertex(sf::Vector2f(m_Interseccion(1).x,m_Interseccion(1).y), sf::Color::Yellow)
                };
                m_Window_Mink.draw(line, 2, sf::Lines);
            }else
            {
                sf::ConvexShape polyInt;
                polyInt.setOutlineThickness(2);
                polyInt.setPointCount(m_Interseccion.GetVertexCount());
                for(int k=0; k < m_Interseccion.GetVertexCount() ; ++k)
                {
                    polyInt.setPoint(k, sf::Vector2f(m_Interseccion(k).x, m_Interseccion(k).y));
                }
                polyInt.setFillColor(sf::Color::Magenta);
                polyInt.setOutlineColor(sf::Color::Yellow);
                m_Window_Mink.draw(polyInt);
            }
        }

#endif
        if( paso_a_paso )// si esta en modo paso a paso
        {
            if (m_PolyPasoAPaso.GetVertexCount() == 2)
            {
                sf::Vertex line[] =
                {
                    sf::Vertex(sf::Vector2f(m_PolyPasoAPaso(0).x,m_PolyPasoAPaso(0).y), sf::Color::Yellow),
                    sf::Vertex(sf::Vector2f(m_PolyPasoAPaso(1).x,m_PolyPasoAPaso(1).y), sf::Color::Yellow)
                };
                m_Window_Mink.draw(line, 2, sf::Lines);
            }else
            {
                sf::ConvexShape polyPaso;
                polyPaso.setOutlineThickness(2);
                polyPaso.setPointCount(m_PolyPasoAPaso.GetVertexCount());
                for(int k=0; k < m_PolyPasoAPaso.GetVertexCount() ; ++k)
                {
                    polyPaso.setPoint(k, sf::Vector2f(m_PolyPasoAPaso(k).x, m_PolyPasoAPaso(k).y));
                }
                polyPaso.setFillColor(sf::Color::Magenta);
                polyPaso.setOutlineColor(sf::Color::Yellow);
                m_Window_Mink.draw(polyPaso);
            }

            switch(m_EstadoPasoAPaso)
            {
                case GJK::Paso:
                    {
                        m_NombreEstadoPasoAPaso = "Modo Paso a Paso: Paso";
                        m_texNombreEstadoPasoAPaso.setString( m_NombreEstadoPasoAPaso );
                        GJK::Point dir = m_DirPasoAPaso;
                        Normalize(dir);
                        sf::Vertex line[] =
                        {
                            sf::Vertex(sf::Vector2f(0,0), sf::Color::Blue),
                            sf::Vertex(sf::Vector2f(dir.x * 100,dir.y * 100), sf::Color::Blue)
                        };
                        m_Window_Mink.draw(line, 2, sf::Lines);

                    }
                    break;
                case GJK::NoInterseccion:
                    {
                        m_NombreEstadoPasoAPaso = "Modo Paso a Paso: NoInterseccion";
                        m_texNombreEstadoPasoAPaso.setString( m_NombreEstadoPasoAPaso );
                        sf::Vertex lineA[] =
                        {
                            sf::Vertex(sf::Vector2f(-10,10), sf::Color::Blue),
                            sf::Vertex(sf::Vector2f(10,-10), sf::Color::Blue)
                        };
                        m_Window_Mink.draw(lineA, 2, sf::Lines);

                        sf::Vertex lineB[] =
                        {
                            sf::Vertex(sf::Vector2f(10,10), sf::Color::Blue),
                            sf::Vertex(sf::Vector2f(-10,-10), sf::Color::Blue)
                        };
                        m_Window_Mink.draw(lineB, 2, sf::Lines);
                    }
                    break;
                case GJK::Interseccion:
                    {
                        m_NombreEstadoPasoAPaso = "Modo Paso a Paso: Interseccion";
                        m_texNombreEstadoPasoAPaso.setString( "Modo Paso a Paso: Interseccion" );
                        sf::CircleShape circle( 5 );
                        circle.setFillColor( sf::Color::Green );
                        m_Window_Mink.draw( circle );
                    }
                    break;
            }
            m_Window_Mink.draw(m_texNombreEstadoPasoAPaso);
        }

        m_Window_Mink.display();
    }
}
