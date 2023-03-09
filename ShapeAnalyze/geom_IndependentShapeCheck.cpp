#include "geom_IndependentShapeCheck.h"
// Changed
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
