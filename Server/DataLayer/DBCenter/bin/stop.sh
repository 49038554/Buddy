if [ $# == 0 ]; then
		pid=$(pidof DBCenter)
		if [ -n "$pid"  ]; then
			kill $pid
			echo "kill DBCenter $pid"
		fi
        exit 1;
fi

if [ $# != 1 ]; then
        echo commond:stop.sh [binfile];
        exit 1;
fi
exe=$1
echo $exe

pid=$(pidof $exe)
if [ -n "$pid"  ]; then
	kill $pid
	echo "kill $exe $pid"
fi
