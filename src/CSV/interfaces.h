#ifndef __CSV_INTERFACES__H_
#define __CSV_INTERFACES__H_

#include <vector>
#include <string>
#include <fstream>

/*
 * ���� ��� �������� ���������� ������� ��� namespace CSV
 *
 * ����������� ��������� ������:
 * - CSVReader
 */

namespace CSV {
	/*
	 * ����� ��� ������ � CSV ������
	 * ������������ ��� ���������� ������ � �����
	 *
	 * @Template T - ���������, � ������� ����� �������� ������ �� ������ �����
	 */
	template <typename T>
	class ICSVReader {
	public:
		// �������� ����������� �����
		virtual size_t size() const = 0;

		// ��������, ��� ������� ������� ����
		virtual bool is_open() const = 0;

		// ��������, ��� �������� ���� ����
		virtual bool is_reading_all() const = 0;

		// ����� ��� ��������������� �������� �����
		virtual void close_file() = 0;

		/*
		 * ������� ��� ���������� ��������� ������ �� CSV �����
		 * ��� ������� ��� ������ ����� �������� ����� line_reader ��� �������������� � ������ ���������
		 */
		virtual T* next() = 0;

		// ������� ��� ��������� ���� ����� �����
		virtual std::vector<T*> read_all() = 0;

		// ������� ��� ��������� �������� �� ��� id
		virtual T* find_by_id(int id) = 0;
	private:
		/*
		 * ������� ��������� ������ ��������� ������ �� csv �����
		 * ����� ��� ����, ���� ������ ������ ����� ������������� � ������ ��� ������
		 */
		virtual T* line_reader(std::vector<std::string>) = 0;
	};

	class ICSVWriter {
		// ��������, ��� ������� ������� ����
		virtual bool is_open() const = 0;

		// ����� ��� ��������������� �������� �����
		virtual void close_file() = 0;

		virtual void write_line(std::vector<std::string> line) = 0;
	};
}


#endif // !__CSV_INTERFACES__H_
