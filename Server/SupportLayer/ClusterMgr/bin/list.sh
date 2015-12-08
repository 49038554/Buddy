pid=$(pidof ClusterMgr)
if [ -n "$pid"  ]; then
	echo "ClusterMgr($pid) is running"
fi
