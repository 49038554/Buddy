#makefile文件编译指令
#如果make文件名是makefile，直接使用make就可以编译
#如果make文件名不是makefile，比如test.txt，那么使用make -f test.txt

#------------------------------------------检查操作系统32位64位--------------------------------------------------------
#SYS_BIT=$(shell getconf LONG_BIT)
#SYS_BIT=$(shell getconf WORD_BIT)
SYS_BIT=$(shell getconf LONG_BIT)
ifeq ($(SYS_BIT),32)
	CPU =  -march=i686 
else 
	CPU = 
endif

#------------------------------------------编辑器--------------------------------------------------------

#c++编译工具
CC = g++ 
#apache模块编译安装工具
APXS = apxs 

#------------------------------------------编辑器End--------------------------------------------------------

#------------------------------------------目录--------------------------------------------------------
#常用目录
#定义一些常用目录的宏，后面可以使用
#※末尾不能有空格
#范例：
#ROOT_DIR = ../../../..
ROOT_DIR = ..
SDK_DIR = ../Server/SDK

#依赖文件全局查找目录
VPATH = $(OBJ_OUTPUT_DIR) $(SRC_DIR) 
SRC_DIR = 

#源文件目录
#每个目录定义一个宏，全部添加到SRC_DIR中
#※末尾不能有空格
#范例：
#Main_Dir = $(ROOT_DIR)/main
Protocl_Dir = $(ROOT_DIR)/Protocl/cpp
Protocl1_Dir = $(ROOT_DIR)/Protocl/cpp/base
Protocl2_Dir = $(ROOT_DIR)/Protocl/cpp/Object/
Protocl3_Dir = $(ROOT_DIR)/Protocl/cpp/Object/Auth
Protocl4_Dir = $(ROOT_DIR)/Protocl/cpp/Object/Mall
Protocl5_Dir = $(ROOT_DIR)/Protocl/cpp/Object/Room
Protocl6_Dir = $(ROOT_DIR)/Protocl/cpp/Object/Operation
Protocl7_Dir = $(ROOT_DIR)/Protocl/cpp/Object/Database
Protocl8_Dir = $(ROOT_DIR)/Protocl/cpp/Object/Notify
Protocl9_Dir = $(ROOT_DIR)/Protocl/cpp/Object/Sort


#输出目录
OBJ_OUTPUT_DIR=./output
OUTPUT_DIR=$(ROOT_DIR)/Server/SDK/lib
$(shell mkdir $(OBJ_OUTPUT_DIR))
$(shell mkdir $(OUTPUT_DIR))



#------------------------------------------目录End--------------------------------------------------------

#------------------------------------------编译选项--------------------------------------------------------

#编译选项
CFLAGS= -O -g -fPIC -Wall -D_REENTRANT -DNO_STRING_CIPHER $(CPU) 

#警告级别
WARNING_LEVEL += -O3 

#头文件目录：-I 目录
INCLUDE = -I. -I../include 
#按照以下格式添加
#INCLUDE += -I目录+空格 
INCLUDE += -I$(ROOT_DIR) 
INCLUDE += -I$(SDK_DIR) 


#库目录，库文件:-L 目录 -库名
#LIB = -lnsl -lc -lm -lpthread -lstdc++ -lrt 

#静态库：.a文件路径名
#按照以下格式添加
#LIB += .a文件路径名+空格

#------------------------------------------编译选项End--------------------------------------------------------


#------------------------------------------输出--------------------------------------------------------
#编译产生的程序文件名
OUTPUT = Protocl.a

