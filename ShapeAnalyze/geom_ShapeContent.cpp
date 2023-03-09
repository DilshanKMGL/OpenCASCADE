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

#include "geom_ShapeContent.h"

//TGenCAE
//OCCT
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <BRep_Tool.hxx>
//QT
//OTHER

//CONSTRUCTOR
//@param shape - target shape
geom_ShapeContent::geom_ShapeContent(const TopoDS_Shape& shape) : myShape(shape)
{
  clear();
  shapeContenet.Perform(shape);
  perform();
}

// Get the shape
// @return resultant shape
TopoDS_Shape geom_ShapeContent::getShape()
{
  return myShape;
}

// Perform shape checking
void geom_ShapeContent::perform()
{
  clear();
  shapeContenet.Perform(myShape);

  TopTools_IndexedMapOfShape mapCompound;
  TopExp::MapShapes(myShape, TopAbs_COMPOUND, mapCompound);
  myNbCompound = mapCompound.Extent();

  TopTools_IndexedMapOfShape mapCompSolid;
  TopExp::MapShapes(myShape, TopAbs_COMPSOLID, mapCompSolid);
  myNbCompSolid = mapCompSolid.Extent();

  TopTools_IndexedMapOfShape mapDegeneratedEdge;
  TopExp::MapShapes(myShape, TopAbs_EDGE, mapDegeneratedEdge);
  for (int count = 1; count <= mapDegeneratedEdge.Extent(); count++)
  {
    if (BRep_Tool::Degenerated(TopoDS::Edge(mapDegeneratedEdge(count))))
      myNbDegeneratedEdges++;
  }

  TopTools_IndexedDataMapOfShapeListOfShape shapeListNonManifoldEdges;
  TopExp::MapShapesAndAncestors(myShape, TopAbs_EDGE, TopAbs_FACE, shapeListNonManifoldEdges);
  const int nEdges = shapeListNonManifoldEdges.Extent();
  for (int eCount = 1; eCount <= nEdges; eCount++)
  {
    const TopoDS_Edge& edge = TopoDS::Edge(shapeListNonManifoldEdges.FindKey(eCount));
    //
    if (BRep_Tool::Degenerated(edge))
      continue;

    const int nOwningFaces = shapeListNonManifoldEdges.FindFromIndex(eCount).Extent();
    if (nOwningFaces > 2)
      myNbNonManifoldEdges++;
  }
}

// Clear data
void geom_ShapeContent::clear()
{
  myNbCompound = 0;
  myNbCompSolid = 0;
  myNbDegeneratedEdges = 0;
  myNbNonManifoldEdges = 0;
}

// get number of compound
Standard_Integer geom_ShapeContent::NbCompound()
{
  return myNbCompound;
}

// get number of compsolid
Standard_Integer geom_ShapeContent::NbCompSolid()
{
  return myNbCompSolid;
}

// get number of degenerated edges
Standard_Integer geom_ShapeContent::NbDegeneratedEdges()
{
  return myNbDegeneratedEdges;
}

// get number of non manifold edges
Standard_Integer geom_ShapeContent::NbNonManifoldEdges()
{
  return myNbNonManifoldEdges;
}
