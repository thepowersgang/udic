CXXFLAGS := -Wall -Werror -std=c++11 -O3
CPPFLAGS := -I include/
LINKFLAGS := -O3
LIBS :=

OBJ := main.o ast.o types.o
OBJ += parse/common.o
OBJ += parse/lex.o parse/preproc.o parse/token.o
OBJ += parse/core.o parse/expr.o parse/types.o

BIN := ../udic

OBJ := $(OBJ:%=.obj/%)

$(BIN): $(OBJ)
	@echo [CXX] $@
	@$(CXX) $(LINKFLAGS) -o $(BIN) $(OBJ) $(LIBS)

.obj/%.o: %.cpp
	@echo [CXX] $@
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $< -MMD -MP

-include $(OBJ:%.o=%.d)

