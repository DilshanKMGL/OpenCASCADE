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
#include <ShapeAnalysis_ShapeContents.hxx>
//OTHER
class ShapeAnalysis_ShapeContents;

class geom_ShapeContent
{
public:
  //CONSTRUCTOR
  //@param shape - target shape
  geom_ShapeContent(const TopoDS_Shape& shape);
  virtual ~geom_ShapeContent() {};
  
public:
  // Get the shape
  // @return resultant shape
  TopoDS_Shape getShape();

  // Perform shape checking
  void perform();

  // Clear data
  void clear();

  // get number of compound
  Standard_Integer NbCompound();
    
  // get number of compsolid
  Standard_Integer NbCompSolid();

   // get number of degenerated edges
  Standard_Integer NbDegeneratedEdges();

   // get number of non manifold edges
  Standard_Integer NbNonManifoldEdges();
    
private:
  // Target shape
  TopoDS_Shape myShape;

  ShapeAnalysis_ShapeContents shapeContenet;

  Standard_Integer myNbCompound;
  Standard_Integer myNbCompSolid;
  Standard_Integer myNbDegeneratedEdges;
  Standard_Integer myNbNonManifoldEdges;
};

