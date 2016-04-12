local skynet = require "skynet"
local uuid = require 'uuid'

skynet.start(function()
	local loginserver = skynet.newservice("logind")
	local gate = skynet.newservice("gated", loginserver)
	--skynet.newservice("debug_console", 8889)
	skynet.call(gate, "lua", "open" , {
		port = 8888,
		maxclient = 64,
		servername = "sample",
	})
end)
