#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#ifndef CLOCK_TYPE
#define CLOCK_TYPE std::chrono::high_resolution_clock
#endif
using clock_type = CLOCK_TYPE;
#ifdef ASSERT_IS_STEADY
#define STRINGIFY_EVAL(x) #x
#define STRINGIFY(x) STRINGIFY_EVAL(x)
static_assert(clock_type::is_steady, "clock_type (" STRINGIFY(CLOCK_TYPE) ") is not steady!");
#undef STRINGIFY
#undef STRINGIFY_EVAL
#endif

void run_test(
	volatile unsigned int& prevent_optimizing_too_much,
	std::vector<int> data
)
{
	std::sort(data.begin(), data.end());
	data.erase(std::unique(data.begin(), data.end()), data.end());
	prevent_optimizing_too_much += data.size();

}

constexpr std::uintmax_t default_iterations {1};

constexpr char iterations_header[] {"iterations"};
constexpr char data_size_header[] {"data size"};
constexpr char time_header[] {"time [ms]"};
constexpr char fake_result_header[] {"fake result"};
constexpr int max_header_length
	{std::max({
		sizeof(iterations_header),
		sizeof(data_size_header),
		sizeof(time_header),
		sizeof(fake_result_header)
	}) - 1};

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
	auto iterations {default_iterations};
	if(is_arg_non_default(current_arg))
	{
		if(!safe_str_to_uintmax_t(current_arg, argv, iterations))
			return EXIT_FAILURE;
	}
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

	std::vector<int> data;
	do
	{
		int value;
		std::cin >> value;
		if(std::cin)
			data.emplace_back(value);
	}
	while(std::cin);

	volatile unsigned int prevent_optimizing_too_much {};
	clock_type::duration min_duration {};
	for(decltype(iterations) iteration {}; iteration < iterations; ++iteration)
	{
		auto const start_time {clock_type::now()};

		run_test(
			prevent_optimizing_too_much,
			data
		);

		auto const end_time {clock_type::now()};

		auto const test_duration {end_time - start_time};
		if((iteration == 1) || (test_duration < min_duration))
			min_duration = test_duration;
	}

	auto const min_duration_in_milliseconds
		{std::chrono::duration_cast<std::chrono::milliseconds>(min_duration)};
	std::intmax_t const milliseconds {min_duration_in_milliseconds.count()};
	std::printf(
		"%*s\t%ju\n%*s\t%zu\n%*s\t%ju\n%*s\t%u\n",
		max_header_length, iterations_header, iterations,
		max_header_length, data_size_header, data.size(),
		max_header_length, time_header, milliseconds,
		max_header_length, fake_result_header, prevent_optimizing_too_much
	);
}
