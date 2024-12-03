#include "GameObject.h"
#include "Game.h"


static unsigned int lastID = 0;

GameObject::GameObject()
{
	ID = lastID++;
	transform = Transform();
}

bool GameObject::operator==(const GameObject& go)
{
	return go.GetID() == this->GetID();
}

bool GameObject::operator!=(const GameObject& go)
{
	return go.GetID() != this->GetID();
}

void GameObject::Update(float deltaTime)
{
}

void GameObject::Draw(Shader ourShader)
{
    // calculate the model matrix for each object and pass it to shader before drawing
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model = glm::translate(model, this->transform.getPosition());
    model = glm::rotate(model, glm::radians(this->transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
    model = glm::rotate(model, glm::radians(this->transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
    model = glm::rotate(model, glm::radians(this->transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
    model = glm::scale(model, this->transform.getScale());

    ourShader.setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}


unsigned int GameObject::GetID() const { return ID; }


