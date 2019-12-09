#pragma once

#include "SFML//Graphics.hpp"
#include "SFML//System.hpp"
#include "SFML//Window.hpp"
#include "SFML//Config.hpp"

#include <vector>
#include "Vertex2D.h"

class ConvexHull2D
{
private:
    void _GrahamScan(std::vector<Vertex2D> &P);

    void _Graham(std::vector<Vertex2D> &P);

    static bool RigthTurn(const Vertex2D &c1, const Vertex2D &c2, const Vertex2D &c3);

public:
    enum Method{GrahamScan, Graham, DivideAndConquer};

    ConvexHull2D(std::vector<Vertex2D> &P, const Method method);

    void Dibujar(sf::RenderWindow &RW);

    std::vector<Vertex2D> CH;
};