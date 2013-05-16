.PHONY: compile

#INCLUDES += -I$(prefix)/include/amlplayer

SRCS = AndroidPlayer.cpp
#SRCS+= AMLUtils.cpp
#SRCS+= FileURLProtocol.cpp

LIB  = androidplayer.a

/home/wangx/xbmc-android/xbmc/system/advancedsettings.xml: $(LIB)
	cp -f androidplayer_advancedsettings.xml $@

include /home/wangx/xbmc-android/xbmc/Makefile.include
-include $(patsubst %.cpp,%.P,$(patsubst %.c,%.P,$(SRCS)))

