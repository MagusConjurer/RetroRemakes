#pragma once

#ifndef OBJECT
#include "Mesh.h"

class Object {
public:
	Object();

	void SetMesh(Mesh* mesh);
	void Update();
	
	~Object();
private:
	Mesh* objMesh;
	
};

#endif
