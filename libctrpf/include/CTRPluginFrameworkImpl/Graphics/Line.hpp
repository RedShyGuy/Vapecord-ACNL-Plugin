#ifndef CTRPLUGINFRAMEWORK_LINE_HPP
#define CTRPLUGINFRAMEWORK_LINE_HPP

#include "CTRPluginFramework/System/Vector.hpp"

namespace CTRPluginFramework
{
    template <typename T>
    class Line
    {
    public:
        Line(T startX, T startY, T endX, T endY);
        Line(Vector<T> startPoint, Vector<T> endPoint);
        ~Line(){}

        Vector<T> start;
        Vector<T> end;
    };

    typedef Line<int> IntLine;
    typedef Line<unsigned int> UIntLine;

    template <typename T>
    Line<T>::Line(T startX, T startY, T endX, T endY):
    start(startX, startY), end(endX, endY)
    {
    }

    template <typename T>
    Line<T>::Line(Vector<T> startPoint, Vector<T> endPoint) :
    start(startPoint), end(endPoint)
    {
    }
}

#endif