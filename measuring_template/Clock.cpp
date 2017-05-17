#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <ratio>

void run_test(
	volatile unsigned int& prevent_optimizing_too_much
	/* possibly other parameters arguments */
)
{
	/* actual test */
	++prevent_optimizing_too_much;
}

constexpr std::uintmax_t default_iterations {1};
/* possibly other parameters defaults */

constexpr char iterations_header[] {"iterations"};
/* possibly other parameters headers */
constexpr char time_header[] {"time [ms]"};
constexpr char fake_result_header[] {"fake result"};
constexpr int max_header_length
	{std::max({
		sizeof(iterations_header),
		/* possibly other parameters header sizes */
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
	/* possibly other parameters setup */
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

	using duration_type =
		std::chrono::duration<std::clock_t, std::ratio<1, CLOCKS_PER_SEC>>;

	volatile unsigned int prevent_optimizing_too_much {};
	duration_type min_duration {};
	for(decltype(iterations) iteration {}; iteration < iterations; ++iteration)
	{
		duration_type const start_time {std::clock()};
		if(start_time.count() == static_cast<std::clock_t>(-1))
		{
			std::fprintf(stderr, "std::clock failed!\n");
			return EXIT_FAILURE;
		}

		run_test(
			prevent_optimizing_too_much
			/* possibly other parameters */
		);

		duration_type const end_time {std::clock()};
		if(end_time.count() == static_cast<std::clock_t>(-1))
		{
			std::fprintf(stderr, "std::clock failed!\n");
			return EXIT_FAILURE;
		}

		auto const test_duration {end_time - start_time};
		if((iteration == 1) || (test_duration < min_duration))
			min_duration = test_duration;
	}

	auto const min_duration_in_milliseconds
		{std::chrono::duration_cast<std::chrono::milliseconds>(min_duration)};
	std::intmax_t const milliseconds {min_duration_in_milliseconds.count()};
	std::printf(
		"%*s\t%ju\n%*s\t%ju\n%*s\t%u\n",
		max_header_length, iterations_header, iterations,
		/* possibly other parameters prints */
		max_header_length, time_header, milliseconds,
		max_header_length, fake_result_header, prevent_optimizing_too_much
	);
}
