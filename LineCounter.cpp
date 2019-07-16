#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;
using namespace filesystem;

int main()
{
	cout << "Path: ";
	path target_path;
	cin >> target_path;

	cout << "Extensions ('.cpp' for example. '.' to end): ";
	unordered_set<string> ext;
	for (string buffer; cin >> buffer && buffer != ".";)
	{
		ext.emplace(move(buffer));
	}
	cout << "\n\n";

	auto total_chars = 0llu;
	auto total_lines = 0llu;
	auto max_chars = 0llu;
	auto max_lines = 0llu;
	auto max_chars_per_lines = 0llu;
	path most_chars;
	path most_lines;
	path most_chars_per_lines;

    for (auto&& e : recursive_directory_iterator{ target_path })
    {
		if (!e.is_regular_file()) continue;
	    if (ext.find(e.path().extension().string()) != ext.end())
	    {
			auto chars = 0llu;
			auto lines = 0llu;
		    ifstream file{ e.path() };
			for (string line; getline(file, line);)
			{
				if (!line.empty())
				{
					++lines;
					chars += line.size();
				}
			}

			auto check_max = [&e](auto current, auto& max, auto& most)
			{
				if (current > max)
				{
					max = current;
					most = e.path();
				}
			};

			const auto chars_per_lines = chars / lines;
			check_max(chars, max_chars, most_chars);
			check_max(lines, max_lines, most_lines);
			check_max(chars_per_lines, max_chars_per_lines, most_chars_per_lines);

			cout << "File " << e.path() << '\n'
	    		<< "  characters: " << chars << "\t  lines: " << lines << "\t characters per lines: " << chars_per_lines << "\n\n";

			total_chars += chars;
			total_lines += lines;
	    }
    }

	cout << "\nTotal characters: " << total_chars << '\n'
		<< "Total lines: " << total_lines << '\n'
		<< "Characters per lines: " << total_chars / total_lines << "\n\n"
		<< "File with most characters:\n  " << max_chars << " characters with\n    " << most_chars << "\n\n"
		<< "File with most lines:\n  " << max_lines << " lines with\n    " << most_lines << "\n\n"
		<< "File with most characters per lines:\n  " << max_chars_per_lines << " characters per lines with\n    " << most_chars_per_lines << '\n';
}
