/* rtspd.c - Simple RTSP server
 *
 * Copyright (C) 2008 Wim Taymans <wim.taymans at gmail.com>
 * Copyright (C) 2011 Gateworks Corporation <tharvey@gateworks.com>
 *
 * Based on gst-rtsp-server test-video.c;:
 * http://cgit.freedesktop.org/gstreamer/gst-rtsp-server/tree/examples/test-video.c
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#include <stdio.h>
#include <string.h>
#include <signal.h>

#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

/* define this if you want the resource to only be available when using
 * user/admin as the password */
#undef WITH_AUTH

#define VERSION "1.0.0"

static gint verbose = 0;

static GstRTSPFilterResult
pool_filter(GstRTSPSessionPool *pool, GstRTSPSession *session, gpointer data)
{
	if (verbose) {
		GTimeVal now;
		g_get_current_time(&now);
		//GstRTSPSessionMedia *media = gst_rtsp_session_get_media(session);
		g_print("Session:%s timeout=%d(%d) expired=%d\n",
			gst_rtsp_session_get_sessionid(session),
			gst_rtsp_session_get_timeout(session),
			gst_rtsp_session_next_timeout(session, &now),
			gst_rtsp_session_is_expired(session, &now)
		);
	}
	return GST_RTSP_FILTER_KEEP;
}

/* this timeout is periodically run to clean up the expired sessions from the
 * pool. This needs to be run explicitly currently but might be done
 * automatically as part of the mainloop. */
static gboolean
timeout (GstRTSPServer * server, gboolean ignored)
{
	GstRTSPSessionPool *pool;
 
	pool = gst_rtsp_server_get_session_pool (server);
	if (verbose) {
		g_print("cleanup: %d active clients\n",
			gst_rtsp_session_pool_get_n_sessions(pool));
	}
	gst_rtsp_session_pool_filter(pool, pool_filter, NULL);
	gst_rtsp_session_pool_cleanup (pool);
	g_object_unref (pool);
 
	return TRUE;
}

#if 0
/** sighandler - signal handler for catching signal and exiting cleanly
 */
static void 
sighandler(int sig)
{
	fprintf(stderr, "%s sig=%d\n", __func__, sig);

	exit(1);
}
#endif

GSList *
parse_config(gchar *configfile)
{
	gchar *contents;
	gchar *path = NULL;
	gchar *pipe = NULL;
	GSList *list = NULL;

	g_print("Parsing %s...\n", configfile);
	if (g_file_get_contents(configfile, &contents, NULL, NULL)) {
		gchar **lines = g_strsplit(contents, "\n", 0);
		g_free(contents);
		for (; NULL != *lines; lines++) {
			if (*lines[0] != '#') {
				if (!isspace(*lines[0]) && *lines[0]) {
					if (path && pipe) {
						list = g_slist_append(list, path);
						list = g_slist_append(list, pipe);
						pipe = path = NULL;
					}
					char *p, *q;
					p = q = *lines;
					while (!isspace(*q)) q++;
					*q++ = 0;
					while (isspace(*q)) q++;
					path = g_strdup(p);
					pipe = g_strdup(q);
				} else {
					pipe = g_strconcat(pipe, g_strstrip(*lines), NULL); 	
				}
			}
			g_free(*lines);
		}
		list = g_slist_append(list, path);
		list = g_slist_append(list, pipe);
	}

	return list;
}

static GstRTSPMediaFactory *
add_stream(GstRTSPMediaMapping *mapping, gchar *_path, gchar *_pipeline,
	gboolean shared, gboolean force_mcast)
{
	GstRTSPMediaFactory *factory;
	gchar *path;
	gchar *pipeline;

	/* make sure path has leading '/' */
	if (_path[0] != '/') {
		path = g_strconcat("/", _path, NULL);
	} else {
		path = g_strdup(_path);
	}

	/* make sure pipeline is in bin notation */
	if (_pipeline[0] != '(') {
		pipeline = g_strconcat("( ", _pipeline, " )", NULL);
	} else {
		pipeline = g_strdup(_pipeline);
	}

	g_print("%s:%s\n", (char*) path, pipeline);

	/* make a media factory for a test stream. The default media factory can use
	 * gst-launch syntax to create pipelines. 
	 * any launch line works as long as it contains elements named pay%d. Each
	 * element with pay%d names will be a stream */
	factory = gst_rtsp_media_factory_new ();

	gst_rtsp_media_factory_set_launch (factory, pipeline);
	gst_rtsp_media_factory_set_shared (factory, shared);
#ifndef NO_FORCE
	if (force_mcast) {
		gst_rtsp_media_factory_set_protocols (factory, 
			GST_RTSP_LOWER_TRANS_UDP_MCAST);
	}
#endif

	/* attach the test factory to the /test url */
	gst_rtsp_media_mapping_add_factory (mapping, path, factory);

	g_free(path);
	g_free(pipeline);

	return factory;
}

