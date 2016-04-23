/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "BasicUserInterface.hpp"

#include <Magnum/Math/Range.h>

#include "Magnum/Ui/BasicPlane.h"

namespace Magnum { namespace Ui {

AbstractUserInterface::AbstractUserInterface(const Vector2& size, const Vector2i& screenSize): _size{size}, _coordinateScaling{size/Vector2{screenSize}} {}

AbstractUserInterface::~AbstractUserInterface() = default;

AbstractPlane* AbstractUserInterface::addPlane(AbstractPlane& plane) {
    CORRADE_INTERNAL_ASSERT(!_activePlane == _planes.empty());

    _planes.emplace_back(plane.rect(), plane);

    if(!_activePlane) {
        _activePlane = &plane;
        return nullptr;
    } else return _activePlane;
}

std::pair<Vector2, AbstractPlane*> AbstractUserInterface::handleEvent(const Vector2i& screenPosition) {
    Vector2 position = Vector2(screenPosition)*_coordinateScaling;
    position.y() = _size.y() - position.y();

    if(_activePlane && _activePlane->rect().contains(position) && !(_activePlane->flags() & AbstractPlane::Flag::Hidden))
        return {position, _activePlane};

    return {position, nullptr};
}

bool AbstractUserInterface::handleMoveEvent(const Vector2i& screenPosition) {
    Vector2 position;
    AbstractPlane* currentActivePlane;
    std::tie(position, currentActivePlane) = handleEvent(screenPosition);

    if(currentActivePlane) return currentActivePlane->handleMoveEvent(position - currentActivePlane->rect().min());
    return false;
}

bool AbstractUserInterface::handlePressEvent(const Vector2i& screenPosition) {
    Vector2 position;
    AbstractPlane* currentActivePlane;
    std::tie(position, currentActivePlane) = handleEvent(screenPosition);

    if(currentActivePlane) return currentActivePlane->handlePressEvent(position - currentActivePlane->rect().min());
    return false;
}

bool AbstractUserInterface::handleReleaseEvent(const Vector2i& screenPosition) {
    Vector2 position;
    AbstractPlane* currentActivePlane;
    std::tie(position, currentActivePlane) = handleEvent(screenPosition);

    if(currentActivePlane) return currentActivePlane->handleReleaseEvent(position - currentActivePlane->rect().min());
    return false;
}

}}