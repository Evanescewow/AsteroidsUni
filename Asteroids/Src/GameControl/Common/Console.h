#pragma once
#include <SFML/Graphics.hpp>
#include <deque>

/* Class Console
 * Brief:
 * Internal shell that can be used to manipulate application variables.
 * works by processing input data from the user, parsing the commands
 * and then returning those commands as a custom data structure.
 * This structure includes the command type and a possible extra
 * float values for situations where needed.
 */
class Console
{

	// Command Structures
public:

	/* enum class CommandType
	 * Brief:
	 * an enum for the type of command. includes a default "invalid command" for
	 * any situation where a command wasn't parsed correctly.
	 */
	enum class CommandType
	{
		INVALID_COMMAND,
		TOGGLE_PLAYER_COLLISION,
		TOGGLE_ASTEROID_COLLISION,
		TOGGLE_BULLET_COLLISION,
		TOGGLE_DRAW_GRID,
		TOGGLE_COLLISION_INFO,
		SET_BROAD_COLLISION_BRUTE_FORCE,
		SET_BROAD_COLLISION_UNIFORM_GRID,
		SET_BROAD_COLLISION_QUADTREE,
		SET_NARROW_COLLISION_AABB,
		SET_NARROW_COLLISION_SAT,
		SPAWN_ASTEROID
	};

	/* struct ParsedCommandData
	 * Brief:
	 * data structure that contains the parsed command data. This is the 
	 * type that is returned by the update method to represent a parsed command.
	 * This will be used to define command behavior wherever a console is managed.
	 */
	struct ParsedCommandData
	{
		// default construct
		ParsedCommandData() {};
		// overload construct for instant assignment
		ParsedCommandData(CommandType type, float value = 0.0f)
		:
		commandType(type),
		additionalValue(value){}

		CommandType commandType = CommandType::INVALID_COMMAND;	// type of command returned
		float additionalValue = 0.0f;							// container for set commands
	};

	// Public methods
public:
	/* Constructor
	 * Brief:
	 *	assigns the correct parameters for the displayed command
	 *  at the bottom of the shell alongside the rect in which
	 *  the console is contained.
	 *  Calculated the maximum number of previous messages that can be
	 *  displayed given the console dimensions.
	 */
	Console();
	~Console();

	/* ParsedCommandData Update
	 * Brief:
	 * Handles all logic for the console. updates text locations,
	 * text display, message push back and the parsing of entered
	 * commands. Returns parsed command data if applicable.
	 * Params:
	 *	<renderwindow*> window	-	window in which the text entered
	 *								events should be polled from
	 * Returns:
	 *  Parsed command data as custom data structure
	 */
	ParsedCommandData Update(sf::RenderWindow* window);

	/* void Draw
	 * Brief:
	 * Draws the console and any text to the screen.
	 * This includes both the current command and any previously
	 * entered messages / responses.
	 * Params:
	 *	<renderwindow*> window	-	window to draw the console to
	 */
	void Draw(sf::RenderWindow* window);

	// Toggle the flag that states if the console should be open
	void Toggle() { this->_isEnabled = !this->_isEnabled; }
	// returns if the console is open
	bool IsOpen() const { return this->_isEnabled; }

	// Private methods
private:
	/* ParsedCommandData ParseCommand
	 * Brief:
	 * Parses the raw command string. works out the main command word
	 * and calls the other parse commands accordingly passing them
	 * the remaining parameters.
	 * Params:
	 * <string&> text		-	string containing possible command
	 * Returns:
	 * <ParsedCommandData>	-	data containing parsed command information
	 */
	ParsedCommandData ParseCommand(std::string& text);

	// parses the set command given the parameters passed
	ParsedCommandData ParseSet(std::deque<std::string>& parameters);
	// parses the toggle command given the parameters passed
	ParsedCommandData ParseToggle(std::deque<std::string>& parameters);
	// parses the spawn command given the parameters passed
	ParsedCommandData ParseSpawn(std::deque<std::string>& parameters);

	/* void PushBackMessage
	 * Brief:
	 *	Pushes a string to the previous messages container. Defaults to a white
	 *  colour however can be changed for messages such as error or success text.
	 * Params:
	 *	<string> text		-	string to push back to console previous messages
	 *  <color> textColor	-	colour to set the pushed back message to
	 * Returns:
	 */
	void PushBackMesage(std::string& text, sf::Color textColour = sf::Color::White);

private:
	bool _isEnabled = false;										// flag to identify if the console should be running
	sf::RectangleShape _consoleRect;								// container rect for the console

	std::string _inputText;											// storage for the text passed into the console
	std::string _commandPrefix = "-> ";								// Prefix to mark the current command

	sf::Font& _consoleFont;											// Font for the console text
	sf::Text _currentCommand;										// Text object storing current message at the bottom
	std::deque<sf::Text> _consoleMessages;							// Container for pushed back messages

	sf::Color CONSOLE_BACKGROUND = sf::Color(20, 20, 20, 200);		// Background color of console rect
	sf::Color MAIN_TEXT_COLOUR = sf::Color::White;					// Color of default text
	sf::Color ERROR_COLOUR = sf::Color::Red;						// Color of error messages
	sf::Color COMMAND_SUCCESS_COLOUR = sf::Color::Green;			// Color for success messages

	unsigned int nMaxMessages = 0;									// Number of messages that can be pushed back

	// Constants
	const unsigned int TEXT_SIZE = 16;								// Size of the console text in px
	const float xPixelSize = 500.0f;								// Size of the console rect in x direction
	const float yPixelSize = 300.0f;								// Size of console rect in y direction
};

