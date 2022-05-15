# 关于Makefile的内容解释，可以参考README.md文件
# 
src := $(shell find src -name "*.cpp")
# 
build := $(src:.cpp=.o)
# 
build := $(build:src/%=build/%)
#
build/%.o : src/%.cpp
	@(echo compile $<, generate $@, path is $(dir $@))
	@(mkdir -p $(dir $@))
	g++ -c $< -o $@ -g -W
# 
debug/compiler: $(build)
	@(mkdir debug)
	@(echo depend [$^])
	@(echo link $@)
	g++ $^ -o $@ -g -W
# 
debug : debug/compiler
	@(echo compiled)
# 
run : debug
	@(cd debug && ./compiler.exe)
# 
clean : 
	(rm -rf debug build)
# 
.PHONY : pro run debug clean