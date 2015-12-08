pid=$(pidof DBCenter)
if [ -n "$pid"  ]; then
	echo "DBCenter($pid) is running"
fi
