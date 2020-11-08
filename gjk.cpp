#include "gjk.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include "Config.h"
#include "ConvexHull2D.h"

using namespace std;

GJK::Point GJK::Point::Zero(0.0f,0.0f);

namespace GJK
{

    Point& Point::operator+=(const Point &b)
    {
        this->x += b.x;
        this->y += b.y;
        return *this;
    }

    Point Point::operator*(const Real &s) const
    {
        return Point(this->x*s,
                     this->y*s);
    }

    Point Point::operator-(const Point &b) const
    {
        return Point(this->x - b.x,
                     this->y - b.y);
    }

    Point Point::operator-() const
    {
        return Point(-this->x, -this->y);
    }

    Point Point::operator+(const Point &b) const
    {
        return Point(this->x + b.x, this->y + b.y);
    }

    bool Point::operator==(const Point &b) const
    {
        return ( this->x == b.x && this->y == b.y );
    }

    void Point::Move(int dx, int dy)
    {
        this->x += dx;
        this->y += dy;
    }

    Real Dot(const Point &a, const Point &b)
    {
        return (a.x*b.x+a.y*b.y);
    }

    Real Cross2D(const Point &a, const Point &b)
    {
        return a.x*b.y-a.y*b.x;
    }

    // Entrada edge ab y direccion ao
    // Devuelve la normal en la direccion que esta ao
    Point TripleCross(const Point &a, const Point &b, const Point &c)
    {
        // Producto vectorial triple: ab x ao x ab
        // que da el vector en la direccion donde esta el ao
        Real z = a.x*b.y-a.y*b.x;
        return Point(-z*c.y, z*c.x);
    }

    Real fastNorm(const Point &a)
    {
        return (a.x*a.x+a.y*a.y);
    }

    Real Norm(const Point &a)
    {
        return sqrt(a.x*a.x+a.y*a.y);
    }

    void Normalize(Point &a)
    {
        Real norm = Norm(a);
        a.x /= norm;
        a.y /= norm;
    }

    // Devuelve referencia a vertice mas alejado en direccion d
    // Jamas debe ser llamada si es que el poligono no tiene vertices!
    // (por motivos de performance no se tiene en cuenta este caso)
    const Point& Polygon::support(Point d) const
    {
        // XXX La idea es recorrer los puntos y devolver el
        // mayor en la direccion d, ni mas ni menos que eso.
        // Lineal; se puede hacer un poco mejor haciendo
        // hill climbing

        // Implementacion naive
        Real aux, maxD = Dot(vertexs[0], d);
        int maxI = 0;// ningun vertice
        for(int k=1; k < vertexs.size() ; ++k)
        {
            aux = Dot(vertexs[k], d);
            (aux > maxD)? (maxD=aux, maxI=k) : 0;
        }

        return vertexs[maxI];
    }

    Point Polygon::GetCenter() const
    {
        Real coef = 1.0/vertexs.size();
        Point center(0,0);
        for(int k=0; k < vertexs.size() ; ++k)
            center += vertexs[k]*coef;
        return center;
    }

    void Polygon::AddVertex(const Point &b)
    {
        vertexs.push_back(b);
    }

    vector<Point>::size_type Polygon::GetVertexCount() const
    {
        return vertexs.size();
    }

    const Point& Polygon::GetLast() const
    {
        return vertexs.back();
    }

    Point& Polygon::operator()(int i)
    {
        return vertexs[i];
    }

    bool Polygon::Contains(const Point &b)
    {
        //Suponemos que los vertices estan en orden CW
        Point planeLine;
        Real z;
        for(int k=1; k < vertexs.size() ; ++k)
        {
            planeLine = vertexs[k]-vertexs[k-1];
            z = Cross2D(planeLine, b-vertexs[k-1]);
            if(z < 0)
                return false;
        }
        planeLine = vertexs.front()-vertexs.back();
        z = Cross2D(planeLine, b-vertexs.back());
        if(z < 0)
            return false;

        return true;
    }

    void Polygon::Remove(int i)
    {
        vertexs.erase(vertexs.begin()+i, vertexs.begin()+i+1);
    }

    void Polygon::Move(int dx, int dy)
    {
        for (int i = 0; i < vertexs.size(); ++i) {
            vertexs[i].Move(dx, dy);
        }
    }

