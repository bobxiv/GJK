#include <algorithm>


#include "ConvexHull2D.h"

ConvexHull2D::ConvexHull2D(std::vector<Vertex2D> &P, const Method method)
{
    switch(method)
    {
        case GrahamScan:
            _GrahamScan(P);
            break;
        case Graham:
            _Graham(P);
            break;
    }
}

bool ConvexHull2D::RigthTurn(const Vertex2D &c1, const Vertex2D &c2, const Vertex2D &c3)
{
    Vertex2D e1 = c2 - c1;
    Vertex2D e2 = c3 - c2;
    return Vertex2D::Cross(e1,e2)<0;
}

bool comp(const Vertex2D &a, const Vertex2D &b)
{
    return (a.x==b.x)? a.y<b.y : a.x<b.x;
}

void ConvexHull2D::_GrahamScan(std::vector<Vertex2D> &P)
{
    std::sort(P.begin(), P.end(), comp);

    //casco superior
    std::vector<Vertex2D> cascoUp;

    cascoUp.push_back(P[0]);
    cascoUp.push_back(P[1]);
    
    for(int v=2; v < P.size() ; ++v)
    {
        cascoUp.push_back(P[v]);

        while( cascoUp.size() > 2 && !ConvexHull2D::RigthTurn(cascoUp[cascoUp.size()-3],cascoUp[cascoUp.size()-2],cascoUp[cascoUp.size()-1]) )
            cascoUp.erase(--(--cascoUp.end()));
    }

    //casco inferior
    std::vector<Vertex2D> cascoDown;

    cascoDown.push_back(P[P.size()-1]);
    cascoDown.push_back(P[P.size()-2]);

    for(int v=P.size()-3; v > -1 ; --v)
    {
        cascoDown.push_back(P[v]);

        while( cascoDown.size() > 2 && !ConvexHull2D::RigthTurn(cascoDown[cascoDown.size()-3],cascoDown[cascoDown.size()-2],cascoDown[cascoDown.size()-1]) )
            cascoDown.erase(--(--cascoDown.end()));
    }

    cascoDown.erase(cascoDown.begin());
    cascoDown.pop_back();

    CH.insert(CH.begin(), cascoDown.begin(), cascoDown.end());
    CH.insert(CH.end(), cascoUp.begin(), cascoUp.end());
}

bool ycomp(const Vertex2D &a, const Vertex2D &b)
{
    return a.y<b.y;
}

bool secondcomp(const std::pair<int,float> &a, const std::pair<int,float> &b)
{
    return a.second<b.second;
}

void ConvexHull2D::_Graham(std::vector<Vertex2D> &P)
{
    std::vector<Vertex2D>::iterator itmin = std::min_element(P.begin(), P.end(), ycomp);
    std::vector<std::pair<int,float>> sortIndex(P.size(), std::pair<int,float>(0,0.0f));
    Vertex2D right(1.0f,0.0f);

    for(int i=0; i < sortIndex.size() ; ++i)
    {
        sortIndex[i].first  = i;
        if( P[i] == *itmin )
            sortIndex[i].second = -999.0f;
        else
            sortIndex[i].second = Vertex2D::Dot(right, P[i]-*itmin)/Vertex2D::Norm(P[i]-*itmin);
    }
    std::sort(sortIndex.begin(), sortIndex.end(), secondcomp);
    

    CH.push_back(P[sortIndex[0].first]);
    CH.push_back(P[sortIndex[1].first]);
    
    for(int v=2; v < P.size() ; ++v)
    {
        CH.push_back(P[sortIndex[v].first]);

        while( CH.size() > 2 && !ConvexHull2D::RigthTurn(CH[CH.size()-3],CH[CH.size()-2],CH[CH.size()-1]) )
            CH.erase(--(--CH.end()));
    }
}

void ConvexHull2D::Dibujar(sf::RenderWindow &RW)
{
    //sf::Shape hull;
    sf::ConvexShape hull;
    hull.setPointCount(CH.size());
    for(int v=0; v < CH.size() ; ++v)
        hull.setPoint(v, sf::Vector2f(CH[v].x, CH[v].y));

    hull.setFillColor(sf::Color(0, 0, 0, 0));
    hull.setOutlineThickness(1.0f);
    hull.setOutlineColor(sf::Color::Yellow);

    RW.draw(hull);
}