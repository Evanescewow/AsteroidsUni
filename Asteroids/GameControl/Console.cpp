#include "Console.h"
#include <sstream>

Console::Console()
{
	// Load console font
	if (!_consoleFont.loadFromFile(fontLocation))
	{
		throw std::exception("Unable to load font for console");
	}

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
	nMaxMessages = (yPixelSize / 16) - 1;
}

Console::~Console()
{

}

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

void Console::Draw(sf::RenderWindow* window)
{
	// Draw the console background
	window->draw(_consoleRect);

	// Draw current command
	window->draw(_currentCommand);

	// Draw previous commands / messages
	for (int i = 0; i < this->_consoleMessages.size(); i++)
	{
		// set position starting from the top and working down
		this->_consoleMessages[i].setPosition({ 0, (float)(0 + (i * TEXT_SIZE)) });
		window->draw(this->_consoleMessages[i]);
	}
}

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

	// Command not valid
	else
	{
		std::string errMsg = ('"' + commandWord + "\" is not a valid command.");
		this->PushBackMesage(errMsg, ERROR_COLOUR);
	}
}

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

Console::ParsedCommandData Console::ParseSet(std::deque<std::string>& parameters)
{
	return ParsedCommandData();
}

Console::ParsedCommandData Console::ParseToggle(std::deque<std::string>& parameters)
{
	ParsedCommandData output;
	std::string outputMessage = "";

	if (parameters[0] == "collideplayer")
	{
		output.commandType = CommandType::TOGGLE_PLAYER_COLLISION;
		outputMessage += "collidePlayer";
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
