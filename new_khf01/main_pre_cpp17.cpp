// your solution should be in problem1.hpp and problem2.hpp
#include "problem1.hpp"
#include "problem2.hpp"
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
//____________________WOODPECKER____________________
namespace woodpecker {
void usage(const char *name) {
  std::cerr << "Usage:" << std::endl;
  std::cerr << "\t" << name
            << " [-v] [-t test_no] [-f logfile] [-c] [-- other arguments]"
            << std::endl;
  std::cerr
      << "-v           Verbose output: print tests and results to stdout\n"
      << "               Default setting when no logfile is specified\n"
      << "-t test_no   Run only specific testcase\n"
      << "-f logifle   Write test results to file\n"
      << "-c           Configuration mode: print only test numbers and names\n";
}

struct woodpecker_controller {
  int test_no =
      0; /**< id of the test that will be runned, 0 to run all tests */
  int counter = 0;          /**< test counter used in macros */
  int total = 0;            /**< total points achieved */
  std::ofstream test_log;   /**< logfile to write test results */
  bool verbose = false;     /**< print logs to stderr */
  bool config_mode = false; /**< output test configs instead of running them*/
  woodpecker_controller(int argc, const char *argv[]) {
    int arg_idx = 1;
    while (arg_idx < argc) {
      if (!strcmp(argv[arg_idx], "-v")) {
        verbose = true;
      } else if (!strcmp(argv[arg_idx], "-t")) {
        test_no = std::atoi(argv[++arg_idx]);
      } else if (!strcmp(argv[arg_idx], "-f")) {
        test_log.open(argv[++arg_idx]);
      } else if (!strcmp(argv[arg_idx], "-c")) {
        config_mode = true;
      } else if (!strcmp(argv[arg_idx], "--")) {
        break;
      } else {
        usage(argv[0]);
        exit(2);
      }
      arg_idx++;
    }
    if (!test_log.is_open() && !verbose) {
      verbose = true;
    }
  }

  ~woodpecker_controller() {
    if (!config_mode) {
      *this << ">>> TOTAL POINTS: " << total << "\n";
    }
    if (test_log.is_open()) {
      test_log.close();
    }
  }

  template <typename T> woodpecker_controller &operator<<(const T &printable) {
    if (test_log.is_open())
      test_log << printable;
    if (verbose)
      std::cerr << printable;
    return *this;
  }
};

struct test_controller {
  bool enabled, run;
  std::string label;
  int points;
  bool ok;
  int &out;
  woodpecker_controller &woodpecker_cntr;
  test_controller(bool enabled, std::string &&label, int points, int &out,
                  woodpecker_controller &woodpecker_cntr)
      : enabled(enabled), run(enabled), label(std::move(label)), points(points), ok(true),
        out(out), woodpecker_cntr(woodpecker_cntr) {
    if (woodpecker_cntr.config_mode) {
      this->enabled = false;
      this->run = false;
      // Output timeout, memory limit, etc.
      // Just the name and points for now.
      woodpecker_cntr << this->points << ";" << this->label << "\n";
    }
    if (this->enabled) {
      woodpecker_cntr << ">>> Test: " << this->label << " (" << points << ")\n";
    }
  }
  ~test_controller() {
    if (enabled) {
      out += ok ? points : 0;
      woodpecker_cntr << ">>> Result: " << (ok ? "PASS" : "FAIL") << "\n";
    }
  }
};

struct measurement_controller {
  std::string name;
  std::chrono::nanoseconds &out;
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  bool run;
  woodpecker_controller &woodpecker_cntr;
  measurement_controller(std::string &&name, std::chrono::nanoseconds &out,
                         woodpecker_controller &woodpecker_cntr)
      : name(std::move(name)), out(out),
        start(std::chrono::high_resolution_clock::now()), run(true),
        woodpecker_cntr(woodpecker_cntr) {}
  ~measurement_controller() {
    auto stop = std::chrono::high_resolution_clock::now();
    out = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    woodpecker_cntr << name << ": " << out.count() << "ns\n";
  }
};

} // end namespace woodpecker

#define TEST(label, points)                                                    \
  for (woodpecker::test_controller cntr(                                       \
          ++woodpecker_cntr.counter == woodpecker_cntr.test_no ||              \
              woodpecker_cntr.test_no == 0,                                    \
          label, points, woodpecker_cntr.total, woodpecker_cntr);              \
      cntr.run; cntr.run = false)

