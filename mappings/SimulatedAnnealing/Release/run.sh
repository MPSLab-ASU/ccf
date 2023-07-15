ist=$(find $1 -mindepth 1 -maxdepth 5 -type d)

for dir in $dirlist
do
        pushd $dir
	for f in *.txt;
	do
        	/home/mahdi/workspace/REGIMap_2.2/Release/REGIMap_2.2 -FILE $f -X 4 -Y 4 -R 8 -REGI &
	        i=0
        	while true;     do
                	ps cax | grep REGIMap_2.2 > /dev/null
	                if [ $? -eq 0 ]; then
        	                i=$((i + 1))
                	        if [ $i -gt 120 ]; then
                        	        kill $! 2>/dev/null && echo "myProgram didn't finish" 
                                	break
	                        fi
        	          echo "Process is running."
                	  sleep 5
	                else
        	        	break
                	fi
	        done
	done
        popd
done
