#ifndef Alusta_H_
#define Alusta_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Main game class.
 */
class Alusta: public Game, public AnimationClip::Listener, public PhysicsCollisionObject::CollisionListener
{
public:

    /**
     * Constructor.
     */
    Alusta();

    /**
     * @see Game::keyEvent
     */
	void keyEvent(Keyboard::KeyEvent evt, int key);
	
    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

	
	/**
	* @see PhysicsCollisionObject::CollisionListener::collisionEvent
	*/
	bool doJump = false;
	void animationEvent(AnimationClip* clip, AnimationClip::Listener::EventType type);

	void collisionEvent(PhysicsCollisionObject::CollisionListener::EventType type,
		const PhysicsCollisionObject::CollisionPair& collisionPair,
		const Vector3& contactPointA = Vector3::zero(),
		const Vector3& contactPointB = Vector3::zero());


	
protected:
	
    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);

private:
	
	
    /**
     * Draws the scene each frame.
     */
	bool isonball;
	bool _kicking;
	bool isOnFloor() const;
	void jump();
	void kick();
	Animation* _animation;
	Animation* _animation1;
	AnimationClip* _currentClip;
	AnimationClip* _jumpClip;
	AnimationClip* _kickClip;
    bool drawScene(Node* node);
	PhysicsCharacter* _character;
	Node* _characterNode;
	Node* _characterMeshNode;
	Node* _characterShadowNode;
	Node* _basketballNode;
    Scene* _scene;
    bool _wireframe;
	void initializeCharacter();
	MaterialParameter* _materialParameterAlpha;
	void play(const char* id, bool repeat, float speed = 1.0f);
	int _rotateX;
	unsigned int _keyFlags;
	Vector2 _currentDirection;
	Node* _ball;
	PhysicsCollisionObject* pentti;
	
};

#endif
