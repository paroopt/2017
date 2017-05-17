#include <cerrno>
#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

constexpr std::uintmax_t default_size {1000000};
constexpr char const default_mode[] {"ascending"};

bool safe_str_to_uintmax_t(int arg, char* argv[], std::uintmax_t& out)
{
	char* endptr = nullptr;
	errno = 0;
	std::uintmax_t const temp = std::strtoumax(argv[arg], &endptr, 10);
	if((endptr == argv[arg]) || (*endptr != '\0') || (errno != 0))
	{
		std::fprintf(
			stderr,
			"Argument %d (`%s') is invalid!\n",
			arg,
			argv[arg]
		);
		return false;
	}
	out = temp;
	return true;
}

int main(int argc, char* argv[])
{
	auto const is_arg_non_default {
		[argc, argv](int i)
		{
			return (argc > i) && (std::strcmp(argv[i], "-") != 0);
		}
	};

	int current_arg {1};
	auto size {default_size};
	if(is_arg_non_default(current_arg))
	{
		if(!safe_str_to_uintmax_t(current_arg, argv, size))
			return EXIT_FAILURE;
	}
	++current_arg;
	auto mode {default_mode};
	if(is_arg_non_default(current_arg))
		mode = argv[current_arg];
	++current_arg;
	if(argc > current_arg)
	{
		std::fprintf(
			stderr,
			"Too many arguments provided (%d), %d expected at most!\n",
			argc,
			current_arg
		);
		return EXIT_FAILURE;
	}

	std::vector<int> data(size);
	if(std::strcmp(mode, "ascending") == 0)
	{
		for(std::vector<int>::size_type i {}; i < data.size(); ++i)
			data[i] = i;
	}
	else if(std::strcmp(mode, "descending") == 0)
	{
		for(std::vector<int>::size_type i {}; i < data.size(); ++i)
			data[i] = data.size() - i - 1;
	}
	else if(std::strcmp(mode, "same") == 0)
	{
		for(std::vector<int>::size_type i {}; i < data.size(); ++i)
			data[i] = data.size() / 2;
	}
	else
	{
		std::fprintf(
			stderr,
			"Mode argument (`%s') is invalid!\n",
			mode
		);
		return false;
	}

	for(std::vector<int>::size_type i {}; i < data.size(); ++i)
		std::cout << data[i] << '\n';
}
