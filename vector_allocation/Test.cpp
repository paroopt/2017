#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
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

struct my_type {};

template<typename T>
void run_test(
	volatile unsigned int& prevent_optimizing_too_much,
	std::uintmax_t reserve,
	std::uintmax_t appends
)
{
	std::vector<T> vec {};
	vec.reserve(reserve);
	for (decltype(appends) append {}; append < appends; ++append)
	{
		vec.emplace_back();
		++prevent_optimizing_too_much;
	}
}

using run_test_function_type = decltype(&run_test<my_type>);

constexpr std::uintmax_t default_iterations {1};
constexpr std::uintmax_t default_reserve {};
constexpr std::uintmax_t default_appends {};
constexpr run_test_function_type default_run_test_function {&run_test<my_type>};

constexpr char iterations_header[] {"iterations"};
constexpr char reserve_header[] {"reserve"};
constexpr char appends_header[] {"appends"};
constexpr char type_header[] {"type"};
constexpr char time_header[] {"time [ms]"};
constexpr char fake_result_header[] {"fake result"};
constexpr int max_header_length
	{std::max({
		sizeof(iterations_header),
		sizeof(reserve_header),
		sizeof(appends_header),
		sizeof(type_header),
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
	auto reserve {default_reserve};
	if(is_arg_non_default(current_arg))
	{
		if(!safe_str_to_uintmax_t(current_arg, argv, reserve))
			return EXIT_FAILURE;
	}
	++current_arg;
	auto appends {default_appends};
	if(is_arg_non_default(current_arg))
	{
		if(!safe_str_to_uintmax_t(current_arg, argv, appends))
			return EXIT_FAILURE;
	}
	++current_arg;
	auto run_test_function {default_run_test_function};
	char const* type_text = "my_type";
	if(is_arg_non_default(current_arg))
	{
#define IF(x) if(std::strcmp(argv[current_arg], #x) == 0) ( run_test_function = &run_test<x> ), ( type_text = argv[current_arg] )
		IF(my_type);
		else IF(bool);
		else IF(char);
		else IF(int);
		else IF(double);
		else
		{
			std::fprintf(
				stderr,
				"Argument %d (`%s') is invalid!\n",
				current_arg,
				argv[current_arg]
			);
			return EXIT_FAILURE;
		}
#undef IF
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

	volatile unsigned int prevent_optimizing_too_much {};
	clock_type::duration min_duration {};
	for(decltype(iterations) iteration {}; iteration < iterations; ++iteration)
	{
		auto const start_time {clock_type::now()};

		(*run_test_function)(
			prevent_optimizing_too_much,
			reserve,
			appends
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
		"%*s\t%ju\n%*s\t%ju\n%*s\t%ju\n%*s\t%s\n%*s\t%jd\n%*s\t%u\n",
		max_header_length, iterations_header, iterations,
		max_header_length, reserve_header, reserve,
		max_header_length, appends_header, appends,
		max_header_length, type_header, type_text,
		max_header_length, time_header, milliseconds,
		max_header_length, fake_result_header, prevent_optimizing_too_much
	);
}
