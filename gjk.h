#pragma once

#ifndef GJK_H
#define GJK_H

#include <vector>
#include "Config.h"

using namespace std;

#define EPS 0.0001

// El valor de epsilon a usar en el algoritmo
#define GJK_epsilon  0.01
#define GJK_epsilon_square GJK_epsilon*GJK_epsilon


namespace GJK
{

    class Point
    {
        public:

            static Point Zero;

            Point () {}
            Point (const Real _x, const Real _y): x(_x), y(_y) {}

            Point& operator+=(const Point &b);

            Point operator*(const Real &s) const;

            Point operator-(const Point &b) const;

            Point operator-() const;

            Point operator+(const Point &b) const;

            bool operator==(const Point &b) const;

            void Move(int dx, int dy);

            Real x, y;
        private:
            friend Real fastDot(const Point &a, const Point &b);
            friend Real Dot(const Point &a, const Point &b);
            friend Real Cross2D(const Point &a, const Point &b);
            friend Point TripleCross(const Point &a, const Point &b, const Point &c);
            friend Real fastNorm(const Point &a);
    };

    Real fastDot(const Point &a, const Point &b);

    Real Dot(const Point &a, const Point &b);

    Real Cross2D(const Point &a, const Point &b);

    Point TripleCross(const Point &a, const Point &b, const Point &c);

    Real fastNorm(const Point &a);

    Real Norm(const Point &a);

    void Normalize(Point &a);

    // XXX Por ahora, un poligono va a ser simplemente
    // un vector de Point's. Creo (espero) que con eso
    // baste. De mas esta decir que no es lo mas eficiente,
    // pero nunca nada es lo mas eficiente, nadie nada nunca.
    class Polygon
    {
        public:
            Polygon() {}
            Polygon(const vector<Point> &_vertexs) : vertexs(_vertexs) {}

            // Devuelve referencia a vertice mas alejado en direccion d
            // Jamas debe ser llamada si es que el poligono no tiene vertices!
            // (por motivos de performance no se tiene en cuenta este caso)
            const Point& support(Point d) const;

            Point GetCenter() const;

            void AddVertex(const Point &b);

            vector<Point>::size_type GetVertexCount() const;

            const Point& GetLast() const;

            Point& operator()(int i);

            bool Contains(const Point &b);

            void Remove(int i);

            // Mueve los vertices del poligono un delta
            void Move(int dx, int dy);

            // Rota los vertices del poligono un angulo angRad (en radianes)
            // respecto al punto PointOfRotation
            void Rotate(float angRad, Point& PointOfRotation);

            Point GetClosestPointToOrigin();

        private:
            vector<Point> vertexs;
    };

    // Funcion de soporte de la diferencia de Minkowski
    // entre a y b en direccion d.
    Point minkowski_difference_support_function(const Polygon &a, const Polygon &b, const Point &d);

    bool doLine(Polygon &S, Point &d);

    bool doTriangle(Polygon &S, Point &d);

    // Devuelve si el simplex contiene el origen, y si no lo contiene devuelve
    // la nueva direccion d y el simplex modificado
    bool updateSimplex(Polygon &S, Point &d);

    // Recibe dos poligonos [convexos]
    // Si se intersectan, devuelve -1;
    // si no, devuelve la distancia minima
    // entre ellos.
    // XXX Voy a tratar de hacerlo para 2d y
    // rezar por que pasarlo a 3d no sea 
    // mucho quilombo.
    #ifdef _DEBUG
    bool gjk_2dIntersection_Test(const Polygon &a, const Polygon &b, Polygon &interseccion);
    #else
    bool gjk_2dIntersection_Test(const Polygon &a, const Polygon &b);
    #endif

    enum EstadoPasoAPaso{Paso, Interseccion, NoInterseccion};
    bool gjk_2dIntersection_TestPasoAPaso(const Polygon &a, const Polygon &b, Polygon &S, Point &d, EstadoPasoAPaso& Estado);

    Polygon minkowski_difference(Polygon &a, Polygon &b);

    double gjk_2dDistance(const Polygon &a, const Polygon &b);
}

#endif // end GJK_H
