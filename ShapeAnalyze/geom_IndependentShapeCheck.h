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

#pragma once

//OCCT
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopTools_ListOfShape.hxx>
//OTHER

enum ShapeCheckError
{
  No_Error = 0,
  Invalid_Shape
};

class geom_IndependentShapeCheck
{
public:
  //CONSTRUCTOR
  //@param shape - target shape
  geom_IndependentShapeCheck(const TopoDS_Shape& shape);
  virtual ~geom_IndependentShapeCheck() {};

private:

  // Check independent edges of the given shape
  // @return - list of shapes of edges
  TopTools_ListOfShape checkIndependentEdges();

  // Check independent vertices of the given shape
  // @return - list of shapes of vertices
  TopTools_ListOfShape checkIndependentVertices();

public:
  // Get number of independent edges in the given shape
  // @return - number of independent edges
  int getIndependentEdgeCount();

  // Remove independent edges of the given shape
  // This function will update original shape
  void removeIndependentEdges();

  // Remove independent edges of the given shape and give new instance
  // @return - updated shape
  TopoDS_Shape removedIndependentEdges();

  // Get number of independent vertices in the given shape
  // @return - number of independent vertices
  int getIndependentVertexCount();

  // Remove independent vertices of the given shape
  // This function will update original shape
  void removeIndependentVertices();

  // Remove independent vertices of the given shape and give new instance
  // @return - updated shape
  TopoDS_Shape removedIndependentVertices();

  // Remove independent shapes from the given shape
  // This function will update the original shape
  void removeIndependentShapes();

  // Remove independent shapes from the given shape and return new instance
  // @return - updated shape
  TopoDS_Shape removedIndependentShapes();

  // Get the shape
  // @return resultant shape
  TopoDS_Shape getShape();

  // Perform independent shape checking
  void perform();

  // Return true if independent shapes are calculated correctly
  bool isDone();

  // Return error if there is any
  ShapeCheckError getError();

private:
  // List of shapes of independent edges
  TopTools_ListOfShape independentEdgeList;

  // Independent edge count
  int independentEdgeSize = -1;

  // List of shapes of independent vertices
  TopTools_ListOfShape independentVertexList;

  // Independent vertex count
  int independentVertexSize = -1;

  // Error of the calculation
  ShapeCheckError error = ShapeCheckError::No_Error;

  // Target shape
  TopoDS_Shape myShape;
};

