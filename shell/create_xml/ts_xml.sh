#! /bin/bash

#author: dengzhaoqun
#date: 20120215

source './create_xml.sh'
put_head 'xml version='1.0' encoding="GBK"'
tag_start '投递信息'
tag_start '硬件'
tag_value '网卡' 1
tag_end '硬件'
tag_end '投递信息'
