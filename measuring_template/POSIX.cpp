#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>

void run_test(
	volatile unsigned int& prevent_optimizing_too_much
	/* possibly other parameters arguments */
)
{
	/* actual test */
	++prevent_optimizing_too_much;
}

constexpr std::uintmax_t default_iterations {1};
constexpr clockid_t default_clock {CLOCK_MONOTONIC};
/* possibly other parameters defaults */

constexpr char iterations_header[] {"iterations"};
constexpr char clock_header[] {"clock"};
/* possibly other parameters headers */
constexpr char time_header[] {"time [ms]"};
constexpr char fake_result_header[] {"fake result"};
constexpr int max_header_length
	{std::max({
		sizeof(iterations_header),
		sizeof(clock_header),
		/* possibly other parameters header sizes */
		sizeof(time_header),
		sizeof(fake_result_header)
	}) - 1};

clockid_t clock_getcpuclockid_value {};

char const* get_clock_name(clockid_t clock)
{
	switch(clock)
	{
#define CASE(x) case x: return #x
	CASE(CLOCK_MONOTONIC);
	CASE(CLOCK_PROCESS_CPUTIME_ID);
	CASE(CLOCK_REALTIME);
	CASE(CLOCK_THREAD_CPUTIME_ID);
	CASE(CLOCK_MONOTONIC_RAW);
#undef CASE
	}

	if(clock == clock_getcpuclockid_value)
		return "clock_getcpuclockid";

	return "unknown clock";
}

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
	if(clock_getcpuclockid(0, &clock_getcpuclockid_value) != 0)
	{
		int const errno_code = errno;
		std::fprintf(
			stderr,
			"clock_getcpuclockid failed, errno %d (%s)!\n",
			errno_code,
			strerror(errno_code)
		);
		return EXIT_FAILURE;
	}

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
	auto clock {default_clock};
	if(is_arg_non_default(current_arg))
	{
#define IF(x) if(std::strcmp(argv[current_arg], #x) == 0) clock = x
		IF(CLOCK_MONOTONIC);
		else IF(CLOCK_PROCESS_CPUTIME_ID);
		else IF(CLOCK_REALTIME);
		else IF(CLOCK_THREAD_CPUTIME_ID);
		else IF(CLOCK_MONOTONIC_RAW);
		else if(std::strcmp(argv[current_arg], "clock_getcpuclockid") == 0)
			clock = clock_getcpuclockid_value;
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

	volatile unsigned int prevent_optimizing_too_much {};
	std::chrono::nanoseconds min_duration {};
	for(decltype(iterations) iteration {}; iteration < iterations; ++iteration)
	{
		timespec start_time_raw {};
		if(clock_gettime(clock, &start_time_raw) != 0)
		{
			int const errno_code = errno;
			std::fprintf(
				stderr,
				"clock_gettime failed for clock %d (%s), errno %d (%s)!\n",
				clock,
				get_clock_name(clock),
				errno_code,
				strerror(errno_code)
			);
			return EXIT_FAILURE;
		}
		auto const start_time
			{std::chrono::seconds(start_time_raw.tv_sec)
				+ std::chrono::nanoseconds(start_time_raw.tv_nsec)};

		run_test(
			prevent_optimizing_too_much
			/* possibly other parameters */
		);

		timespec end_time_raw {};
		if(clock_gettime(clock, &end_time_raw) != 0)
		{
			int const errno_code = errno;
			std::fprintf(
				stderr,
				"clock_gettime failed for clock %d (%s), errno %d (%s)!\n",
				clock,
				get_clock_name(clock),
				errno_code,
				strerror(errno_code)
			);
			return EXIT_FAILURE;
		}
		auto const end_time
			{std::chrono::seconds(end_time_raw.tv_sec)
				+ std::chrono::nanoseconds(end_time_raw.tv_nsec)};

		auto const test_duration {end_time - start_time};
		if((iteration == 1) || (test_duration < min_duration))
			min_duration = test_duration;
	}

	auto const min_duration_in_milliseconds
		{std::chrono::duration_cast<std::chrono::milliseconds>(min_duration)};
	std::intmax_t const milliseconds {min_duration_in_milliseconds.count()};
	std::printf(
		"%*s\t%ju\n%*s\t%d (%s)\n%*s\t%jd\n%*s\t%u\n",
		max_header_length, iterations_header, iterations,
		max_header_length, clock_header, clock, get_clock_name(clock),
		/* possibly other parameters prints */
		max_header_length, time_header, milliseconds,
		max_header_length, fake_result_header, prevent_optimizing_too_much
	);
}
