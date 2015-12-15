# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#����λ��Android.mk�ļ����ʼ��������λԴ�ļ���λ�ã�$(call my-dir)�����þ��Ƿ��ص�ǰĿ¼��·����
#$(call my-dir)ֻ��Ҫ������1��
LOCAL_PATH := $(call my-dir)

#���һЩ������ֵ������LOCAL_PATH����
include $(CLEAR_VARS)

#������ָ����ǰ������ģ�����
LOCAL_MODULE    := NetWork

#������ָ����������Դ�����ļ� �������ӷ�\.
tool_source		:= ../tool/mapi.cpp \
					../tool/Executor.cpp \
					../tool/Lock.cpp \
					../tool/Signal.cpp \
					../tool/Socket.cpp \
					../tool/Task.cpp \
					../tool/Thread.cpp \
					../tool/ConfigFile.cpp \
					../tool/Logger.cpp 
common_source	:=	../../../common/Ecliptic.cpp 

main_source		:= ../WorkSupper.cpp server.cpp JavaObject.cpp
LOCAL_SRC_FILES := $(tool_source) $(main_source) $(common_source) 


#�����ⲿ��Cͷ�ļ������ù���ͬmake�ļ��ⲿͷ�ļ�
#LOCAL_CFLAGS := / -I/home/huoyu/include 
LOCAL_CPPFLAGS += -fexceptions


#�����ⲿ���ļ������ù���ͬmake�ļ��ⲿ���ļ�
LOCAL_LDLIBS := -L./ libbstruct.a

#����ǰģ�����Ϊ����⣬ǰ׺Ϊlib����׺Ϊ.so
include $(BUILD_SHARED_LIBRARY)

#����ǰģ�����Ϊ����⣬ǰ׺Ϊlib����׺Ϊ.a��androidֻ�ܵ���so��ʽ�ı��ؿ�
#include $(BUILD_STATIC_LIBRARY)

#����������Ŀ¼�е�Android.mk�ļ�
#include $(call all-subdir-makefiles)
