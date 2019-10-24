#include "Model.h"


class Light : public Model{


private:

	glm::vec3 position;

public:

	Light();

	~Light();
	
	void SetPosition(glm::vec3 _position);

	void SetPosition(float x, float y, float z);

	glm::vec3 GetPosition();

};