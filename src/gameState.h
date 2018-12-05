


class GameState
{
public
	virtual void handleEvents() = 0;
	virtual void logic() = 0;
	virtual void render() = 0;
	virtual ~GameState() {};

	void setNextState(int newState);
	void changeState();

	int stateID = 0;
	int nextState = 0;

	GameState *currentState = NULL;

};