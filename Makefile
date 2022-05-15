# 关于Makefile的内容解释，可以参考README.md文件
# 
src := $(shell find src -name "*.cpp")
# 
objs := $(src:.cpp=.o)
# 
objs := $(objs:src/%=objs/%)
#
objs/%.o : src/%.cpp
	@(echo compile $<, generate $@, path is $(dir $@))
	@(mkdir -p $(dir $@))
	g++ -c $< -o $@
# 
workspace/pro : $(objs)
	@(echo 这里的依赖项所有是[$^])
	@(echo 链接$@)
	g++ $^ -o $@
# 
pro : workspace/pro
	@(echo 编译完成)
# 
rum : pro
	@(cd workspace && ./pro)
# 
clean : 
	rm -rf workspace/pro objs
# 
.PHONY : pro run debug clean
# 
debug :
	@(echo objs is[$(objs)])