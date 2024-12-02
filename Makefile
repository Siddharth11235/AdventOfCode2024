CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra


DAYS = $(wildcard day*)

define make_day
$1: $1/main.cc
	$$(CXX) $$(CXXFLAGS) -o $1/$1 $$<
endef

$(foreach day,$(DAYS),$(eval $(call make_day,$(day))))

.PHONY: all clean $(DAYS)
all: $(DAYS)

clean:
	rm -f day*/day*
