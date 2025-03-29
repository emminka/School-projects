/**
 * @file    tree_mesh_builder.cpp
 *
 * @author  EMMA KROMPASCIKOVA <xkromp00@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    December 2024
 **/

#include <iostream>
#include <math.h>
#include <limits>

#include "tree_mesh_builder.h"

TreeMeshBuilder::TreeMeshBuilder(unsigned gridEdgeSize)
    : BaseMeshBuilder(gridEdgeSize, "Octree")
{

}


unsigned TreeMeshBuilder::buildMeshFromOctree(const Vec3_t<float> &cornerPosition, const float edgeLength, const ParametricScalarField &field) 
{
    unsigned triangles = 0;
    const float halfLenght = edgeLength / 2.0f  ;
    
    //returning builded cube when there is no more recursion
    if (halfLenght < 1.0f) {
        return buildCube(cornerPosition, field);
    }

    Vec3_t<float> centerPosition(
        (cornerPosition.x + halfLenght) * mGridResolution,
        (cornerPosition.y + halfLenght) * mGridResolution,
        (cornerPosition.z + halfLenght) * mGridResolution
    );

    // counted distance to surface from the center position
    float distToSurface = evaluateFieldAt(centerPosition, field);

    //used formula from the assignment
    //skip if the surface is too far from its surface - cut off
    if (distToSurface > (mIsoLevel + sqrt(3) * halfLenght * mGridResolution)) {
        return 0;
    }

    //in for cycle we generate 8 new children
    for (int i = 0; i < 8; ++i) {
        #pragma omp task shared(triangles, field)
        {
            float xOffset = 0.0f;
            float yOffset = 0.0f;
            float zOffset = 0.0f;

            //finding out if x y z are based on the value i
            if (i & 1) {
                xOffset = halfLenght; //moving in x axis
            } 
            if (i & 2){
                yOffset = halfLenght; //moving in y axis
            } 
            if (i & 4) {
                zOffset = halfLenght;  //moving in z axis
            }
 
            Vec3_t<float> newStartPos(
                cornerPosition.x + xOffset,
                cornerPosition.y + yOffset,
                cornerPosition.z + zOffset
            );

            #pragma omp atomic update
            // recursive calling for every subcube
            triangles += buildMeshFromOctree(newStartPos, halfLenght, field);
        }
    }
    #pragma omp taskwait
    return triangles;

}

unsigned TreeMeshBuilder::marchCubes(const ParametricScalarField &field)
{
    unsigned totalTriangles = 0;

    Vec3_t<float> startingPosition(0,0,0);

    #pragma omp parallel shared(totalTriangles, field)
    #pragma omp master
    totalTriangles = buildMeshFromOctree(startingPosition, mGridSize, field);

    return totalTriangles;
}

float TreeMeshBuilder::evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field)
{
    // NOTE: This method is called from "buildCube(...)"!

    // 1. Store pointer to and number of 3D points in the field
    //    (to avoid "data()" and "size()" call in the loop).
    const Vec3_t<float> *pPoints = field.getPoints().data();
    const unsigned count = unsigned(field.getPoints().size());

    float value = std::numeric_limits<float>::max();

    // 2. Find minimum square distance from points "pos" to any point in the
    //    field.
    for(unsigned i = 0; i < count; ++i)
    {
        float distanceSquared  = (pos.x - pPoints[i].x) * (pos.x - pPoints[i].x);
        distanceSquared       += (pos.y - pPoints[i].y) * (pos.y - pPoints[i].y);
        distanceSquared       += (pos.z - pPoints[i].z) * (pos.z - pPoints[i].z);

        // Comparing squares instead of real distance to avoid unnecessary
        // "sqrt"s in the loop.
        value = std::min(value, distanceSquared);
    }

    // 3. Finally take square root of the minimal square distance to get the real distance
    return sqrt(value);
}

void TreeMeshBuilder::emitTriangle(const BaseMeshBuilder::Triangle_t &triangle)
{
     // NOTE: This method is called from "buildCube(...)"!

    // Store generated triangle into vector (array) of generated triangles.
    // The pointer to data in this array is return by "getTrianglesArray(...)" call
    // after "marchCubes(...)" call ends.

    #pragma omp critical
    {
    mTriangles.push_back(triangle);
    }
    
}
