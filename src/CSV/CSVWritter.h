#ifndef __CSV_CSVWRITTER__H_
#define __CSV_CSVWRITTER__H_

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "./interfaces.h"

namespace CSV {
	class CSVWriter : ICSVWriter {
	public:
		CSVWriter(std::string path_to_file)
		{
			if (!this->init_file_stream(path_to_file)) {
				std::cerr << "ERROR::CSVWritter => create class" << std::endl;
				std::exit(-1);
			}
		}

		CSVWriter(std::string path_to_file, std::string separator)
		{
			if (!this->init_file_stream(path_to_file)) {
				std::cerr << "ERROR::CSVWritter => create class" << std::endl;
				std::exit(-1);
			}

			this->separator = separator;
		}

		~CSVWriter()
		{
			if (this->file_stream.is_open())
			{
				this->file_stream.close();
			}
		}

		bool is_open() const { return this->file_stream.is_open(); }

		void close_file()
		{
			if (this->file_stream.is_open())
			{
				this->file_stream.close();
			}
		}

		void write_line(std::vector<std::string> line)
		{
			if (!this->is_open())
			{
				std::cerr << "ERROR::CSVWritter => file is not open for writting" << std::endl;
				return;
			}

			std::stringstream s;
			copy(line.begin(), line.end(), std::ostream_iterator<std::string>(s, this->separator.c_str()));

			this->file_stream << s.str() << std::endl;
		}

	private:
		std::string separator = ";";
		std::ofstream file_stream;

		bool init_file_stream(std::string path_to_file) {
			this->file_stream = std::ofstream(path_to_file);

			if (!this->file_stream.is_open())
			{
				std::cerr << "ERROR::CSVWritter => init_file_stram " << path_to_file << " for reading" << std::endl;

				return false;
			}

			return true;
		}


	};
}

#endif // !__CSV_CSVWRITTER__H_