    void Polygon::Rotate(float angRad, const Point& PointOfRotation)
    {
        // TODO
    }

    Point Polygon::GetClosestPointToOrigin()
    {
        Point closest;
        switch( vertexs.size() )
        {
            case 1:
                closest = vertexs[0];
                break;
            case 2:
                {
                const Point& _0a = vertexs[0];
                const Point& _0b = vertexs[1];
                Point _a0 = -_0a;
                Point _ab = _0b-_0a;
                Normalize(_ab);
                float t = Dot(_a0,_ab);
                closest = _ab * t + _0a;	//NOTE: it is not indeterminited because
                                            //_ab really is a vector, not a point
                }
                break;
            case 3:
                //NOT IMPLEMENTED
                assert(false);
                break;
            default:
                //NOT IMPLEMENTED
                assert(false);

        }

        return closest;
    }

    // Funcion de soporte de la diferencia de Minkowski
    // entre a y b en direccion d.
    Point minkowski_difference_support_function(const Polygon &a, const Polygon &b, const Point &d)
    {
        Point sa, sb;

        sa = a.support(d);
        sb = b.support(-d);
        return sa-sb;
    }

    bool doLine(Polygon &S, Point &d)
    {
        /*
             1  |     3   |
              A *---------* B
                |     4   |   2

                                Leyenda:
                                A      ultimo punto agregado al simplex
                                B      penultimo punto agregado al simplex
                                |      linea divisoria entre las regiones de voronoi de linea
                                numero identificador de region de voronoi
            En las regiones 1 y 2 no puede estar el origen O.
            Entonces solo debemos comprobar si no se encuentra en las regiones 3, 4 o sobre la linea.
            Si esta sobre la linea tenemos que devolver true porque "encerramos" al origen O.
            Notar que 1 y 2 son las regiones de los vertices y 3 y 4 son las de las lineas.
        */

        const Point &b = S(0);
        const Point &a = S(1);
        Point ab= b-a;
        d = TripleCross(ab, -a, ab);
        if( fastNorm(d) < GJK_epsilon_square )
            return true;// la linea contiene el origen
        else
        {
            Normalize(d);//para probar
            return false;// esta en 3 o en 4
        }
    }

    bool doTriangle(Polygon &S, Point &d)
    {
        /*
             1       4      2
              A *---------* B
                -    7    |  6
                 ---      |
                    ------* C
                    5		3	Leyenda:
                                A      ultimo punto agregado al simplex (ultimo elemento de S)
                                B      penultimo punto agregado al simplex
                                C      antepenultimo punto agregado al simplex
                                |      linea divisoria de la region de voronoi del interior del simplex
                                numero identificador de region de voronoi
            En las regiones 1, 2, 3 ni 6 (por la comprobacion de linea anterior) puede estar el origen O.
            Si esta en 7 tenemos que devolver true porque encerramos el origen O.
            Entonces solo debemos comprobar si no se encuentra en las regiones 4, 5 o 7.
            Notar que 1, 2 y 3 son las regiones de los vertices y 4, 5 y 6 son las de las lineas exteriores.
            La region 7 es el interior del triangulo.
        */

        const Point &c = S(0);
        const Point &b = S(1);
        const Point &a = S(2);
        Point ab= b-a;
        Point ac= c-a;
        Point perpAB = TripleCross(ac, ab, ab);
        Point perpAC = TripleCross(ab, ac, ac);

        if( Dot(perpAB, -a) > 0 )// esta en la region 4?
        {
            S.Remove(0);//remove C
            d = perpAB;
            Normalize(d);//para probar
            return false;
        }
        if( Dot(perpAC, -a) > 0 )// esta en la region 5?
        {
            S.Remove(1);//remove B
            d = perpAC;
            Normalize(d);//para probar
            return false;
        }

        return true;//esta en region 7
    }

