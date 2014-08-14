LIBS=gstreamer-0.10 gst-rtsp-server-0.10 glib-2.0 libxml-2.0
CFLAGS+=`pkg-config --cflags $(LIBS)`
LDFLAGS+=`pkg-config --libs $(LIBS)`

all:
	$(CC) $(CFLAGS) gst-rtsp.c $(LDFLAGS) -o gst-rtsp

clean:
	rm gst-rtsp
