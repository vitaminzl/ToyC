# 关于Makefile的内容解释，可以参考README.md文件
# 使用shell命令find src -name "*.cpp"得到src目录下所有后缀为cpp的文件名，格式为src/*.cpp
src := $(shell find src -name "*.cpp")
# 将src变量中所有后缀为.cpp改为.o，因为编译之后的.cpp文件与.o文件名字是一样的
build := $(src:.cpp=.o)
# 将所有build变量中所有src/开头的改为build/开头
build := $(build:src/%=build/%)
# 模式匹配变量，对于src文件夹的所有.cpp文件作为依赖，附带以下命令
# $<, $@为自动化变量，$<表示规则的第一个依赖文件名，即src/*.cpp
# $@表示规则的目标文件名，即build/*.o，其中dir表示取路径名的目录，即把文件名去掉
# 命令前加@表示不回显，好像windows系统下需要加括号，不知道为什么
build/%.o : src/%.cpp
	@(echo compile $<, generate $@, path is $(dir $@))
	@(mkdir -p $(dir $@))
	g++ -c $< -o $@ -g -W
# 将所有build目录下的.o文件生成最后的可执行文件
debug/compiler: $(build)
	@(mkdir -p debug)
	@(echo depend [$^])
	@(echo link $@)
	g++ $^ -o $@ -g -W
# 输入make debug可完成以上所有任务
debug : debug/compiler
	@(echo compiled)
# 输入make run可执行文件
run : debug
	@(echo ----------- Start Running ------------)
	@(cd debug && ./compiler)
# 清除所有目标文件、可执行文件
clean : 
	(rm -rf debug/compiler.exe build)
# 伪目标，避免因为run、debug、clean存在而不执行该命令
.PHONY : run debug clean