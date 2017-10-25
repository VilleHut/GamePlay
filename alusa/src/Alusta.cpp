#include "Alusta.h"
#include "Bundle.h"
#include <Bundle.h>
// Declare our game instance
Alusta game;

// Input flags
#define NORTH 1
#define SOUTH 2
#define EAST 4
#define WEST 8
#define RUNNING 16

// Character defines
#define WALK_SPEED  100.0f
#define STRAFE_SPEED 1.5f
#define RUN_SPEED 300.0f
#define CAMERA_FOCUS_DISTANCE 16.0f

Alusta::Alusta()
    : _scene(NULL), _wireframe(false)
{
}

void Alusta::initialize()
{

	AudioSource* angel = AudioSource::create("res/angel.wav");
	angel->setGain(0.5f);
	//angel->play();
	

    // Load game scene from file
    _scene = Scene::load("res/demo.scene");


	_ball = _scene->findNode("ball");
	pentti = static_cast<PhysicsCollisionObject*>(_ball->getCollisionObject());
	
	_ball->setAudioSource(angel);
	angel->setVelocity(0.5, 0.0, 1.0);
	angel->play();
	
    // Get the box model and initialize its material parameter values and bindings
    
    // Set the aspect ratio for the scene's camera to match the current resolution
    //_scene->getActiveCamera()->setAspectRatio(getAspectRatio());

	initializeCharacter();

	


}



void Alusta::initializeCharacter()
{
	Node* node = _scene->findNode("boycharacter");
	Bundle* bundle =  Bundle::create("res/sample.gpb");
	Scene* scene1 = bundle->loadScene();
	//SAFE_RELEASE(bundle);
	Node* getoriginal = scene1->findNode("boycharacter");
	// Store the physics character object.
	_character = static_cast<PhysicsCharacter*>(node->getCollisionObject());

	// Store character nodes.
	_characterNode = node->findNode("boyScale");
	_characterMeshNode = _scene->findNode("boymesh");
	_characterShadowNode = _scene->findNode("boyshadow");
	
	// Load character animations.
	_animation = node->getAnimation("animations");
	_animation->createClips("res/boy.animation");


	_jumpClip = _animation->getClip("jump");
//	_jumpClip->addListener(this, _jumpClip->getDuration() - 100);
	_kickClip = _animation->getClip("kick");
	// when to cross fade
//	_kickClip->addListener(this, _kickClip->getDuration() - 100);
	// when to turn on _isKicking.
	//_kickClip->addListener(this, 416);

	
	play("idle", true);

	std::cout << _currentClip << std::endl;
	
	
}

void Alusta::play(const char* id, bool repeat, float speed)
{

	AnimationClip* clip = _animation->getClip(id);

	// Set clip properties
	clip->setSpeed(speed);
	clip->setRepeatCount(repeat ? AnimationClip::REPEAT_INDEFINITE : 1);

	// Is the clip already playing?
	if (clip == _currentClip && clip->isPlaying())
		return;

	// If a current clip is playing, crossfade into the new one
	if (_currentClip && _currentClip->isPlaying())
	{
		_currentClip->crossFade(clip, 150);
	}
	else
	{
		clip->play();
	}
	_currentClip = clip;


	

}

void Alusta::finalize()
{
    SAFE_RELEASE(_scene);
}

void Alusta::update(float elapsedTime)
{

	if (doJump == true)
	{
		_character->jump(60.0f);
	}

	_currentDirection.set(Vector2::zero());

	if (_currentDirection.isZero())
	{
		// Construct direction vector from keyboard input
		if (_keyFlags & NORTH)
			_currentDirection.y = 1;
		else if (_keyFlags & SOUTH)
			_currentDirection.y = -1;
		else
			_currentDirection.y = 0;

		if (_keyFlags & EAST)
			_currentDirection.x = 1;
		else if (_keyFlags & WEST)
			_currentDirection.x = -1;
		else
			_currentDirection.x = 0;

		_currentDirection.normalize();
		if ((_keyFlags & RUNNING) == 0)
			_currentDirection *= 0.5f;
	}

	// Update character animation and velocity
	if (_currentDirection.isZero())
	{
		play("idle", true);
		_character->setVelocity(Vector3::zero());
	}
	else
	{
		bool running = (_currentDirection.lengthSquared() > 0.75f);
		float speed = running ? RUN_SPEED : WALK_SPEED;

		play(running ? "running" : "walking", true, 1.0f);

		// Orient the character relative to the camera so he faces the direction we want to move.
		const Matrix& cameraMatrix = _scene->getActiveCamera()->getNode()->getWorldMatrix();
		Vector3 cameraRight, cameraForward;
		cameraMatrix.getRightVector(&cameraRight);
		cameraMatrix.getForwardVector(&cameraForward);

		// Get the current forward vector for the mesh node (negate it since the character was modelled facing +z)
		Vector3 currentHeading(-_characterNode->getForwardVectorWorld());

		// Construct a new forward vector for the mesh node
		Vector3 newHeading(cameraForward * _currentDirection.y + cameraRight * _currentDirection.x);

		// Compute the rotation amount based on the difference between the current and new vectors
		float angle = atan2f(newHeading.x, newHeading.z) - atan2f(currentHeading.x, currentHeading.z);
		if (angle > MATH_PI)
			angle -= MATH_PIX2;
		else if (angle < -MATH_PI)
			angle += MATH_PIX2;
		angle *= (float)elapsedTime * 0.001f * MATH_PIX2;
		_characterNode->rotate(Vector3::unitY(), angle);

		// Update the character's velocity
		Vector3 velocity = -_characterNode->getForwardVectorWorld();
		velocity.normalize();
		velocity *= speed;
		_character->setVelocity(velocity);
	}

	
}

