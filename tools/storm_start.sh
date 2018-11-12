#!/bin/sh
# Copyright (c) 2014, HelightXu
# Author: Zhwen Xu<HelightXu@gmail.com>
# Created: 2014-04-05
# Description:
#

storm nimbus&
storm supervisor&
storm ui&

