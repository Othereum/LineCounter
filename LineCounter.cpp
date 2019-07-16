#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;
using namespace filesystem;

int main()
{
	wcout << L"Path: ";
	path target_path;
	wcin >> target_path;

	wcout << L"Extensions ('.cpp' for example. '.' to end): ";
	unordered_set<wstring> ext;
	for (wstring buffer; wcin >> buffer && buffer != L".";)
	{
		ext.emplace(move(buffer));
	}
	wcout << L"\n\n";

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
	    if (ext.find(e.path().extension().wstring()) != ext.end())
	    {
			auto chars = 0llu;
			auto lines = 0llu;
		    wifstream file{ e.path() };
			for (wstring line; getline(file, line);)
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

			wcout << L"File " << e.path() << L'\n'
	    		<< L"  characters: " << chars << L"\t  lines: " << lines << L"\t characters per lines: " << chars_per_lines << L"\n\n";

			total_chars += chars;
			total_lines += lines;
	    }
    }

	wcout << L"\nTotal characters: " << total_chars << L'\n'
		<< L"Total lines: " << total_lines << L'\n'
		<< L"Characters per lines: " << total_chars / total_lines << L"\n\n"
		<< L"File with most characters:\n  " << max_chars << L" characters with\n    " << most_chars << L"\n\n"
		<< L"File with most lines:\n  " << max_lines << L" lines with\n    " << most_lines << L"\n\n"
		<< L"File with most characters per lines:\n  " << max_chars_per_lines << L" characters per lines with\n    " << most_chars_per_lines << L'\n';
}