void Alusta::render(float elapsedTime)
{
	// Clear the color and depth buffers.
	clear(CLEAR_COLOR_DEPTH, Vector4(0.41f, 0.48f, 0.54f, 1.0f), 1.0f, 0);

	// Visit all the nodes in the scene for drawing
	_scene->visit(this, &Alusta::drawScene);
	
	
}

bool Alusta::drawScene(Node* node)
{
    // If the node visited contains a drawable object, draw it
    Drawable* drawable = node->getDrawable(); 
    if (drawable)
        drawable->draw(_wireframe);

    return true;
}

void Alusta::keyEvent(Keyboard::KeyEvent evt, int key)
{
	if (evt == Keyboard::KEY_PRESS)
	{
		switch (key)
		{
		case Keyboard::KEY_ESCAPE:
			exit();
			break;
		case Keyboard::KEY_W:
		case Keyboard::KEY_CAPITAL_W:
			_keyFlags |= NORTH;
			_keyFlags &= ~SOUTH;
			break;
		case Keyboard::KEY_S:
		case Keyboard::KEY_CAPITAL_S:
			_keyFlags |= SOUTH;
			_keyFlags &= ~NORTH;
			break;
		case Keyboard::KEY_A:
		case Keyboard::KEY_CAPITAL_A:
			_keyFlags |= WEST;
			_keyFlags &= ~EAST;
			break;
		case Keyboard::KEY_D:
		case Keyboard::KEY_CAPITAL_D:
			_keyFlags |= EAST;
			_keyFlags &= ~WEST;
			break;
		case Keyboard::KEY_B:
			
			break;
		case Keyboard::KEY_SPACE:
			jump();
			break;
		case Keyboard::KEY_SHIFT:
			_keyFlags |= RUNNING;
			break;
		case Keyboard::KEY_M:
		case Keyboard::KEY_CAPITAL_M:
			_wireframe = !_wireframe;
			break;
		case Keyboard::KEY_C:
		case Keyboard::KEY_CAPITAL_C:
			
			break;
		}
	}
	else if (evt == Keyboard::KEY_RELEASE)
	{
		switch (key)
		{
		case Keyboard::KEY_W:
		case Keyboard::KEY_CAPITAL_W:
			_keyFlags &= ~NORTH;
			break;
		case Keyboard::KEY_S:
		case Keyboard::KEY_CAPITAL_S:
			_keyFlags &= ~SOUTH;
			break;
		case Keyboard::KEY_A:
		case Keyboard::KEY_CAPITAL_A:
			_keyFlags &= ~WEST;
			break;
		case Keyboard::KEY_D:
		case Keyboard::KEY_CAPITAL_D:
			_keyFlags &= ~EAST;
			break;
		case Keyboard::KEY_SHIFT:
			_keyFlags &= ~RUNNING;
			break;
		
		}
	}
}

void Alusta::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
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
		_character->getNode()->rotateY(-MATH_DEG_TO_RAD(deltaX * 0.5f));
	}
	break;
	default:
		break;
	}
}

bool Alusta::isOnFloor() const
{
	return (std::fabs(_character->getCurrentVelocity().y) < MATH_EPSILON);
}


void Alusta::jump()
{
	if (isOnFloor() && !_kickClip->isPlaying())
	{
		play("jump", false, 50.55f);
		_character->jump(80.0f);
	}
}

void Alusta::kick()
{
	if (!_jumpClip->isPlaying())
		play("kick", false, 1.75f);
	_kicking = true;
}

void Alusta::animationEvent(AnimationClip* clip, AnimationClip::Listener::EventType type)
{
	if (clip == _kickClip )
	{
		_keyFlags = 0;
		
	}
	else
	{
		clip->crossFade(_currentClip, 100);
	}
}

void Alusta::collisionEvent(PhysicsCollisionObject::CollisionListener::EventType type,
	const PhysicsCollisionObject::CollisionPair& collisionPair,
	const Vector3& contactPointA,
	const Vector3& contactPointB)
{
	if (type == PhysicsCollisionObject::CollisionListener::COLLIDING && collisionPair.objectA == pentti &&  collisionPair.objectB == _character)
	{

		if (!isOnFloor())
		{
			isonball = true;

			
		}
	}
		


}