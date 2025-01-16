CC=g++
PROC_D_FLAGS=-D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
PROC_R_FLAGS=-D NDEBUG

ASS_D_FLAGS=-D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
ASS_R_FLAGS=-D NDEBUG

PROC_SOURCES = processor.cpp ass_funcs.cpp proc_funcs.cpp cmds.cpp stack.cpp color_print/color_print.cpp
ASS_SOURCES = assembler.cpp ass_funcs.cpp proc_funcs.cpp cmds.cpp stack.cpp color_print/color_print.cpp

PROC_D_OBJECTS=$(addprefix debug_object_files/,$(notdir $(PROC_SOURCES:.cpp=.o)))
PROC_R_OBJECTS=$(addprefix release_object_files/,$(notdir $(PROC_SOURCES:.cpp=.o)))
ASS_D_OBJECTS=$(addprefix debug_object_files/,$(notdir $(ASS_SOURCES:.cpp=.o)))
ASS_R_OBJECTS=$(addprefix release_object_files/,$(notdir $(ASS_SOURCES:.cpp=.o)))

PROC_D_EXE=dproc
PROC_R_EXE=proc
ASS_D_EXE=dass
ASS_R_EXE=ass

dproc: $(PROC_SOURCES) $(PROC_D_EXE)
$(PROC_D_EXE): $(PROC_D_OBJECTS)
	$(CC) $(PROC_D_FLAGS) $(PROC_D_OBJECTS) -o $@

debug_object_files/%.o: %.cpp
	$(CC) -c $(PROC_D_FLAGS) $< -o $@
debug_object_files/%.o: color_print/%.cpp
	$(CC) -c $(PROC_D_FLAGS) $< -o $@

dass: $(ASS_SOURCES) $(ASS_D_EXE)
$(ASS_D_EXE): $(ASS_D_OBJECTS)
	$(CC) $(ASS_D_FLAGS) $(ASS_D_OBJECTS) -o $@

debug_object_files/%.o: %.cpp
	$(CC) -c $(ASS_D_FLAGS) $< -o $@
debug_object_files/%.o: color_print/%.cpp
	$(CC) -c $(ASS_D_FLAGS) $< -o $@

clean:
	rm -rf *.o
	rm -rf debug_object_files/*.o
	rm -rf release_object_files/*.o

proc: $(PROC_SOURCES) $(PROC_R_EXE)
$(PROC_R_EXE): $(PROC_R_OBJECTS)
	$(CC) $(PROC_R_FLAGS) $(PROC_R_OBJECTS) -o $@

release_object_files/%.o: %.cpp
	$(CC) -c $(PROC_R_FLAGS) $< -o $@
release_object_files/%.o: color_print/%.cpp
	$(CC) -c $(PROC_R_FLAGS) $< -o $@

ass: $(ASS_SOURCES) $(ASS_R_EXE)
$(ASS_R_EXE): $(ASS_R_OBJECTS)
	$(CC) $(ASS_R_FLAGS) $(ASS_R_OBJECTS) -o $@

release_object_files/%.o: %.cpp
	$(CC) -c $(ASS_R_FLAGS) $< -o $@
release_object_files/%.o: color_print/%.cpp
	$(CC) -c $(ASS_R_FLAGS) $< -o $@
