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
ROOT_DIR = ../..
SDK_DIR = $(ROOT_DIR)/SDK
Protocl_DIR = $(ROOT_DIR)/..
MDK_DIR = $(SDK_DIR)/Micro-Development-Kit
MYSQL_DIR = $(SDK_DIR)/mysql

#依赖文件全局查找目录
VPATH = $(OBJ_OUTPUT_DIR) $(SRC_DIR) 
SRC_DIR = 

#源文件目录
#每个目录定义一个宏，全部添加到SRC_DIR中
#※末尾不能有空格
#范例：
#Main_Dir = $(ROOT_DIR)/main
Main_dir = $(ROOT_DIR)/SupportLayer/ClusterMgr
MysqlClient_dir = $(MYSQL_DIR)
Interface_dir = $(SDK_DIR)/Interface
ClusterMgr_dir = $(SDK_DIR)/Interface/ClusterMgr/cpp


#输出目录
OBJ_OUTPUT_DIR=./output
OUTPUT_DIR=./bin
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
INCLUDE += -I$(SDK_DIR) 
INCLUDE += -I$(MDK_DIR)/include 
INCLUDE += -I$(Protocl_DIR) 
INCLUDE += -I$(MYSQL_DIR)/linux/include 


#库目录，库文件:-L 目录 -库名
#LIB = -lnsl -lc -lm -lpthread -lstdc++ -lrt 
LIB = -lnsl -lc -lm -lpthread -lstdc++ -lrt 

#静态库：.a文件路径名
#按照以下格式添加
#LIB += .a文件路径名+空格
LIB += $(MYSQL_DIR)/linux/lib/libmysqlclient.a 
LIB += $(SDK_DIR)/lib/common.a 
LIB += $(SDK_DIR)/lib/Protocl.a 
LIB += $(SDK_DIR)/lib/mdk_ex.a 
LIB += $(MDK_DIR)/lib/mdk.a 

#------------------------------------------编译选项End--------------------------------------------------------


#------------------------------------------输出--------------------------------------------------------
#编译产生的程序文件名
OUTPUT = ClusterMgr

#目标文件
#范例
#目标文件列表 = $(notdir $(patsubst %.cpp,%.o,$(wildcard [cpp源文件目录]/*.cpp))) 
#cpp分布在多少个目录中,就定义多少个[目标文件列表]
#范例：
#OBJ列表1 = $(notdir $(patsubst %.cpp,%.o,$(wildcard mdk目录/source/mdk/*.cpp))) 
#OBJ列表2 = $(notdir $(patsubst %.cpp,%.o,$(wildcard mdk目录/source/frame/netserver/*.cpp))) 
#OBJ列表2 = $(notdir $(patsubst %.cpp,%.o,$(wildcard mdk目录/source/frame/netserver/*.cpp))) 
Main_OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(Main_dir)/*.cpp))) 
MysqlClient_OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(MysqlClient_dir)/*.cpp))) 
Interface_OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(Interface_dir)/*.cpp))) 
ClusterMgr_OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(ClusterMgr_dir)/*.cpp))) 

#依赖的目标文件
DEPENDENCE = 
#将目标文件中定义的列表宏全部添加到DEPENDENCE中，每个项目都并以空格结束
#范例：
#DEPENDENCE += $(OBJ列表1) 
#DEPENDENCE += $(OBJ列表2) 
DEPENDENCE += $(Main_OBJ) 
DEPENDENCE += $(MysqlClient_OBJ) 
DEPENDENCE += $(Interface_OBJ) 
DEPENDENCE += $(ClusterMgr_OBJ) 

#连接项目，需要的所有其它源文件或目标文件(带目录)
OBJ = 
#将目标文件中定义的列表宏，按照如下格式的指令，全部添加到OBJ中，每个项目都以空格结束
#指令：$(addprefix $(OBJ_OUTPUT_DIR)/, $(OBJ列表)) 
#范例：
#OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(OBJ列表1)) 
#OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(OBJ列表2)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(Main_OBJ)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(MysqlClient_OBJ)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(Interface_OBJ)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(ClusterMgr_OBJ)) 

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

$(OUTPUT_DIR)/$(OUTPUT): $(DEPENDENCE)										
	@echo "Complie (OUTPUT_DIR)/$(OUTPUT)"
	$(CC) -o $@ $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE)$(OBJ)$(LIB)		
	@echo ""
	@echo "$(OUTPUT_DIR)/$(OUTPUT) complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""


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


$(Main_OBJ):%.o:$(Main_dir)/%.cpp $(Main_dir)/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $(Main_dir)/$*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

$(MysqlClient_OBJ):%.o:$(MysqlClient_dir)/%.cpp $(MysqlClient_dir)/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $(MysqlClient_dir)/$*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

$(Interface_OBJ):%.o:$(Interface_dir)/%.cpp $(Interface_dir)/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $(Interface_dir)/$*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

$(ClusterMgr_OBJ):%.o:$(ClusterMgr_dir)/%.cpp $(ClusterMgr_dir)/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) $(ClusterMgr_dir)/$*.cpp
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
