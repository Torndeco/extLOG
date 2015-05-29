#include <boost/algorithm/string.hpp>

#include "spdlog/sinks/base_sink.h"

namespace spdlog
{
namespace sinks
{

class windows_console_sink_mt : public base_sink < std::mutex >
{
public:
	windows_console_sink_mt(std::unordered_map<int, std::string> colors) :
		_hConsole(INVALID_HANDLE_VALUE)
	{
		_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(_hConsole, &csbi);

		_colors[0] = csbi.wAttributes;
		for( int i = 1; i <= 9; i=i+1 )
		{
			if (boost::algorithm::iequals(colors[i], std::string("BLACK")) == 1)
			{
				_colors[i] = BLACK;
			}
			else if (boost::algorithm::iequals(colors[i], std::string("RED")) == 1)
			{
				_colors[i] = RED;
			}
			else if (boost::algorithm::iequals(colors[i], std::string("GREEN")) == 1)
			{
				_colors[i] = GREEN;
			}
			else if (boost::algorithm::iequals(colors[i], std::string("BROWN")) == 1)
			{
				_colors[i] = BROWN;
			}
			else if (boost::algorithm::iequals(colors[i], std::string("BLUE")) == 1)
			{
				_colors[i] = BLUE;
			}
			else if (boost::algorithm::iequals(colors[i], std::string("MAGENTA")) == 1)
			{
				_colors[i] = MAGENTA;
			}
			else if (boost::algorithm::iequals(colors[i], std::string("CYAN")) == 1)
			{
				_colors[i] = CYAN;
			}
			else if (boost::algorithm::iequals(colors[i], std::string("GRAY")) == 1)
			{
				_colors[i] = GRAY;
			}
			else if (boost::algorithm::iequals(colors[i], std::string("DARKGRAY")) == 1)
			{
				_colors[i] = DARKGRAY;
			}
			else if (boost::algorithm::iequals(colors[i], std::string("LIGHTRED")) == 1)
			{
				_colors[i] = LIGHTRED;
			}
			else if (boost::algorithm::iequals(colors[i], std::string("LIGHTGREEN")) == 1)
			{
				_colors[i] = LIGHTGREEN;
			}
			else if (boost::algorithm::iequals(colors[i], std::string("YELLOW")) == 1)
			{
				_colors[i] = YELLOW;
			}
			else if (boost::algorithm::iequals(colors[i], std::string("LIGHTBLUE")) == 1)
			{
				_colors[i] = LIGHTBLUE;
			}
			else if (boost::algorithm::iequals(colors[i], std::string("LIGHTMAGENTA")) == 1)
			{
				_colors[i] = LIGHTMAGENTA;
			}
			else if (boost::algorithm::iequals(colors[i], std::string("LIGHTCYAN")) == 1)
			{
				_colors[i] = LIGHTCYAN;
			}
			else if (boost::algorithm::iequals(colors[i], std::string("WHITE")) == 1)
			{
				_colors[i] = WHITE;
			}
			else
			{
				_colors[i] = _colors[0];
			}
		}
	}

protected:
	enum Color
	{
		BLACK        = 0x0000,
		RED          = 0x0004,
		GREEN        = 0x0002,
		BROWN        = 0x0006,
		BLUE         = 0x0001,
		MAGENTA      = 0x0005,
		CYAN         = 0x0003,
		GRAY         = 0x0007,
		DARKGRAY     = 0x0008,
		LIGHTRED     = 0x000C,
		LIGHTGREEN   = 0x000A,
		YELLOW       = 0x000E,
		LIGHTBLUE    = 0x0009,
		LIGHTMAGENTA = 0x000D,
		LIGHTCYAN    = 0x000B,
		WHITE        = 0x000F
	};


	void _sink_it(const details::log_msg& msg) override
	{

		WORD attr = _colors[0];
		attr &= 0xFFF0;
		attr |= _colors[msg.level + 1];
		SetConsoleTextAttribute(_hConsole, attr);

		//std::wstring utext = std::string(msg.formatted.c_str());
		std::string text(msg.formatted.c_str());
		DWORD written;
		//WriteConsoleW(_hConsole, utext.data(), static_cast<DWORD>(utext.size()), &written, NULL);
		WriteFile(_hConsole, text.data(), static_cast<DWORD>(text.size()), &written, NULL);
		SetConsoleTextAttribute(_hConsole, _colors[0]);
	}


private:
	HANDLE _hConsole;
	WORD   _colors[9];
};
}
}