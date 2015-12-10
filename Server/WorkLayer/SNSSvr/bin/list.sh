pid=$(pidof SNSSvr)
if [ -n "$pid"  ]; then
	echo "SNSSvr($pid) is running"
fi
