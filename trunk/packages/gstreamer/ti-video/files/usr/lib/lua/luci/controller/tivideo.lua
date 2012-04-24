module("luci.controller.tivideo", package.seeall)

function index()
	if not nixio.fs.access("/etc/ti/encodeCombo.x64P") then
		return
	end
	
	entry({"admin", "status", "tivideo"}, template("tivideo"), "Video", 1).dependent = false;
	entry({"admin", "services", "tivideo"}, cbi("tivideo/tivideo"), "Video", 1).dependent = false;
end
