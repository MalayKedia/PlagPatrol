CXX = clang++
CXXWARNINGS = -Wall -Wshadow -Wuninitialized -Wtype-limits -Wno-unused-variable
# CXXFLAGS = -std=gnu++20 -g -fsanitize=address,undefined $(CXXWARNINGS)
CXXFLAGS = -std=gnu++20 -g $(CXXWARNINGS)
INCLUDEFLAGS = -I/usr/lib/llvm-18/include
LDFLAGS = -lclang -L/usr/lib/llvm-18/lib -rpath /usr/lib/llvm-18/lib -fuse-ld=lld

CHECKERS = checker_zero checker_one checker_two checker_three checker_four checker_five

run: clean all

all: $(CHECKERS)
	@echo "All checkers run successfully."

$(CHECKERS):
	@echo "Building and running $@\n"
	@echo '#include "$@.hpp"' > temp_main.cpp
	@cat body.cpp >> temp_main.cpp
	@$(CXX) $(CXXFLAGS) $(INCLUDEFLAGS) $(LDFLAGS) temp_main.cpp tokenizer.cpp -o temp_main
	@./temp_main
	@rm temp_main temp_main.cpp

clean:
	@rm -rf temp_main temp_main.cpp *.dSYM

.PHONY: all $(CHECKERS) clean