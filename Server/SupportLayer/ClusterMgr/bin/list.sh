listType="run"
if [ $# = 1 ]; then
        listType=$1
fi

pid=$(pidof ClusterMgr)
if [ -n "$pid"  ]; then
        if [ "run" = $listType ]; then
                echo "ClusterMgr($pid) is running"
        fi
else
        if [ "stop" = $listType ]; then
                echo "ClusterMgr is not run"
        fi
fi