#define CHECK_EQ(val, expr)                                                    \
  if (cntr.ok) {                                                               \
    try {                                                                      \
      cntr.ok = val == (expr);                                                 \
    } catch (...) {                                                            \
      cntr.ok = false;                                                         \
      woodpecker_cntr << ">>> unexpected exception\n";                         \
    }                                                                          \
    if (!cntr.ok) {                                                            \
      woodpecker_cntr << ">>> " << #val << " == " << #expr << " failed at "    \
                      << __LINE__ << " Expr result: " << (expr) << "\n";       \
    }                                                                          \
  }

#define CHECK_EXC(exc, code)                                                   \
  if (cntr.ok) {                                                               \
    try {                                                                      \
      code;                                                                    \
      cntr.ok = false;                                                         \
      woodpecker_cntr << ">>> expected exception\n";                           \
    } catch (exc) {                                                            \
    } catch (...) {                                                            \
      woodpecker_cntr << ">>> unexpected exception\n";                         \
      cntr.ok = false;                                                         \
    }                                                                          \
    if (!cntr.ok) {                                                            \
      woodpecker_cntr << #code << " failed at" << __LINE__ << "\n";            \
    }                                                                          \
  }

#define MEASURE(label, var)                                                    \
  std::chrono::nanoseconds var;                                                \
  for (woodpecker::measurement_controller mcntr(label, var, woodpecker_cntr);  \
      mcntr.run; mcntr.run = false)

#define WOODPECKER_INIT                                                        \
  woodpecker::woodpecker_controller woodpecker_cntr(argc, argv)

//____________________WOODPECKER_END____________________

class TesterGridLocator : public GridLocator {
  mutable size_t tests_left;
  const int sol_x, sol_y;

public:
  TesterGridLocator(const size_t &N, const size_t &K, const int &x,
                    const int &y)
      : GridLocator(N, K), tests_left(K), sol_x(x), sol_y(y) {}

  direction_t check_compass(int x, int y) const override {
    if (tests_left == 0)
      return direction_t::CENTER;
    tests_left--;
    if (x < sol_x) {
      if (y < sol_y)
        return direction_t::SE;
      if (y > sol_y)
        return direction_t::NE;
      return direction_t::E;
    } else if (x > sol_x) {
      if (y < sol_y)
        return direction_t::SW;
      if (y > sol_y)
        return direction_t::NW;
      return direction_t::W;
    }
    if (y < sol_y)
      return direction_t::S;
    if (y > sol_y)
      return direction_t::N;
    return direction_t::CENTER;
  }
};

//Helper function for random generation
competitor_t gen_competitor(std::uniform_int_distribution<> &coord_rand,
                        std::uniform_int_distribution<> &dir_rand,
                        std::mt19937 &gen, const int size) {
  int x = coord_rand(gen);
  int y = coord_rand(gen);
  competitor_t::direction_t dir = competitor_t::direction_t(dir_rand(gen));
  if (x == 0 && dir == competitor_t::W)
    dir = competitor_t::E;
  if (x == size - 1 && dir == competitor_t::E)
    dir = competitor_t::W;
  if (y == 0 && dir == competitor_t::N)
    dir = competitor_t::S;
  if (y == size - 1 && dir == competitor_t::S)
    dir = competitor_t::N;
  return {x, y, dir};
}

