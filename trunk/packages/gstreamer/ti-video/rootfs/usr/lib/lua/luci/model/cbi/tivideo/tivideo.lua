--[[
LuCI - Lua Configuration Interface

tivideo.lua - configuration options for RTP/UDP video streaming client/server

Note: not all possible options are represented here, but these are the 
      values that have been tested
]]--
m = Map("tivideo", "Video", "Setup Video Configuration")

s = m:section(TypedSection, "video", "General")
s.anonymous = true
s.addremove = false

p = s:option(ListValue, "type", "Server/Client")
p:value("server", "Server")
p:value("client", "Client")
p.default = "server"

p1 = s:option(Value, "host", "Host", "The address to send/receive on")
p1.datatype = "ip4addr"
p1.placeholder = "225.3.3.3"
p1.rmempty = false

p2 = s:option(Value, "port", "Port", "The port to send/receive on")
p2.datatype = "port"
p2.placeholder = "5000"
p2.rmempty = false

-- Note: only NTSC resolutions currently suported by init script
p5 = s:option(ListValue, "resolution", "Resolution", "The width/height of the video output before encoding")
p5:depends("type","server")
p5:value("720x480", "720x480")
p5:value("640x480", "640x480 (VGA 4/3)")
p5:value("352x240", "352x240")
p5:value("320x240", "320x240 (QVGA 4/3)")
p5.default = "720x480"
--p5 = s:option(Value, "width", "Width", "The width of the video output before encoding")
--p5:depends("type","server")
--p5.default = 720

--p6 = s:option(Value, "height", "Height", "The height of the video output beforeencoding")
--p6:depends("type","server")
--p6.default = 480

p3 = s:option(Value, "fps", "Frames per Second (1-30, 30=29.97)", "The FrameRate to send")
p3:depends("type","server")
p3.datatype = "range(1,30)"
p3.default = 30

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
p4.datatype = "range(200,6000000)"
p4.default = 2000000

--p2 = s:option(ListValue, "codec", "Codec", "The Codec to use")
--p2:value("h264", "H.264")
--p2:value("mpeg4", "MPEG4")

p9 = s:option(Value, "jitter", "Jitter Buffer", "Time in mS to buffer")
p9:depends("type","client")
p9.default = 0
p9.datatype = "uinteger"

return m
