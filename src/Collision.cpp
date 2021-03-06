#include "Collision.h"

bool Collision::collidesWith_rad(const Movable& m1, const Movable& m2) {
    return norm(m1.center-m2.center) < m1._radius + m2._radius;
}

bool Collision::collidesWith_vertexSep(const Movable& m1, const Movable& m2) {
    for (int k=0; k<m2.vert.getVertexCount(); ++k) {
        int L = k, U = k == m2.vert.getVertexCount()-1 ? 0 : k+1;
        sf::Vector2f ort = {
            m2.vert[U].position.y-m2.vert[L].position.y,
            m2.vert[L].position.x-m2.vert[U].position.x
        };
        auto proj1 = dot(m1.vert[0].position, ort); 
        auto proj2 = dot(m2.vert[0].position, ort);
        short comp = proj1 < proj2 ? +1 : -1;
        int j=1;
        for (; j < m1.vert.getVertexCount() && comp*proj1 <= comp*proj2; ++j)
            proj1 = comp*max(comp*proj1, comp*dot(m1.vert[j].position,ort));
        if (j == m1.vert.getVertexCount()) {
            j = 1;
            for (; j < m2.vert.getVertexCount() && comp*proj1 <= comp*proj2; ++j)
                proj2 = comp*min(comp*proj2, comp*dot(m2.vert[j].position,ort));
            if (j == m2.vert.getVertexCount()) return false;
        }  

    }
    return true;
}