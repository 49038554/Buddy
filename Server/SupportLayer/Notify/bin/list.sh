listType="run"
if [ $# = 1 ]; then
        listType=$1
fi

pid=$(pidof Notify)
if [ -n "$pid"  ]; then
        if [ "run" = $listType ]; then
                echo "Notify($pid) is running"
        fi
else
        if [ "stop" = $listType ]; then
                echo "Notify is not run"
        fi
fi