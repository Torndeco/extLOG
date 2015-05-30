/*
Copyright (C) 2014 Declan Ireland <http://github.com/torndeco/extBES>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


#include "ext.h"


#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Util/IniFileConfiguration.h>

#include <iostream>


Ext::Ext(std::string shared_library_path)
{
	try
	{
		//      File Logger
		Poco::DateTime current_dateTime;

		boost::filesystem::path log_relative_path;
		log_relative_path = boost::filesystem::path(shared_library_path);
		log_relative_path = log_relative_path.parent_path();
		log_relative_path /= "extLOG";
		log_relative_path /= "logs";
		log_relative_path /= Poco::DateTimeFormatter::format(current_dateTime, "%Y");
		log_relative_path /= Poco::DateTimeFormatter::format(current_dateTime, "%n");
		log_relative_path /= Poco::DateTimeFormatter::format(current_dateTime, "%d");
		boost::filesystem::create_directories(log_relative_path);
		log_path_str = log_relative_path.make_preferred().string();

		boost::filesystem::path config_path;
		config_path = boost::filesystem::path(shared_library_path);
		config_path = config_path.parent_path();
		config_path /= "extLOG.ini";

		if (boost::filesystem::exists(config_path))
		{
			Poco::AutoPtr<Poco::Util::IniFileConfiguration> pConf(new Poco::Util::IniFileConfiguration(config_path.make_preferred().string()));
			enable_console = pConf->getBool(("Main.Enable Console"), "true");

			colors[0] = "default";
			colors[1] = pConf->getString(("Main.Colour 1"), "default");
			colors[2] = pConf->getString(("Main.Colour 2"), "default");
			colors[3] = pConf->getString(("Main.Colour 3"), "default");
			colors[4] = pConf->getString(("Main.Colour 4"), "default");
			colors[5] = pConf->getString(("Main.Colour 5"), "default");
			colors[6] = pConf->getString(("Main.Colour 6"), "default");
			colors[7] = pConf->getString(("Main.Colour 7"), "default");
			colors[8] = pConf->getString(("Main.Colour 8"), "default");
			colors[9] = pConf->getString(("Main.Colour 9"), "default");
		}

		if (enable_console)
		{
			AllocConsole();
			HWND hwnd = ::GetConsoleWindow();
			if (hwnd != NULL)
			{
				HMENU hMenu = ::GetSystemMenu(hwnd, FALSE);
				if (hMenu != NULL) DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
			}
			std::wstring title(L"extLOG Output");
			SetConsoleTitle(title.c_str());
		}

		spdlog::set_level(spdlog::level::trace);
		spdlog::set_pattern("[%H:%M:%S:%e] %v");
		console = spdlog::windows_console_sink_mt("extLOG Console Logger", colors);

		// ASIO IO SERVICE
		io_work_ptr.reset(new boost::asio::io_service::work(io_service));
		threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));
	}
	catch (const boost::filesystem::filesystem_error& e)
	{
		std::cout << "BOOST FILESYSTEM ERROR: " << e.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}


Ext::~Ext(void)
{
	stop();
}


void Ext::stop()
{
	#ifdef DEBUG_TESTING
		console->info("extLOG: Stopping ...");
	#endif
	io_work_ptr.reset();
	threads.join_all();
	io_service.stop();
}


void Ext::processMessage(std::string input_str)
{
	std::string::size_type found = input_str.find(':', 2);
	std::string message = input_str.substr(found + 1);

	if (enable_console)
	{
		switch (input_str[0])
		{
		case '1':
			console->trace("{0}", message);
			break;
		case '2':
			console->debug("{0}", message);
			break;
		case '3':
			console->info("{0}", message);
			break;
		case '4':
			console->notice("{0}", message);
			break;
		case '5':
			console->warn("{0}", message);
			break;
		case '6':
			console->error("{0}", message);
			break;
		case '7':
			console->critical("{0}", message);
			break;
		case '8':
			console->alert("{0}", message);
			break;
		case '9':
			console->emerg("{0}", message);
			break;
		}
	}

	std::string filename = input_str.substr(2, found - 2);
	if (!(filename.empty()))
	{
		if (loggers.count(filename) == 0)
		{
			loggers[filename] = spdlog::rotating_logger_mt(filename, log_path_str + "\\" + filename, 1048576 * 100, 3, true);
		}
		loggers[filename]->info("{0}", input_str);
	}
}

void Ext::callExtension(char *output, const int &output_size, const char *function)
{
	std::string input_str(function);
	if (input_str.size() > 2)
	{
		io_service.post(boost::bind(&Ext::processMessage, this, std::move(input_str)));
		std::strcpy(output, "[1]");
	}
}
