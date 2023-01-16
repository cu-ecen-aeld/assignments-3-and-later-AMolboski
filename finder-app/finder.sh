#!/bin/bash
# Finder Script
# Written For Linux System Programming And Introduction to Buildroot: Assignment 1
# Written By Andrew Molboski, 4 Jan 2023

if [ $# -lt 2 ]
then
	echo "Invalid number of arguments; please enter 2 arguments: "
	echo " - Argument 1 must be a directory path (the directory to be searched)"
        echo " - Argument 2 must be a text string (the search parameter)"
	exit 1
else
	filesdir=$1
	searchstr=$2
	if [ ! -d "$filesdir"  ]
	then
		echo "Path does not exist"
		exit 1
	else
		cd $filesdir
		X=$(find . -type f | wc -l)
		Y=$(grep -r $searchstr | wc -l)
		echo "The number of files are $X and the number of matching lines are $Y"
		exit 0
	fi
fi	
