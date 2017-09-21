#ifndef Testing_H_
#define Testing_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Main game class.
 */
class Testing: public Game
{
public:

    /**
     * Constructor.
     */
    Testing();

	 enum CameraMovement
	{
		MOVE_FORWARD = (1 << 0),
		MOVE_BACKWARD = (1 << 1),
		MOVE_LEFT = (1 << 2),
		MOVE_RIGHT = (1 << 3),
	};
	 void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);



    /**
     * @see Game::keyEvent
     */
	void keyEvent(Keyboard::KeyEvent evt, int key);
	
    /**
     * @see Game::touchEvent
     */
    

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
	Vector3 _cameraAcceleration;
	unsigned _inputMask;
	int _prevX, _prevY;
	Vector3 _cameraforward;
	Vector2 _currentDirection;
    /**
     * Draws the scene each frame.
     */
    bool drawScene(Node* node);
	Node* _banaaniNode;
    Scene* _scene;
    bool _wireframe;
	int _banaaniMovement;
	Node* _cameraNode;
	int _rotateX;
	Node* _wallNode;
};

#endif
