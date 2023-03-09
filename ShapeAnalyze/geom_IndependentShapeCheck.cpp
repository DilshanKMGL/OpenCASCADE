//-----------------------------------------------------------------------------
// Created on: 09 March 2023
//-----------------------------------------------------------------------------
// Copyright (c) 2023, Lahiru Dilshan
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the copyright holder(s) nor the
//      names of all contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "geom_IndependentShapeCheck.h"

//TGenCAE
//OCCT
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <ShapeBuild_ReShape.hxx>
//QT
//OTHER

//CONSTRUCTOR
//@param shape - target shape
geom_IndependentShapeCheck::geom_IndependentShapeCheck(const TopoDS_Shape& shape) : myShape(shape)
{
  if (shape.IsNull())
    error = Invalid_Shape;
}

// Check independent edges of the given shape
// @return - list of shapes of edges
TopTools_ListOfShape geom_IndependentShapeCheck::checkIndependentEdges()
{
  if (error != No_Error)
    return TopTools_ListOfShape();

  TopTools_IndexedDataMapOfShapeListOfShape shapeMap;
  TopExp::MapShapesAndAncestors(myShape, TopAbs_EDGE, TopAbs_FACE, shapeMap);

  TopTools_ListOfShape indEdgeList;
  for (int i = 1; i <= shapeMap.Extent(); i++)
  {
    const TopoDS_Edge& edge = TopoDS::Edge(shapeMap.FindKey(i));
    const TopoDS_ListOfShape& faceList = shapeMap.FindFromIndex(i);
    if (!faceList.Size())
    {
      indEdgeList.Append(edge);
    }
  }

  return indEdgeList;
}

// Check independent vertices of the given shape
// @return - list of shapes of vertices
TopTools_ListOfShape geom_IndependentShapeCheck::checkIndependentVertices()
{
  if (error != No_Error)
    return TopTools_ListOfShape();

  TopTools_IndexedDataMapOfShapeListOfShape shapeMap;
  TopExp::MapShapesAndAncestors(myShape, TopAbs_VERTEX, TopAbs_EDGE, shapeMap);

  TopTools_ListOfShape indVertexVec;
  for (int i = 1; i <= shapeMap.Extent(); i++)
  {
    const TopoDS_Vertex& vertex = TopoDS::Vertex(shapeMap.FindKey(i));
    const TopoDS_ListOfShape& faceList = shapeMap.FindFromIndex(i);
    if (!faceList.Size())
    {
      indVertexVec.Append(vertex);
    }
  }

  return indVertexVec;
}

// Get number of independent edges in the given shape
// @return - number of independent edges
int geom_IndependentShapeCheck::getIndependentEdgeCount()
{
  return independentEdgeSize;
}

// Remove independent edges of the given shape
// This function will update original shape
void geom_IndependentShapeCheck::removeIndependentEdges()
{
  myShape = removedIndependentEdges();
}

// Remove independent edges of the given shape and give new instance
// @return - updated shape
TopoDS_Shape geom_IndependentShapeCheck::removedIndependentEdges()
{
  Handle(ShapeBuild_ReShape) reShape = new ShapeBuild_ReShape();
  TopTools_ListOfShape indEdgeVec = checkIndependentEdges();
  for each (TopoDS_Shape & edge in indEdgeVec)
  {
    reShape->Remove(TopoDS::Edge(edge));
  }
  return reShape->Apply(myShape);
}

// Get number of independent vertices in the given shape
// @return - number of independent vertices
int geom_IndependentShapeCheck::getIndependentVertexCount()
{
  return independentVertexSize;
}

// Remove independent vertices of the given shape
// This function will update original shape
void geom_IndependentShapeCheck::removeIndependentVertices()
{
  myShape = removedIndependentVertices();
}

// Remove independent vertices of the given shape and give new instance
// @return - updated shape
TopoDS_Shape geom_IndependentShapeCheck::removedIndependentVertices()
{
  Handle(ShapeBuild_ReShape) reShape = new ShapeBuild_ReShape();
  TopTools_ListOfShape indVerticeVec = checkIndependentVertices();
  for each (TopoDS_Shape & vertex in indVerticeVec)
  {
    reShape->Remove(TopoDS::Vertex(vertex));
  }
  return reShape->Apply(myShape);
}

// Remove independent shapes from the given shape
// This function will update the original shape
void geom_IndependentShapeCheck::removeIndependentShapes()
{
  myShape = removedIndependentShapes();
}

// Remove independent shapes from the given shape and return new instance
// @return - updated shape
TopoDS_Shape geom_IndependentShapeCheck::removedIndependentShapes()
{
  TopoDS_Shape tempShape = removedIndependentVertices();
  tempShape = removedIndependentEdges();
  return tempShape;
}

// Get the shape
// @return resultant shape
TopoDS_Shape geom_IndependentShapeCheck::getShape()
{
  return myShape;
}

// Perform independent shape checking
void geom_IndependentShapeCheck::perform()
{
  independentEdgeList = checkIndependentEdges();
  independentEdgeSize = independentEdgeList.Extent();

  independentVertexList = checkIndependentVertices();
  independentVertexSize = independentVertexList.Extent();
}

// Return true if independent shapes are calculated correctly
bool geom_IndependentShapeCheck::isDone()
{
  if (error != No_Error)
    return false;
  else
    return (!(independentVertexSize < 0 || independentEdgeSize < 0));
}

// Return error if there is any
ShapeCheckError geom_IndependentShapeCheck::getError()
{
  return error;
}