#目标文件
#范例
#目标文件列表 = $(notdir $(patsubst %.cpp,%.o,$(wildcard [cpp源文件目录]/*.cpp))) 
#cpp分布在多少个目录中,就定义多少个[目标文件列表]
#范例：
#OBJ列表1 = $(notdir $(patsubst %.cpp,%.o,$(wildcard mdk目录/source/mdk/*.cpp))) 
#OBJ列表2 = $(notdir $(patsubst %.cpp,%.o,$(wildcard mdk目录/source/frame/netserver/*.cpp))) 
#OBJ列表2 = $(notdir $(patsubst %.cpp,%.o,$(wildcard mdk目录/source/frame/netserver/*.cpp))) 
Protocl_OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(Protocl_Dir)/*.cpp))) 
Protocl1_OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(Protocl1_Dir)/*.cpp))) 
Protocl2_OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(Protocl2_Dir)/*.cpp))) 
Protocl3_OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(Protocl3_Dir)/*.cpp))) 
Protocl4_OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(Protocl4_Dir)/*.cpp))) 
Protocl5_OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(Protocl5_Dir)/*.cpp))) 
Protocl6_OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(Protocl6_Dir)/*.cpp))) 
Protocl7_OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(Protocl7_Dir)/*.cpp))) 
Protocl8_OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(Protocl8_Dir)/*.cpp))) 
Protocl9_OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(Protocl9_Dir)/*.cpp))) 


#依赖的目标文件
DEPENDENCE = 
#将目标文件中定义的列表宏全部添加到DEPENDENCE中，每个项目都并以空格结束
#范例：
#DEPENDENCE += $(OBJ列表1) 
#DEPENDENCE += $(OBJ列表2) 
DEPENDENCE += $(Protocl_OBJ) 
DEPENDENCE += $(Protocl1_OBJ) 
DEPENDENCE += $(Protocl2_OBJ) 
DEPENDENCE += $(Protocl3_OBJ) 
DEPENDENCE += $(Protocl4_OBJ) 
DEPENDENCE += $(Protocl5_OBJ) 
DEPENDENCE += $(Protocl6_OBJ) 
DEPENDENCE += $(Protocl7_OBJ) 
DEPENDENCE += $(Protocl8_OBJ) 
DEPENDENCE += $(Protocl9_OBJ) 

#连接项目，需要的所有其它源文件或目标文件(带目录)
OBJ = 
#将目标文件中定义的列表宏，按照如下格式的指令，全部添加到OBJ中，每个项目都以空格结束
#指令：$(addprefix $(OBJ_OUTPUT_DIR)/, $(OBJ列表)) 
#范例：
#OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(OBJ列表1)) 
#OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(OBJ列表2)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(Protocl_OBJ)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(Protocl1_OBJ)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(Protocl2_OBJ)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(Protocl3_OBJ)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(Protocl4_OBJ)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(Protocl5_OBJ)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(Protocl6_OBJ)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(Protocl7_OBJ)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(Protocl8_OBJ)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(Protocl9_OBJ)) 

#------------------------------------------输出End--------------------------------------------------------

#-------------------------------------------编译指令-----------------------------------------------------
#生成EXE
#范例：
#$(OUTPUT_DIR)/$(OUTPUT): $(DEPENDENCE)										依赖关系
#	@echo "Complie (OUTPUT_DIR)/$(OUTPUT)"
#	$(CC) -o $@ $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE)$(OBJ)$(LIB)		gcc编译指令
#	@echo ""
#	@echo "$(OUTPUT_DIR)/$(OUTPUT) complie finished"
#	@echo ""
#	@echo ""
#	@echo ""
#	@echo ""



#-----------------------------------------编译生成.A静态库---------------------------------------------------
#$(OUTPUT_DIR)/$(OUTPUT):$(DEPENDENCE)
#	@echo "Complie $(OUTPUT_DIR)/$(OUTPUT)"
#	@echo ""
#	ar -r $@ $(OBJ)
#	@echo ""
#	@echo "$(OUTPUT_DIR)/$(OUTPUT) complie finished"
#	@echo ""
#	@echo ""
#	@echo ""
#	@echo ""

$(OUTPUT_DIR)/$(OUTPUT):$(DEPENDENCE)
	@echo "Complie $(OUTPUT_DIR)/$(OUTPUT)"
	@echo ""
	ar -r $@ $(OBJ)
	@echo ""
	@echo "$(OUTPUT_DIR)/$(OUTPUT) complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

#-----------------------------------------编译生成SO动态库---------------------------------------------------
#范例：
#$(OUTPUT_DIR)/$(OUTPUT): $(DEPENDENCE)											依赖关系
#	@echo "Complie $(OUTPUT_DIR)/$(OUTPUT)"
#	$(CC) -o $@ -shared $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE)$(OBJ)$(LIB)		gcc编译指令
#	@echo ""
#	@echo "$(OUTPUT_DIR)/$(OUTPUT) complie finished"
#	@echo ""
#	@echo ""
#	@echo ""
#	@echo ""


#------------------------------------------编译Object----------------------------------------------------
#范例：单个cpp编译为object编译
#$(OBJ_OUTPUT_DIR)/GameSerFrm.o: main/GameSerFrm.cpp main/GameSerFrm.h main/GameSerCPU.h com/ComDef.h com/XXSocket.h tool/DBTool.h	依赖关系
#	@echo "Complie GameSerFrm.o"
#	$(CC) -c -o $*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE)main/GameSerFrm.cpp											gcc编译指令
#	@echo ""
#	@echo "$(OBJ_OUTPUT_DIR)/GameSerFrm.o complie finished"
#	@echo ""
#	@echo ""

#范例：目录批量编译，所有cpp目录下的cpp生成object
#$(OBJ列表1):%.o:[OBJ列表1的cpp源文件目录]/%.cpp [OBJ列表1的cpp源文件目录]/%.h
#	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
#	@echo ""
#	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) [OBJ列表1的cpp源文件目录]/$*.cpp
#	@echo ""
#	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
#	@echo ""
#	@echo ""
#	@echo ""
#	@echo ""

#$(OBJ列表2):%.o:[OBJ列表2的cpp源文件目录]/%.cpp [OBJ列表2的cpp源文件目录]/%.h
#	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
#	@echo ""
#	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) [OBJ列表2的cpp源文件目录]/$*.cpp
#	@echo ""
#	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
#	@echo ""
#	@echo ""
#	@echo ""
#	@echo ""

$(Protocl_OBJ):%.o:$(Protocl_Dir)/%.cpp $(Protocl_Dir)/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(shell mkdir $(OUTPUT_DIR))
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $(Protocl_Dir)/$*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

$(Protocl1_OBJ):%.o:$(Protocl1_Dir)/%.cpp $(Protocl1_Dir)/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(shell mkdir $(OUTPUT_DIR))
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $(Protocl1_Dir)/$*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

$(Protocl2_OBJ):%.o:$(Protocl2_Dir)/%.cpp $(Protocl2_Dir)/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(shell mkdir $(OUTPUT_DIR))
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $(Protocl2_Dir)/$*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

$(Protocl3_OBJ):%.o:$(Protocl3_Dir)/%.cpp $(Protocl3_Dir)/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(shell mkdir $(OUTPUT_DIR))
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $(Protocl3_Dir)/$*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

$(Protocl4_OBJ):%.o:$(Protocl4_Dir)/%.cpp $(Protocl4_Dir)/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(shell mkdir $(OUTPUT_DIR))
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $(Protocl4_Dir)/$*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

$(Protocl5_OBJ):%.o:$(Protocl5_Dir)/%.cpp $(Protocl5_Dir)/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(shell mkdir $(OUTPUT_DIR))
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $(Protocl5_Dir)/$*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

$(Protocl6_OBJ):%.o:$(Protocl6_Dir)/%.cpp $(Protocl6_Dir)/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(shell mkdir $(OUTPUT_DIR))
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $(Protocl6_Dir)/$*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

$(Protocl7_OBJ):%.o:$(Protocl7_Dir)/%.cpp $(Protocl7_Dir)/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(shell mkdir $(OUTPUT_DIR))
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $(Protocl7_Dir)/$*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

$(Protocl8_OBJ):%.o:$(Protocl8_Dir)/%.cpp $(Protocl8_Dir)/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(shell mkdir $(OUTPUT_DIR))
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $(Protocl8_Dir)/$*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

$(Protocl9_OBJ):%.o:$(Protocl9_Dir)/%.cpp $(Protocl9_Dir)/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(shell mkdir $(OUTPUT_DIR))
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $(Protocl9_Dir)/$*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""



install:
#------------------------------------------安装----------------------------------------------------
#------------------------------------------安装apache模块----------------------------------------------------
#范例：apxs -a -i -n 模块名 so文件名
#apxs -a -i -n test $(OBJ_OUTPUT_DIR)/$(OUTPUT)
	apxs -a -i -n pfadurl $(OBJ_OUTPUT_DIR)/$(OUTPUT)





#------------------------------------------清理重新编译----------------------------------------------------
clean:
	-rm -f $(OUTPUT_DIR)/$(OUTPUT) $(OBJ_OUTPUT_DIR)/*.o
	
.PHONY: clean


#------------------------------------------apache控制----------------------------------------------------

start:
	$(APACHE_BIN)/apachectl start

stop:
	$(APACHE_BIN)/apachectl stop

ps:
	ps -ae|grep http
