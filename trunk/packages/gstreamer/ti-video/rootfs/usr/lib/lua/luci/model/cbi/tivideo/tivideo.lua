--[[
LuCI - Lua Configuration Interface

Copyright 2008 Steven Barth <steven@midlink.org>
Copyright 2008 Jo-Philipp Wich <xm@leipzig.freifunk.net>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

$Id: tivideo.lua 5941 2010-03-27 02:41:29Z jow $
]]--
m = Map("tivideo", "Video", "Setup Video Configuration, reboot to apply")

s = m:section(TypedSection, "video", "General")
s.anonymous = true
s.addremove = false

p = s:option(ListValue, "type", "Server/Client")
p:value("server", "Server")
p:value("client", "Client")
p.default = "server"

s:option(Value, "host", "Host", "The address to send/receive on")

s:option(Value, "port", "Port", "The port to send/receive on")

p3 = s:option(Value, "fps", "Frames per Second(30=29.97)", "The FrameRate to send")
p3:depends("type","server")
p3.default = 30

p5 = s:option(Value, "width", "Width", "The width of the video output before encoding")
p5:depends("type","server")
p5.default = 720

p6 = s:option(Value, "height", "Height", "The height of the video output beforeencoding")
p6:depends("type","server")
p6.default = 480

p8 = s:option(ListValue, "quality", "Encoding Type")
p8:depends("type","server")
p8:value("2", "High Quality")
p8:value("3", "High Speed")
p8.default = "3"

p7 = s:option(ListValue, "ratecontrol", "Rate Control")
p7:depends("type","server")
p7:value("2", "Constant BitRate")
p7:value("3", "Variable BitRate")
p7.default = "2"

p4 = s:option(Value, "bps", "BitRate", "The BitRate to Send")
p4:depends("type","server")
p4.default = 2000000

p2 = s:option(ListValue, "codec", "Codec", "The Codec to use")
p2:value("h264", "H.264")
p2:value("mpeg4", "MPEG4")

p9 = s:option(Value, "jitter", "Jitter Buffer", "Time in mS to buffer")
p9:depends("type","client")
p9.default = 0

return m
