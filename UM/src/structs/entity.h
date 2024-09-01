#include "../include.h"
#include "vector.h"

namespace pawn{
	class Entity
	{
	public:
		uintptr_t pawnAddress;
		uintptr_t controllerAddress;
		Vector3 origin;
		Vector3 view;
		int health;
		int lifeState;
		float distance;
		uint8_t team;

		view_matrix_t viewMatrix;

	private:

	};

	class Local
	{
	public:
		uintptr_t pawnAddress;
		uintptr_t controllerAddress;
		Vector3 origin;
		Vector3 view;
		int health;
		int lifeState;
		float distance;
		uint8_t team;

		view_matrix_t viewMatrix;
	private:

	};
}