    // Devuelve si el simplex contiene el origen, y si no lo contiene devuelve
    // la nueva direccion d y el simplex modificado
    bool updateSimplex(Polygon &S, Point &d)
    {
        switch( S.GetVertexCount() )
        {
            case 2:
                {
                    return doLine(S,d);
                }
                break;
            case 3:
                {
                    return doTriangle(S,d);
                }
                break;
            default:
                assert(false);//No deberia entrar con 1 vertice o mas de 3 jamas!
        }
    }
#ifdef _DEBUG
    bool gjk_2dIntersection_Test(const Polygon &a, const Polygon &b, Polygon &interseccion)
#else
    bool gjk_2dIntersection_Test(const Polygon &a, const Polygon &b)
#endif
    {
        Polygon S;//simplex
        Point d = -(b.GetCenter()-a.GetCenter());//para comenzar bien... nada mas
        S.AddVertex( minkowski_difference_support_function(a,b,d) );//primer punto
        d = -d;

        while (true)
        {
            S.AddVertex( minkowski_difference_support_function(a,b,d) );//agregar punto

            if( Dot( S.GetLast() , d ) <= 0 )
                return false; // NO TNTERSECCION - si no pasamos el origen entonces no hay interseccion
            else
            {
                if( updateSimplex(S,d) )
                {
#ifdef _DEBUG
                    interseccion = S;
#endif
                    return true;// INTERSECCION si contenemos al origen estamos intersectando
                }
            }
        }
    }

    bool gjk_2dIntersection_TestPasoAPaso(const Polygon &a, const Polygon &b, Polygon &S, Point &d, EstadoPasoAPaso& Estado)
    {
        // S es el simplex
        if( S.GetVertexCount() == 0 )
        {
            d = -(b.GetCenter()-a.GetCenter());//para comenzar bien... nada mas
            S.AddVertex( minkowski_difference_support_function(a,b,d) );//primer punto
            d = -d;
            S.AddVertex( minkowski_difference_support_function(a,b,d) );//agregar punto
            return false;
        }

        while (true)
        {
            if( Dot( S.GetLast() , d ) <= 0 )
            {
                Estado = NoInterseccion;
                return false; // NO TNTERSECCION - si no pasamos el origen entonces no hay interseccion
            }else
            {
                if( updateSimplex(S,d) )
                {
                    d = Point(0,0);
                    Estado = Interseccion;
                    return true;// INTERSECCION si contenemos al origen estamos intersectando
                }
            }

            S.AddVertex( minkowski_difference_support_function(a,b,d) );//agregar punto
            Estado = Paso;
            return false;
        }
    }

    // una funcion gjk_calcular_distancia

    Polygon minkowski_difference(Polygon &a, Polygon &b)
    {
        Polygon result;
        std::vector<Vertex2D> puntos;
        Point pd;
        for (int i = 0; i < a.GetVertexCount(); ++i)
        {
            for (int j = 0; j < b.GetVertexCount(); ++j)
            {
                pd = a(i) - b(j);
                puntos.push_back(Vertex2D(pd.x, pd.y));
            }
        }

        ConvexHull2D mink_diff(puntos, ConvexHull2D::GrahamScan);

        for (int i = 0; i < mink_diff.CH.size(); ++i)
        {
            result.AddVertex(Point(mink_diff.CH[i].x, mink_diff.CH[i].y));
        }

        return result;
    }

    double gjk_2dDistance(const Polygon &a, const Polygon &b)
    {
        Polygon S;//simplex
        Point d = -(b.GetCenter()-a.GetCenter());//para comenzar bien... nada mas
        S.AddVertex( minkowski_difference_support_function(a,b,d) );//primer punto
        d = -d;
        S.AddVertex( minkowski_difference_support_function(a,b,d) );//segundo punto

        while (true)
        {
            const Point closest = S.GetClosestPointToOrigin();

            if( closest == Point::Zero )
                return 0.0;//they are touching

            Point d = -closest;
            Normalize(d);

            const Point c = minkowski_difference_support_function(a,b,d);

            const float dc = Dot(c,d);		//Note that d is normalize so dc is distance
            const float da = Dot(S(0),d);
            const float db = Dot(S(1),d);

            if( (dc - da < EPS) || (dc - db < EPS)  )
            {
                //there is nothing more close to the origin
                //return dc;
                std::cout<<closest.x<<" "<<closest.y<<std::endl;
                return Norm(closest);
                //return Dot(closest,d);
            }

            if( Norm(S(0)) < Norm(S(1)) )
            {
                S(1) = c;
            }else
            {
                S(0) = c;
            }
        }
    }

}
