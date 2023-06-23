#include <iostream>
#include <fstream>
#include <variant>
#include <map>
#include <regex>
#include <string>
#include <Windows.h>

using namespace std;

class ini_parser {
public:
	ini_parser(const string& file_name) {
		fin.open(file_name);
		if (!fin.is_open()) { throw runtime_error("File " + file_name + " is not found!"); }
		else { parse(); }
	}

	ini_parser() = delete;
	ini_parser& operator=(const ini_parser&) = delete;
	ini_parser(const ini_parser&) = delete;
	
	template <typename T>
	T get_value(string val_name) {};
	template<>
	int get_value(string val_name) {
		if (val.count(val_name) > 0) {
			if (regex_match(val[val_name], regex("[+-]?[0-9]+")) || regex_match(val[val_name], regex("[+-]?[0-9]+[.]?[0-9]+"))) {
				int res = stoi(val[val_name]);
				return res;
			}
			else throw runtime_error("Value int is not found!");
		}
		else {
			throw runtime_error(val_name + " is not found");
		}
	}
	template<>
	double get_value(string val_name) {
		if (val.count(val_name) > 0) {
			if (regex_match(val[val_name], regex("[+-]?[0-9]+")) || regex_match(val[val_name], regex("[+-]?[0-9]+[.]?[0-9]+"))) {
				double res = stod(val[val_name]);
				return res;
			}
			else throw runtime_error("Value double is not found!");
		}
		else {
			throw runtime_error(val_name + " is not found");
		}
	}
	template<>
	string get_value(string val_name) {
		if (val.count(val_name) > 0) {
			string res = val[val_name];
			return res;
		}
		else {
			throw runtime_error(val_name + " is not found");
		}
	}
	
private:
	ifstream fin;
	string section;
	map<string, string> val;
	void parse() {
		int line_num = 1;
		string line;
		while (!fin.eof() && getline(fin, line)) {
			size_t comm = line.find(";");
			if (comm >= 0 && comm < 100) {
				line = line.substr(0, comm);
			}
			if (line.length() > 0) {
				auto sect = line.find("Section");
				if (sect == 1) {
					section = line.substr(1, 8);
				}
				auto ravno = line.find("=");
				if (ravno > 0 && ravno < 100) {
					auto probel = line.find(" ");
					if (probel > 0 && probel < 100) {
						line = line.substr(0, probel);
					}
					string name = line.substr(0, ravno);
					auto value = line.substr((ravno + 1), line.length());
					if (value == "") { val[(section + "." + name)] = "not value"; }
					else {
						val[(section + "." + name)] = value;
					}
				}
			}
			++line_num;
		}
		fin.close();
	}
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	try {
		ini_parser parser("ini_file.ini");
		auto value = parser.get_value<double>("Section1.var1");
		cout << "Section1.var1: " << value << "\n";
		auto value_int = parser.get_value<int>("Section1.var1");
		cout << "Section1.var1: " << value << "\n";
		auto value_string = parser.get_value<string>("Section1.var1");
		cout << "Section1.var1: " << value << "\n";
		auto value1 = parser.get_value<string>("Section1.var2");
		cout << "Section1.var2: " << value1 << "\n";
		auto value2 = parser.get_value<int>("Section2.var1");
		cout << "Section2.var1: " << value2 << "\n";
		auto value3 = parser.get_value<string>("Section2.var2");
		cout << "Section2.var2: " << value3 << "\n";
		auto value4 = parser.get_value<string>("Section4.Mode");
		cout << "Section4.Mode: " << value4 << "\n";
		auto value5 = parser.get_value<string>("Section6.var1");
	}
	catch (runtime_error& e) { cout << e.what() << "\n"; }
	catch (...) { cout << "Another mistake"; }
	return 0;
}