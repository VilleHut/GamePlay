#include "Testing.h"

// Declare our game instance
Testing game;


static const float MOUSE_SPEED = 0.125f;
static const float MASS = 1.8f;
static const float FRICTION = 0.9f;
static const float SPEED = 5.5f;
static const unsigned int BUFFER_SIZE = 512;
static const float WATER_OFFSET = 0.55f;








Testing::Testing()
	: _scene(NULL), _wireframe(false), _banaaniMovement(0)
{
}

void Testing::initialize()
{
	Camera* camera = Camera::createPerspective(45.0f, (float)getWidth() / (float)getHeight(), 0.25f, 2000.0f);
    // Load game scene from file
    _scene = Scene::load("res/demo.scene");

    // Get the box model and initialize its material parameter values and bindings
     _cameraNode = _scene->findNode("camera1");
    //Model* cameraModel = dynamic_cast<Model*>(_cameraNode->getDrawable());
	_cameraNode->setCamera(camera);
	_scene->setActiveCamera(camera);

	


}

void Testing::finalize()
{
    SAFE_RELEASE(_scene);
}

void Testing::update(float elapsedTime)
{
	Vector2 axis;
	Vector3 force;
	const float minVal = 0.1f;

	if ((_inputMask & MOVE_FORWARD) || axis.y > minVal)
	{
		Vector3 cameraPosition = _cameraNode->getForwardVector();
		_cameraNode->translate(cameraPosition * SPEED);




	}



}

	

void Testing::render(float elapsedTime)
{
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);

    // Visit all the nodes in the scene for drawing
    _scene->visit(this, &Testing::drawScene);
}

bool Testing::drawScene(Node* node)
{
    // If the node visited contains a drawable object, draw it
    Drawable* drawable = node->getDrawable(); 
    if (drawable)
        drawable->draw(_wireframe);

    return true;
}

void Testing::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
	// This should only be called if the gamepad did not handle the touch event.
	switch (evt)
	{
	case Touch::TOUCH_PRESS:
	{
		_rotateX = x;
	}
	break;
	case Touch::TOUCH_RELEASE:
	{
		_rotateX = 0;
	}
	break;
	case Touch::TOUCH_MOVE:
	{
		int deltaX = x - _rotateX;
		_rotateX = x;
		_cameraNode->rotateY(-MATH_DEG_TO_RAD(deltaX * 0.5f));
	}
	break;
	default:
		break;
	}
}




void Testing::keyEvent(Keyboard::KeyEvent evt, int key)
{
   
	if (evt == Keyboard::KEY_PRESS)
	{
		switch (key)
		{
	
			
		case Keyboard::KEY_W:
		case Keyboard::KEY_UP_ARROW:
			_inputMask |= MOVE_FORWARD;
			break;
		case Keyboard::KEY_S:
		case Keyboard::KEY_DOWN_ARROW:
			_inputMask |= MOVE_BACKWARD;
			break;
		case Keyboard::KEY_A:
		case Keyboard::KEY_LEFT_ARROW:
			_inputMask |= MOVE_LEFT;
			break;
		case Keyboard::KEY_D:
		case Keyboard::KEY_RIGHT_ARROW:
			_inputMask |= MOVE_RIGHT;
			break;
		}
	}
	else if (evt == Keyboard::KEY_RELEASE)
	{
		switch (key)
		{
		case Keyboard::KEY_W:
		case Keyboard::KEY_UP_ARROW:
			_inputMask &= ~MOVE_FORWARD;
			break;
		case Keyboard::KEY_S:
		case Keyboard::KEY_DOWN_ARROW:
			_inputMask &= ~MOVE_BACKWARD;
			break;
		case Keyboard::KEY_A:
		case Keyboard::KEY_LEFT_ARROW:
			_inputMask &= ~MOVE_LEFT;
			break;
		case Keyboard::KEY_D:
		case Keyboard::KEY_RIGHT_ARROW:
			_inputMask &= ~MOVE_RIGHT;
			break;
		}
	}

    
}


