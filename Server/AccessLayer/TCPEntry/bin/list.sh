pid=$(pidof TCPEntry)
if [ -n "$pid"  ]; then
	echo "TCPEntry($pid) is running"
fi
