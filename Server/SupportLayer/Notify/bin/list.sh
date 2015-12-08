pid=$(pidof Notify)
if [ -n "$pid"  ]; then
	echo "Notify($pid) is running"
fi
