#makefile文件编译指令
#如果make文件名是makefile，直接使用make就可以编译
#如果make文件名不是makefile，比如test.txt，那么使用make -f test.txt

#include dependence

#————————————编译子模块————————————#
#方法一：(make -C subdir) 
#方法二：(cd subdir && make)
#加-w 可以查看执行指令时，当前工作目录信息

#————————————编译Exist————————————#
Output_lib_dir=./lib
Output_bin_dir=./bin
$(shell mkdir $(Output_lib_dir))
$(shell mkdir $(Output_bin_dir))

all: 
	@echo "Complie common"
	(make -C ./common -w)
	@echo ""
	@echo "common Complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""
	@echo "Complie DBCenter"
	(make -C ./DBCenter -w)
	@echo ""
	@echo "DBCenter Complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""
	@echo "Complie mdk_ex"
	(make -C ./mdk_ex -w)
	@echo ""
	@echo "mdk_ex Complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""
	@echo "Complie Protocl"
	(make -C ./Protocl -w)
	@echo ""
	@echo "Protocl Complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""
	@echo "Complie ClusterMgr"
	(make -C ./Interface/ClusterMgr/cpp -w)
	@echo ""
	@echo "ClusterMgr Complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""


clean:
	rm $(Output_lib_dir)/*.a
	(make -C ./common -w clean)
	(make -C ./DBCenter -w clean)
	(make -C ./mdk_ex -w clean)
	(make -C ./Protocl -w clean)