int main(int argc, const char *argv[]) {
  WOODPECKER_INIT;
  // TASK 1
  TEST("Find small", 1) {
    size_t N = 2;
    size_t K = 4;
    TesterGridLocator locator(N, K, 0, 0);
    std::pair<int, int> guess = locator.search();
    CHECK_EQ(0, guess.first);
    CHECK_EQ(0, guess.second);
  }
  TEST("Find it faster", 1) {
    size_t N = 2;
    size_t K = 2;
    TesterGridLocator locator(N, K, 0, 0);
    std::pair<int, int> guess = locator.search();
    CHECK_EQ(0, guess.first);
    CHECK_EQ(0, guess.second);
  }
  TEST("Bottom right", 1) {
    size_t N = 5;
    size_t K = 4;
    TesterGridLocator locator(N, K, 4, 4);
    std::pair<int, int> guess = locator.search();
    CHECK_EQ(4, guess.first);
    CHECK_EQ(4, guess.second);
  }
  TEST("Middle", 1) {
    const size_t N = 128, K = 16;
    TesterGridLocator locator(N, K, 63, 65);
    std::pair<int, int> guess = locator.search();
    CHECK_EQ(63, guess.first);
    CHECK_EQ(65, guess.second);
  }
  TEST("Middle, but x is faster", 1) {
    const size_t N = 128, K = 16;
    TesterGridLocator locator(N, K, 32, 65);
    std::pair<int, int> guess = locator.search();
    CHECK_EQ(32, guess.first);
    CHECK_EQ(65, guess.second);
  }
  TEST("Big", 1) {
    const size_t N = 500000, K = 25;
    TesterGridLocator locator(N, K, 128128, 424242);
    std::pair<int, int> guess = locator.search();
    CHECK_EQ(128128, guess.first);
    CHECK_EQ(424242, guess.second);
  }
  TEST("Pi, 42", 1) {
    const size_t N = 100000000, K = 27;
    TesterGridLocator locator(N, K, 31415926, 42424242);
    std::pair<int, int> guess = locator.search();
    CHECK_EQ(31415926, guess.first);
    CHECK_EQ(42424242, guess.second);
  }
  // TASK 2
  TEST("Go north", 1) {
    size_t N = 10;
    competitor_t competitors[] = {{5, 5, competitor_t::S}};
    std::pair<int, int> sol = search(competitors, 1, N);
    CHECK_EQ(0, sol.first);
    CHECK_EQ(6, sol.second);
  }

  TEST("Middle points", 1) {
    size_t N = 10;
    competitor_t c2[] = {{2, 4, competitor_t::S},
                       {2, 6, competitor_t::N},
                       {1, 5, competitor_t::E},
                       {3, 5, competitor_t::W}};
    std::pair<int, int> sol = search(c2, 4, N);
    CHECK_EQ(2, sol.first);
    CHECK_EQ(5, sol.second);
  }

  TEST("Test SN", 1) {
    size_t N = 10;
    competitor_t c3[] = {{0, 2, competitor_t::N}, {0, 3, competitor_t::S},
                       {0, 3, competitor_t::S}, {0, 4, competitor_t::S},
                       {0, 5, competitor_t::N}, {0, 5, competitor_t::N},
                       {0, 8, competitor_t::N}, {1, 5, competitor_t::W}};
    std::pair<int, int> sol = search(c3, 8, N);
    CHECK_EQ(0, sol.first);
    CHECK_EQ(4, sol.second);
  }

  TEST("One point", 1) {
    size_t N = 10;
    competitor_t c2[] = {{4, 4, competitor_t::S},
                       {4, 4, competitor_t::N},
                       {4, 4, competitor_t::E},
                       {4, 4, competitor_t::W}};
    std::pair<int, int> sol = search(c2, 4, N);
    CHECK_EQ(0, sol.first);
    CHECK_EQ(0, sol.second);
  }

  std::mt19937 gen;
  TEST("Random test", 1) {
    gen.seed(42);
    const size_t N = 100;
    std::uniform_int_distribution<> rand(0, N - 1);
    std::uniform_int_distribution<> randsmall(0, 3);
    constexpr size_t N_c = 500;
    competitor_t competitors[N_c];
    for (competitor_t &c : competitors) {
      c = gen_competitor(rand, randsmall, gen, N);
    }

    auto sol = search(competitors, N_c, N);
    CHECK_EQ(92, sol.first);
    CHECK_EQ(1, sol.second);
    for (competitor_t &c : competitors) {
      c = gen_competitor(rand, randsmall, gen, N);
    }

    sol = search(competitors, N_c, N);
    CHECK_EQ(94, sol.first);
    CHECK_EQ(85, sol.second);
  }

  TEST("Big test 1", 1) {
    gen.seed(24);
    size_t N = 100000000;
    constexpr size_t N_c = 500;
    std::uniform_int_distribution<> rand(0, N - 1);
    std::uniform_int_distribution<> randsmall(0, 3);
    competitor_t competitors[N_c];
    for (competitor_t &c : competitors) {
      c = gen_competitor(rand, randsmall, gen, N);
    }

    auto sol = search(competitors, N_c, N);
    CHECK_EQ(90619194, sol.first);
    CHECK_EQ(3537882, sol.second);
  }

  TEST("Big test 2", 1) {
    gen.seed(42);
    size_t N = 100000000;
    constexpr size_t N_c = 500;
    std::uniform_int_distribution<> rand(0, N - 1);
    std::uniform_int_distribution<> randsmall(0, 3);
    competitor_t competitors[N_c];
    for (competitor_t &c : competitors) {
      c = gen_competitor(rand, randsmall, gen, N);
    }

    auto sol = search(competitors, N_c, N);
    CHECK_EQ(99145932, sol.first);
    CHECK_EQ(7200834, sol.second);
  }

  return 0;
}
