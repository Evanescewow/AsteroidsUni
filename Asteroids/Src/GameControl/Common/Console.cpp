#include "Console.h"
#include <sstream>
#include "ResourceManager.h"

/* Constructor
 * Brief:
 *	assigns the correct parameters for the displayed command
 *  at the bottom of the shell alongside the rect in which
 *  the console is contained.
 *  Calculated the maximum number of previous messages that can be
 *  displayed given the console dimensions.
 */
Console::Console()
	:
	_consoleFont(ResourceManager::getInstance().GetFont())
{

	// Setup console background
	_consoleRect.setSize({ xPixelSize, yPixelSize });
	_consoleRect.setFillColor(CONSOLE_BACKGROUND);
	_consoleRect.setPosition({ 0 ,0 });

	// Setup the current command object (one always at the bottom)
	_currentCommand.setFont(_consoleFont);
	_currentCommand.setCharacterSize(TEXT_SIZE);
	_currentCommand.setFillColor(MAIN_TEXT_COLOUR);
	_currentCommand.setPosition({ 0, yPixelSize - TEXT_SIZE });
	_currentCommand.setString(_commandPrefix);

	// work out number of messages that can be shown
	nMaxMessages = static_cast<unsigned int>((yPixelSize / 16.0f) - 1.0f);
}

Console::~Console(){}

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
Console::ParsedCommandData Console::Update(sf::RenderWindow* window)
{
	// Output
	ParsedCommandData outputData;
	outputData.commandType = CommandType::INVALID_COMMAND;


	// Event loop, duplicating doesn't really matter here as 
	// event loop in main only handles 1 even and only once every game loop
	// using while on poll event here gives priority
	sf::Event evt;
	while(window->pollEvent(evt))
	{
		// Handle the text input to the console
		if (evt.type == sf::Event::TextEntered)
		{
			// if tab ignore
			if (evt.text.unicode == 9)
				continue;
			// If backspace reduce string by one
			else if (evt.text.unicode == 8)
			{
				// if no characters to delete just ignore
				if (_inputText.length() > 0)
					_inputText.pop_back();
			}
			// If enter add command to history list, reset current command display and parse command
			else if (evt.text.unicode == 13)
			{
				// Create new text object to be pushed back to messages
				sf::Text newMessageText = sf::Text(_inputText, _consoleFont, TEXT_SIZE);

				// add old message prefix Push back message
				this->PushBackMesage(_inputText);

				// Parse the message
				outputData = this->ParseCommand(_inputText);

				this->_inputText = "";
			}
			else // Normal text just add to input		
				_inputText += evt.text.unicode;

			// Update current command sfText object
			_currentCommand.setString(_commandPrefix + _inputText);
		}
	}


	return outputData;
}

/* void Draw
 * Brief:
 * Draws the console and any text to the screen.
 * This includes both the current command and any previously
 * entered messages / responses.
 * Params:
 *	<renderwindow*> window	-	window to draw the console to
 */
void Console::Draw(sf::RenderWindow* window)
{
	// Draw the console background
	window->draw(_consoleRect);

	// Draw current command
	window->draw(_currentCommand);

	// Draw previous commands / messages
	for (unsigned int i = 0; i < this->_consoleMessages.size(); i++)
	{
		// set position starting from the top and working down
		this->_consoleMessages[i].setPosition({ 0, (float)(0 + (i * TEXT_SIZE)) });
		window->draw(this->_consoleMessages[i]);
	}
}

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
Console::ParsedCommandData Console::ParseCommand(std::string& text)
{
	//transform the string to lowercase
	std::transform(text.begin(), text.end(), text.begin(),
		[](unsigned char c) { return std::tolower(c); });

	// split the string into words
    std::istringstream iss(text);
	std::deque<std::string> words((std::istream_iterator<std::string>(iss)),
		std::istream_iterator<std::string>());

	// Check if is empty
	if (words.size() == 0)
		return ParsedCommandData(CommandType::INVALID_COMMAND);
	

	// Get the command word and shorten deque
	std::string commandWord = words[0];
	words.pop_front();

	// Parse set command
	if (commandWord == "set")
		return this->ParseSet(words);

	// Parse toggle command
	else if (commandWord == "toggle")
		return this->ParseToggle(words);

	// Parse spawn commands (probably only asteroids
	else if (commandWord == "spawn")
		return this->ParseSpawn(words);

	// Command not valid
	else
	{
		std::string errMsg = ('"' + commandWord + "\" is not a valid command.");
		this->PushBackMesage(errMsg, ERROR_COLOUR);
	}

	// no valid commands, return invalid
	return ParsedCommandData(CommandType::INVALID_COMMAND);
}

