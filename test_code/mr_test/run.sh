# Copyright (c) 2014, HelightXu
# Author: Zhwen Xu<HelightXu@gmail.com>
# Created: 2014-03-28
# Description:
#
pathconf="-conf ./job_config.xml -input /home/test/input/* -output /home/test/output/"
hadoop pipes $pathconf  -program /home/test/mr_word_count
