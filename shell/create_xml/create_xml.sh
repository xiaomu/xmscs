#! /bin/bash

#author: dengzhaoqun
#date: 201202/14

outfile=out
tabs=0

put(){
	echo '<'${*}'>' >> $outfile
}

put_head(){
	put '?'${1}'?'
}

out_tabs(){
	tmp=0
	tabsstr=""
	while [ $tmp -lt $((tabs)) ]
	do
		tabsstr=${tabsstr}'\t'
		tmp=$((tmp+1))
	done
	echo -e -n $tabsstr >> $outfile
}

tag_start(){
	out_tabs
	put $1
	tabs=$((tabs+1))
}

tag_end(){
	tabs=$((tabs-1))
	out_tabs
	put '/'${1}
}

tag_value(){
	out_tabs
	str=""
	str=${1}' value="'${2}'"/'
	put $str
}

