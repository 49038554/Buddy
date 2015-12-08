pid=$(pidof Auth)
if [ -n "$pid"  ]; then
	echo "Auth($pid) is running"
fi
