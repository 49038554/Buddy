listType="run"
if [ $# = 1 ]; then
        listType=$1
fi

pid=$(pidof TCPEntry)
if [ -n "$pid"  ]; then
        if [ "run" = $listType ]; then
                echo "TCPEntry($pid) is running"
        fi
else
        if [ "stop" = $listType ]; then
                echo "TCPEntry is not run"
        fi
fi