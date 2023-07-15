#!/bin/bash

shopt -s extglob
rm !(livein*|delete.sh|run_command|loop91_node*|loop91_edge*|loop_91DFG.dot) 
shopt -u extglob