int
main (int argc, char *argv[])
{
	GMainLoop *loop;
	GstRTSPServer *server;
	GstRTSPMediaMapping *mapping;
#ifdef WITH_AUTH
	GstRTSPAuth *auth;
	gchar *basic;
#endif
	GSList *streams = NULL;
	gchar *address = "0.0.0.0";
	gchar *service = "rtsp";
	gint backlog = 0;
	gboolean shared = FALSE;
	gboolean force_mcast = FALSE;
	gchar *host = NULL; 
	//gchar *configfile = "/etc/rtspd.conf";
	gchar *configfile = NULL;
	GError *err = NULL;
	GOptionContext *ctx;
	int i;
	GOptionEntry options[] = {
		{"config", 'f', 0, G_OPTION_ARG_STRING, &configfile, "config file", "file"},
		{"address", 'i', 0, G_OPTION_ARG_STRING, &address, "address to listen on", "addr"},
		{"service", 0, 0, G_OPTION_ARG_STRING, &service, "service to listen on", "service"},
		{"shared", 's', 0, G_OPTION_ARG_NONE, &shared, "share streams where possible", NULL},
		{"force-mcast", 0, 0, G_OPTION_ARG_NONE, &force_mcast, "force multicast", NULL},
		{"debug", 'd', 0, G_OPTION_ARG_NONE, &verbose, "debug messages", NULL},
		{NULL}
	};

	printf("gst-rtsp-server v%s\n", VERSION);

	if (!g_thread_supported())
		g_thread_init(NULL);

	ctx = g_option_context_new ("[<path1> <pipeline1> ...]");
	g_option_context_add_main_entries(ctx, options, NULL);
	g_option_context_add_group(ctx, gst_init_get_option_group());
	if (!g_option_context_parse(ctx, &argc, &argv, &err)) {
		g_print ("Error initializing: %s\n", GST_STR_NULL(err->message));
		exit (1);
	}
	
	gst_init (&argc, &argv);

#if 0
	/* install signal handler */
	signal(SIGINT, sighandler);
	signal(SIGSEGV, sighandler);
#endif

	loop = g_main_loop_new (NULL, FALSE);

	/* create a server instance */
	server = gst_rtsp_server_new ();
	gst_rtsp_server_set_service(server, service);
	gst_rtsp_server_set_address(server, address);
	if (backlog)
		gst_rtsp_server_set_backlog(server, backlog);

	/* get the mapping for this server, every server has a default mapper object
	 * that be used to map uri mount points to media factories
	 */
	mapping = gst_rtsp_server_get_media_mapping (server);

#ifdef WITH_AUTH
	/* make a new authentication manager. it can be added to control access to all
	 * the factories on the server or on individual factories. */
	auth = gst_rtsp_auth_new ();
	basic = gst_rtsp_auth_make_basic ("user", "admin");
	gst_rtsp_auth_set_basic (auth, basic);
	g_free (basic);
	/* configure in the server */
	gst_rtsp_server_set_auth (server, auth);
#endif

/*
	streams = g_slist_append(streams, add_stream(mapping, "/test",
		"videotestsrc ! video/x-raw-yuv,width=320,height=240,framerate=10/1 ! "
		"x264enc ! queue ! rtph264pay name=pay0 pt=96 ! audiotestsrc ! audio/x-raw-int,rate=8000 ! alawenc ! rtppcmapay name=pay1 pt=97 "")", shared, gboolean force_mcast));
*/

	if (configfile) {
		GSList *list;

		list = parse_config(configfile);
		if (list == NULL || g_slist_length(list) == 0) {
			g_print("Error: no pipelines found in %s\n", configfile);
			return -1;
		}
	
		/* iterate over pipelines adding mappings for each
		 */
		for (i = 0; i < g_slist_length(list); i+=2) {
			GstRTSPMediaFactory *factory = add_stream(mapping,
				g_slist_nth_data(list, i),
				g_slist_nth_data(list, i+1),
				shared, force_mcast);
			streams = g_slist_append(streams, factory);
		}
		g_slist_free(list);
	}

	/* parse commandline arguments */
	i = 1;
	while ( (argc - i) >= 2) {
		GstRTSPMediaFactory *factory = add_stream(mapping, argv[i],
			argv[i+1], shared, force_mcast);
		streams = g_slist_append(streams, factory);
		i+=2;
	}

	/* ensure we have streams mapped */
	if (g_slist_length(streams) == 0) {
		g_print ("Error: no streams defined\n");
		g_print ("%s\n", g_option_context_get_help(ctx, 0, NULL));
		return -1;
	}
	g_option_context_free(ctx);

	/* don't need the ref to the mapper anymore */
	g_object_unref (mapping);

	/* attach the server to the default maincontext */
	if (gst_rtsp_server_attach (server, NULL) == 0) {
		g_error ("Failed to attach to %s:%s\n", address, service);
	}

	/* add a timeout for the session cleanup */
	g_timeout_add_seconds (2, (GSourceFunc) timeout, server);

	/* configure pools */
/*
	GstRTSPSessionPool *pool = gst_rtsp_server_get_session_pool (server);
	gst_rtsp_session_pool_set_max_sessions(max_sessions);
	g_object_unref (pool);
*/

	/* start serving */
	g_print("Listening on %s:%s\n", address, service);
	g_main_loop_run (loop);

	return 0;
}
