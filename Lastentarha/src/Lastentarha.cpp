#include "Lastentarha.h"

// Declare our game instance
Lastentarha game;

Lastentarha::Lastentarha()
    : _scene(NULL), _wireframe(false)
{
}

void Lastentarha::initialize()
{

    // Load game scene from file
    _scene = Scene::load("res/lastentarha.scene");
	// Update the aspect ratio for our scene's camera to match the current device resolution.
	_scene->getActiveCamera()->setAspectRatio(getAspectRatio());
	

	_scene->visit(this, &Lastentarha::initializeScene);
}

bool Lastentarha::initializeScene(Node* node)
{
	Model* model = dynamic_cast<Model*>(node->getDrawable());
	if (model && model->getMaterial())
	{
		initializeMaterial(_scene, node, model->getMaterial());
	}

	return true;
}

void Lastentarha::initializeMaterial(Scene* scene, Node* node, Material* material)
{
	// Bind light shader parameters to dynamic objects only
	if (node->hasTag("dynamic"))
	{
		material->getParameter("u_ambientColor")->bindValue(scene, &Scene::getAmbientColor);
		Node* lightNode = scene->findNode("sun");
		if (lightNode)
		{
			material->getParameter("u_directionalLightColor[0]")->bindValue(lightNode->getLight(), &Light::getColor);
			material->getParameter("u_directionalLightDirection[0]")->bindValue(lightNode, &Node::getForwardVectorView);
		}
	}
}


void Lastentarha::finalize()
{
    SAFE_RELEASE(_scene);
}

void Lastentarha::update(float elapsedTime)
{
   
}

void Lastentarha::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene for drawing
    _scene->visit(this, &Lastentarha::drawScene);
}

bool Lastentarha::drawScene(Node* node)
{
    // If the node visited contains a drawable object, draw it
    Drawable* drawable = node->getDrawable(); 
    if (drawable)
        drawable->draw(_wireframe);

    return true;
}

void Lastentarha::keyEvent(Keyboard::KeyEvent evt, int key)
{
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
        }
    }
}

void Lastentarha::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
   /* switch (evt)
    {
   
    };*/
}
