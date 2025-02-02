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

void GameObject::Draw(Shader shader)
{
    // calculate the model matrix for each object and pass it to shader before drawing
    glm::mat4 model_mat = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model_mat = glm::translate(model_mat, transform.position);
    model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
    model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
    model_mat = glm::rotate(model_mat, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
    model_mat = glm::scale(model_mat, transform.getScale());
    shader.SetMatrix4("model", model_mat);
    objectModel.Draw(shader);
}

unsigned int GameObject::GetID() const { return ID; }

std::string GameObject::GetTag()
{
    return std::string();
}

bool GameObject::CompareTag(std::string otherTag)
{
    return this->tag == otherTag;
}


