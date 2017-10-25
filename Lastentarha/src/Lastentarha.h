#ifndef Lastentarha_H_
#define Lastentarha_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Main game class.
 */
class Lastentarha: public Game
{
public:

    /**
     * Constructor.
     */
    Lastentarha();

    /**
     * @see Game::keyEvent
     */
	void keyEvent(Keyboard::KeyEvent evt, int key);
	
    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

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
	Scene* _scene;

	Node* _cameraNode;


	bool initializeScene(Node* node);
	void initializeMaterial(Scene* scene, Node* node, Material* material);
    bool _wireframe;
	bool drawScene(Node* node);
};

#endif
