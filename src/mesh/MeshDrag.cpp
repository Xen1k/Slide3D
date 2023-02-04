#include "MeshDrag.h"
#include "Selector.h"
#include "SelectionPolygon.h"
#include "MathUtils.h"
#include "Mouse.h"


void MoveSelectedFace(Object* obj)
{
	for (int i = 0; i < Selector::lastSelection->GetNumOfSelections(); i++)
	{
		obj->mesh->vertices[Selector::lastSelection->selectedVerticesIndexNumbers[i]].position += 
			Math::DotV3(Mouse::GetRaysFromMousePointDelta(), glm::distance(Camera::main->position, obj->GetPosition()));
	}
	if (SelectionPolygon::GetInstance().polygon)
		SelectionPolygon::GetInstance().UpdateVertices();
	obj->CalculateFlatNormals();
}

void MoveObject(Object* obj) 
{ 
	obj->Translate(Math::DotV3(Mouse::GetRaysFromMousePointDelta(), glm::distance(Camera::main->position, obj->GetPosition())));
}

void MeshDrag::PerformDragWithMouse(Object* obj)
{
	if (!obj)
		return;
	if (Selector::selectionMode == SelectionMode::Face)
		MoveSelectedFace(obj);
	else
		MoveObject(obj);
}