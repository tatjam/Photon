#include "Transform.h"


namespace ph
{

	glm::mat4 Transform::buildMatrix()
	{
		mat = glm::mat4();
		mat = glm::translate(mat, pos);
		glm::mat4 rotm = glm::mat4_cast(rot);
		mat *= rotm;
		mat = glm::scale(mat, scale);

		return mat;
	}

	void Transform::rotEuler(glm::vec3 eu)
	{
		glm::quat qP = glm::angleAxis(glm::radians(eu.x), glm::vec3(1, 0, 0));
		glm::quat qY = glm::angleAxis(glm::radians(eu.y), glm::vec3(0, 1, 0));
		glm::quat qR = glm::angleAxis(glm::radians(eu.z), glm::vec3(0, 0, 1));

		rot = qP * qY * qR;
	}

	glm::vec3 Transform::getEuler()
	{
		return glm::eulerAngles(rot);
	}

	void Transform::move(glm::vec3 o)
	{
		pos += o;
		buildMatrix();
	}

	void Transform::rotate(glm::vec3 eu)
	{
		glm::vec3 curr = getEuler();
		curr += eu;
		glm::quat qP = glm::angleAxis(glm::radians(curr.x), glm::vec3(1, 0, 0));
		glm::quat qY = glm::angleAxis(glm::radians(curr.y), glm::vec3(0, 1, 0));
		glm::quat qR = glm::angleAxis(glm::radians(curr.z), glm::vec3(0, 0, 1));
		rot = qP * qY * qR;
	}

	Transform::Transform()
	{

	}

	Transform::~Transform()
	{

	}

}

