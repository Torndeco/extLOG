/*
Copyright (C) 2014 Declan Ireland <http://github.com/torndeco/extDB2>

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


#pragma once

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>

#include <memory>
#include <string>
#include <unordered_map>

#include "spdlog/spdlog.h"



#define EXTDB_VERSION "57"
#define EXTDB_CONF_VERSION 5

class Ext
{
	public:
		Ext(std::string shared_libary_path);
		~Ext();
		void callExtension(char *output, const int &output_size, const char *function);


	private:
		enum Color
		{
			CC_BLACK        = 0x0000,
			CC_RED          = 0x0004,
			CC_GREEN        = 0x0002,
			CC_BROWN        = 0x0006,
			CC_BLUE         = 0x0001,
			CC_MAGENTA      = 0x0005,
			CC_CYAN         = 0x0003,
			CC_GRAY         = 0x0007,
			CC_DARKGRAY     = 0x0008,
			CC_LIGHTRED     = 0x000C,
			CC_LIGHTGREEN   = 0x000A,
			CC_YELLOW       = 0x000E,
			CC_LIGHTBLUE    = 0x0009,
			CC_LIGHTMAGENTA = 0x000D,
			CC_LIGHTCYAN    = 0x000B,
			CC_WHITE        = 0x000F
		};

		// ASIO Thread Queue
		std::unique_ptr<boost::asio::io_service::work> io_work_ptr;
		boost::asio::io_service io_service;
		boost::thread_group threads;

		bool enable_console = true;
		std::unordered_map<int, std::string> colors;

		std::string log_path_str;

		std::shared_ptr<spdlog::logger> console;

		std::unordered_map<std::string, std::shared_ptr<spdlog::logger> > loggers;

		void stop();
		void processMessage(std::string input_str);
};