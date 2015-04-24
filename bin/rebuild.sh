#!/bin/bash

source_files=$*

rehash()
{
    cat $source_files | md5sum | awk '{ print $1 }'
}

rebuild()
{
    N=$(expr $N + 1)
    M=$(rehash)
    colorgcc.pl $source_files -o answer \
	&& echo "Rebuild $N successful"
}

rebuild

while (( 1 )) ; do
    H=$(rehash)
    [ "$H" != "$M" ] && M="$H" && rebuild
    sleep 0.2
done