// parses the set command given the parameters passed
Console::ParsedCommandData Console::ParseSet(std::deque<std::string>& parameters)
{
	ParsedCommandData output;
	std::string outputMessage;

	sf::Color outputMessageColor = COMMAND_SUCCESS_COLOUR;

	// Check number of parameters matches expected
	if (parameters.size() != 2)
	{
		output.commandType = CommandType::INVALID_COMMAND;
		outputMessage = "Incorrect number of parameters given.";
		outputMessageColor = ERROR_COLOUR;
	}


	// Set command for broad phase collision
	else if (parameters[0] == "col-broad")
	{
		// setup the output message sentence (only used on successful commands)
		outputMessage = "Broad phase collision set to " + parameters[1];

		// set broad phase to brute force
		if (parameters[1] == "bruteforce")
			output.commandType = CommandType::SET_BROAD_COLLISION_BRUTE_FORCE;
		// set broad phase to uniform grid
		else if (parameters[1] == "uniformgrid")
			output.commandType = CommandType::SET_BROAD_COLLISION_UNIFORM_GRID;
		// set broad phase to quadtree
		else if (parameters[1] == "quadtree")
			output.commandType = CommandType::SET_BROAD_COLLISION_QUADTREE;
		// invalid mode type given
		else
		{
			output.commandType = CommandType::INVALID_COMMAND;
			outputMessage = "Invalid broad phase collision mode given";
			outputMessageColor = ERROR_COLOUR;
		}
	}

	// set command for narrow phase collision
	else if (parameters[0] == "col-narrow")
	{
		outputMessage = "Narrow phase collision set to " + parameters[1];

#		// Set narrow phase collision to Axis-Aligned bounding box
		if (parameters[1] == "aabb")
			output.commandType = CommandType::SET_NARROW_COLLISION_AABB;
		// Set narrow phase collision to separated axis theorem
		else if (parameters[1] == "sat")
			output.commandType = CommandType::SET_NARROW_COLLISION_SAT;
		// Invalid narrow mode given
		else
		{
			output.commandType = CommandType::INVALID_COMMAND;
			outputMessage = "Invalid narrow phase collision mode given";
			outputMessageColor = ERROR_COLOUR;
		}
	}
	// Invalid set parameter
	else
	{
		output.commandType = CommandType::INVALID_COMMAND;
		outputMessage = parameters[0] + " is not a valid set parameter";
		outputMessageColor = ERROR_COLOUR;
	}

	// finish the output message sentence
	outputMessage += ".";
	// print output and return result
	this->PushBackMesage(outputMessage, outputMessageColor);
	return output;
}

// parses the toggle command given the parameters passed
Console::ParsedCommandData Console::ParseToggle(std::deque<std::string>& parameters)
{
	ParsedCommandData output;
	std::string outputMessage = "";

	// Toggle collision for the player
	if (parameters[0] == "col-player")
	{
		output.commandType = CommandType::TOGGLE_PLAYER_COLLISION;
		outputMessage += "Player collision";
	}

	// toggle collision between asteroids
	else if (parameters[0] == "col-asteroid")
	{
		output.commandType = CommandType::TOGGLE_ASTEROID_COLLISION;
		outputMessage += "Asteroid collision";
	}

	// toggle collision for bullets and asteroids
	else if (parameters[0] == "col-bullet")
	{
		output.commandType = CommandType::TOGGLE_BULLET_COLLISION;
		outputMessage += "Bullet collision";
	}

	// toggle drawing of grid
	else if (parameters[0] == "draw-grid")
	{
		output.commandType = CommandType::TOGGLE_DRAW_GRID;
		outputMessage += "Drawing of spatial grid";
	}

	// toggle drawing of collision info
	else if (parameters[0] == "col-info")
	{
		output.commandType = CommandType::TOGGLE_COLLISION_INFO;
		outputMessage += "collision info.";
	}


	// not a valid toggle target, print error return invalid command
	else
	{
		output.commandType = CommandType::INVALID_COMMAND;
		std::string errMsg = ('"' + parameters[0] + "\" is not a toggle target.");
		this->PushBackMesage(errMsg, ERROR_COLOUR);
		return output;
	}

	// return output
	outputMessage += " has been toggled.";
	this->PushBackMesage(outputMessage, COMMAND_SUCCESS_COLOUR);
	return output;
}

// parses the spawn command given the parameters passed
Console::ParsedCommandData Console::ParseSpawn(std::deque<std::string>& parameters)
{
	int nSpawnAmount = 0;
	std::string outputMessage;
	ParsedCommandData outputData;
	sf::Color messageColor = COMMAND_SUCCESS_COLOUR;

	outputData.commandType = CommandType::INVALID_COMMAND;

	// Check for correct number of parameters before int test
	if (parameters.size() != 2)
	{
		outputMessage = "Invalid number of parameters given.";
		messageColor = ERROR_COLOUR;
	}
	// if not correct number of parameters no further tests are needed
	else {
		// Test for valid integer
		try {
			nSpawnAmount = std::stoi(parameters[1]);
		}
		// if the parameter isn't an int give an error message and return
		catch (...)
		{
			messageColor = ERROR_COLOUR;
			outputMessage = "Invalid number of spawn objects given.";
			this->PushBackMesage(outputMessage, messageColor);
			return outputData;
		}

		// User wants to spawn asteroids
		if (parameters[0] == "asteroid")
		{
			outputData.commandType = CommandType::SPAWN_ASTEROID;
			outputData.additionalValue = static_cast<float>(nSpawnAmount);
			outputMessage = parameters[1] + " asteroids have been spawned.";
		}

		// Invalid spawn object given
		else
		{
			outputMessage = parameters[0] + " is not a spawnable object.";
			messageColor = ERROR_COLOUR;
		}
	}

	// return message and data
	this->PushBackMesage(outputMessage, messageColor);
	return outputData;
}

/* void PushBackMessage
 * Brief:
 *	Pushes a string to the previous messages container. Defaults to a white
 *  colour however can be changed for messages such as error or success text.
 * Params:
 *	<string> text		-	string to push back to console previous messages
 *  <color> textColor	-	colour to set the pushed back message to
 * Returns:
 */
void Console::PushBackMesage(std::string& text, sf::Color textColour)
{
	// Create new text object to be pushed back to messages
	sf::Text newMessageText = sf::Text(text, _consoleFont, TEXT_SIZE);
	newMessageText.setFillColor(textColour);

	// Check if max number of messages has been reached
	if (this->_consoleMessages.size() >= nMaxMessages)
	{
		this->_consoleMessages.pop_front();	// pop the first element out of the deque
	}
	// Push back the new message and clear the input text
	this->_consoleMessages.push_back(newMessageText);
}